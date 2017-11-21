#include "log.h"
#include "../defaultLogs.h"

#include "stuff/osprocess.h"

#include <thread>
#include <mutex>
#include <map>
																					   
namespace logs{

extern std::recursive_mutex cs;


namespace implementation{

	struct OSStorage : std::map < std::string, std::ofstream >
	{
		std::ofstream& get(const std::string& fileName)
		{
			auto& result = (*this)[fileName];
			result.open(fileName);
			return result;
		}
	};

	//
	//
	//
	BaseLog& BaseLog::add(const std::string& filename)
	{
		std::lock_guard<std::recursive_mutex> lock(cs);

		static OSStorage storage;
		files.insert(filename);
		return add( storage.get(filename) );
	}

	BaseLog& BaseLog::add(std::ostream& s)
	{
		std::lock_guard<std::recursive_mutex> lock(cs);

		push_back(&s);
		return *this;
	}

	BaseLog& BaseLog::operator<<(const char* str)
	{
		std::lock_guard<std::recursive_mutex> lock(cs);

		for(std::ostream* os : *this)
		{
			*os << str;
		}

		out();
		return *this;
	}


 	//
	Err::~Err()
	{
		showContent();
	}

	void Err::out()
	{
		for (auto os : *this)
		{
			os->flush();
		}
	}

	void Err::showContent() const
	{
		if (!empty() && !files.empty())
		{
			Base::spawn_process("notepad", files.begin()->c_str());
		}
	}

}
}

