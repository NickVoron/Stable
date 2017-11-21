#include "factory.h"


//
//
//
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

//
//
//
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
//			unit->reset_start();
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



//
//
//
//
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
	//ComponentBase::CreationFunc creator = (classIndex >= 0 && classIndex < classesCount()) ? data.data[classIndex].creator : NullComponent::createInBuffer;
	CM_KERNEL_ENFORCE(classIndex < classesCount());
	CreationFunc creator = data.data[classIndex].creator;
	ComponentBase* res = creator(memoryBlock);
	CM_KERNEL_ENFORCE(res);
	return res;
}

void ComponentsFactory::invokeConstructor(std::size_t classIndex, void* addr)
{
	//ComponentBase::ConstructorInvoker constructorInvoker = (classIndex >= 0 && classIndex < classesCount()) ? data.data[classIndex].constructorInvoker : NullComponent::invokeConstructor;
	CM_KERNEL_ENFORCE((classIndex >= 0 && classIndex < classesCount()));
	ConstructorInvoker constructorInvoker = data.data[classIndex].constructorInvoker;
	constructorInvoker(addr);
}

void ComponentsFactory::createExecutionList(ExecutionList& result, const ExecutionListFilter& filter)
{
// 	std::vector< DependencyGraph<ExecutionUnitCreator>::Node* > creators;
// 	data.dependencies.linearize(creators);

	for (auto pi : filter)
	{
		result.emplace_back((pi >= 0) ? data.data[pi].executionUnitCreator() : nullptr);
	}		 	

	result.debug();
}