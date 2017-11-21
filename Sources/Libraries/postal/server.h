#pragma once

#include "componentModel/library.include.h"
#include "udtnet/library.include.h"


namespace postal
{
	template<class Address, class MessageIOStream, class MessageIStream, class Connection, class Connections, class BaseState, class TransitionsList>
   	class Server
	{
	public:		
		void debugName(const std::string& name) {  }
		void debugName() const {  }

		struct DataDispatcher : public DataDispatcherT<MessageIOStream, MessageIStream, Connection, BaseState, TransitionsList> {};

		template<class Function>
		bool start(const Address& address, Function&& processor)
		{
			bool result = internalProcessor.internalMessageProcessor.server(address.port());

			if (result)
			{
				internalProcessor.processor = processor;
			}

			return result;
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

		void runFSM()
		{
			internalProcessor.internalMessageProcessor.runFSM();
		}
		
		std::size_t connectionsCount() const { return internalProcessor.internalMessageProcessor.connectionsCount(); }

	private:	
		InternalProcessor<MessageIOStream, Connections> internalProcessor;
	};
}