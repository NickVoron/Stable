#include "resourceDriver.h"
#include "factory.h"
#include "class.h"
															  
void ComponentResource::init(const ComponentBase& component)
{
	table = ComponentsFactory::resourceTable(component);
}

bool ComponentResource::empty() const
{
	return !table;
}

void ComponentResource::dump()
{
	if(!empty())
	{
		//table->dump(*resource.get());
	}
	else
	{
		LOG_WARNING("ClassResource::dump: try to dump empty Resource");
	}
}

void ClassResources::create(const Class& cls)
{
	parent = &cls;
	auto cc = cls.getComponentsCount();
	if(cc > 0)
	{
		Entity entity;
		cls.createInstance(entity);

		init(cc); 
		zero();

		for (std::size_t i = 0; i < cc; ++i)
		{
			data[i].init(entity.getComponent(i));
		}
	}
}

// void ClassResources::flush(EntityData& entity)
// {
// 	CM_KERNEL_ENFORCE(entity.cls == parent);
// 
// 	int cc = entity.getComponentsCount();
// 	for (int i = 0; i < cc; ++i)
// 	{
// //		(*this)[i].flush(entity.getComponent(i));
// 	}
// }

void ClassResources::dump() const
{
	LOG_ERROR(parent->name() << ":")
		for (std::size_t i = 0; i < size(); ++i)
		{
			if(!data[i].empty())
			{
				LOG_ERROR(logs::tabs(1) << ComponentsFactory::className( parent->getComponentClassIndex(i) ) << "(" << parent->getComponentName(i) << ")" );
				data[i].dump();
			}
		}
	LOG_ERROR(logs::repeat('-', 20));
}

//
//
//
void edit(ComponentResource& componentResource)
{
	if(!componentResource.empty())
	{
//		componentResource.table->edit(*componentResource.resource.get());
	}
	else
	{
		LOG_WARNING("ClassResource::dump: try to edit empty Resource");
	}
}

void edit(ClassResources& classResources)
{
	for (std::size_t i = 0; i < classResources.size(); ++i)
	{
		edit(classResources[i]);
	}
}

