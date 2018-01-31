// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "Loki/library.include.h"




typedef const char* (*ClassNameFunc)();

struct InheritanceTable : public std::unordered_map<ClassNameFunc, std::unordered_set<ClassNameFunc> >
{
	void dump();
	void insert(ClassNameFunc parent, ClassNameFunc child);
	bool valid(const std::string& parent, const std::string& candidate);
};

template<class CL, class CList, class ComponentClass> 
struct InheritanceDetectorSecondImpl
{
	static void detect(InheritanceTable& inheritanceTable)
	{
		if (std::is_base_of<typename CList::Head, ComponentClass>::value && !std::is_abstract<ComponentClass>::value)
		{
			inheritanceTable.insert(&CList::Head::ClassName, &ComponentClass::ClassName);
		}

		InheritanceDetectorSecondImpl<CL, typename CList::Tail, ComponentClass>::detect(inheritanceTable);
	}
};

template<class CL, class ComponentClass> 
struct InheritanceDetectorSecondImpl<CL, Loki::NullType, ComponentClass> { static void detect(InheritanceTable& inheritanceTable) {} };

template<class CL, class CList> struct InheritanceDetectorFirstImpl
{
	static void detect(InheritanceTable& inheritanceTable)
	{
		InheritanceDetectorSecondImpl<CL, CList, typename CList::Head>::detect(inheritanceTable);
		InheritanceDetectorFirstImpl<CL, typename CList::Tail>::detect(inheritanceTable);
	}
};

template<class CL> struct InheritanceDetectorFirstImpl<CL, Loki::NullType> { static void detect(InheritanceTable& inheritanceTable) {} };

template<class ComponentsList>
struct InheritanceDetector
{
	template<class CL, class ComponentClass>
	struct Second
	{
		static void detect(InheritanceTable& inheritanceTable) { InheritanceDetectorSecondImpl<CL, CL, ComponentClass>::detect(inheritanceTable); }
	};

	template<class CL, class CL0>
	struct First
	{
		static void detect(InheritanceTable& inheritanceTable) { InheritanceDetectorFirstImpl<CL, CL0>::detect(inheritanceTable); }
	};

	static void detect(InheritanceTable& inheritanceTable) { First<ComponentsList, ComponentsList>::detect(inheritanceTable); }
};



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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