#pragma once
#include <string>
#include <unordered_map>
#include <optional>


namespace Expressions
{
	// это собственное пространство имен переменной, к примеру у струтуры все дочерние свойства будут храниться здесь
	// такая же штука есть у класса. У ProxyExpression собственное отсутвует вместо него пространство имен класса
	// Если Epxression является массивом то все ссылки на дочерние элементы тут то же будут храниться 
	// Это все нужно что что бы Path могло раскручивать адресации вида var.property[1],
	// в данном случае нужно найти var у него в пространстве имен найти property а у него найди [1], 
	// то есть первый элемент массива

	enum class InsertMethod
	{
		INSERT,
		REPLACE,
		REPLACE_OR_INSERT,
		IGNORE_IF_EXIST
	};

	uint64_t newid();

	template<typename ExpressionType>
	class NameScope: public std::map<std::string, ExpressionType*>
	{
	public:		
		NameScope(const std::string& scopeName_, NameScope* parent_) : scopeName(scopeName_), parent(parent_) {	}

		void add(const std::string& name, ExpressionType* expr, InsertMethod method);
		bool exists(const std::string& name) const;
		ExpressionType* get(const std::string& name) const;
		ExpressionType* getByType(const std::string& type) const;
		void trace(const std::string& name, std::string& result) const;
		void trace_parents(std::string& result) const;

		void remove(const std::string& name);
		std::optional<std::string> name(ExpressionType* expr) const;

		std::string string() const
		{
			str::stringize result;
			for (auto& val : *this)
			{
				result("{ ", val.first, ": ", val.second->string(), " }");
			}
			return result.str();
		}

		std::string path() const
		{
			return parent ? (str::stringize(parent->path(), ".", scopeName)) : scopeName;
		}

					
		bool isClassMember(const ExpressionType* expr) const { return classMembers.count(expr); }

		uint64_t id = newid();
		std::string scopeName;
		std::set<const ExpressionType*> classMembers;

		NameScope * const parent = nullptr;
	};

	template<typename ExpressionType>
	void NameScope<ExpressionType>::add(const std::string& name, ExpressionType* expr, InsertMethod method)
	{
		auto found = this->find(name);
		ENFORCE_MSG(method != InsertMethod::INSERT || found == std::end(*this), "INSERT: scopenames alredy have property by name: " + name);
		ENFORCE_MSG(method != InsertMethod::REPLACE || found != std::end(*this), "REPLACE: scopenames not have property by name: " + name);

		if (method == InsertMethod::IGNORE_IF_EXIST && found != std::end(*this))
		{
			return;
		}

		(*this)[name] = expr;
	}

	template<typename ExpressionType>
	ExpressionType* NameScope<ExpressionType>::get(const std::string& name) const
	{
		auto found = this->find(name);
		ExpressionType* result = (found != std::end(*this)) ? found->second : nullptr;
		if (!result && parent)
		{
			result = parent->get(name);
		}

		return result;
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::trace_parents(std::string& result) const
	{
		result += str::stringize(id, "(", scopeName, ")");
		if (parent)
		{
			result += ".";
			parent->trace_parents(result);
		}
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::trace(const std::string& name, std::string& result) const
	{
		if(result.empty())
		{
			result += str::stringize(name, "<-");
		}

		result += str::stringize(id, "(", scopeName, ")");
		auto found = this->find(name);
		if(found != std::end(*this))
		{
			result += "=" + name;
		}
		else
		{
			if (parent)
			{
				result += ".";
				parent->trace(name, result);
			}
			else
			{
				result += "->NOT_FOUND";
			}
		}
	}

	template<typename ExpressionType>
	bool NameScope<ExpressionType>::exists(const std::string& name) const
	{
		return get(name)!=nullptr;
	}

	template<typename ExpressionType>
	ExpressionType* NameScope<ExpressionType>::getByType(const std::string& type) const
	{
		auto found = std::find_if(std::begin(*this), std::end(*this), [type](auto& iter)
		{
			return (iter.second) && (iter.second->typeName() == type);
		});

		ExpressionType* result = (found != std::end(*this)) ? found->second : nullptr;

		if (!result && parent)
		{
			result = parent->getByType(type);
		}

		return result;
	}

	template<typename ExpressionType>
	std::optional<std::string> NameScope<ExpressionType>::name(ExpressionType* expr) const
	{
		auto found = std::find_if(std::begin(*this), std::end(*this), [expr](auto& iter)
		{
			return iter.second == expr;
		});

		if (found != this->end())
		{
			return found->first;
		}

		return std::nullopt;
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::remove(const std::string& name)
	{
		auto iter = find(name);
		ENFORCE(iter != end());
		erase(iter);
	}  	
}
	