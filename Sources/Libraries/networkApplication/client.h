#pragma once

#include "componentModel/library.include.h"
#include "udtnet/library.include.h"

namespace network
{
	template<class BaseState, class TransitionsList, class InitialState>
	class Client : public InternalProcessor
	{
	public:					
		struct ProcessFSM : public ProcessFSMT<ProcessFSM, BaseState, TransitionsList, InitialState, DefaultAdapter> {};
		struct DataDispatcher : public DataDispatcherT<ProcessFSM, typename BaseState::RecvEvent> {};

		bool start(const udtnet::Address& addr, std::function<bool(DataBuffer&)> processor)
		{
			server.bindDispatcher(&dispatcher);
			dispatcher.fsm.connection = &server;

			bool connected = server.connect(addr);
			if (connected)
			{
				startProcessing(processor);
				startTranslation();
			}

			return connected;
		}

		typename ProcessFSM::ContentType& content = dispatcher.fsm.additionalData;

	private:
		virtual void internalMessage(udtnet::IOStream& is) { server.internalMessage(is); }
		udtnet::Connection server;
		DataDispatcher dispatcher;
	};
}