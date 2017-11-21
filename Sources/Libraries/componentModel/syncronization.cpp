#include "syncronization.h"

//
//
//
void ENTITY_ADD_MSG::save(stream::ostream& os)
{
	os << ENTITY_ADD;
	
	os << entity << entity->getClass().getIndex();
//	LOG_MSG("send add: " << (int) entity << " pointer: " << entity->getClass().getIndex());
//	entity->syncronize(os);
	
}

void ENTITY_ADD_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	int classIndex;
	Entity* uid;
	is >> uid >> classIndex;
//	LOG_MSG("recv add: " << uid << " classIndex: " << classIndex);

	Entity* entity = entities.create(classIndex);
//	entity->syncronize(is);
	entity->activate(true);
	ENFORCE(syncronizer.syncronizedUIDS[uid] == 0);
	syncronizer.syncronizedUIDS[uid] = entity;
}


void ENTITY_DEL_MSG::save(stream::ostream& os) 
{
	os << ENTITY_DEL;
	os << entity;
//	LOG_MSG("send del: " << (int) entity);
}

void ENTITY_DEL_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	Entity* uid; is >> uid;
//	LOG_MSG("recv del: " << uid);
	Entity* entity = syncronizer.syncronizedUIDS[uid];
	syncronizer.syncronizedUIDS.erase(uid);
	ENFORCE(entity);
	entity->destroy();
}

void ENTITY_UPDATE_MSG::save(stream::ostream& os)	
{
	os << ENTITY_UPDATE;

	os << entity;
//	LOG_MSG("send update: " << (int)entity);
//	entity->syncronize(os); 
}

void ENTITY_UPDATE_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	Entity* uid; is >> uid;	
//	LOG_MSG("recv update: " << uid);

	Entity* entity = syncronizer.syncronizedUIDS[uid];
	ENFORCE(entity);
//	entity->syncronize(is);
}

//
//
//
EntitiesSyncronizer::EntitiesSyncronizer(EntitiesList& ents) : entities(ents) {}

void EntitiesSyncronizer::syncronize(stream::ostream& os)
{
	std::vector<ENTITY_ADD_MSG>	add;
	std::vector<ENTITY_DEL_MSG>	del;
	std::vector<ENTITY_UPDATE_MSG> update;

	EntityUIDS presentUIDS;

	for (auto& entity : entities)
	{
		Entity* uid = &entity;
		presentUIDS[uid] = uid;

		if (syncronizedUIDS.find(uid) == syncronizedUIDS.end())
		{
			add.push_back(ENTITY_ADD_MSG(uid));
		}

 		update.push_back(ENTITY_UPDATE_MSG(uid));
	}

	for (auto& val : syncronizedUIDS)
	{
		if (presentUIDS.find(val.first) == presentUIDS.end())
		{
			del.push_back(ENTITY_DEL_MSG(val.second));
		}
	}
		
	syncronizedUIDS = presentUIDS;

	for (auto& msg : add) { msg.save(os); }
  	for (auto& msg : del) { msg.save(os); }
  	for (auto& msg : update) { msg.save(os); }
}

void EntitiesSyncronizer::syncronize(stream::istream& is)
{
 	while (is.good())
 	{
 		SYNC_MESSAGE_TYPE msgt;
 		is >> msgt;
 		switch (msgt)
 		{
 		case ENTITY_UPDATE: { ENTITY_UPDATE_MSG msg; msg.load(is, *this, entities); } break;
 		case ENTITY_ADD: { ENTITY_ADD_MSG msg; msg.load(is, *this, entities); } break;
 		case ENTITY_DEL: { ENTITY_DEL_MSG msg; msg.load(is, *this, entities); } break;
 		default: ENFORCE(0); break;
 		}
 	}		
}
