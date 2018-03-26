// Copyright (C) 2015-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/library.include.h"
#include "defaultLogs/library.include.h"

#undef foreach

namespace ObjectsDB
{
	template<class Object> struct Container;

	template<class Object>
	struct ObjectBase : public Base::IntrusiveList<Object, ObjectBase<Object>>::Node
	{
		typedef Container<Object> Database;

		virtual ~ObjectBase(){}
	};

	template<class Object>
	struct Container : public Base::IntrusiveList<Object, ObjectBase<Object>>
	{
		struct ResultEntry : public Base::IntrusiveList<ResultEntry>::Node
		{
			Object* object = nullptr;

			ResultEntry& set(Object* obj) 
			{ 
				ENFORCE(obj); 
				object = obj; 
				return *this; 
			}
		};

		struct Result : public Base::IntrusiveList<ResultEntry>
		{
			template<class Function>
			Function foreach(Function&& func) const
			{
				for (auto& val : *this)
				{
					ENFORCE(val.object);
					func(*val.object);
				}
				
				return std::move(func);
			}

			template<class Function>
			Function foreach(Function&& func)
			{
				for (auto& val : *this)
				{
					ENFORCE(val.object);
					func(*val.object);
				}

				return std::move(func);
			}
		};

		template<class Predicate>
		void find_first(Result& result, Predicate&& predicate)
		{
			for (auto& obj : *this)
			{
				if (predicate(obj))
				{
					result.push_back(free_entry().set(&obj));
					break;
				}
			}
		}

		template<class Predicate>
		Result find_first(Predicate&& predicate)
		{
			Result result;
			find_first(result, std::forward<Predicate>(predicate));
			return std::move(result);
		}

		template<class Predicate>
		int find_first_n(Result& result, int count, Predicate&& predicate)
		{
			int found = 0;
			for (auto& obj : *this)
			{
				if (predicate(obj))
				{
					result.push_back(free_entry().set(&obj));
					++found;

					if (found == count)
						break;
				}
			}

			return found;
		}

		template<class Predicate>
		Result find_first_n(int count, Predicate&& predicate)
		{
			Result result;
			find_first_n(result, count, std::forward<Predicate>(predicate));
			return std::move(result);
		}

		template<class Predicate>
		int find_all(Result& result, Predicate&& predicate)
		{
			int found = 0;
			for (auto& obj : *this)
			{
				if (predicate(obj))
				{
					result.push_back(free_entry().set(&obj));
					++found;
				}
			}

			return found;
		}

		template<class Predicate>
		Result find_all(Predicate&& predicate)
		{
			Result result;
			find_all(result, std::forward<Predicate>(predicate));
			return std::move(result);
		}

		template<class Predicate>
		int find_rand_n(Result& result, int count, Predicate&& predicate)
		{
			LOG_WARNING("find_rand_n identical to find_first_n");
			return find_first_n(result, count, std::forward<Predicate>(predicate));
		}

		template<class Predicate>
		Result find_rand_n(int count, Predicate&& predicate)
		{
			Result result;
			find_rand_n(result, count, std::forward<Predicate>(predicate));
			return std::move(result);
		}

		template<class Predicate, class ParameterType>
		void find_best(Result& result, ParameterType& currentValue, Predicate&& predicate)
		{	 			
			Object* resobj = 0;
			for (auto& obj : *this)
			{
				resobj = predicate(currentValue, resobj, obj);
			}

			if (resobj)
			{
				result.push_back(free_entry().set(resobj));
			}
		}

		template<class Predicate, class ParameterType>
		Result find_best(ParameterType& currentValue, Predicate&& predicate)
		{
			Result result;
			find_best(result, currentValue, std::forward<Predicate>(predicate));
			return std::move(result);
		}


		template<class Predicate, class Action>
		void apply_to_all(Predicate&& predicate, Action&& action)
		{
			for (auto& obj : *this)
			{
				if (predicate(obj)) 
				{
					action(obj);
				}
			}
		}

	private:

		ResultEntry& free_entry()
		{
			return *(new ResultEntry());
		}
 	};

}



// Copyright (C) 2015-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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