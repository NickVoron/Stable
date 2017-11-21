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
	WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
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
