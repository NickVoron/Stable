#include "debug.h"

#include "reflection/library.include.h"
#include "componentModel/library.include.h"

namespace Expressions{
	void uneval_break(unsigned int line);
	void report_uneval(bool enabled);

namespace Functions{

	template<class... T>
	std::string get_text(const std::vector<std::tuple<T...>>& fields, bool recursive, std::size_t level)
	{
		str::stringize text;
		for (auto& field : fields)
		{
			text("\n", logs::spaces(level*2), std::get<0>(field));
			if (recursive)
			{
				text(get_text(std::get<1>(field)->fields(), true, level + 1));
			}				
		}
		return text.str();
	}

	EvaluationUnit* print(EvaluationUnit* unit)
	{
		ENFORCE_POINTER(unit);
 		//str::spaced text(unit->string(), get_text(unit->fields(), false, 1));
		str::spaced text(unit->string());
 		LOG_MSG(text.str());
		return unit;
	}

	EvaluationUnit* print_location(EvaluationUnit* unit)
	{
		ENFORCE_POINTER(unit);
		str::spaced text(unit->protoexpr.location_str());
		LOG_MSG(text.str());
		return unit;
	}

	EvaluationUnit* print_recursive(EvaluationUnit* unit)
	{
		ENFORCE_POINTER(unit);
 		//str::spaced text(unit->string(), get_text(unit->fields(), true, 1));
		str::spaced text(unit->string());
 		LOG_MSG(text.str());
		return unit;
	}

	EvaluationUnit* empty(EvaluationUnit* unit)
	{
		return unit;
	}

	std::string typeInfo(const mirror::runtime::Type& type)
	{
		str::stringize res(type.name(), "(", str::comma());
		for (auto& field : type.fields())
		{
			res(field);
		}
		res(str::nodelim(), ")");

		return res.str();
	}

	std::vector<std::string> mirror_types()
	{
		std::vector<std::string> result;
		auto& types = mirror::runtime::types();
		for (auto& type : types)
		{		
			result.push_back(type.second->name());
		}
		return result;
	}


	std::string mirror_type(std::string typeName)
	{
		auto& types = mirror::runtime::types();
		auto it = types.find(typeName);
		if (it != types.end())
		{
			return typeInfo(*types[typeName]);
		}
		else
		{
			LOG_MSG("type with name: " << typeName << " is not exists, available types is:");
		}

		return "";
	}

	std::vector<std::string> functions_list()
	{
		std::vector<std::string> result;
		for (auto& f : Expressions::FUNCTIONS::functions())
		{
			result.push_back(f.first);
		}

		return result;
	}

	std::vector<std::string> components_list()
	{
		std::vector<std::string> result;
		auto count = ComponentsFactory::classesCount();
		for (std::size_t i = 0; i < count; ++i)
		{
			result.push_back(ComponentsFactory::className(i));
		}
		return result;
	}

	EvaluationUnit* invoke(std::string name, EvaluationUnitsList params)
	{
		return Expressions::FUNCTIONS::execute(name, params);
	}

	std::map<std::string, int> test_map()
	{
		std::map<std::string, int> result;

		result["map"] = 1;

		return result;
	}

	std::set<std::string> test_set()
	{
		std::set<std::string> result;

		result.insert("set");

		return result;
	}

	std::unordered_map<std::string, int> test_umap()
	{
		std::unordered_map<std::string, int> result;

		result["unordered_map"] = 1;

		return result;
	}

	std::unordered_set<std::string> test_uset()
	{
		std::unordered_set<std::string> result;

		result.insert("unordered_set");

		return result;
	}

	EvaluationUnit* cls(EvaluationUnit* unit)
	{
		system("cls");
		return unit;
	}

	int zero() { return 0; }

	void expect(EvaluationUnit* unit0, EvaluationUnit* unit1)
	{
		if(unit0 && unit1)
		{
			auto result = unit0->equal(unit1);
			ENFORCE(result);
		}
	}

	void debug()
	{
		BIND_EXPRESSION_FUNCTION(uneval_break);
		BIND_EXPRESSION_FUNCTION(report_uneval);
		BIND_EXPRESSION_FUNCTION(expect);
		BIND_EXPRESSION_FUNCTION(zero);
		BIND_EXPRESSION_FUNCTION(print);
		BIND_EXPRESSION_FUNCTION(print_location);
		BIND_EXPRESSION_FUNCTION(print_recursive);
		BIND_EXPRESSION_FUNCTION(empty);
		BIND_EXPRESSION_FUNCTION(mirror_types);
		BIND_EXPRESSION_FUNCTION(mirror_type);
		BIND_EXPRESSION_FUNCTION(functions_list);
		BIND_EXPRESSION_FUNCTION(components_list);
		BIND_EXPRESSION_FUNCTION(invoke);
		BIND_EXPRESSION_FUNCTION(test_set);
		BIND_EXPRESSION_FUNCTION(test_map);
		BIND_EXPRESSION_FUNCTION(test_uset);
		BIND_EXPRESSION_FUNCTION(test_umap);
		BIND_EXPRESSION_FUNCTION(cls);
	}
}
}