// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "021.staticTypization.h"

#include "utils.h"
#include "utils.h"


#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;


	StaticTypization::StaticTypization()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/021.staticTypization.desc");

		ObjectParser::Compiler comp(path.c_str());
		Expressions::EvaluatedScope worldScopename = unroll(comp.result.classes(), "Main", "main");

		const Expression* utilizerObjectRef = get(worldScopename, "main.objectUtilizer.utilizer.references[0]");
		const Expression* blueObjectRef = get(worldScopename, "main.blue.ref");

		ENFORCE_MSG(utilizerObjectRef == blueObjectRef, "");

		const Expression* utilizerComponentRef = get(worldScopename, "main.objectUtilizer.utilizer.references[1]");
		const Expression* blueComponentRef = get(worldScopename, "main.blueRef");

		ENFORCE_MSG(utilizerComponentRef == blueComponentRef, "");
	}
}
#endif



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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