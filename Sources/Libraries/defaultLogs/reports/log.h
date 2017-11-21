#pragma once

#include "stuff/stringUtils.h"

#include <iostream>
#include <list>
#include <set>

namespace logs{
namespace implementation{
	
	//
	class BaseLog : public std::list<std::ostream*>
	{
	public:
		virtual ~BaseLog() {}

		template<class T>
		BaseLog& operator<<(const T& obj)
		{
			return operator<<( str::spaced(obj).c_str() );
		}

		BaseLog& operator<<(const char* str);

		BaseLog& add(const std::string& filename);
		BaseLog& add(std::ostream& s);

		bool empty() const { return empty_; }

	protected:
		virtual void out() { empty_ = false; }
		std::set<std::string> files;
		bool empty_ = true;
	};

	//
	struct Log		: public BaseLog{};
	struct Info		: public BaseLog{};
	struct Warning	: public BaseLog{};

	struct Err : public Log 
	{ 
		virtual ~Err(); 
		virtual void out(); 
		void showContent() const;
	};
}
}

