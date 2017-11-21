#pragma once

#include "componentModel/library.include.h"
#include "udtnet/library.include.h"


namespace network
{
	template<class BaseState, class TransitionsList, class InitialState>
   	class Server : public InternalProcessor
	{
	public:		
		struct ProcessFSM : public ProcessFSMT<ProcessFSM, BaseState, TransitionsList, InitialState, DefaultAdapter> {};
		struct DataDispatcher : public DataDispatcherT<ProcessFSM, typename BaseState::RecvEvent> {};

		bool start(unsigned short port, std::function<bool(DataBuffer&)> processor)
		{
			bool result = connections.server(port, [this] { return &dispatcher; });
			if (result)
			{
				startProcessing(processor); // ��������� ����� ��������� �������� ������
				startTranslation(); // ��������� ����� ��������� ������ ���������� �� ������ ������ ��������� ������
			}

			return result;
		}

		typename ProcessFSM::ContentType& content = dispatcher.fsm.additionalData;

	private:		
		virtual void internalMessage(udtnet::IOStream& is) { connections.internalMessage(is); }
		udtnet::Connections connections;
		DataDispatcher dispatcher;
	};
}