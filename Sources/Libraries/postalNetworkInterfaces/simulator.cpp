#include "simulator.h"
#include "common/waitable_queue.h"
#include "defaultLogs/library.include.h"

namespace postal
{
	mt::waitable_queue<ConnectionSimulator*> localConnections;
	

	//
	//
	//
	ConnectionSimulator::~ConnectionSimulator()
	{
		static std::mutex mtx;
		localConnections.replace(this, nullptr);
	
		std::unique_lock<std::mutex> lock(mtx);
		if (correspond)
		{			
			std::unique_lock<std::mutex> lock(correspond->monitor);
			correspond->correspond = nullptr;						
			correspond = nullptr;
		}		
	}

	bool ConnectionSimulator::connect(const NetworkAddress& addr)
	{
		correspond = new ConnectionSimulator();

		name = "client-side";
		correspond->name = "server-side";
		correspond->correspond = this;
		localConnections.send(correspond);
		return true;
	}
	
	std::size_t ConnectionSimulator::sendmsg(const char* msgdata, std::size_t msgsize) const
	{		
		std::unique_lock<std::mutex> lock(monitor);
		if (correspond)
		{
			std::unique_lock<std::mutex> lock(correspond->monitor);
			correspond->messages.emplace_back(new stream::dmemstream());
			correspond->messages.back()->write(msgdata, msgsize);
			return msgsize;
 		}

		return 0;
	}

	std::size_t ConnectionSimulator::recvmsg(char* msgdata, std::size_t msgsize) const
	{
		std::unique_lock<std::mutex> lock(monitor);

		if (!messages.empty())
		{
			auto& message = messages.front();
			auto msz = message->size();
			ENFORCE(msgsize >= msz);

			message->read(msgdata, msz);
			messages.pop_front();

			return msz;
 		}

		return 0;
	}


	//
	//
	//
	std::atomic_uint gindex;
	ConnectionsSimulator::ConnectionsSimulator()
	{
		index = gindex++;
		localConnections.restart();
	}

	bool ConnectionsSimulator::start(unsigned short port)
	{
		return true;
	}

	void ConnectionsSimulator::stop(std::thread& joinable)
	{
		localConnections.cancel();
		if (joinable.joinable())
		{
			joinable.join();
		}
	}

	ConnectionSimulator* ConnectionsSimulator::incoming()
	{
		ConnectionSimulator* res = nullptr;
		localConnections.receive(res);

		return res;
	}
}