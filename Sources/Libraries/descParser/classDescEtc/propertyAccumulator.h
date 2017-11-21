#pragma once
#include <vector>

#include "expressions/library.include.h"


namespace ObjectParser
{
	enum RuntimeDirection { In, Out };

	struct RuntimeProperty
	{
		std::string paramName;
		Expressions::Reference* path;
		RuntimeDirection direction;
	};

	typedef std::vector<RuntimeProperty> RuntimePropeties;

	//PropertyAccumulator
	struct PropertyAccumulator
	{
	public:
		void bindRuntimeProperty(const std::string& paramName, Expressions::Reference* path, RuntimeDirection direction);
		RuntimePropeties runtimeProperties() { return runtimeProps; }

		void clear();


	private:
		RuntimePropeties runtimeProps;
		

	};

}//