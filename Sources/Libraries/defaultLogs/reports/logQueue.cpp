// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "logQueue.h"

namespace logs{
namespace implementation{
	
	LogQueue& LogQueue::push(const std::string& str)
	{
		std::lock_guard<std::recursive_mutex> lock(mtx);

		size_t endlPos = str.find_first_of('\n');
		if(endlPos != std::string::npos)
		{
			push( str.substr(0, endlPos) );
			strings.push_back("");

			if(endlPos < str.length())
			{
				push(str.substr(endlPos + 1));
			}
		}
		else
		{
			if (strings.empty())
			{
				strings.push_back("");
			}

			strings.back() += str;
			if (strings.size() > 20)
			{
				pop();
			}
		}	 	

		return *this;
	}

	void LogQueue::pop() 
	{ 
		std::lock_guard<std::recursive_mutex> lock(mtx);
		if(!strings.empty())
		{ 
			strings.pop_front(); 
		} 
	}

	void LogQueue::clear()
	{
		std::lock_guard<std::recursive_mutex> lock(mtx);
		strings_stable = strings;
		strings.clear();
	}

	bool LogQueue::empty() const
	{
		std::lock_guard<std::recursive_mutex> lock(mtx);
		return strings.empty();
	}

    LogQueue::StringsList LogQueue::list() const
	{
		std::lock_guard<std::recursive_mutex> lock(mtx);
		return strings_stable;
	}

	std::string LogQueue::string() const
	{
		std::lock_guard<std::recursive_mutex> lock(mtx);
		str::stringize report;
		for (auto& str : strings_stable)
		{
			report(str, "\n");
		}

		return str::stringize(report).str();
	}
}
}





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