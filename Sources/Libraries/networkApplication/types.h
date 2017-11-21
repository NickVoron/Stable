#pragma once

#include <functional>

#include "common/fsm.h"
#include "common/waitable_queue.h"

#include "udtnet/library.include.h"

namespace network
{
	enum class EVENT_CLASS : char
	{
		CHANGE_STATE,
		CHANGE_STATE_NO_WAIT,
		STATE_CHANGE_CONFIRM,
		STATE_CHANGE_CONFIRM_ANSWER,
		DATA,
	};

	template<class EVENT_TYPE>
	struct EventHeader 
	{
		EventHeader():eventcls(EVENT_CLASS::DATA) {}
		EventHeader(EVENT_TYPE ev) :event(ev), eventcls(EVENT_CLASS::DATA) {}

		EVENT_TYPE event;
		EVENT_CLASS eventcls;
	};

	template<class EVENT_TYPE>
	struct RecvEventMessage
	{
		RecvEventMessage() : is(0) {}
		RecvEventMessage(udtnet::IStream& is_) : is(&is_) { is_ >> header; }

		EventHeader<EVENT_TYPE> header;
		udtnet::IStream* is;
	};

	template<class EVENT_TYPE>
	struct SendEventMessage
	{
		template <class PackedData>
		static void SEND(EVENT_TYPE event, udtnet::IOStream& os, const PackedData& data, udtnet::Connection* connection)
		{
			os.reset();
			EventHeader<EVENT_TYPE> header(event);
			os << header << data;
			connection->sendmsg(os.data());
		}

		template <class PackedData>
		static void SEND(EVENT_TYPE event, const PackedData& data, udtnet::Connection* connection)
		{
			udtnet::IOStream os;
			SEND(event, os, data, connection);
		}

		template <class Function>
		static void SEND_FUNC(EVENT_TYPE event, udtnet::IOStream& os, Function& func, udtnet::Connection* connection)
		{
			os.reset();
			EventHeader<EVENT_TYPE> header(event);
			os << header;
			func(os);
			connection->sendmsg(os.data());
		}

		template <class Function>
		static void SEND_FUNC(EVENT_TYPE event, const Function& func, udtnet::Connection* connection)
		{
			udtnet::IOStream os;
			SEND_FUNC(event, os, func, connection);
		}

		static void SEND_HEADER(EVENT_TYPE event, EVENT_CLASS eventClass, udtnet::Connection* connection)
		{
			EventHeader<EVENT_TYPE> header(event);
			header.eventcls = eventClass;
			connection->sendmsg(header);
		}
	};


	template<class EVENT_TYPE, class AdditionalData>
	struct FSMMessageInterface
	{
		typedef AdditionalData ContentType;

		virtual void internal_post(const RecvEventMessage<EVENT_TYPE>& event) = 0;
		AdditionalData additionalData;
	};


	class BaseState
	{
	friend struct DefaultAdapter;
	public:
		virtual void enter(udtnet::Connection* connection) = 0;
		virtual void exit(udtnet::Connection* connection) = 0;
	};

	template<class EVENT_TYPE>
	class BaseStateMessagesT : public BaseState
	{
	public:
		typedef EVENT_TYPE EVENT_TYPE;

		struct RecvEvent : public RecvEventMessage<EVENT_TYPE>
		{
			RecvEvent() {}
			RecvEvent(udtnet::IStream& is_) : RecvEventMessage<EVENT_TYPE>(is_) {}

			static RecvEvent initial()
			{
				RecvEvent event;
				event.header.event = network::initial<EVENT_TYPE>();
				event.header.eventcls = EVENT_CLASS::CHANGE_STATE;

				return event;
			}
		};

		typedef SendEventMessage<EVENT_TYPE> SendEvent;
	};

	struct EmptyData {};

	template<class EVENT_TYPE, class AdditionalData = EmptyData>
	class BaseStateT : public BaseStateMessagesT<EVENT_TYPE>
	{
	public:
		typedef AdditionalData AdditionalData;

		virtual void enter(udtnet::Connection* connection) = 0;
		virtual void exit(udtnet::Connection* connection) = 0;
		virtual void internalMessage(udtnet::IOStream& is, udtnet::Connection* connection) = 0;
		virtual void externalMessage(udtnet::IStream& is, udtnet::Connection* connection) = 0;

		void remoteChangeState(udtnet::Connection* connection, EVENT_TYPE event, bool confirm)
		{
			//LOG_MSG("sys_enter: " << typeid(BaseStateT).name());
			SendEvent::SEND_HEADER(event, confirm ? EVENT_CLASS::STATE_CHANGE_CONFIRM : EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER, connection);
		}

		FSMMessageInterface<EVENT_TYPE, AdditionalData>* parentFSM = 0;
		udtnet::DataDispatcher* dispatcher = 0;
	};

	template<class BaseClass>
	struct StartState : public BaseClass
	{
		virtual void enter(udtnet::Connection* connection) {}
		virtual void exit(udtnet::Connection* connection) {}
		virtual void internalMessage(udtnet::IOStream& is, udtnet::Connection* connection) {}
		virtual void externalMessage(udtnet::IStream& is, udtnet::Connection* connection) {}
	};


	template<class FSMT, class EventMessageType>
	struct DataDispatcherT : public udtnet::DataDispatcher
	{
		virtual void start(udtnet::Connection& connection)
		{
			fsm.connection = &connection;
			fsm.post(EventMessageType::initial());
		}

		virtual void recvmsg(udtnet::IStream& is)
		{
			fsm.post(EventMessageType(is));			
		}

		virtual void internalMessage(udtnet::IOStream& is)
		{
			fsm.internalMessage(is);
		}

		FSMT fsm;
	};

	struct DefaultAdapter
	{
		template<class FSM, class EVENT_TYPE, class AdditionalData>
		void enter(FSM& sm, BaseStateT<EVENT_TYPE, AdditionalData>* state, EVENT_TYPE event)
		{
			state->enter(sm.connection);
		}

		template<class FSM, class EVENT_TYPE, class AdditionalData>
		void exit(FSM& sm, BaseStateT<EVENT_TYPE, AdditionalData>* state, EVENT_TYPE event)
		{
			state->exit(sm.connection);
		}
	};

	template<class ClientFSM, class BaseStateType, class TransitionsList, class InitialState, class BaseStateAdapter>
	struct ProcessFSMT : public fsm::FSM<ClientFSM, BaseStateType, BaseStateAdapter, TransitionsList>, public FSMMessageInterface<typename BaseStateType::EVENT_TYPE, typename BaseStateType::AdditionalData>
	{
		typedef fsm::FSM<ClientFSM, BaseStateType, BaseStateAdapter, TransitionsList> Parent;

		ProcessFSMT()
		{
			Loki::TupleFunc::exec(transitions.transitions.states, [this](BaseStateT<typename BaseStateType::EVENT_TYPE, ContentType>& state)
			{ 
				state.parentFSM = this;
			});
		}

		void post(const typename BaseStateType::RecvEvent& event)
		{
			if (event.header.eventcls != EVENT_CLASS::DATA)
			{
				BaseStateType* nexts = next(event.header.event);
				ENFORCE(nexts);
				if (nexts)
				{
					switch (event.header.eventcls)
					{
					case EVENT_CLASS::CHANGE_STATE:
					{
						nexts->remoteChangeState(connection, event.header.event, true);
						break;
					}
					case EVENT_CLASS::CHANGE_STATE_NO_WAIT:
					{
						Parent::post(event.header.event);
						nexts->remoteChangeState(connection, event.header.event, false);
						break;
					}
					case EVENT_CLASS::STATE_CHANGE_CONFIRM:
					{
						Parent::post(event.header.event);

						BaseStateType::SendEvent::SEND_HEADER(event.header.event, EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER, connection);
						break;
					}
					case EVENT_CLASS::STATE_CHANGE_CONFIRM_ANSWER:
					{
						Parent::post(event.header.event);
						break;
					}
					}
				}
			}	
			else
			{
				externalMessage(*event.is);
			}	
		}

		virtual void internal_post(const RecvEventMessage<typename BaseStateType::EVENT_TYPE>& event)
		{				
			post((BaseStateType::RecvEvent&)event);
		}

		virtual void internalMessage(udtnet::IOStream& is) { current.state->internalMessage(is, connection); }
		virtual void externalMessage(udtnet::IStream& is) {	current.state->externalMessage(is, connection); }

		udtnet::Connection* connection = 0;
	};

	struct DataBuffer
	{
		bool empty() const { return ios.size() == 0; }
		udtnet::IOStream ios;
	};

	template<int count>
	struct DataBuffers
	{
		DataBuffers()
		{												
			for (int i = 0; i < count; ++i)
			{
				freequeue.send(&buffers[i]);
			}
		}
		
		mt::waitable_queue<DataBuffer*> freequeue;
		DataBuffer buffers[count];
	};


	class InternalProcessor
	{
	protected:
		virtual ~InternalProcessor();
		void startProcessing(std::function<bool(DataBuffer&)> processor);
		void startTranslation();
		virtual void internalMessage(udtnet::IOStream& is) = 0;

		DataBuffer* sender();
		bool receive(DataBuffer*& buffer);
		void unlock(DataBuffer* buffer);
		void cancel();

	private:
		mt::waitable_queue<DataBuffer*> usedbuf;
		DataBuffers<1> buffers;
		bool finished;
		bool processing;
		bool transtation;
	};
}

