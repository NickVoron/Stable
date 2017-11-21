#include "drawers.h"
#include "geometry/library.include.h"

#include "resourceFont/library.include.h"

#include "dx9.h"
#include "dx11.h"

#include "renderers/statesBucket.h"

namespace unigui
{
	void initIndexedTableFromAtlas(IndexedSizeableTable& table, const RectAtlas& atlas)
	{
		int rc, cc;
		table.getStructSize(cc, rc);

		for(int i = 0; i < cc; ++i)
		{
			float maxSize = 0.0f;
			for(int j = 0; j < rc; ++j)
			{
				int index = table.addData[i][j];
				float size = atlas.absoluteRect(index).sizeX();
				if(maxSize < size)
					maxSize = size;
			}

			IndexedSizeableTable::Line& column = table.column(i);
			column.minSize = maxSize;			
		}

		for(int i = 0; i < rc; ++i)
		{
			float maxSize = 0.0f;
			for(int j = 0; j < cc; ++j)
			{
				int index = table.addData[j][i];
				float size = atlas.absoluteRect(index).sizeY();
				if(maxSize < size)
					maxSize = size;
			}

			IndexedSizeableTable::Line& row = table.row(i);
			row.minSize = maxSize;			
		}
	}

	namespace draw{

		//
		//
		//
		//
		//
		//
		void init()
		{
			if(gapi::initialized(gapi::DX9))
			{
				draw::dx9::init();
			}		
			else if(gapi::initialized(gapi::DX11))
			{
				draw::dx11::init();
			}
		}

		//
		//
		//
		void begin(Viewports::Viewport* viewport)
		{
			if(gapi::initialized(gapi::DX9))
			{
				draw::dx9::begin(viewport);
			}
			else if(gapi::initialized(gapi::DX11))
			{
				draw::dx11::begin(viewport);
			}
		}

		void end()
		{
			if (gapi::initialized(gapi::DX9))
			{
				
			}
			else if (gapi::initialized(gapi::DX11))
			{
				draw::dx11::end();
			}
		}

		void flush()
		{
			if(gapi::initialized(gapi::DX9))
			{
				draw::dx9::flush();
			}
			else if(gapi::initialized(gapi::DX11))
			{
				draw::dx11::flush();
			}
		}

		void release()
		{
			if(gapi::initialized(gapi::DX9))
			{
				draw::dx9::release();
			}
			else if(gapi::initialized(gapi::DX11))
			{
				draw::dx11::release();
			}
		}


		//
		//
		//
		//
		template<class P0, class P1>
		void universal_impl(const P0& p0, const P1& p1, const Color& color, RendererUsingInterface& state)
		{
			state.add(p0, p1, color);
		}

		//
		template<class Key, class P0, class P1>
		void universal(const P0& p0, const P1& p1, const Color& color, Key key)
		{
			universal_impl(p0, p1, color, renderer(key));
		}

		//
		template<class P0>
		void universal(const P0& p0, const Color& color, RendererUsingInterface& state)
		{
			state.add(p0, color);
		}

		
		//
		//
		//
		void lines( const Point2* vertices, int count, const Color& color, float thickness, bool antialiased )
		{
			if(gapi::initialized(gapi::DX9))
			{
				//draw::dx9::lines(vertices, count, color, thickness, antialiased);
			}
			else if (gapi::initialized(gapi::DX11))
			{
				for (int i = 0; i < count / 2; ++i)
				{
					int idx = i * 2;

					auto& state = renderer<SolidLine>();
					math::Rect<float> r(vertices[idx], vertices[idx + 1] - vertices[idx]);
					state.add(r, vertices[idx], vertices[idx + 1], thickness, color);
				}							
			}
		}

		void line(const Point2& p0, const Point2& p1, float thickness, const Color& color )
		{
			Point2 v[2] = {p0, p1};
			lines(v, 2, color, thickness, p0.x != p1.x && p0.y != p1.y);
		}

		//
		//
		//
		void line(const Point2& p0, const Point2& p1, const Color& color)
		{
			line(p0, p1, 1.0f, color);
		}

		//
		//
		//
		void linerect(const Rect& rect, const Color& color)
		{
			line(rect.leftTop(), rect.rightTop(), color);
			line(rect.leftBottom(), rect.rightBottom(), color);
			line(rect.leftTop(), rect.leftBottom(), color);
			line(rect.rightTop(), rect.rightBottom(), color);
		}

		void linestrip( const Point2* points, int count, float thickness, const Color& color )
		{
			lines(points, count, color, thickness, true);
		}

		void lineloop( const Point2* points, int count, float thickness, const Color& color )
		{
			Point2* vertices = new Point2[count + 1];
			mem::memcpy(vertices, points, count * sizeof(Point2));
			vertices[count] = points[0];

			lines( vertices, count, color, thickness, true );

			delete [] vertices;
		}

		void circle( Point2 center, float radius, int segments, float thickness, const Color& color )
		{
			int npoints = segments * 2;

			Point2* points = new Point2 [npoints];

			float delta = nm::TwoPI / segments;


			float m = 0.0f;
			for( int i = 0; i < npoints; ++i )
			{
				float x = points[i].x = center.x + ( cosf( m ) * radius );
				float y = points[i].y = center.y + ( sinf( m ) * radius );

				m += delta;
			}
			lineloop( points, npoints, thickness, color );

			delete [] points;
		}

		void linerect( Rect& rect, float thickness, const Color& color )
		{
			line( rect.rightTop(), rect.rightBottom(), thickness, color );
 			line( rect.leftTop(), rect.leftBottom(), thickness, color );
//  			line(rect.leftTop(), rect.rightTop(), thickness, color);
//  			line(rect.rightBottom(), rect.leftBottom(), thickness, color);
		}

		template<class AddData>
		void sizeable_table(const math::SizeableTable<float, AddData>& table, float thickness, const Color& color)
		{
			int rc, cc;
			table.getStructSize(cc, rc);

			for (int i = 0; i < cc; ++i)
			{
				for (int j = 0; j < rc; ++j)
				{
					linerect(table.cell(i, j), thickness, color);
				}
			}
		}

		namespace Private
		{
			void curvePoints(Point2*& points, int& count, const Rect& rect, const nm::curve<float>& crv)
			{
				Range<float> keyRange; crv.getParameterRange(keyRange);
				Range<float> valRange; crv.getValueRange(valRange);

				float keyRangeWidth = keyRange.maxValue - keyRange.minValue;
				float valRangeWidth = valRange.maxValue - valRange.minValue;
				float invKeyRangeWidth = 1.0f / keyRangeWidth;
				float invValRangeWidth = 1.0f / valRangeWidth;
				float w = rect.size.x;
				float h = rect.size.y;
				float x = rect.pos.x;
				float y = rect.pos.y;

				int cnt = crv.size();
				points = new Point2[cnt];
				for (int i = 0; i < cnt; ++i)
				{
					float px, py;
					crv.getValue(i, px, py);
					px *= invKeyRangeWidth * w;
					py *= invValRangeWidth * h;
					points[i].x = px + x;
					points[i].y	= h*0.5f - (py - y);
				}
				count = cnt;
			}
		}

		void curve(const Rect& rect, const nm::curve<float>& crv, float thickness, const Color& color)
		{
			linerect(rect, color);

			Point2* points;
			int count;
			Private::curvePoints(points, count, rect, crv);
			lines(points, count, color, thickness, true);

			delete [] points;
		}

		//
		//
		//
		template<class TexType>
		void texrectT(const Rect& rect, const Rect& texrect, TexType texture, const Color& clr)
		{
			//делаем поправку на пол пикселя чтобы попасть точно
			Rect trect = texrect;
 			trect.pos.x += 0.5f;
 			trect.pos.y += 0.5f;
 			trect.size.x -= 0.5f;
 			trect.size.y -= 0.5f;
			universal(rect, trect, clr, texture);
		}


		template<>	void texrect(const Rect& rect, const Rect& texrect, Resources::Texture* texture, const Color& clr)	{	texrectT(rect, texrect, texture, clr);	}
		template<>	void texrect(const Rect& rect, const Rect& texrect, Resources::Texture11* texture, const Color& clr)	{	texrectT(rect, texrect, texture, clr);	}

		template<>
		void texrect(const Rect& rect, const Rect& texrect, LPDIRECT3DTEXTURE9 texture, const Color& clr)
		{
			texrectT(rect, texrect, texture, clr);
			unigui::draw::flush();
		}

		template<>
		void texrect(const Rect& rect, const Rect& texrect, multiapi::Texture* texture, const Color& clr)
		{
			texrectT(rect, texrect, texture, clr);
			unigui::draw::flush();
		}

 		template<>
 		void texrect(const Rect& rect, const Rect& texrect, ::dx11::PlainTarget* texture, const Color& clr)
 		{
 			texrectT(rect, texrect, texture, clr);
 			unigui::draw::flush();
 		}

		template<>
		void texrect(const Rect& rect, const Rect& texrect, ::dx11::DepthStencil* texture, const Color& clr)
		{
			texrectT(rect, texrect, texture, clr);
			unigui::draw::flush();
		}


		void texrect(const Point2& screenPos, const Point2& aspect, Resources::Atlas* atlas, int index, const Color& clr)
		{
			atlas->resource([index, &screenPos, &aspect, &clr, atlas](auto& a)
			{
				RectAtlas::Rect r = a.atlas.absoluteRect(index);
				Rect trect(r.minX, r.minY, r.sizeX(), r.sizeY());
				Rect rect(screenPos.x, screenPos.y, trect.size.x, trect.size.y);
				rect.pos *= aspect;
				rect.size *= aspect;
				universal(rect, trect, clr, atlas);
			});
		}

		void texrect(const Point2& screenPos, Resources::Atlas* atlas, int index, const Color& clr)
		{
			atlas->resource([index, &screenPos, &clr, atlas](auto& a)
			{
				RectAtlas::Rect r = a.atlas.absoluteRect(index);
				Rect trect(r.minX, r.minY, r.sizeX(), r.sizeY());
				Rect rect(screenPos.x, screenPos.y, trect.size.x, trect.size.y);
				universal(rect, trect, clr, atlas);
			});
		}

		void texrect(Resources::Atlas* atlas, int index, const Color& clr)
		{
			atlas->resource([index, &clr, atlas](auto a)
			{
				const RectAtlas::Rect& absRect = a.atlas.absoluteRect(index);
				Point2 screenPos;
				screenPos.x = absRect.orgX;
				screenPos.y = absRect.orgY;
				texrect(screenPos, atlas, index, clr);
			});
		}

		Rect texrect(const Rect& parentRect, Resources::Atlas* atlas, Align align, int index, const Color& clr)
		{
			Rect rect(0, 0, 0, 0);
			atlas->resource([index, &clr, &parentRect, align, &rect, atlas](auto& a)
			{
				RectAtlas::Rect r = a.atlas.absoluteRect(index);
				Rect trect(r.minX, r.minY, r.sizeX(), r.sizeY());
				rect = Rect(0, 0, trect.size.x, trect.size.y);

				alignRect(rect, parentRect, align);
				universal(rect, trect, clr, atlas);
			});

			return rect;
		}

		void texrect(const IndexedSizeableTable& t, const RectAtlasGAPI& atl, const Color& clr)
		{
			int rows, columns;
			t.getStructSize(columns, rows);

			for (int c = 0; c < columns; ++c)
			{
				for (int r = 0; r < rows; ++r)
				{
					Rect cell = t.cell(c, r);
					int index = t.addData[c][r];

					const RectAtlas::Rect& ar = atl.atlas.absoluteRect(index);
					Rect trect(ar.minX, ar.minY, ar.sizeX(), ar.sizeY());
					universal(cell, trect, color(1.0f, 1.0f, 1.0f, index >= 0 ? 1.0f : 0.0f), (RectAtlasGAPI*) &atl);
				}
			}
		}

		void texrect(const IndexedSizeableTable& t, Resources::Atlas* atlas, const Color& clr)
		{
			atlas->resource([&t, &clr](auto& a)
			{
				texrect(t, (const RectAtlasGAPI&)a, clr);
			});
		}

		void texrect(const IndexedSizeableTable& table, float leftScissor, float rightScissor, float topScissor, float bottomScissor, Resources::Atlas* atlas, const Color& clr)
		{
			atlas->resource([&table, &clr, atlas, &leftScissor, &rightScissor, &topScissor, &bottomScissor](auto& a)
			{
				const RectAtlas& atl = a.atlas;

				IndexedSizeableTable t = table;

				int rows, columns;
				t.getStructSize(columns, rows);

				int startC;
				for (startC = 0; startC < columns; ++startC)
				{
					IndexedSizeableTable::Line& column = t.column(startC);
					leftScissor -= column.size;
					if(leftScissor <=0)
					{
						leftScissor = 0;
						break;
					}
				}


				for (int c = startC; c < columns; ++c)
				{
					IndexedSizeableTable::Line& column = t.column(c);

					for (int r = 0; r < rows; ++r)
					{
						int index = t.addData[c][r];
						Rect cell = t.cell(c, r);
						RectAtlas::Rect ar = atl.absoluteRect(index);

						if(column.autosize)
						{
							cell.pos.x += leftScissor;
						}
						else
						{
							cell.pos.x += leftScissor;
							ar.minX += leftScissor;
						}						

						Rect trect(ar.minX, ar.minY, ar.sizeX(), ar.sizeY());
						universal(cell, trect, color(1.0f,1.0f,1.0f,index >= 0 ? 1.0f: 0.0f), atlas);
					}
				}	
			});
		}


		//
		//
		//
		void rect(const Rect& rect, const Color& color)
		{
			universal(rect, color, renderer<SolidRect>() );
		}

		//
		//
		//
		void character(const Rect& rect, const Rect& texrect, Resources::Font* font, const Color& c, bool stroked)
		{
			universal(rect, texrect, c, font);
		}

		//
		//
		//
		void print(const char* text, const Rect& rect, Resources::Font* font, const Color& color, bool stroked)
		{
			font->resource([text, &rect, &color, stroked](auto& f)
			{
				f.print(text, rect, color, stroked);
			});
		}

		void print(const char* text, const Rect& rect, Resources::Font* font, bool stroked)
		{
			print(text, rect, font, color(1,1,1,1), stroked);
		}

		int stringWidth(const char* text, Resources::Font* font)
		{
			int result = 0;
			font->resource([text, &result](auto& f)
			{
				result = f.face.stringWidth(text);
			});

			return result;
		}

		int stringHeight(const char* text, Resources::Font* font)
		{
			int result = 0;
			font->resource([text, &result](auto& f)
			{
				result = f.face.stringHeight(text);
			});

			return result;
		}

		int charWidth(char c, Resources::Font* font)
		{
			char str[2] = {c, 0};
			return stringWidth(str, font);
		}

		//
		//
		//
		void textureSize(float& w, float& h, LPDIRECT3DTEXTURE9 texture)
		{
			nm::index2 s = texutils::dx9::GetTextureSize(texture);
			w = (float) s.x;
			h = (float) s.y;
		}

		void textureSize(float& w, float& h, multiapi::Texture* texture)
		{
			w = (float) texture->sizeX;
			h = (float) texture->sizeY;
		}

		void textureSize(float& w, float& h, Resources::Texture* texture)
		{
			w = (float) texture->sizeX;
			h = (float) texture->sizeY;
		}

		void textureSize(float& w, float& h, Resources::Texture11* texture)
		{
			int iw, ih;
			texture->Size(iw, ih);
			w = (float) iw;
			h = (float) ih;
		}

		Rect textureRect(LPDIRECT3DTEXTURE9 texture)
		{
			float fSizeX, fSizeY;
			textureSize(fSizeX, fSizeY, texture);
			return Rect(0, 0, fSizeX, fSizeY);
		}

		Rect textureRect(multiapi::Texture* texture)
		{
			float fSizeX, fSizeY;
			textureSize(fSizeX, fSizeY, texture);
			return Rect(0, 0, fSizeX, fSizeY);
		}

		Rect textureRect(Resources::Texture* texture)
		{
			float fSizeX, fSizeY;
			textureSize(fSizeX, fSizeY, texture);
			return Rect(0, 0, fSizeX, fSizeY);
		}

		Rect textureRect(Resources::Texture11* texture)
		{
			float fSizeX, fSizeY;
			textureSize(fSizeX, fSizeY, texture);
			return Rect(0, 0, fSizeX, fSizeY);
		}

}
}
