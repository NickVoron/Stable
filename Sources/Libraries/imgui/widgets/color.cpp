#include "color.h"
#include "slider.h"
#include "panel.h"

#include "../focus.h"
#include "../control.h"
#include "../controlTypes.h"
#include "../globals.h"
#include "../imgui.h"

#include "../drawing/drawing.h"

#include "../layout/layout.h"
#include "../layout/transform.h"
#include "../layout/area.h"
#include "../layout/shift.h"


#include "colorSpaces/library.include.h"

namespace imgui
{
	struct ColorBoxResource
	{
		void init()
		{
			detail::guiAtlas->resource([this](auto& a)
			{
				const RectAtlas& atl = a.atlas;
				table.setStructSize(3, 3);

				table.addData[0][0] = -1;
				table.addData[1][0] = atl.index("CB_M");
				table.addData[2][0] = -1;

				table.addData[0][1] = atl.index("CB_M");
				table.addData[1][1] = atl.index("CB_M");
				table.addData[2][1] = atl.index("CB_M");

				table.addData[0][2] = -1;
				table.addData[1][2] = atl.index("CB_M");
				table.addData[2][2] = -1;

				unigui::initIndexedTableFromAtlas(table, atl);

				table.column(1).autosize = true;
				table.row(1).autosize = true;
				table.rebuild();
			});
		}

		void build(const Rect& rect)
		{
			table.build(rect);
		}

		unigui::IndexedSizeableTable table;
	};

	struct ColorLettersResource
	{
		enum
		{
			R, G, B,
			H, S, V,
			A,
			COUNT
		};

		void init()
		{
			detail::guiAtlas->resource([this](auto& a)
			{
				const char* letterNames[] = {"LT_R", "LT_G", "LT_B", "LT_H", "LT_S", "LT_V", "LT_A"};
				const RectAtlas& atl = a.atlas;

				for (int i = R; i < COUNT; ++i)
				{
					tables[i].setStructSize(1, 1);

					tables[i].addData[0][0] = atl.index( letterNames[i] );
					unigui::initIndexedTableFromAtlas(tables[i], atl);
					tables[i].rebuild();
				}				
			});
		}

		float draw(int letterIndex, const Point2& pos)
		{
			tables[letterIndex].build(Rect(pos, Point2(0, 0)));
			float bottom = tables[letterIndex].getActualRect().bottom();
			unigui::draw::texrect(tables[letterIndex], detail::guiAtlas);			

			return bottom;
		}

		unigui::IndexedSizeableTable tables[COUNT];
	};

	Point2 drawColorBox(const Point2& pos, int colorsCount,const unigui::Color& c)
	{
		float h = (float)colorsCount * 11 + 2 * (colorsCount-1);
		Point2 colorBoxPos = pos;
		Point2 colorBoxSize(37.0f, h);
		Rect colorBoxRect(colorBoxPos, colorBoxSize);
		ColorBoxResource resource;
		resource.init();
		resource.build(colorBoxRect);
		Point2 leftD(1.0f, 1.0f);
		Point2 rightD(-2.0f, -2.0f);
		unigui::draw::texrect(resource.table, detail::guiAtlas);
		unigui::draw::rect(Rect(colorBoxPos + leftD, colorBoxSize + rightD), c);
		

		return resource.table.getActualRect().rightBottom();
	}

	template<bool rgb>
	void letters(const Point2& pos, bool alpha)
	{
		ColorLettersResource resource;
		resource.init();

		int start = rgb ? ColorLettersResource::R : ColorLettersResource::H;

		Point2 curPos = pos;
		for (int i = start; i < start + 3; ++i)
		{
			float bottom = resource.draw(i, curPos);
			curPos.y = bottom + 2.0f;
		}

		if(alpha)
		{
			resource.draw(ColorLettersResource::A, curPos);
		}
	}

	template<bool rgb>
	void colorInfo(const Point2& pos, float& v0, float& v1, float& v2, float* v3)
	{
		float r, g, b;

		if(rgb)
		{
			r = v0; g = v1; b = v2;
		}
		else
		{
			image::hsv_to_rgb(v0, v1, v2, r, g, b);
		}

		Point2 colorBoxPos = pos;
		Point2 rightBottom = drawColorBox(colorBoxPos, 3, unigui::color(r, g, b, 1.0f));

		if(v3)
		{
			Point2 alphaBoxPos = pos;
			alphaBoxPos.y = rightBottom.y + 2;
			float a = *v3;
			drawColorBox(alphaBoxPos, 1, unigui::color(a, a, a, 1.0f));
		}  		

		Point2 lettersPos = colorBoxPos;
		lettersPos.x = rightBottom.x + 2;
		letters<rgb>(lettersPos, v3 != 0);
	}

	void sliders(const Point2& pos, float max0, float max1, float max2, float& v0, float& v1, float& v2, float* v3)
	{
		const int inputFieldLen = 27;
		const int sliderLen = 126;

		Point2 basePos = pos; basePos.x += 52;

		slider(basePos, inputFieldLen, sliderLen, 0.0f, max0, v0); basePos.y += 13;
		slider(basePos, inputFieldLen, sliderLen, 0.0f, max1, v1); basePos.y += 13;
		slider(basePos, inputFieldLen, sliderLen, 0.0f, max2, v2); basePos.y += 13;

		if(v3)
		{
			slider(basePos, inputFieldLen, sliderLen, 0.0f, 1.0f, *v3);
		}
	}

	static const float controlW = 186.0f;
	static const float controlW3 = 45.0f;
	static const float controlW4 = 58.0f;

	template<bool control3>
	struct ColorControl
	{
		ColorControl(const char* caption, const Point2& pos)
			:ctrl(controls::COLOR_EDITOR, Rect(pos.x, pos.y, controlW, (control3 ? controlW3 : controlW4) + unigui::draw::stringHeight(caption, detail::font) + 3 ), false)
		{
			layout::pause(true);
			transform::pause(true);
		}

		~ColorControl()
		{
			layout::pause(false);
			transform::pause(false);
		}


		const Rect& rect() const { return ctrl.rect; }

		Control ctrl;
	};	

	template<bool rgb>
	void color_universal(const char* caption, const Rect& rect, float& v0, float& v1, float& v2, float* v3)
	{
		//unigui::draw::linerect(rect, unigui::color(1.0f, 1.0f, 1.0f, 1.0f));
		core::checked_begin();

		Rect panelRect = panel_draw(caption, rect, false);
		
		Point2 basePos = panelRect.pos;
		basePos.x += 4;
		basePos.y += 4;
		sliders(basePos, rgb ? 1.0f : 360.0f, 1.0f, 1.0f, v0, v1, v2, v3);
		colorInfo<rgb>(basePos, v0, v1, v2, v3);

		core::checked_end();
	}

	template<bool control3, bool rgb>
	void color_universal(const char* caption, const Point2& pos, float& v0, float& v1, float& v2, float* v3 = 0)
	{
		ColorControl<control3> ctrl(caption, pos);
		color_universal<rgb>(caption, ctrl.rect(), v0, v1, v2, v3);
	}
 

	void color_rgb(const char* caption, const Point2& pos, float& r, float& g, float& b)				{ color_universal<true, true>(caption, pos, r, g, b); }
	void color_rgba(const char* caption, const Point2& pos, float& r, float& g, float& b, float& a)		{ color_universal<false, true>(caption, pos, r, g, b, &a); }
	void color_hsv(const char* caption, const Point2& pos, float& h, float& s, float& v)				{ color_universal<true, false>(caption, pos, h, s, v);	}
	void color_hsva(const char* caption, const Point2& pos, float& h, float& s, float& v, float& a)		{ color_universal<false, false>(caption, pos, h, s, v, &a);	}

	void color_rgb(const char* caption, float& r, float& g, float& b)				{ color_rgb(caption, Point2(0, 0), r, g, b); }
	void color_rgba(const char* caption, float& r, float& g, float& b, float& a)	{ color_rgba(caption, Point2(0, 0), r, g, b, a); }
	void color_hsv(const char* caption, float& h, float& s, float& v)				{ color_hsv(caption, Point2(0, 0), h, s, v); }
	void color_hsva(const char* caption, float& h, float& s, float& v, float& a)	{ color_hsva(caption, Point2(0, 0), h, s, v, a); }

	void color_rgb_as_hsv(const char* caption, const Point2& pos, float& r, float& g, float& b)
	{
		float h, s, v;
		image::rgb_to_hsv(r, g, b, h, s, v);

		color_universal<true, false>(caption, pos, h, s, v);

		image::hsv_to_rgb(h, s, v, r, g, b);
	}

	void color_rgb_as_hsv(const char* caption, float& r, float& g, float& b)
	{
		color_rgb_as_hsv(caption, Point2(0, 0), r, g, b);
	}

	void color_rgb(const Point2& pos, float& r, float& g, float& b)				{ color_rgb("", pos, r, g, b); }
	void color_rgba(const Point2& pos, float& r, float& g, float& b, float& a)	{ color_rgba("", pos, r, g, b, a); }
	void color_hsv(const Point2& pos, float& h, float& s, float& v)				{ color_hsv("", pos, h, s, v); }
	void color_hsva(const Point2& pos, float& h, float& s, float& v, float& a)	{ color_hsva("", pos, h, s, v, a); }
	void color_rgb_as_hsv(const Point2& pos, float& r, float& g, float& b)		{ color_rgb_as_hsv("", pos, r, g, b); }

	void color_rgb(float& r, float& g, float& b)			{ color_rgb("", r, g, b); }
	void color_rgba(float& r, float& g, float& b, float& a)	{ color_rgba("", r, g, b, a); }
	void color_hsv(float& h, float& s, float& v)			{ color_hsv("", h, s, v); }
	void color_hsva(float& h, float& s, float& v, float& a)	{ color_hsva("", h, s, v, a); }
	void color_rgb_as_hsv(float& r, float& g, float& b)		{ color_rgb_as_hsv("", r, g, b); }

}