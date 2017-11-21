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
								//LOG_MSG(index << "> " << "start: \t" << loader->desc());
								try
								{
									loader->load();
									//LOG_MSG(index << "> " << "end: \t" << loader->desc());
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
	{
		loader->load();
		delete loader;
		return;
	}

	if (threads)
	{
		threads->queries.emplace(loader);
	}
}

void Streamer::commit()
{
	//threads().commit();
}

}
