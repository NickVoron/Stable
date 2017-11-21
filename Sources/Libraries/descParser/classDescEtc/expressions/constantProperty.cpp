#include "constantProperty.h"


namespace ObjectParser
{

	ConstantProperty::ConstantProperty(const std::string& name_, const Expressions::Expression* value_) :
		name(name_),
		value(value_)
	{
	}

	void ConstantProperty::reset()
	{
		
	}

	void ConstantProperty::calculate()  const
	{


	}

	std::string ConstantProperty::string() const
	{
		return str::stringize("private property Expression: ", "name= ", name, ", target= ",value->string());

	}


	


}//