// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <stdio.h>
#include <string>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

namespace stream
{

#ifdef __ANDROID__
	extern AAssetManager* manager;
#endif


class cfile
{
public:
	cfile();
	cfile(const char* fileName, bool readOnly, bool useAssetsManager = false);
	cfile(const std::string& fileName, bool readOnly, bool useAssetsManager = false);
	virtual ~cfile(); 

	std::size_t fread(void* buf, std::size_t size);
	std::size_t fwrite(const void* buf, std::size_t size);
	void fskip(std::size_t bytes);
	void fopen(const char* fileName, bool readOnly);
	void fclose();
	void fflush();
	std::size_t fgetpos() const;
	std::size_t fsize() const;
	std::size_t fleft() const;
	bool fgood() const;

	const std::string& getName() const{return fileName;}

private:
#ifdef __ANDROID__
	AAssetManager *mgr = nullptr;
#endif
	void* file = nullptr;
	std::string fileName;
};


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