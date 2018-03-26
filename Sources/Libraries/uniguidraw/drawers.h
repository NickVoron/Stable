// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "unigui/library.include.h"
#include "curve/library.include.h"
#include "rectAtlas/library.include.h"
#include "viewports/library.include.h"

#if defined(USE_WINDOWS)
#include "resourceTexture/library.include.h"
#include "resourceTexture11/library.include.h"
#include "resourceAtlas/library.include.h"
#include "texutils/library.include.h"
#endif

namespace Resources{ class Font; }

namespace unigui
{
	void initIndexedTableFromAtlas(IndexedSizeableTable& table, const RectAtlas& atlas);

namespace draw
{
	void init();
	void release();

	void begin(Viewports::Viewport* viewport);
	void end();
	void flush();

	void rect(const Rect& rect, const Color& color);
	void character(const Rect& rect, const Rect& texrect, Resources::Font* font, const Color& color, bool stroked);

	void print(const char* text, const Rect& rect, Resources::Font* font, bool stroked);
	void print(const char* text, const Rect& rect, Resources::Font* font, const Color& color, bool stroked);
	int stringWidth(const char* text, Resources::Font* font);
	int stringHeight(const char* text, Resources::Font* font);
	int charWidth(char c, Resources::Font* font);

	void line(const Point2& p0, const Point2& p1, float thickness, const Color& color );
	void line(const Point2& p0, const Point2& p1, const Color& color);
	void linerect(const Rect& rect, const Color& color);
	void linestrip( const Point2* points, int count, float thickness, const Color& color );
	void lineloop( const Point2* points, int count, float thickness, const Color& color );
	void circle( Point2 center, float radius, int segments, float thickness, const Color& color );
	void linerect( Rect& rect, float thickness, const Color& color );

	void curve(const Rect& rect, const nm::curve<float>& crv, float thickness, const Color& color);

#if defined(USE_WINDOWS)
	
	
	
	void textureSize(float& w, float& h, LPDIRECT3DTEXTURE9 texture);
	void textureSize(float& w, float& h, multiapi::Texture* texture);
	void textureSize(float& w, float& h, Resources::Texture* texture);
	void textureSize(float& w, float& h, Resources::Texture11* texture);
	Rect textureRect(LPDIRECT3DTEXTURE9 texture);
	Rect textureRect(multiapi::Texture* texture);
	Rect textureRect(Resources::Texture* texture);
	Rect textureRect(Resources::Texture11* texture);

	template<class TexType>
	void texrect(const Rect& rect, const Rect& texrect, TexType texture, const Color& clr = color(1, 1, 1, 1));

	template<class TexType>	void texrect(const Rect& rect, const Point2& texStartPoint, TexType texture, const Color& clr = color(1,1,1,1))												{ texrect(rect, Rect(texStartPoint.x, texStartPoint.y, rect.size.x, rect.size.y), texture, clr);	}
	template<class TexType>	void texrect(const Point2& screenPos, const Rect& texRect, TexType texture, const Color& clr = color(1, 1, 1, 1))											{ texrect(Rect(screenPos.x, screenPos.y, texRect.size.x, texRect.size.y), texRect, texture, clr);	}
	template<class TexType>	void texrect(const Rect& screenRect, TexType texture, const Color& clr = color(1, 1, 1, 1))																	{ texrect(screenRect, textureRect(texture), texture, clr); }
	template<class TexType>	void texrect(const Point2& screenPos, TexType texture, const Color& clr = color(1, 1, 1, 1))																{ texrect(screenPos, textureRect(texture), texture, clr); }
	template<class TexType> void texrect(const Rect& rect, const Rect& parentRect, const Rect& trect, TexType texture, Align align, const Color& clr = color(1, 1, 1, 1))				{ texrect(alignedRect(rect, parentRect, align), trect, texture, clr); }
	template<class TexType>	void texrect(const Rect& rect, const Rect& parentRect, const Point2& texStartPoint, TexType texture, Align align, const Color& clr = color(1, 1, 1, 1))		{ texrect(alignedRect(rect, parentRect, align), texStartPoint, texture, clr); }
	template<class TexType>	void texrect(const Rect& parentRect, const Rect& trect, TexType texture, Align align, const Color& clr = color(1, 1, 1, 1))									{ texrect(alignedRect(trect, parentRect, align), trect, texture, clr); }
	template<class TexType> void texrect(const Rect& parentRect, TexType texture, Align align, const Color& clr = color(1, 1, 1, 1))													{ texrect(alignedRect(textureRect(texture), parentRect, align).pos, texture, clr); }

	void texrect(const Point2& screenPos, Resources::Atlas* atlas, int index, const Color& clr = color(1, 1, 1, 1));
	void texrect(const Point2& screenPos, const Point2& aspect, Resources::Atlas* atlas, int index, const Color& clr = color(1, 1, 1, 1));
	Rect texrect(const Rect& parentRect,  Resources::Atlas* atlas, Align align, int index, const Color& clr = color(1, 1, 1, 1));
	void texrect(Resources::Atlas* atlas, int index, const Color& clr = color(1, 1, 1, 1));

	void texrect(const IndexedSizeableTable& table, const RectAtlasGAPI& atl, const Color& clr = color(1, 1, 1, 1));
	void texrect(const IndexedSizeableTable& table, Resources::Atlas* atlas, const Color& clr = color(1, 1, 1, 1));
	void texrect(const IndexedSizeableTable& table, float leftScissor, float rightScissor, float topScissor, float bottomScissor, Resources::Atlas* atlas, const Color& clr = color(1, 1, 1, 1));
#endif
}
}




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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