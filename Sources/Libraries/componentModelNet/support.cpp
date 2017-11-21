#include "support.h"

#include "componentModelUtils/library.include.h"

namespace ComponentModel
{				 
	SyncronizationHelper::SyncronizationHelper() :syncronizer(entities)
	{
	}

	SyncronizationHelper::SyncronizationHelper(const char* fileName, const char* category, bool createObjects) :syncronizer(entities) 
	{ 
		load(fileName, category, createObjects); 
	}

	void SyncronizationHelper::load(const char* fileName, const char* category, bool createObjects)
	{
		std::unique_lock<std::mutex> lock(mtx);

		ComponentsIO::load_script(fileName, entities, category, createObjects, true);
		//LOG_MSG(fileName << " category: " << category << " objects count: " << entities.size());
	}

	void SyncronizationHelper::execute()
	{
		std::unique_lock<std::mutex> lock(mtx);
		entities.execute();
		//LOG_MSG("SyncronizationHelper::execute(): " << entities.size());
	}

	void SyncronizationHelper::syncronizeIn(stream::istream& is)
	{
		std::unique_lock<std::mutex> lock(mtx);
		syncronizer.syncronize(is);
	}

	void SyncronizationHelper::syncronizeOut(stream::ostream& os)
	{
		std::unique_lock<std::mutex> lock(mtx);
		syncronizer.syncronize(os);
	}

	void SyncronizationHelper::save(stream::ostream& os) const
	{
		entities.save(os);
	}

	void SyncronizationHelper::load(stream::istream& is)
	{
		std::unique_lock<std::mutex> lock(mtx);
		entities.load(is);
	}

	//
	//
	//
	void WorldMaster::load(const char* fileName)
	{
		world.load(fileName, "server", true);
		

 		EntitiesList clientEntities;
 		ComponentsIO::load_script(fileName, clientEntities, "client", false, false);
 		clientEntities.save(clientData);
	}

	void WorldMaster::syncronize(stream::ostream& os)
	{
		world.syncronizeOut(os);
	}

	//
	//
	//
	void WorldSlave::load(stream::istream& is)
	{
		world.load(is);
	}

	void WorldSlave::syncronize(stream::istream& is)
	{
		world.syncronizeIn(is);
	}
}