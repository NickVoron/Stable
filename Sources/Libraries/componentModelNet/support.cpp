// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
		
	}

	void SyncronizationHelper::execute()
	{
		std::unique_lock<std::mutex> lock(mtx);
		entities.execute();
		
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

	
	
	
	void WorldSlave::load(stream::istream& is)
	{
		world.load(is);
	}

	void WorldSlave::syncronize(stream::istream& is)
	{
		world.syncronizeIn(is);
	}
}



// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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