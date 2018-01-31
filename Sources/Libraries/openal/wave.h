// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <windows.h>
#include <stdio.h>
#include <MMSystem.h>
#include <Ks.h>
#include <KsMedia.h>

#include "buffer.h"
#include "stream/library.include.h"

enum WAVEFILETYPE
{
	WF_EX  = 1,
	WF_EXT = 2
};


struct WAVEFILEINFO
{
	WAVEFILETYPE	wfType;
	WAVEFORMATEXTENSIBLE wfEXT;		
	char			*pData;
	unsigned long	ulDataSize;
	unsigned long	ulDataOffset;
};

class Wave
{
public:
	Wave();
	~Wave();
	void clear();
	void defaults();
	bool load(const char* fileame);
	WAVEFILEINFO info;
};

bool bufferWAVE(const char* filename, openal::buffer& buffer);
bool bufferWAVE(const char* filename, openal::buffer& buffer, Wave& wave);
bool bufferWAVE(openal::buffer& buffer, const Wave& wave);

stream::ostream& operator<<(stream::ostream& os, const WAVEFILEINFO& wave);
stream::istream& operator>>(stream::istream& is, WAVEFILEINFO& wave);

stream::ostream& operator<<(stream::ostream& os, const Wave& wave);
stream::istream& operator>>(stream::istream& is, Wave& wave);




// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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