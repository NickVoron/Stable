#pragma once
#include "expressions/library.include.h"
#include "../propertyAssignment.h"

// mix - означает что все содержимое класса надо вмешать в наследуемый, и компоненты и проперти и они станут свойствами наследника к ним можно будет получать жестки ссылки через link 
// есть способ перезадать значения по умолчанию пропертей базового класса color = Color(1,1,1) перезадовать можно только константами( функция то же константа )
// нельзя никак задавать значения по умолчанию которые будут выводится из пропертей наследника, они там не доступны
//
//class Object : mix DebugObserver(color = Color(1, 1, 1))	 // в скобках определяется новые значения по умолчанию для проперти родителя
//{
//
//}
//
//class Object : mix DebugObserver(color = simpleColor) // ошибка, использование пропети класса(simpleColor) в качестве значения проперти родителя
//{														// в момент переорпредения значений по умолчанию для пропертей родителя ещё не существуют проперти потомка
//	property simpleColor = Color(1, 1, 1)
//}

namespace ObjectParser
{
	struct MixInheritance : public Expressions::Expression
	{
		MixInheritance(){}
		MixInheritance(const std::string& t):type(t){}

		virtual std::string string() const;
		virtual Expressions::EvaluationUnit* evaluated(Expressions::EvaluatedScope& environment) const;


		
		std::string type;
		PropertyAssignmentList params; // параметры инициализации родительского класса
	};

	typedef std::map<std::string, MixInheritance*> ParentsList;
}//