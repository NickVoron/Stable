#pragma once
#include "../propertyAssignment.h"

namespace ObjectParser
{	 
	// ��������� ����������� ��������� � ���������� �������� �������, ����� ��� ���� ��� �� InstanceDefinitionExpression ����� ���� 
	// ������� � ������� ���������� ����������� ������� � ����� �������� �������� Expression ������ � ��������� �������� ������������ 
	// ������ ����� ��������� 

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
		Expressions::Expression* arrayData = nullptr;//������ �� ������� ����� ��������������� ������ ���������, ���� ������� ����(�� ������), �� ��� �������������� � ������� �� ������ ��������
	};
}//