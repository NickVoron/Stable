// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "settingsRegistry.h"

#include "stuff/library.include.h"
#include "defaultLogs/library.include.h"

static const char* fileName = "settings.bin";
#ifdef __ANDROID__
static const char*applicationDir = "/storage/emulated/0/Android/data/com.example.nikolayvoronetskiy.myapplication2/";
#else
static const char*applicationDir = "";
#endif

void SettingsRegistry::save()
{
	try
	{
        auto filename = boost::filesystem::path(applicationDir) / fileName;
		stream::ofstream<dataop::zlib> os(filename.string().c_str());
		os << Base::ExplicitMeyersSingleton<SettingsRegistry>::get().data;
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e);
	}
}

void SettingsRegistry::load()
{
    auto filename = boost::filesystem::path(applicationDir) / fileName;

	if(Base::FileUtils::FileExists(filename.string().c_str()))
	{
		try
		{
			stream::ifstream<dataop::zlib> is(filename.string().c_str());
			is >> Base::ExplicitMeyersSingleton<SettingsRegistry>::get().data;
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}
}




// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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