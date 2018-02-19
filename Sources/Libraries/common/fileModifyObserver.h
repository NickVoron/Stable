// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <sys/stat.h>

namespace Base 
{
namespace FileUtils 
{

inline bool operator==(const timespec& lref, const timespec& rref) { return (lref.tv_nsec == rref.tv_nsec) && (lref.tv_sec == rref.tv_sec); }
inline bool operator!=(const timespec& lref, const timespec& rref) { return !(lref == rref); }

struct file_time
{
	file_time() = default;
#if defined(WIN32) || defined(__APPLE__)
	file_time(struct ::stat& st) : value(st.st_mtime){}
	time_t value;
#else
	file_time(struct ::stat& st) : value(st.st_mtim) {}
	timespec value;
#endif 
};

inline bool operator==(const file_time& lref, const file_time& rref) { return lref.value == rref.value; }
inline bool operator!=(const file_time& lref, const file_time& rref) { return lref.value != rref.value; }

class ModifyObserver 
{
public:
	static bool& enabled()
	{
		static bool flag = true;
		return flag;
	}

	
	ModifyObserver() = default;
	ModifyObserver( const std::string& fn ){ SetFileName( fn ); }
	ModifyObserver( const ModifyObserver& mo ):fileName(mo.fileName), time(mo.time) {}

	
	inline bool CheckModified() const;
	inline bool CheckAndClearModified();
	inline void ClearModified();
	inline void SetFileName( const std::string& fn );
	
	
	bool operator < ( const ModifyObserver& a ) const {return fileName < a.fileName; }

	std::string fileName;
	file_time time;
};

bool ModifyObserver::CheckModified() const
{
	if (!enabled())
		return false;

	struct ::stat newStats;
	auto exists = (stat(fileName.c_str(), &newStats) == 0);
	file_time t { newStats };
	return exists && (time != t);
}


bool ModifyObserver::CheckAndClearModified()
{
	if (!enabled()) 
		return false;

	struct ::stat newStats;
	auto exists = (stat(fileName.c_str(), &newStats) == 0);
	file_time t { newStats };
	bool res = exists && (time != t);
	time = t;
	return res;
}

void ModifyObserver::ClearModified()
{
	struct ::stat newStats;
	if(stat(fileName.c_str(), &newStats) == 0)
	{
		time = file_time{ newStats };
	}
}

void ModifyObserver::SetFileName(const std::string& fn)
{
	fileName = fn;
	ClearModified();
}


} 
}




// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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