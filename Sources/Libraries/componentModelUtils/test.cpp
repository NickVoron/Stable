// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "test.h"
#include "io.h"



namespace ComponentsTest
{
	void test(EntitiesList& content, int iterationsCount)
	{
		try
		{
			for (auto& cls : content.classes.classes)
			{
				
			};

			content.activate(true);
			content.activate(false);
			content.activate(true);

			for (int i = 0; i < iterationsCount; ++i)
			{
				content.execute();
			};

			LOG_MSG("aftercycle");
			content.execute();
			LOG_MSG("completed");
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

	void test(const char* fileName, int iterationsCount)
	{
		LOG_MSG("Test: " << fileName << " iterations count = " << iterationsCount);

		try
		{
			EntitiesList content;
			ComponentsIO::load_script(fileName, content, "", true, false);

			test(content, iterationsCount);
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

	void test(const char* fileName)
	{
		test(fileName, 0);
	}
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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