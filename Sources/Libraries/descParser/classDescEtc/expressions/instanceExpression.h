#pragma once
#include "../propertyAssignment.h"

namespace ObjectParser
{	 
	// описывает абстрактный интерфейс к созданному системой объекту, нужен для того что бы InstanceDefinitionExpression можно было 
	// связать с данными созданными компонетной моделью и через механизм передачи Expression данные о созданном инстансе передавались 
	// дальше через параметры 

	class ClassDesc;
	struct ClassTable;

	class InstanceDefinitionExpression : public Expressions::Expression
	{
	public:
		InstanceDefinitionExpression(const std::string& type, const std::string& name, const PropertyAssignmentList& params, bool noinst);
		InstanceDefinitionExpression(const std::string& type, const std::string& name);
		
		virtual std::string string() const override;
		virtual Expressions::EvaluationUnit* evaluated(Expressions::EvaluatedScope& namescope) const override;
				
		InstanceDefinitionExpression* instance() const;

		const std::string name;
		const std::string type;
		PropertyAssignmentList params;
		const bool noinstance = false;
		Expressions::Expression* arrayData = nullptr;//данные по которым будет разворачиваться массив инстансов, если инстанс один(не массив), то это приравнивается к массиву из одного элемента
	};
}//