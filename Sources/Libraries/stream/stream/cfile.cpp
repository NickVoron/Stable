// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "cfile.h"
#include "errors.h"


namespace stream
{

#ifdef __ANDROID__
	AAssetManager* manager;
#endif

cfile::cfile(){}

cfile::cfile(const char* fileName, bool readOnly, bool useAssetsManager)
{ 
#ifdef __ANDROID__
	if (useAssetsManager)
	{
		mgr = manager;
	}
#endif

	fopen(fileName, readOnly); 
}

cfile::cfile(const std::string& fileName, bool readOnly, bool useAssetsManager) : cfile(fileName.c_str(), readOnly, useAssetsManager)
{ 
}

cfile::~cfile() 
{
	if (file)
	{
		fclose();
	}		
}


std::size_t cfile::fread(void* buf, std::size_t size)
{
	if (!file)
	{
		throw file_not_open();
	}

#ifdef __ANDROID__
	if (mgr)
	{
		auto readed = AAsset_read((AAsset*)file, buf, size);
		if (readed != size)
		{
			throw cant_read_file(fileName.c_str(), size, fleft());
		}
	}
	else
#endif
	{
		size_t readed = ::fread(buf, size, 1, (FILE*)file);
		if (readed != 1)
		{
			throw cant_read_file(fileName.c_str(), size, fleft());
		}
	}		

	return size;
}

std::size_t cfile::fwrite(const void* buf, std::size_t size)
{
	if (!file)
	{
		throw file_not_open();
	}		

#ifdef __ANDROID__
	if (mgr)
	{
		std::string msg = "can't write to asset file: " + fileName;
		throw cant_write_file(msg.c_str(), size);
	}
#endif
	{
		size_t writed = ::fwrite(buf, size, 1, (FILE*) file);
		if (writed != 1)
		{
			throw cant_write_file(fileName.c_str(), size);
		}
	}

	return size;
}

void cfile::fskip(std::size_t bytes)
{
#ifdef __ANDROID__
	if (mgr)
	{
		AAsset_seek64((AAsset*) file, bytes, SEEK_CUR);
	}
#endif
	{
		fseek((FILE*) file, bytes, SEEK_CUR);
	}
}

std::size_t cfile::fsize() const
{
#ifdef __ANDROID__
	if (mgr)
	{
		return AAsset_getLength64((AAsset*) file);
	}
	else
#endif
	{
		if (!file)
			throw file_not_open();

		auto cur = ::ftell((FILE*) file);
		::fseek((FILE*) file, 0, SEEK_END);
		auto end = ::ftell((FILE*) file);
		::fseek((FILE*) file, cur, SEEK_SET);
		return end;
	}
}

std::size_t cfile::fleft() const
{
#ifdef __ANDROID__
	if (mgr)
	{
		return AAsset_getRemainingLength64((AAsset*) file);
	}
	else
#endif
	{
		if (file)
		{
			auto cur = ::ftell((FILE*) file);
			::fseek((FILE*) file, 0, SEEK_END);
			auto end = ::ftell((FILE*) file);
			::fseek((FILE*) file, cur, SEEK_SET);
			return end - cur;
		}		
	}

	return 0;
}

std::size_t cfile::fgetpos() const
{
#ifdef __ANDROID__
	if (mgr)
	{
		return fsize() - fleft();
	}
	else
#endif
	{
		if (!file)
			throw file_not_open();

		return ::ftell((FILE*) file);
	}
}

void cfile::fopen(const char* fn, bool readOnly)
{
	if(file) 
		fclose();

	fileName = fn;
#ifdef __ANDROID__
	if (mgr)
	{
		file = AAssetManager_open(mgr, fn, AASSET_MODE_STREAMING);
	}
	else
#endif
	{
		file = ::fopen(fn, readOnly ? "rb" : "wb");
		if (!file)
			throw cant_open_file(fn);
	}
}

void cfile::fclose()
{
	if(file) 
	{
#ifdef __ANDROID__
		if (mgr)
		{
			AAsset_close((AAsset*) file);
		}
		else
#endif
		{
			::fclose((FILE*) file);
		}

		file = nullptr;
	}
}

bool cfile::fgood() const
{
	return fleft() > 0;
}

void cfile::fflush()
{
#ifdef __ANDROID__
	if (mgr)
	{
		if (!file)
			throw file_not_open();
	}
	else
#endif
	{
		if (!file)
			throw file_not_open();

		::fflush((FILE*) file);
	}
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