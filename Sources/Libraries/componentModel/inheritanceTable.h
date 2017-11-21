#pragma once

#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "Loki/library.include.h"

//
//
//
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