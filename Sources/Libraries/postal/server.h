// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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