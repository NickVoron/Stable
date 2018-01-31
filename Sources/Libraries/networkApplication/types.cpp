// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "types.h"

#include "DefaultLogs/library.include.h"

namespace network
{
	void InternalProcessor::startProcessing(std::function<bool (DataBuffer&)> processor)
	{
		processing = true;
		finished = false;

		std::thread userThread([this, processor]
		{
			while (!finished)
			{
				if (DataBuffer* buffer = sender())
				{
					if (processor(*buffer))
					{
						usedbuf.send(buffer);
					}
				}
				else
				{
					break;
				}
			}
			processing = false;
			LOG_MSG("SERVER EXIT USER THREAD")
		});
		userThread.detach();
	}

	void InternalProcessor::startTranslation()
	{
		transtation = true;
		finished = false;

		std::thread ct1([this]
		{
			DataBuffer* buffer = 0;
			while (!finished && receive(buffer))
			{
				if (!buffer->empty())
				{
					internalMessage(buffer->ios);
				}

				unlock(buffer);
			}

			transtation = false;
			LOG_MSG("STOP PROCESS LOCAL MSG")
		});
		ct1.detach();
	}

	InternalProcessor::~InternalProcessor()
	{
		cancel();
	}

	DataBuffer* InternalProcessor::sender()
	{
		DataBuffer* buffer = 0;
		if (buffers.freequeue.receive(buffer))
		{
			buffer->ios.reset();
			return buffer;
		}

		return 0;
	}

	bool InternalProcessor::receive(DataBuffer*& buffer)
	{
		return usedbuf.receive(buffer);
	}

	void InternalProcessor::unlock(DataBuffer* buffer)
	{
		buffer->ios.reset();
		buffers.freequeue.send(buffer);
	}

	void InternalProcessor::cancel()
	{
		finished = true;
		while (processing || transtation)
		{
			buffers.freequeue.cancel();
			usedbuf.cancel();
		}
	}


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