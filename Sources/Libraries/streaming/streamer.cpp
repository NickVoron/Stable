// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "streamer.h"

#include <thread>
#include <mutex>
#include <atomic>

#include "stuff/enforce.h"
#include "stuff/fileUtils.h"

#include "containers/intrusiveList.h"
#include "common/waitable_queue.h"
#include "common/exception_safety.h"
#include "common/thread_utils.h"

namespace Streaming
{

struct StreamerThreads 
{
	StreamerThreads(unsigned int count)
	{
		for (unsigned int index = 0; index < count; ++index)
		{
			launch_named_thread("StreamerThread", [this, index]()
			{
				std::unique_lock<std::mutex> lock(mtxs[index]);
								   				
				try_execute([index, this]
				{
					std::unique_ptr<LoaderBase> loader;
					while (queries.receive(loader))
					{
						try_execute([&loader, index]
						{
							if (loader)
							{
								
								try
								{
									loader->load();
									
								}
								catch (std::exception& e)
								{
									LOG_ERROR(index << "> " << "error while stream: \t" << loader->desc() << " exception: " << e);
									loader->remove();
								}
							}
						});
					}
				});
			}).detach();
		}
	}

	~StreamerThreads()
	{
		queries.clear(true);
		for (auto& mtx : mtxs)
		{
			std::unique_lock<std::mutex> lock(mtx);
		}
	}

	std::mutex mtxs[16];
	FilesQueue queries;
};

std::unique_ptr<StreamerThreads> threads;

void Streamer::init()
{
	threads = std::make_unique<StreamerThreads>(12);
}

void Streamer::release()
{
	threads = nullptr;
}

bool Streamer::active()
{
	return threads ? !threads->queries.empty() : false;
}

void Streamer::add(LoaderBase* loader)
{
	loader->load();
	return;

	if (threads)
	{
		threads->queries.emplace(loader);
	}
}

void Streamer::commit()
{
	
}

}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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