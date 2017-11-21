#pragma once

#include "stream/library.include.h"
#include "multiapiUtlis/library.include.h"

#include "io.h"

#ifdef WIN32
#ifdef STREAM_IO_LIBRARY

class RectAtlasGAPI
{
friend class TextureAtlasComposer;
public:
	~RectAtlasGAPI();
								   	
	void clear();
	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	multiapi::Texture texture;
	RectAtlasStreamIO atlas;
};

#endif
#endif