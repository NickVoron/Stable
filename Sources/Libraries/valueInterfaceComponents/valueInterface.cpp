#include "valueInterfaceComponents.h"

namespace ValueInterfaceComponents
{
	void ValueInterface::Resource::properties(ValueInterface& component)
	{
		property("source", component.source);
	}

	void ValueInterface::linker()
	{
		link(parent);
	}

	void* ValueInterface::valueptr()
	{
		if(source.componentIndex)
		{
			auto& component = parent->getComponent(source.componentIndex.value());
			return (void*) ((char*) &component + source.componentShift);
		}
		
		return nullptr;
	}

	void ValueInterfaceMonitor::update()
	{
		auto pos = (Vector3*) component<ValueInterface>().valueptr();
		pos->z = sin(pos->x);
		//LOG_EXPRESSION(*pos);
	}
}