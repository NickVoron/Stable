#include "panel.h"
#include "../drawing/drawing.h"
#include "../layout/shift.h"
#include "../layout/area.h"
#include "../layout/transform.h"
#include "../layout/layout.h"
#include "../focus.h"
#include "../control.h"
#include "../controlTypes.h"
#include "../globals.h"
#include "../imgui.h"

#include "slider.h"

namespace imgui
{
	struct PanelResource
	{
		void init(bool dark)
		{
			detail::guiAtlas->resource([this, dark](auto& a)
			{
				const RectAtlas& atl = a.atlas;
 				table.setStructSize(3, 3);
 
 				table.addData[0][0] = atl.index("GB_L_UP");
 				table.addData[1][0] = atl.index("GB_M_UP");
 				table.addData[2][0] = atl.index("GB_R_UP");
 
  				table.addData[0][1] = atl.index("GB_M_Hor_L");
				table.addData[1][1] = atl.index(dark ? "CP_GR1_TB" : "CP_GR0_TB");
  				table.addData[2][1] = atl.index("GB_M_Hor_R");
  
  				table.addData[0][2] = atl.index("GB_L_DOWN");
  				table.addData[1][2] = atl.index("GB_M_DOWN");
  				table.addData[2][2] = atl.index("GB_R_DOWN");

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

	struct PanelTextDecorResource
	{
		void init(bool dark)
		{
			detail::guiAtlas->resource([this, dark](auto& a)
			{
				const RectAtlas& atl = a.atlas;

				left.setStructSize(2, 2);
				right.setStructSize(2, 2);

				const char* type = dark ? "GR0_TB_M" : "GR1_TB_M";
				left.addData[0][0] = atl.index(type);
				left.addData[1][0] = atl.index(type);

				left.addData[0][1] = atl.index(type);
				left.addData[1][1] = -1;

				right.addData[0][0] = atl.index(type);
				right.addData[1][0] = atl.index(type);

				right.addData[0][1] = -1;
				right.addData[1][1] = atl.index(type);

				unigui::initIndexedTableFromAtlas(left, atl);
				unigui::initIndexedTableFromAtlas(right, atl);

				left.column(1).autosize = true;
				left.row(1).autosize = true;
				left.rebuild();

 				right.column(0).autosize = true;
 				right.row(1).autosize = true;
 				right.rebuild();
			});
		}

		void draw(const Rect& parentRect, const Rect& rect)
		{
			float rShift = 5.0f;
			float prShift = 20.0f;

			//unigui::draw::linerect(rect, unigui::color(1,1,1,1));

			float ld = rect.left() - parentRect.left() - rShift;
   			Rect leftRect = rect;
			leftRect.pos.x = parentRect.left() + prShift;
			leftRect.size.x = ld - prShift;
			leftRect.size.y = rect.halfHeight();
			leftRect.pos.y += rect.halfHeight();

			float rd = parentRect.right() - rect.right() - rShift;
			Rect rightRect = rect;
			rightRect.pos.x = rect.right() + rShift;
			rightRect.size.x = rd - prShift;
			rightRect.size.y = rect.halfHeight();
			rightRect.pos.y += rect.halfHeight();


			left.build(leftRect);
			right.build(rightRect);
			unigui::draw::texrect(left, detail::guiAtlas);
			unigui::draw::texrect(right, detail::guiAtlas);
		}

		unigui::IndexedSizeableTable left;
		unigui::IndexedSizeableTable right;
	};



	void panel_draw(const Rect& rect, bool dark)
	{
		PanelResource resource;
		resource.init(dark);
		resource.build(rect);
		unigui::draw::texrect(resource.table, detail::guiAtlas);
	}

	void panel_draw(float x, float y, float w, float h, bool dark)
	{
		panel_draw(Rect( x, y, w, h ), dark);
	}

	Rect panel_draw(const char* caption, const Rect& rect, bool dark)
	{
		core::checked_begin();

//		unigui::draw::linerect(rect, unigui::color(1.0f, 1.0f, 1.0f, 1.0f));
		int h = unigui::draw::stringHeight(caption, detail::font) + 3;
		int w = unigui::draw::stringWidth(caption, detail::font);
		
		Rect panelRect = rect;

		panelRect.pos.y += h;
		panelRect.size.y -= h;

		Rect captionRect = rect;
		captionRect.size.y = (float)h;
		captionRect.size.x = (float)w;
		captionRect.toCenter(rect);
		captionRect.toTop(rect);

		if(w > 0)
		{
			PanelTextDecorResource textDecor;
			textDecor.init(dark);
			textDecor.draw(panelRect, captionRect);		
		}	

		unigui::draw::print(caption, captionRect, detail::font, unigui::color(1.0f, 1.0f, 1.0f, 1.0f), true);
		panel_draw(panelRect, dark);

		core::checked_end();

		return panelRect;
	}

	Rect panel_draw(const char* caption, float x, float y, float w, float h, bool dark)
	{
	   return panel_draw(caption, Rect( x, y, w, h ), dark);
	}

	void panel_begin(const Rect& irect, bool visible)
	{
		Control ctrl(controls::PANEL, irect, false);

		if(visible)
		{
			panel_draw(ctrl.rect, true);			
		}
		
		area::begin(ctrl.rect, shift::total());
		shift::begin(ctrl.rect);
		transform::begin(irect);
	}

	void panel_begin(float x, float y, float w, float h, bool visible)
	{
		panel_begin(Rect( x, y, w, h ), visible);
	}

	void panel_end()
	{
		transform::end();
		shift::end();
		area::end();
	}

	void vertical_panel_begin(const Rect& rect, bool visible)
	{
		panel_begin( rect, visible );
		layout::begin_default_vertical();
	}

	void vertical_panel_begin(float x, float y, float w, float h, bool visible)
	{
		vertical_panel_begin(Rect(x, y, w, h), visible);
	}

	void vertical_panel_end()
	{
		layout::end();
		panel_end();
	}

	void scrollable_panel_begin(const Rect& rect)
	{
		panel_begin(rect, false);
	}

	void scrollable_panel_end()
	{
		if(area::rect().contain(detail::mouse.pos))
		{
			Point2 ns = area::neededSize();
			if( ns.y > 0 || ns.x > 0 )
			{
				Point2 s = shift::current();

				s.y += detail::mouse.wheelDelta;
				s.y = nm::clamp(s.y, -ns.y, 0.0f);

// 				const int w = 20;
// 				if( ns.y > 0)
// 				{
// 					Rect sldr = area::rect();
// 					sldr.pos.x += sldr.size.x;
// 					sldr.size.x = w;
// 
// 					float y = s.y;
// 					vslider(sldr, 0.0f, -(float)ns.y, y);
// 					s.y = y;
// 					shift::set(s);
// 				}			

				/*
				if( ns.x > 0)
				{
				Rect sldr = area::rect();
				sldr.pos.y += sldr.size.y;
				sldr.size.y = w;

				float x = s.x;
				slider(sldr, 0.0f, -(float)ns.x, x);
				s.x = x;
				}	*/


				shift::set(s);
			}
			else
			{
				shift::set(Point2());
			}
		}
		panel_end();
	}

	void dragable_begin(const Rect& rect)
	{
		panel_begin(rect, false);
	}

	void dragable_end()
	{
		panel_end();
	}

}

