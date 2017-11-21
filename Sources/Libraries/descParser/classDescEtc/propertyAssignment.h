#pragma once
#include <list>

#include "stuff/library.include.h"
#include "expressions/library.include.h"

namespace ObjectParser
{

	// описывает заполнение свойст родительского класса или объекта или компонента значениями
	//class Main
	//{
	//	property value = 1
	//	Object obj
	//	{
	//		position = Vector3(0,0,0)	//PropertyAssignment
	//	}
	//
	//	component SimpleComponent
	//	{
	//		param = value				//PropertyAssignment
	//	}
	//};

	struct PropertyAssignment: public Base::NonCopyable
	{
	public:
		PropertyAssignment(const std::string& propertyName_,const Expressions::Expression* value_);
		std::string propertyName;
		const Expressions::Expression* value = nullptr;
	};

	struct PropertyAssignmentList : public std::vector<std::unique_ptr<const PropertyAssignment>>
	{
		PropertyAssignmentList() {}
		PropertyAssignmentList(const PropertyAssignmentList& params) { operator=(params); }

		PropertyAssignmentList& operator=(const PropertyAssignmentList& params) 
		{
			for (auto& p : params)
			{
				emplace_back(new PropertyAssignment(p->propertyName, p->value));
			}

			return *this;
		}

		bool exist(const std::string& name) const;
	};
}