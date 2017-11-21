#include "io.h"

#include <stdio.h>

namespace Private
{
	template<class T> 	void save(const T& i, FILE* f)	{	fwrite(&i, sizeof(T), 1, f);	}
	template<class T>	void load(T& i, FILE* f)		{	fread(&i, sizeof(T), 1, f);		}

	void save(const std::string& s, FILE* f)
	{
		int size = (int)s.size();
		save(size, f);
		if(size > 0)
		{
			fwrite(&s[0], size, 1, f);
		}		
	}

	void load(std::string& s, FILE* f)
	{
		int size;
		load(size, f);

		if(size > 0) 
		{
			s.resize(size);
			fread(&s[0], size, 1, f);
		}
		else
		{
			s.clear();
		}
	}

	void save(const std::map<std::string, int>& m, FILE* f)
	{
		int size = (int)m.size();
		fwrite(&size, sizeof(size), 1, f);

		std::map<std::string, int>::const_iterator it = m.begin();
		std::map<std::string, int>::const_iterator end = m.end();
		for(; it != end; ++it)
		{	
			save(it->first, f);
			save(it->second, f);
		}
	}

	void load(std::map<std::string, int>& m, FILE* f)
	{
		int size;
		load(size, f);

		for (int i = 0; i < size; ++i)
		{
			std::string key;
			load(key, f);
			load(m[key], f);
		}
	}
	//
	//
	//
	void save(RectAtlas::Bitmask& mask, FILE* f)
	{
		save(mask.size, f);
		fwrite(mask.data, mask.memsize(), 1, f);
	}

	void save(RectAtlas::Bitmask* masks, int count, FILE* f)
	{
		for (int i = 0; i < count; ++i)
		{
			save(masks[i], f);
		}
	}

	void load(RectAtlas::Bitmask& mask, FILE* f)
	{
		RectAtlas::Size size;
		load(size, f);
		mask.create(size);
		fread(mask.data, mask.memsize(), 1, f);
	}

	void load(RectAtlas::Bitmask* masks, int count, FILE* f)
	{
		for (int i = 0; i < count; ++i)
		{
			load(masks[i], f);
		}
	}

	//
	//
	//
#ifdef STREAM_IO_LIBRARY
	void save(stream::ostream& os, RectAtlas::Bitmask& mask)
	{
		os << mask.size;
		os.write(mask.data, mask.memsize());

	}

	void save(stream::ostream& os, RectAtlas::Bitmask* masks, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			save(os, masks[i]);
		}
	}

	void load(stream::istream& is, RectAtlas::Bitmask& mask)
	{
		RectAtlas::Size size; is >> size;
		mask.create(size);
		is.read(mask.data, mask.memsize());
	}

	void load(stream::istream& is, RectAtlas::Bitmask* masks, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			load(is, masks[i]);
		}
	}

#endif
}
  
//
//
//
void RectAtlasFileIO::save(const char* fileName) const
{
	FILE* f = fopen(fileName, "wb");
	fwrite(&cnt, sizeof(cnt), 1, f);
	fwrite(&sz, sizeof(sz), 1, f);
	
	if(cnt > 0)
	{
		fwrite(rects, sizeof(Rect) * cnt, 1, f);
		Private::save(bitmasks, cnt, f);
	}

	Private::save(namesTable, f);
	fclose(f);
}

void RectAtlasFileIO::load(const char* fileName)
{
	FILE* f = fopen(fileName, "rb");
	fread(&cnt, sizeof(cnt), 1, f);
	fread(&sz, sizeof(sz), 1, f);
	if(cnt > 0)
	{
		alloc(cnt);
		fread(rects, sizeof(Rect) * cnt, 1, f);
		Private::load(bitmasks, cnt, f);
	}

	Private::load(namesTable, f);
	fclose(f);	
}

//
//
//
#ifdef STREAM_IO_LIBRARY
void RectAtlasStreamIO::save(stream::ostream& os) const
{
	os << cnt << sz << sourceSz;
	if(cnt > 0)
	{
		os.write(rects, sizeof(Rect) * cnt);
		Private::save(os, bitmasks, cnt);
	}

	os << namesTable;
}

void RectAtlasStreamIO::load(stream::istream& is)
{
	is >> cnt >> sz >> sourceSz;
	if(cnt > 0)
	{
		alloc(cnt);
		is.read(rects, sizeof(Rect) * cnt);
		Private::load(is, bitmasks, cnt);
	}

	is >> namesTable;
}
#endif


void RectAtlasJsonIO::save(const char* folderName) const
{

}