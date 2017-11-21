#pragma once

#include "componentModel/library.include.h"

namespace ValueInterfaceComponents
{
	class ValueInterface : public SimpleComponent<ValueInterface>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ValueInterface);

		struct Resource : public ResourceBase
		{
			void properties(ValueInterface& component);
		};

		void* valueptr();
		void linker();

		ValueAddress source;
		Entity* parent;
	};

	class ValueInterfaceMonitor : public UpdatableComponentAutoLink<ValueInterfaceMonitor, ValueInterface>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ValueInterfaceMonitor);

		void update();
	};
}