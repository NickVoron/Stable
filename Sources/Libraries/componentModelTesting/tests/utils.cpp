#include "utils.h"
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>
#include <iostream>

#ifdef ENABLE_UTILS


namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	const InstanceDefinitionExpression* findInstance(const ObjectParser::ClassDesc* cl, const std::string& instanceName)
	{
		const InstanceDefinitionExpression* result = cl->properties().at(instanceName)->cast<InstanceDefinitionExpression>();
		return result;

	}

	const Component* findComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName)
	{
		const Component* result = cl->properties().at(componentName)->cast<Component>();
		return result;
	}

	const Component* testComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName)
	{
		const Component* component = findComponent(cl, componentName);
		ENFORCE_MSG(component, "Can't find component '" + componentName + "' in '" + cl->typeName + "' class");

		return component;
	}

	bool testType(const ObjectParser::Component* cm, const std::string& type)
	{
		bool flag = cm->componentType == type;
		ENFORCE_MSG(flag, "Component '" + cm->name + "' is no type '" + type + "'")

		return flag;
	}

	const ComponentHandle* findComponent(const InstanceHandle* inst, int index, const std::string& componentName)
	{
		auto expr = inst->scope().get(componentName);
		ENFORCE_MSG(expr, "No '" + componentName + "' expression in '" + inst->definition.name + "'");

		const ComponentHandle* compo = expr->cast<ComponentHandle>();
		ENFORCE_MSG(compo, "Epression '" + componentName + "' is no component");
		return compo;
	}


	const ComponentHandle* testComponent(const ObjectParser::InstanceHandle* inst, int index, const std::string& componentName, const std::string& componentType)
	{
		const ComponentHandle* compo = findComponent(inst, index, componentName);
		ENFORCE_MSG(compo->type == componentType, "Component '" + componentName + "' no type '" + componentType + "'");

		return compo;
	}


	bool testType(const ComponentHandle* cm, const std::string& type)
	{
		bool flag = cm->type == type;
		ENFORCE_MSG(flag, "Component '" + cm->name + "' is no type '" + type + "'")

		return flag;
	}

	bool testClassesCount(const ObjectParser::GrammarComposition& result, std::size_t count)
	{
		const ObjectParser::ClassTable& table = result.classes();
		ENFORCE_EQUAL(table.size(), count);
		return table.size() == count;
	}

	const ObjectParser::InstanceHandle* testInstance(std::unique_ptr<Expressions::EvaluatedScope>& scopeName, const std::string& instanceName)
	{
		auto unrolledMainExpr = scopeName->get("main");
		ENFORCE_MSG(unrolledMainExpr, "Main was not unroll");
		const ObjectParser::InstanceHandle* unrolledMainInstace = unrolledMainExpr->cast<const ObjectParser::InstanceHandle>();
		ENFORCE_MSG(unrolledMainInstace, "'main' expr in not ObjectParser::Instance");

		return unrolledMainInstace;
	}


	//parse
	bool isIndex(const std::string& val, size_t& index)
	{
		char* end;
		index = strtoul(val.c_str(), &end, 10);
		return end != val.c_str() && (errno != ERANGE);
	}

	Expressions::Reference* parse(const std::string& path)
	{
		Expressions::Reference* proxy = Expressions::add<Expressions::Reference>();
		
		typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
		boost::char_separator<char> sep(".[]");
		Tokenizer tokens(path, sep);
		for (auto& token : tokens)
		{
			size_t index;
			Reference::PathElement* pathElement = 0;
			if (isIndex(token, index))
			{
				auto constant = convertType(index)->cast<Const<decltype(index)>>();
				pathElement = new ArrayPathElement(ConstExprList(constant));
			}
			else
			{
				pathElement = new PropertyPathElement(token);

			}
			
			ENFORCE_MSG(pathElement, "");
			proxy->addPathElement(pathElement);

		}
		return proxy;
	}

	const Expressions::EvaluationUnit* get(Expressions::EvaluatedScope& scopename, const std::string& path)
	{
		Reference* proxy = parse(path);
		return Expressions::unrollExpression(*proxy, scopename, false);
	}

	const Expressions::EvaluationUnit* get(std::unique_ptr<Expressions::EvaluatedScope>& scopename, const std::string& path)
	{
		return get(*scopename, path);
	}


}//

#endif // ENABLE_TEST
