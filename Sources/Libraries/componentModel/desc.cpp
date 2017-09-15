// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "class.h"
#include "property.h"
#include "factory.h"
#include "common/clearPtrContainer.h"

ComponentDesc::ComponentDesc(const std::string& className_, const std::string& name_, const std::string& category_, const ClassDesc& parent_) :
	className(className_), 
	name(name_), 
	category(category_),
	parent(parent_),
	query(ComponentsFactory::constructQuery(className_)) 
{
}


void ComponentDesc::debug() const
{
	LOG_MSG(className << " " << name);
	for (auto& link : dependencies)
	{
		LOG_MSG("\t" << link->className);		
	}
}




ClassDesc::ComponentsList::~ComponentsList()
{
	Base::clearPtrContainer(*this);
}

int ClassDesc::ComponentsList::index(const std::string& name) const
{
	int idx = 0;
	for (auto* desc : *this)
	{
		if (desc->name == name)
			return idx;

		++idx;
	}

	THROW("");

	return -1;
}

std::pair<std::size_t, bool> ComponentDesc::linkIndex(const std::string& clsId, const std::string& alias) const
{
	for (auto& link : dependencies)
	{
		if (ComponentsFactory::isInherited(clsId, link->className))
		{
			if (link->name == alias)
			{
				return std::make_pair(parent.components.index(alias), true);
			}
		}
	}

	return std::make_pair(-1, false);
}




ClassDesc::ClassDesc(const std::string& clsName, std::size_t classIdx)
:className(clsName), classIndex(classIdx)
{
}

ClassDesc::~ClassDesc()
{
	clear();
}


void ClassDesc::clear()
{
	className.clear();
	Base::clearPtrContainer(components);
}

ComponentDesc* ClassDesc::addComponent(const std::string& className, const std::string& name, const std::string& category)
{
	ENFORCE_MSG(isComponentRegistred(className), className);
	
	categories.insert(category);
	components.push_back( new ComponentDesc(className, name, category, *this));
	return components.back();
}

void ClassDesc::reorder()
{
	std::sort(components.begin(), components.end(), [](auto c0, auto c1) 
	{ 
		return std::tie(c0->className, c0->name, c0->category) < std::tie(c1->className, c1->name, c1->category);
	});

	std::list<ComponentDesc*> graph;
	for(ComponentDesc* desc : components)
	{
		unrollGraph(graph, desc);
	}
	
	std::list<ComponentDesc*> graphu;
	for(ComponentDesc* desc : graph)
	{
		if (std::find(components.begin(), components.end(), desc) != components.end())
		{
			if (std::find(graphu.begin(), graphu.end(), desc) == graphu.end())
			{
				graphu.push_back(desc);
			}
		}				
	}

	std::vector<ComponentDesc*> newOrder;
	newOrder.reserve(graphu.size());
	for(ComponentDesc* d : graphu)
	{
		newOrder.push_back(d);		
	}						   

	components.swap(newOrder);
}

void ClassDesc::unrollGraph(std::list<ComponentDesc*>& graph, ComponentDesc* cd)
{
	graph.push_front(cd);

	for(ComponentDesc* d : cd->dependencies)
	{
		unrollGraph(graph, d);
	}
}

void ClassDesc::finalize()
{
	reorder();
}

void ClassDesc::categoryClass(const std::string& categoryName, ClassDesc& desc) const
{
	desc.className = className;
	desc.localObjects = localObjects;
	
	for (ComponentDesc* comp : components)
	{
		if (comp->category == categoryName || comp->category.empty())
		{
			spliceLinks(comp, desc.addComponent(comp->className, comp->name, categoryName), desc);
	 	}		
	}
}

void ClassDesc::spliceLinks(const ComponentDesc* srcComponent, ComponentDesc* dstComponent, ClassDesc& desc) const
{
	dstComponent->dependencies = srcComponent->dependencies;

	for (auto& link : srcComponent->dependencies)
	{
		for (ComponentDesc* cd : components)
		{
			if (cd->name == link->name)
			{
				bool exist = false;
				for (ComponentDesc* comp : desc.components)
				{
					if (comp->name == cd->name)
					{
						exist = true;
						break;
					}						
				}
				
				if (!exist)
				{
					spliceLinks(cd, desc.addComponent(link->className, link->name, srcComponent->category), desc);
				}				
			}
		}
	}
}


void ClassDesc::debug() const
{
	LOG_EXPRESSION_VALUE(className);
	for (auto& desc : components)
	{
		desc->debug();
	}
}






ClassDesc& ClassDescList::add(const std::string& className)
{
	emplace_back(new ClassDesc(className, size()));
	return *back();
}


std::pair<std::size_t, bool> ClassDescList::findIndex(const std::string& className) const
{
	std::size_t count = size();
	for (std::size_t i = 0; i < count; ++i)
	{
		if (operator[](i)->className == className)
			return std::make_pair(i, true);
	}

	return std::make_pair(-1, false);
}

void ClassDescList::categorySlice(const std::string& category, ClassDescList& slice) const
{
	for (auto& desc : *this)
	{
		desc->categoryClass(category, slice.add(desc->className));
	}

	slice.finalize();
}

void ClassDescList::finalize()
{
	for (auto& desc : *this)
	{
		desc->finalize();
	}
}

void ClassDescList::debug() const
{
	for (auto& desc : *this)
	{
		desc->debug();
	}
}



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