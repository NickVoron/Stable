#include "button.h"
#include "../drawing/drawing.h"
#include "../controlTypes.h"
#include "../focus.h"
#include "../control.h"
#include "../globals.h"
#include "../imgui.h"

#include "resourceAtlas/library.include.h"
#include "resourceFont/library.include.h"


namespace imgui					
{
	namespace detail
	{		
		bool buttonLogic(Rect& rect, ButtonState& resultState)
		{		  
			Control ctrl(controls::BUTTON, rect);
			rect = ctrl.rect;
			if( ctrl.mouseHover )
			{
				resultState = UNDER_MOUSE;
				if( ctrl.mouseLeftDrag )
				{
					resultState = HOT;
					return false;
				}
				else if(ctrl.mouseLeftClick)
				{	
					return true;
				}
			}
			return false;
		}

	}// detail

	struct ButtonResource 
	{
		void init(Rect& rect)
		{
			rcolors[0] = unigui::color(0.5f, 0.5f, 0.5f, 1.0f);
			rcolors[1] = unigui::color(0.9f, 0.9f, 0.5f, 1.0f);
			rcolors[2] = unigui::color(1.0f, 0.5f, 0.5f, 1.0f);

			tcolors[0] = unigui::color(1.0f, 1.0f, 1.0f, 1.0f);
			tcolors[1] = unigui::color(1.0f, 1.0f, 0.0f, 1.0f);
			tcolors[2] = unigui::color(1.0f, 0.0f, 0.0f, 1.0f);

			static const char* normal[] = { "Base_L", "Base_Scale", "Base_R" };
			static const char* select[] = { "Select_L", "Select_Scale", "Select_R" };
			static const char* push[]	= { "Push_L", "Push_Scale", "Push_R" };

			detail::guiAtlas->resource([this](auto& a)
			{
				const RectAtlas& atl = a.atlas;
				for (int i = NORMAL; i < BUTTON_STATES_COUNT; ++i)
				{
					const char** current = (i == NORMAL) ? normal : ( (i == UNDER_MOUSE) ? select : push );

					table[i].setStructSize(3, 1);

					table[i].addData[0][0] = atl.index( current[0] );
					table[i].addData[1][0] = atl.index( current[1] );
					table[i].addData[2][0] = atl.index( current[2] );

					unigui::initIndexedTableFromAtlas(table[i], atl);
					table[i].column(1).autosize = true;
				}				
			});

			rect.size.y = table[0].row(0).minSize;

		}

		unigui::IndexedSizeableTable table[BUTTON_STATES_COUNT];
		Color rcolors[BUTTON_STATES_COUNT];
		Color tcolors[BUTTON_STATES_COUNT];
	};

bool button(const char* caption, const Point2& pos, int length)
{
	core::checked_begin();

	Rect rect(pos.x, pos.y, (float)length, 0.0f);
	
	ButtonResource resource;
	resource.init( rect );

	ButtonState state = NORMAL;
	bool res = detail::buttonLogic(rect, state);

	resource.table[state].build(rect);

	/*if(res)	focus::set(ctrl.id);*/

	unigui::draw::texrect(resource.table[state], detail::guiAtlas);
	unigui::draw::print(caption, rect, detail::font, resource.tcolors[state], true);

	core::checked_end();
	return res;
}

bool button(const char* caption)
{
	return button(caption, Point2(0.0f, 0.0f), 100);
}


}//