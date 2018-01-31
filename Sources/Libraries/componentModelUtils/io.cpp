// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "io.h"



namespace ComponentsIO
{
	void save(const char* fileName, EntitiesList& entities)
	{
		stream::ofstream<> os(fileName);
		entities.save(os);
	}

	void load(const char* fileName, EntitiesList& entities)
	{
		if(Base::FileUtils::FileExists(fileName))
		{
			stream::ifstream<> is(fileName);	
			entities.load(is);
			entities.activate(true);
		}
	}

	void load_script(const std::string& source, EntitiesList& entities, const std::string& category, bool createObjects, bool activate)
	{
		INCOMPLETE;
		

		
		
		
		
	}

	Resources::Module* load_script(const std::string& source)
	{
		Resources::Module* loaded = 0;
		Resources::load(loaded, source, false);
		return loaded;
	}
}



// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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