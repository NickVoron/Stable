#pragma once

#include "stream/library.include.h"
#include "defaultLogs/library.include.h"
#include "stuff/meyersSingleton.h"

class SettingsRegistry : public Base::ExplicitMeyersSingleton<SettingsRegistry>
{
public:
	~SettingsRegistry(){ save(); }
	typedef stream::dmemstream StreamType;
	
	template<class T>
	static void get(const std::string& name, T& d);

	template<class T>
	static void add(const std::string& name, const T& d);

	static void load();
	static void save();

private:
	std::map<std::string, StreamType> data;
};

template<class T>
void SettingsRegistry::get(const std::string& name, T& d)
{
	try
	{
		SettingsRegistry& sr = Base::ExplicitMeyersSingleton<SettingsRegistry>::get();

		auto it = sr.data.find(name);
		if(it != sr.data.end()) 
		{
			StreamType& s = it->second;
			s.resetReadCursor();
			s >> d;
		}
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e.what());
	} 	
}

template<class T>
void SettingsRegistry::add(const std::string& name, const T& d)
{
	try
	{
		SettingsRegistry& sr = Base::ExplicitMeyersSingleton<SettingsRegistry>::get();

		StreamType& s = sr.data[name];
		s.resetReadCursor();
		s.resetWriteCursor();
		s << d;

		save();
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e.what());
	}	
}
