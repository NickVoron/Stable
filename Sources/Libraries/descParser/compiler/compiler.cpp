// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "compiler.h"

#include "expressions/library.include.h"
#include "reflection/library.include.h"

#include "../parser/lexer.h"
#include "../classDescEtc/classDesc.h"
#include "../classDescEtc/expressions/objectConverter.h"
#include "../classDescEtc/expressions/componentRefConverter.h"


bool convertLink(Expressions::EvaluationUnit& expr, mirror::Type<LinkDesc>& client) { return Expressions::convert(expr, client.val()); }
bool convertLinkList(Expressions::EvaluationUnit& expr, mirror::Type<LinksDescList>& client) { return Expressions::convert(expr, client.val()); }
bool convertValueAddress(Expressions::EvaluationUnit& expr, mirror::Type<ValueAddress>& client) { return Expressions::convert(expr, client.val()); }
bool convertPrototype(Expressions::EvaluationUnit& expr, mirror::Type<EntitiesStream>& client) { return Expressions::convert(expr, client.val()); }

struct ComponentModelInitializer
{
	ComponentModelInitializer()
	{	
		Expressions::FUNCTIONS::init();

		MIRROR_TYPE_DECLARE(LinkDesc);
		MIRROR_TYPE_DECLARE(LinksDescList);
		MIRROR_TYPE_DECLARE(ValueAddress);
		MIRROR_TYPE_DECLARE(EntitiesStream);
		MIRROR_STRUCT_DECLARE(Vector2, x, y);
		MIRROR_STRUCT_DECLARE(Vector3, x, y, z);
		MIRROR_STRUCT_DECLARE(Vector4, x, y, z, w);

		Expressions::multimethodsConversionTable.add(&convertLink);
		Expressions::multimethodsConversionTable.add(&convertLinkList);
		Expressions::multimethodsConversionTable.add(&convertValueAddress);
		Expressions::multimethodsConversionTable.add(&convertPrototype);
	}
};

namespace ObjectParser
{

Compiler::Compiler() : result(*this, *this)
{
	static ComponentModelInitializer helper;
	ComponentNameSource::reset();
}

Compiler::Compiler(const char* fileName):Compiler()						 
{
	compile(boost::filesystem::path(fileName));
}

Compiler::~Compiler()
{
	Expressions::collect_garbage();
}

void Compiler::compile(const boost::filesystem::path& fileName)
{
	LOG_MSG("compile script file: " << fileName.string());
	if (boost::filesystem::exists(fileName))
	{
		loadClassDescInfo(fileName.string().c_str());
	}	
}

void Compiler::loadClassDescInfo(const boost::filesystem::path& current)
{
	includes.add(current, [this](auto path)
	{
		Lexer lexer(*this, path);
		Parser(lexer).parse();
	});
}

}



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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