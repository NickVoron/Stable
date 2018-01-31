// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "classesLib.h"
#include "class.h"

ClassesLib::~ClassesLib()
{
	clear();
}

void ClassesLib::create(const ClassDescList& importedClasses, ExecutionList& executionList)
{
	clear();
	
	for (std::size_t i = 0; i < importedClasses.size(); ++i)
	{
		classes.emplace_back(new Class());
		classes.back()->init(*importedClasses[i]);
	}

	finalize(executionList);
}

void ClassesLib::finalize(ExecutionList& executionList)
{
	filter.clear();
	filter.resize(ComponentsFactory::classesCount(), -1);

	for (auto& cls : classes)
	{
		for (std::size_t cidx = 0; cidx < cls->getComponentsCount(); ++cidx)
		{
			auto componentClassIndex = cls->getComponentClassIndex(cidx);
			auto& filterval = filter[componentClassIndex];

			if (filterval < 0 && ComponentsFactory::componentClassID(componentClassIndex))
			{
				filterval = componentClassIndex;
			}
		}
	}

	executionList.create(filter);

	for (std::size_t i = 0; i < classes.size(); ++i)
	{
		Class& cls = *classes[i];

		cls.classes = this;
		cls.index = i;

		cls.executionUnits.resize(cls.getComponentsCount(), 0);

		if (!executionList.empty())
		{
			for (std::size_t j = 0; j < cls.executionUnits.size(); ++j)
			{
				auto componentClassIdx = cls.getComponentClassIndex(j);
				cls.executionUnits[j] = executionList[componentClassIdx].get();
			}			
		}		
	}
}


const Class& ClassesLib::find(const std::string& className) const
{
	for(auto& cls : classes)
	{
		if(cls->name() == className)
			return *cls;
	}

	CM_KERNEL_THROW("class: " + className + " not registred");
}


void ClassesLib::save(stream::ostream& os) const
{
	os << (std::vector<int>&)filter;
	os << classes;
	prototypes.save(os);
}

void ClassesLib::load(stream::istream& is, ExecutionList& executionList)
{
	is >> (std::vector<int>&)filter;
	executionList.create(filter);
	is >> classes;
	prototypes.load(is);
	finalize(executionList);
}

void ClassesLib::merge(ClassesLib& other)
{
	ENFORCE(this != &other);

	for (auto& src : other.classes)
	{
		classes.emplace_back(src.release());
	}

	for (std::size_t idx = 0; idx < classes.size(); ++idx)
	{
		classes[idx]->index = idx;
	}
}

void ClassesLib::clear()
{
	filter.clear();
	prototypes.clear();
	mapping.clear();
	classes.clear();
}

void ClassesLib::debugOutput() const
{
	LOG_INFO("=============begin================");
	LOG_INFO("classes library size: " << classes.size() << " classes:");
	for (auto& cls : classes)
	{
		LOG_INFO("\t" << cls->debugstr());
	}
	LOG_INFO("");
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