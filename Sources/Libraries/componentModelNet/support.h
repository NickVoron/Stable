#pragma once

#include "common/waitable_queue.h"

#include "componentModel/library.include.h"
#include "networkApplication/library.include.h"

namespace network
{
	enum COMPONENT_MODEL_EVENT
	{
		COMPONENT_MODEL_EVENT_START,
		COMPONENT_MODEL_EVENT_DATA_EXCHANGE,
		COMPONENT_MODEL_EVENT_WORK,
		COMPONENT_MODEL_EVENT_STOP_WORK,
	};

	template<> inline COMPONENT_MODEL_EVENT initial<COMPONENT_MODEL_EVENT>() { return COMPONENT_MODEL_EVENT_START; }
}

namespace ComponentModel
{
	class SyncronizationHelper
	{
	public:
		SyncronizationHelper();
		SyncronizationHelper(const char* fileName, const char* category, bool createObjects);

		void load(const char* fileName, const char* category, bool createObjects);
		void execute();
		void syncronizeIn(stream::istream& is);
		void syncronizeOut(stream::ostream& os);
		void save(stream::ostream& os) const;
		void load(stream::istream& is);

	private:
		std::mutex mtx;
		EntitiesList entities;
		EntitiesSyncronizer syncronizer;		
	};

	struct World
	{
		SyncronizationHelper world;
	};

	class WorldMaster : public World
	{
	public:
		void load(const char* fileName);
		void syncronize(stream::ostream& os);
		
		udtnet::IOStream clientData;
	};

	class WorldSlave : public World
	{
	public:
		void load(stream::istream& is);
		void syncronize(stream::istream& is);
	};

}