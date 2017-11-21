#pragma once
#include "expressions/library.include.h"
#include "../propertyAssignment.h"

// mix - �������� ��� ��� ���������� ������ ���� ������� � �����������, � ���������� � �������� � ��� ������ ���������� ���������� � ��� ����� ����� �������� ������ ������ ����� link 
// ���� ������ ���������� �������� �� ��������� ��������� �������� ������ color = Color(1,1,1) ������������ ����� ������ �����������( ������� �� �� ��������� )
// ������ ����� �������� �������� �� ��������� ������� ����� ��������� �� ��������� ����������, ��� ��� �� ��������
//
//class Object : mix DebugObserver(color = Color(1, 1, 1))	 // � ������� ������������ ����� �������� �� ��������� ��� �������� ��������
//{
//
//}
//
//class Object : mix DebugObserver(color = simpleColor) // ������, ������������� ������� ������(simpleColor) � �������� �������� �������� ��������
//{														// � ������ �������������� �������� �� ��������� ��� ��������� �������� ��� �� ���������� �������� �������
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
		PropertyAssignmentList params; // ��������� ������������� ������������� ������
	};

	typedef std::map<std::string, MixInheritance*> ParentsList;
}//