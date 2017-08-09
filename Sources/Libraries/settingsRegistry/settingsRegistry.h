// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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