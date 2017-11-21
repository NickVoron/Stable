#include "holder.h"

#include <vector>
#include <mutex>
#include <unordered_set>

namespace Expressions
{
	template<class StoredType>
	struct Holder
	{
		~Holder()
		{
			std::lock_guard<std::mutex> lock(mtx);

			ENFORCE(counter == 0);
			ENFORCE(expressions.empty());
			ENFORCE(expressions_dict.empty());
		}

		StoredType* add(StoredType* expr)
		{
			std::lock_guard<std::mutex> lock(mtx);

			ENFORCE(expressions_dict.find(expr) == expressions_dict.end());

			expressions.emplace_back(std::move(std::unique_ptr<StoredType>(expr)));
 			expressions_dict.insert(expr);
 			++counter;
 			ENFORCE(expressions.size() == counter);
 			ENFORCE(expressions_dict.size() == counter);
			return expr;
		}

		void clear()
		{
 			std::lock_guard<std::mutex> lock(mtx);
 
 			std::size_t reset_cursor = 0;
 			for (auto& ptr : expressions)
 			{
 				ptr = nullptr;
 				++reset_cursor;
 				--counter;
 			}
 
 			ENFORCE(counter == 0);
 			expressions_dict.clear();
 			expressions.clear();
		}

		std::mutex mtx;
		std::size_t counter = 0;
		std::vector<std::unique_ptr<StoredType>> expressions;
		std::unordered_set<StoredType*> expressions_dict;
	};
	

	Holder<Expression> expressions;
	Holder<EvaluationUnit> units;
	 
	Expression* addExpression(Expression* expr)
	{
		return expressions.add(expr);
	}

	EvaluationUnit* addExpression(EvaluationUnit* unit)
	{
		return units.add(unit);
	}

	void collect_garbage()
	{
		expressions.clear();
		units.clear();
	}
}