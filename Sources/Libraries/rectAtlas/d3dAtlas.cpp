#include "d3dAtlas.h"

#ifdef WIN32
#ifdef STREAM_IO_LIBRARY
//
//
//
RectAtlasGAPI::~RectAtlasGAPI()
{
	clear();
}

void RectAtlasGAPI::clear()
{
	texture.clear();
	atlas.clear();
}

void RectAtlasGAPI::save(stream::ostream& os) const
{
	atlas.save(os);
	texture.save(os);
	
}

void RectAtlasGAPI::load(stream::istream& is)
{
	clear();
	atlas.load(is);
	texture.load(is);
}

#endif
#endif