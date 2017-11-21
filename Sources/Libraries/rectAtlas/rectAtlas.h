#pragma once

#include <string>
#include <map>
#include "strings/library.include.h"

class RectAtlas 
{
friend class TextureAtlasComposer;
public:

#pragma pack(push,1)
	struct Size
	{
		Size():x(0),y(0){}
		int x;
		int y;
	};

	struct Bitmask 
	{

		typedef unsigned char Element;

		Bitmask();
		~Bitmask();

		void create(const Size& size);
		void create(void* data, int sizeX, int sizeY);
		void clear();

		unsigned int memsize() const;

		bool inside(int orgX, int orgY, int x, int y) const;

		unsigned char* data;		
		Size size;		
	};

	struct Rect
	{
		Rect();
		~Rect();

		bool inside(int orgX, int orgY, int x, int y) const;
		float sizeX() const;
		float sizeY() const;

		float orgX;
		float orgY;
		float minX;
		float minY;
		float maxX;
		float maxY;
	};
#pragma pack(pop)

	

	RectAtlas();
	RectAtlas(int cnt, int sizeX, int sizeY);

	~RectAtlas();

	void init(int count, int sizeX, int sizeY);
	void clear();

	Bitmask& bitmask(int idx);
	const Bitmask& bitmask(int idx) const;

	Rect& absoluteRect(int idx);
	const Rect& absoluteRect(int idx) const;

	bool inside(int rectIdx, int orgX, int orgY, int x, int y, bool useBitmask) const;

	Rect relativeRect(int idx, bool halfPixelCorrection) const;
	int count() const;

	const Size& size() const { return sz; } 
	const Size& sourceSize() const { return sourceSz; } 

	int index(const str::string32& name) const;
	const str::string32& name(int index) const;
	const Rect& absoluteRect(const str::string32& name) const;
	Rect relativeRect(const str::string32& name, bool halfPixelCorrection) const;


protected:
	void alloc(int count);
	int cnt;
	Rect* rects;
	Bitmask* bitmasks;

	typedef std::map<str::string32, int> NamesTable;
	NamesTable namesTable;
	
	Size sz;
	Size sourceSz;
};

