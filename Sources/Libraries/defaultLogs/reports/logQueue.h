#pragma once

#include "stuff/stringUtils.h"

#include <deque>
#include <string>
#include <mutex>

namespace logs{
namespace implementation{
	class LogQueue
	{
	public:
		typedef std::deque<std::string> StringsList;

		template<class T> 
		LogQueue& operator<< (const T& obj)
		{			
			return push(str::stringize(obj).str());
		}

		void clear();
		bool empty() const;

		LogQueue& push(const std::string& str);
		virtual void pop();
		
		StringsList list() const;
		std::string string() const;

	private:
		mutable std::recursive_mutex mtx;
		StringsList strings;
		StringsList strings_stable;
	};
}
}

