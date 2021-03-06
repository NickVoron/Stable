// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.