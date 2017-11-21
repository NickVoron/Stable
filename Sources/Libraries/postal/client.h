#pragma once

namespace postal
{
	template<class Address, class MessageIOStream, class MessageIStream, class Connection, class BaseState, class TransitionsList>
	class Client
	{
	public:			
		void debugName(const std::string& name) { internalProcessor.debugName(name); }
		void debugName() const { internalProcessor.debugName(); }

		struct DataDispatcher : public DataDispatcherT<MessageIOStream, MessageIStream, Connection, BaseState, TransitionsList> {};

		template<class Function>
		bool start(const Address& addr, Function&& processor)
		{
			bool connected = internalProcessor.internalMessageProcessor.connect(addr);
			if (connected)
			{
				internalProcessor.processor = processor;
			}

			return connected;
		}

		void process()
		{
			MessageIOStream is;
			internalProcessor.process(is);
		}

		void updateFSM()
		{
			MessageIStream is;
			internalProcessor.internalMessageProcessor.updateFSM(is);
		}

		bool isInInitialState() const { return internalProcessor.internalMessageProcessor.isInInitialState(); }
		bool isAlive() const { return internalProcessor.internalMessageProcessor.isAlive(); }
		float aliveFactor() const { return internalProcessor.internalMessageProcessor.activityTracker.aliveFactor(); }

		auto& dataTrackers() const { return internalProcessor.internalMessageProcessor.dataTrackers; }

	private:
		InternalProcessor<MessageIOStream, Connection> internalProcessor;
	};
}