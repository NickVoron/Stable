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

