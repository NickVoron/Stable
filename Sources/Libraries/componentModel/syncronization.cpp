// Copyright (C) 2015-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "syncronization.h"




void ENTITY_ADD_MSG::save(stream::ostream& os)
{
	os << ENTITY_ADD;
	
	os << entity << entity->getClass().getIndex();


	
}

void ENTITY_ADD_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	int classIndex;
	Entity* uid;
	is >> uid >> classIndex;


	Entity* entity = entities.create(classIndex);

	entity->activate(true);
	ENFORCE(syncronizer.syncronizedUIDS[uid] == 0);
	syncronizer.syncronizedUIDS[uid] = entity;
}


void ENTITY_DEL_MSG::save(stream::ostream& os) 
{
	os << ENTITY_DEL;
	os << entity;

}

void ENTITY_DEL_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	Entity* uid; is >> uid;

	Entity* entity = syncronizer.syncronizedUIDS[uid];
	syncronizer.syncronizedUIDS.erase(uid);
	ENFORCE(entity);
	entity->destroy();
}

void ENTITY_UPDATE_MSG::save(stream::ostream& os)	
{
	os << ENTITY_UPDATE;

	os << entity;


}

void ENTITY_UPDATE_MSG::load(stream::istream& is, EntitiesSyncronizer& syncronizer, EntitiesList& entities)
{
	Entity* uid; is >> uid;	


	Entity* entity = syncronizer.syncronizedUIDS[uid];
	ENFORCE(entity);

}




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




// Copyright (C) 2015-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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