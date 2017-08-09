// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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