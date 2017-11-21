#pragma once

#include "rectAtlas.h"

class RectAtlasFileIO : public RectAtlas
{
public:
	void save(const char* fileName) const;
	void load(const char* fileName);
};

#ifdef STREAM_IO_LIBRARY

class RectAtlasStreamIO : public RectAtlas
{
public:
	void save(stream::ostream& os) const;
	void load(stream::istream& is);
};

#endif

class RectAtlasJsonIO : public RectAtlas
{
public:
	void save(const char* folderName) const;
};
