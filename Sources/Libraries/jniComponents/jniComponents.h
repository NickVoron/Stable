#pragma once

#include "componentModel/library.include.h"

namespace JniComponents
{


	class JavaStaticMethodCaller : public UpdatableComponent<JavaStaticMethodCaller>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(JavaStaticMethodCaller);

		struct Resource : public ResourceBase
		{
			void properties(JavaStaticMethodCaller& component);
		};

		void update();
		void call();
	
	private:
		std::string classPath;
		std::string functionName;
		std::string functionSignature;
	};

	typedef ComponentModel::Module<JavaStaticMethodCaller> Module;
}