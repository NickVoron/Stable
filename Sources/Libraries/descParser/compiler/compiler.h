// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../classDescEtc/grammarComposition.h"
#include "../classDescEtc/unroller.h"
#include "../classDescEtc/debugConfigurator.h"
#include "../classDescEtc/utils/nameSource.h"

#include <boost/filesystem.hpp>
#include "expressions/library.include.h"
#include "stuff/atof.h"
#include "common/includes.h"

namespace ObjectParser
{
	
	class ExtensionTable : std::unordered_map<std::string, std::unique_ptr<Expressions::Extension>>
	{
	public:
		virtual ~ExtensionTable(){}

		void addExtension(Expressions::Extension* ext) { operator[](ext->name()).reset(ext); }
		Expressions::Extension* extension(const std::string& name){ return operator[](name).get(); }
	};

	
	class Compiler : public ExtensionTable
	{
	friend class Parser;
	public:
		Compiler();
		Compiler(const char* fileName);
		~Compiler();

		void compile(const boost::filesystem::path& fileName);
		GrammarComposition result;

	private:
		void loadClassDescInfo(const boost::filesystem::path& current);
		Includes includes;
	};
};



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