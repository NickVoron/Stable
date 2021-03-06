// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "007.complexData.h"

#ifdef ENABLE_TEST

namespace ComponentModelTesting
{

	using namespace Expressions;
	using namespace ObjectParser;

	ComplexData::ComplexData()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/007.complexData.desc");

		
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 1);

		auto worldScopename = unroll(comp.result.classes(), "Main", "main");
				
		for(int i=0; i < 3; ++i)
		{
			std::string indexPath = str::stringize("main.complexData[", i, "].index");
			test(worldScopename, indexPath, i);
		}

		test(worldScopename, "main.complexData[0].text", std::string("first"));
		test(worldScopename, "main.complexData[1].text", std::string("second"));
		test(worldScopename, "main.complexData[2].text", std::string("third"));
	}
}

#endif



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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