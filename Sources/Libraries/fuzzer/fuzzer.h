// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <utility>
#include <limits>
#include <thread>

#include "common/apply.h"


namespace fuzzer
{
	template<typename T>
	T mutate(T value, std::size_t index)
	{
		return value;
	}
	
	template<>
	int mutate<int>(int value, std::size_t index)
	{
		int values [] = 
		{
			0, 
			std::numeric_limits<int>::lowest(), 
			std::numeric_limits<int>::min(),
			std::numeric_limits<int>::max()
		};

		return (index < (sizeof(values) / sizeof(values[0]))) ? values[index] : (int) index;
	}

	template<>
	bool mutate<bool>(bool value, std::size_t index)
	{
		bool values [] =
		{
			false,
			true,
		};

		return values[index % 2];
	}

	template<>
	double mutate<double>(double value, std::size_t index)
	{
		return (double) index;
	}
		
	template<class... P>
	struct mutator
	{
		auto operator()(const std::tuple<P...>& input, std::size_t index)
		{
			return stl::apply([index](auto... param) { return std::make_tuple(mutate(param, index)...); }, input);
		}
	};
	
	template<class ReturnType, class... P>
	struct invoker
	{
		typedef ReturnType(*Procedure)(P...);

		invoker(Procedure proc) : procedure(proc) {}

		void invoke(int count) const
		{
			mutator<P...> mtr;
			if (count > 0)
			{
				for (int i = 0; i < count; ++i)
				{
					stl::apply(procedure, mtr(params, (std::size_t)i));
				}
			}
			else
			{
				std::size_t mutationIndex = 0;
				while (true)
				{
					stl::apply(procedure, mtr(params, mutationIndex++));
				}
			}
		}

		ReturnType operator()(P... p) const
		{
			return procedure(p...);
		}

		std::tuple<P...> params;
		Procedure procedure;
	};

	template<class ReturnType, class... P>
	invoker<ReturnType, P...> fuzz(ReturnType(*proc)(P...), int count)
	{
		invoker<ReturnType, P...> fzr(proc);
	
		try
		{
			fzr.invoke(count);
		}
		catch (const std::exception& e)
		{
			LOG_MSG(e);
		}

		return fzr;
	}

	template<class ReturnType, class... P>
	invoker<ReturnType, P...> fuzz(ReturnType(*proc)(P...), std::size_t threadsCount, int count)
	{
		try
		{
			std::vector<std::thread> threads(threadsCount);
			for (auto&& thread : threads)
			{
				thread = std::thread([proc, count]
				{
					fuzz(proc, count);
				});
			}

			for (auto&& thread : threads)
			{
				thread.join();
			}
		}
		catch (const std::exception& e)
		{
			LOG_MSG(e);
		}

		return fuzz(proc, count > 1);		
	}

}

#define FUZZ(function) fuzzer::fuzz(&function, 10)



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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