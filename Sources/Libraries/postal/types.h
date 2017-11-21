#pragma once

#include <functional>

#include "common/fsm.h"
#include "common/waitable_queue.h"
#include "stuff/enforce.h"
#include "defaultLogs/library.include.h"

namespace postal
{
	enum class EVENT_CLASS : char
	{
		CHANGE_STATE,
		CHANGE_STATE_NO_WAIT,
		STATE_CHANGE_CONFIRM,
		STATE_CHANGE_CONFIRM_ANSWER,
		DATA,
	};

	template<class EVENT_TYPE> EVENT_TYPE initial();

	template<class EVENT_TYPE>
	struct EventHeader 
	{
		EventHeader():eventcls(EVENT_CLASS::DATA) {}
		EventHeader(EVENT_TYPE ev) :event(ev), eventcls(EVENT_CLASS::DATA) {}

		EVENT_TYPE event;
		EVENT_CLASS eventcls;

	};

	template<class EVENT_TYPE>
	std::string to_string(const EventHeader<EVENT_TYPE>& header)
	{
		std::string result;

		switch (header.eventcls)
		{
		case EVENT_CLASS::CHANGE_STATE:result += "CHANGE_STATE"; break;
		case EVENT_CLASS::CHANGE_STATE_NO_WAIT: result += "CHANGE_STATE_NO_WAIT"; break;
		case EVENT_CLASS::STATE_CHANGE_CONFIRM:result += "STATE_CHANGE_CONFIRM"; break;
		case EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER:result += "STATE_CHANGE_CONFIRM_ANSWER"; break;
		case EVENT_CLASS::DATA: result += "DATA"; break;
		default: result += "UNKNOWN_CLASS"; break;
		}

		return result;
	}

	template<class MessageIOStream, class EVENT_TYPE>
	struct RecvEventMessage
	{
		RecvEventMessage() : is(0) {}
		RecvEventMessage(MessageIOStream& is_) : is(&is_) { is_ >> header; }

		EventHeader<EVENT_TYPE> header;
		MessageIOStream* is;
	};

	struct SendEventMessage
	{
		template <class EVENT_TYPE, class Connection, class PackedData>
		static void SEND(EVENT_TYPE event, typename Connection::MessageIOStream& os, const PackedData& data, Connection& connection)
		{
			os.reset();
			EventHeader<EVENT_TYPE> header(event);
			os << header << data;
			connection.sendmsg(os.data());
		}

		template <class EVENT_TYPE, class Connection, class PackedData>
		static void SEND(EVENT_TYPE event, const PackedData& data, Connection& connection)
		{
			typename Connection::MessageIOStream os;
			SEND(event, os, data, connection);
		}

		template <class EVENT_TYPE, class Connection, class Function>
		static void SEND_FUNC(EVENT_TYPE event, typename Connection::MessageIOStream& os, Function& func, Connection& connection)
		{
			os.reset();
			EventHeader<EVENT_TYPE> header(event);
			os << header;
			func(os);
			connection.sendmsg(os.data());
		}

		template <class EVENT_TYPE, class Connection, class Function>
		static void SEND_FUNC(EVENT_TYPE event, const Function& func, Connection& connection)
		{
			typename Connection::MessageIOStream os;
			SEND_FUNC(event, os, func, connection);
		}

		template <class EVENT_TYPE, class Connection>
		static void SEND_HEADER(EVENT_TYPE event, EVENT_CLASS eventClass, Connection& connection)
		{
			EventHeader<EVENT_TYPE> header(event);
			header.eventcls = eventClass;
			connection.sendmsg(header);
		}
	};


	template<class EVENT_TYPE, class MessageIOStream, class AdditionalData>
	struct FSMMessageInterface
	{
		typedef AdditionalData ContentType;

		auto empty_message() const { return RecvEventMessage<MessageIOStream, EVENT_TYPE>(); }

		virtual void internal_post(const RecvEventMessage<MessageIOStream, EVENT_TYPE>& event) = 0;
		AdditionalData additionalData;
	};


	template<class Connection>
	class BaseState
	{
	friend struct DefaultAdapter;
	public:
		virtual void enter(Connection& connection) = 0;
		virtual void exit(Connection& connection) = 0;
	};

	template<class MessageIStream, class Connection, class EVENT_TYPE_T>
	class BaseStateMessagesT : public BaseState<Connection>
	{
	public:
		typedef EVENT_TYPE_T EVENT_TYPE;

		struct RecvEvent : public RecvEventMessage<MessageIStream, EVENT_TYPE>
		{
			RecvEvent() {}
			RecvEvent(MessageIStream& is) : RecvEventMessage<MessageIStream, EVENT_TYPE>(is) {}

			static RecvEvent initial()
			{
				RecvEvent event;
				event.header.event = postal::initial<EVENT_TYPE>();
				event.header.eventcls = EVENT_CLASS::CHANGE_STATE;

				return event;
			}
		};
	};

	struct EmptyData {};

	template<class MessageIOStream, class MessageIStream, class Connection, class DataDispatcher, class EVENT_TYPE, class AdditionalDataT = EmptyData>
	class BaseStateT : public BaseStateMessagesT<MessageIStream, Connection, EVENT_TYPE>
	{
	public:
		typedef AdditionalDataT AdditionalData;

		virtual void enter(Connection& connection) = 0;
		virtual void exit(Connection& connection) = 0;
		virtual void internalMessage(MessageIOStream& is, Connection& connection) = 0;
		virtual void externalMessage(MessageIStream& is, Connection& connection) = 0;

		void remoteChangeState(Connection& connection, EVENT_TYPE event, bool confirm)
		{
			//LOG_MSG("sys_enter: " << typeid(BaseStateT).name());
			SendEventMessage::SEND_HEADER(event, confirm ? EVENT_CLASS::STATE_CHANGE_CONFIRM : EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER, connection);
		}

		FSMMessageInterface<EVENT_TYPE, MessageIOStream, AdditionalData>* parentFSM = nullptr;
		DataDispatcher* dispatcher = nullptr;
	};

	template<class MessageIOStream, class MessageIStream, class Connection, class BaseClass>
	struct StartState : public BaseClass
	{
		virtual void enter(Connection& connection)  override { }
		virtual void exit(Connection& connection)  override { }
		virtual void internalMessage(MessageIOStream& is, Connection& connection)  override { }
		virtual void externalMessage(MessageIStream& is, Connection& connection)  override { }
	};


	struct DefaultAdapter
	{
		template<class FSM, class State>
		static void enter(FSM& sm, State& state)
		{
			ENFORCE(sm.connection);
			state.enter(*sm.connection);
		}

		template<class FSM, class State>
		static void exit(FSM& sm, State& state)
		{
			ENFORCE(sm.connection);
			state.exit(*sm.connection);
		}
	};

	template<class MessageIOStreamT, class MessageIStreamT, class ConnectionT, class DataDispatcherT, class ClientFSM, class BaseStateTypeT, class TransitionsList>
	struct ProcessFSMT :	public fsm::FSM<ClientFSM, BaseStateTypeT, DefaultAdapter, TransitionsList>,
							public FSMMessageInterface<typename BaseStateTypeT::EVENT_TYPE, MessageIOStreamT, typename BaseStateTypeT::AdditionalData>
	{
		typedef fsm::FSM<ClientFSM, BaseStateTypeT, DefaultAdapter, TransitionsList> Parent;
		typedef MessageIOStreamT MessageIOStream;
		typedef MessageIStreamT MessageIStream;
		typedef BaseStateTypeT BaseStateType;
		typedef ConnectionT Connection;
		typedef DataDispatcherT DataDispatcher;
		

		ProcessFSMT()
		{
			Loki::TupleFunc::exec(this->transitions.transitions.states, [this](auto& state)
			{ 
				state.parentFSM = this;
			});
		}

		void post(const typename BaseStateType::RecvEvent& event)
		{
			ENFORCE(connection);
			if (event.header.eventcls != EVENT_CLASS::DATA)
			{
				BaseStateType* nexts = this->next(event.header.event);
				if (!nexts)
				{
					nexts = this->next(event.header.event);
					//LOG_ERROR(connection->debugName() << " " << connection << " " << to_string(event.header));
				}	

				// LOG_ERROR(connection->debugName() << " " << connection << " " << to_string(event.header));
				
				ENFORCE(nexts);
				if (nexts)
				{
					switch (event.header.eventcls)
					{
					case EVENT_CLASS::CHANGE_STATE:
					{
						nexts->remoteChangeState(*connection, event.header.event, true);
						break;
					}
					case EVENT_CLASS::CHANGE_STATE_NO_WAIT:
					{
						Parent::post(event.header.event);
						nexts->remoteChangeState(*connection, event.header.event, false);
						break;
					}
					case EVENT_CLASS::STATE_CHANGE_CONFIRM:
					{
						Parent::post(event.header.event);

						SendEventMessage::SEND_HEADER(event.header.event, EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER, *connection);
						break;
					}
					case EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER:
					{
						Parent::post(event.header.event);
						break;
					}
					case EVENT_CLASS::DATA:
					{
						throw std::runtime_error("DATA is abnormal tag");
					}
					}
				}
			}	
			else
			{
				externalMessageFSM(*event.is);
			}	
		}

		virtual void internal_post(const RecvEventMessage<MessageIOStream, typename BaseStateType::EVENT_TYPE>& event)
		{				
			post((typename BaseStateType::RecvEvent&)event);
		}

		void internalMessageFSM(MessageIOStream& is) { this->current.state->internalMessage(is, *connection); }
		void externalMessageFSM(MessageIStream& is) { this->current.state->externalMessage(is, *connection); }

		Connection* connection = nullptr;
	};

	template<class MessageIOStream, class MessageIStream, class Connection, class BaseState, class TransitionsList>
	struct DataDispatcherT
	{
		struct ProcessFSM : public ProcessFSMT<MessageIOStream, MessageIStream, Connection, DataDispatcherT, ProcessFSM, BaseState, TransitionsList> {};
		
		void start()
		{
			ENFORCE(!started);
			fsm.post(BaseState::RecvEvent::initial());
			started = true;
		}

		void recvmsg(MessageIStream& is)
		{
			fsm.post(typename BaseState::RecvEvent(is));
		}

		void internalMessage(MessageIOStream& is)
		{
			fsm.internalMessageFSM(is);
		}

		bool started = false;
		ProcessFSM fsm;
	};

	template<class MessageIOStream, class InternalMessageProcessor>
	class InternalProcessor
	{
	public:
		void debugName(const std::string& name) { internalMessageProcessor.debugName(name); }
		void debugName() const { internalMessageProcessor.debugName(); }

		virtual ~InternalProcessor()
		{
			internalMessageProcessor.stop();
		}
		
		void process(MessageIOStream& ios)
		{
			ios.reset();
			if (processor(ios))
			{
				internalMessageProcessor.internalMessage(ios);
			}
		}
				
		std::function<bool(MessageIOStream&)> processor;
		InternalMessageProcessor internalMessageProcessor;
	};
}

