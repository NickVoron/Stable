// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "factory.h"





void ExecutionListFilter::debug()
{
	for (std::size_t i = 0; i < size(); ++i)
	{
		if (operator[](i))
		{
			LOG_MSG("filter : " << ComponentsFactory::className(i));
		}		
	}
}




void ExecutionList::execute()
{
	for (auto& unit : *this)
	{
		if (unit)
		{
			unit->execute();
		}		
	}

	for (auto& unit : *this)
	{
		if (unit)
		{

		}
	}
}

void ExecutionList::create(const ExecutionListFilter& filter)
{
	destroy();
	ComponentsFactory::createExecutionList(*this, filter);
}

void ExecutionList::destroy()
{
	for(auto& unit : *this)
	{
		if (unit)
		{
			unit->release();
		}
	}

	clear();
}

void ExecutionList::debug()
{
	return;
	LOG_INFO("execution list size: " << size());
	LOG_INFO("execution order:");
	for (auto& unit : *this)
	{
		if (unit)
		{
			unit->dump();
		}		
	}
}







ComponentsFactory::Data ComponentsFactory::data;
InheritanceTable ComponentsFactory::inheritanceTable;

bool ComponentsFactory::isRegistred(const std::string& classId) 
{ 
	return classIndex(classId).second;
}

std::pair<std::size_t, bool> ComponentsFactory::classIndex(const std::string& classId)
{
	for (std::size_t i = 0; i < classesCount(); ++i)
	{
		if(data.data[i].className() == classId)
			return std::make_pair(i, true);
	}

	return std::make_pair(-1, false);
}

ComponentsFactory::ExecutionUnitCreator ComponentsFactory::componentClassID(std::size_t clsIdx)
{
	return data.data[clsIdx].executionUnitCreator;
}

const char* ComponentsFactory::className(std::size_t clsIdx)
{
	return data.data[clsIdx].className();
}

const char* ComponentsFactory::className(const ComponentBase& component)
{
	return className(component.classIndex());
}

unsigned int ComponentsFactory::sizeOf(std::size_t clsIdx)
{
	return data.data[clsIdx].sizeOf;
}

ResourceTable* ComponentsFactory::resourceTable(std::size_t clsIdx)
{
	return &data.data[clsIdx].resourceTable;
}

ResourceTable* ComponentsFactory::resourceTable(const ComponentBase& component)
{
	return resourceTable(component.classIndex());
}

void ComponentsFactory::inheriteProperties(ComponentBase& parent, ComponentBase& child)
{
	CM_KERNEL_ENFORCE(parent.classIndex() == child.classIndex());

	data.data[parent.classIndex()].inheriteParentProperties(parent, child);
}

std::size_t ComponentsFactory::classesCount()
{
	return data.data.size();
}	  

Serializer& ComponentsFactory::constructSerializer(std::size_t classIdx)
{
	return data.data[ classIdx ].serial;
}

FinalizeQuery& ComponentsFactory::constructQuery(std::size_t classIdx)
{
	return data.data[classIdx].resourceTable.query;
}


FinalizeQuery& ComponentsFactory::constructQuery(const std::string& classId)
{
	auto index = classIndex(classId);
	CM_KERNEL_ENFORCE(index.second);
	return constructQuery(index.first);
}

ComponentBase* ComponentsFactory::create(const std::string& classId, ComponentsMemoryBlock& memoryBlock)
{
	auto index = classIndex(classId);
	CM_KERNEL_ENFORCE(index.second);
	return create(index.first, memoryBlock);
}

ComponentBase* ComponentsFactory::create(std::size_t classIndex, ComponentsMemoryBlock& memoryBlock)
{
	
	CM_KERNEL_ENFORCE(classIndex < classesCount());
	CreationFunc creator = data.data[classIndex].creator;
	ComponentBase* res = creator(memoryBlock);
	CM_KERNEL_ENFORCE(res);
	return res;
}

void ComponentsFactory::invokeConstructor(std::size_t classIndex, void* addr)
{
	
	CM_KERNEL_ENFORCE((classIndex >= 0 && classIndex < classesCount()));
	ConstructorInvoker constructorInvoker = data.data[classIndex].constructorInvoker;
	constructorInvoker(addr);
}

void ComponentsFactory::createExecutionList(ExecutionList& result, const ExecutionListFilter& filter)
{



	for (auto pi : filter)
	{
		result.emplace_back((pi >= 0) ? data.data[pi].executionUnitCreator() : nullptr);
	}		 	

	result.debug();
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