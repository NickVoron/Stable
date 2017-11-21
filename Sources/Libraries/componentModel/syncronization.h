#pragma once

#include "entityList.h"


enum SYNC_MESSAGE_TYPE
{
	ENTITY_ADD,
	ENTITY_DEL,
	ENTITY_UPDATE,
};

struct EntitiesSyncronizer;

struct ENTITY_MSG_BASE 
{
	ENTITY_MSG_BASE() : entity(0), syncronizer(0) {}
	ENTITY_MSG_BASE(Entity* e) : entity(e), syncronizer(0) {}

	Entity* entity;
	EntitiesSyncronizer* syncronizer;
};


struct ENTITY_ADD_MSG : public ENTITY_MSG_BASE
{
	ENTITY_ADD_MSG() {}
	ENTITY_ADD_MSG(Entity* e) : ENTITY_MSG_BASE(e) {}

	void save(stream::ostream& os);
	void load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities);
};

struct ENTITY_DEL_MSG : public ENTITY_MSG_BASE
{
	ENTITY_DEL_MSG() {}
	ENTITY_DEL_MSG(Entity* e) : ENTITY_MSG_BASE(e) {}

	void save(stream::ostream& os);
	void load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities);
};

struct ENTITY_UPDATE_MSG : public ENTITY_MSG_BASE
{
	ENTITY_UPDATE_MSG() {}
	ENTITY_UPDATE_MSG(Entity* e) : ENTITY_MSG_BASE(e) {}

	void save(stream::ostream& os);
	void load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities);
};

struct EntityUIDS : public std::map<Entity*, Entity*>
{

};


struct EntitiesSyncronizer
{
	EntitiesSyncronizer(EntitiesList& ents);

	void syncronize(stream::ostream& os);
	void syncronize(stream::istream& is);

	EntityUIDS syncronizedUIDS;
	EntitiesList& entities;
};