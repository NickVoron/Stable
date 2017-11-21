#pragma once
#include "expression.h"
#include "holder.h"
#include "struct.h"
#include <unordered_map>

namespace Expressions
{
	template<class PropertyType>
	struct Properties : public std::unordered_map<std::string, const PropertyType*>
	{
		void add(const std::string& name, const PropertyType* value)
		{
			this->emplace(name, value);
		}

		const PropertyType* get(const std::string& name) const
		{
			auto it = find(name);
			return it != end() ? it->second : nullptr;
		}

	};
	

	//
	struct PropertiesStruct : public Expression, public Properties<Expression>
	{
	public:
		PropertiesStruct(const std::string& typeName);
		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;

		std::string structType;
	};

	//
	struct PropertiesStructUnit : public EvaluationUnit
	{
	public:
		PropertiesStructUnit(Expressions::EvaluatedScope& parent, const PropertiesStruct& prototype);
		virtual std::string typeName() const override;
		virtual std::string string() const override;
		void add(const std::string& name, EvaluationUnit* value);
		const EvaluationUnit* get(const std::string& name) const;

		const PropertiesStruct& prototype;
	};
}
