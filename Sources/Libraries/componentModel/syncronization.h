// Copyright (C) 2015-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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