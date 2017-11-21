#include "propertyAssignment.h"
#include "expressions/library.include.h"


namespace ObjectParser
{

	PropertyAssignment::PropertyAssignment(const std::string& propertyName_, const Expressions::Expression* value_):
		propertyName(propertyName_),
		value(value_)
	{


	}

	bool PropertyAssignmentList::exist(const std::string& name) const
	{
		auto it = find_if(begin(), end(), [&name](auto& element)
		{
			return element->propertyName == name;
		});

		return it != end();
	}
}