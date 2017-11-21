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
