#pragma once 
#include "component.h"
#include "inheritance/mix.h"

#include "expressions/library.include.h"
#include "stuff/dependencyGraph.h"

#include "expressions/instanceExpression.h"
#include "expressions/componentRefConverter.h"
#include "expressions/constantProperty.h"

namespace ObjectParser
{
	// ClassDesc
	class ClassDesc : public Base::NonCopyable
	{
	public:
		//���������� ������� ������
		Expressions::Expression* addProperty(const std::string& name, Expressions::Expression* propert);
		void addConstant(const std::string& name, Expressions::Expression* expr);

		//����������
		Component* addComponent(Component* component);
		Component* addComponent(Component* component, const PropertyAssignmentList& attributes);
		
		//������������ ������
		void addMixInheritance(const std::string& type, const PropertyAssignmentList& params);

		// �������� �������
		void addInstance(InstanceDefinitionExpression* instance);
		void addPrototype(InstanceDefinitionExpression* prototype);

		void addCallable(Expressions::Callable* callable);
		void resolveCallables();

		// ������ ����������
		const Expressions::Properties<Expressions::Expression>& properties() const { return _properties; }

		std::string typeName;

		std::vector<std::unique_ptr<MixInheritance>> mixInheritanceList;

		bool isInterface = false;

	private:
		Expressions::Properties<Expressions::Expression> _properties;
		std::vector<Expressions::Callable*> callables;
	};

	struct ClassTable : public std::unordered_map<std::string, std::unique_ptr<ClassDesc>>
	{
		ClassDesc* get(const std::string& type) const;
	};
}//