// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "holder.h"

#include <vector>
#include <mutex>
#include <unordered_set>

namespace Expressions
{
	struct Holder
	{
		~Holder()
		{
			std::lock_guard<std::mutex> lock(mtx);

			ENFORCE(counter == 0);
			ENFORCE(expressions.empty());
			ENFORCE(expressions_dict.empty());
		}

		Expression* add(Expression* expr)
		{
			std::lock_guard<std::mutex> lock(mtx);

			ENFORCE(expressions_dict.find(expr) == expressions_dict.end());

			expressions.emplace_back(std::move(std::unique_ptr<Expression>(expr)));
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
		std::vector<std::unique_ptr<Expression>> expressions;
		std::unordered_set<Expression*> expressions_dict;
	};
	

	Holder holder;
	 
	Expression* addExpression(Expression* expr)
	{
		
		return holder.add(expr);
	}

	void collect_garbage()
	{
		holder.clear();
	}
}



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.