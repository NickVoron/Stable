#pragma once

#include "valueInterface.h"

#include "runtimeFunctions/library.include.h"

namespace ValueInterfaceComponents
{
	class ValueOperation : public SimpleComponentAutoLink<ValueOperation>
	{
	public:
		CM_DEFINE_BASE_CLSID(ValueOperation);

		struct Resource : public ResourceBase
		{
			void properties(ValueOperation& component);
		};

		virtual ComponentLink<ValueOperation>& invoke();
		
	protected:
		ComponentLinkList<ValueOperation> input;
		ComponentLink<ValueOperation> output;
	};

	//
	//
	//
	class OpSource : public ValueOperation
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(OpSource);

		struct Resource : public ResourceBase
		{
			void properties(OpSource& component);
		};

		RuntimeFunctions::Store operation;

	private:
		ValueAddress value;
	};

	//
	//
	//
	class OpTarget : public ValueOperation
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(OpTarget);

		struct Resource : public ResourceBase
		{
			void properties(OpTarget& component);
		};

		RuntimeFunctions::Store operation;

	private:
		ValueAddress value;
	};

	//
	//
	//
	class OpAdd : public ValueOperation
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(OpAdd);

		RuntimeFunctions::Add operation;
	};

	//
	//
	//
	class OperationsTick : public UpdatableComponentAutoLink<OperationsTick>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(OperationsTick);

		struct Resource : public ResourceBase
		{
			void properties(OperationsTick& component);
		};

		void update();

	protected:
		ComponentLinkList<ValueOperation> operations;
	};
}