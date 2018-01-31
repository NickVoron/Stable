// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "progressBar.h"
#include "../focus.h"
#include "../control.h"
#include "../controlTypes.h"
#include "../drawing/drawing.h"
#include "../globals.h"
#include "../imgui.h"


namespace imgui
{

	struct V 
	{
		template<class T, class... Args> void operator()(T& t, Args&&... args)
		{
			t.val(std::forward<Args>(args)...);
		}
	};

	
	namespace detail {

		
		void progressBarLogic(const Control& ctrl, Rect& rect, float startVal, float endVal, float& val)
		{
			if (ctrl.mouseLeftDown || ctrl.mouseLeftDrag)
			{
				float x;
				Point2 mPos = mouse.pos - rect.pos;
				x = mPos.x / rect.size.x;

				if (x >= 0 || ctrl.mouseLeftDrag)
				{
					val = nm::lerp(startVal, endVal, x);
				}
			}

			val = nm::clamp(val, startVal, endVal);
		}
	}

	namespace
	{
		int inputFieldWidth()
		{
			return 50;
		}

		int charMaxWidth(Resources::Font* font)
		{
			int res = -1;

			static const char* s = "0123456789-,.";
			for (int i = 0; i < 13; ++i)
			{
				int w = unigui::draw::charWidth(s[i], font);
				if (res < w)
					res = w;
			}

			return res;
		}

		const char* selectFormat(float minVal, float maxVal, int inputFieldLen, Resources::Font* font)
		{
			static const int formatsCount = 5;
			static const char* formats[formatsCount] = { "%-02.4f", "%-02.3f", "%-02.2f", "%-02.1f", "%-02.0f" };

			char buf[32];

			int cmw = charMaxWidth(font);
			int rlen = 1000000;
			int ri = 0;
			for (int i = 0; i < formatsCount; ++i)
			{
				sprintf(buf, formats[i], minVal);
				int minLen = strlen(buf);

				sprintf(buf, formats[i], maxVal);
				int maxLen = strlen(buf);

				int len = std::max(minLen, maxLen) * cmw;

				if (len <= inputFieldLen)
					return formats[i];

				if (rlen > len)
				{
					rlen = len;
					ri = i;
				}
			}

			return formats[ri];
		}
	}


	struct ProgressBarResource
	{
		void init(int inFieldLen)
		{
			detail::guiAtlas->resource([this](auto& a)
			{
				const RectAtlas& atl = a.atlas;
				table.setStructSize(3, 1);

				table.addData[0][0] = atl.index("Slider_El_L");
				table.addData[1][0] = atl.index("Slider_El_M");
				
				
				table.addData[2][0] = atl.index("Slider_El_R");

				unigui::initIndexedTableFromAtlas(table, atl);

				
				table.column(1).autosize = true;

				gauge.setStructSize(1, 1);
				gauge.addData[0][0] = atl.index("Slider_Gauge");

				unigui::initIndexedTableFromAtlas(gauge, atl);
				gauge.column(0).autosize = true;

				table.rebuild();
				fieldSize = table.cellsUnion(0, 1, 0, 0).size;
			});
		}

		void build(const Control& ctrl, Rect& gaugeRect, Rect& fieldRect, Rect& emptyRect, float& leftScissor, float minVal, float maxVal, float& val)
		{
			calculateGaugeRect(ctrl.rect, gaugeRect);

			fieldRect.pos = ctrl.rect.pos;
			fieldRect.size = fieldSize;
			fieldRect.pos.x += 2;
			fieldRect.size.x -= 2;

			detail::progressBarLogic(ctrl, gaugeRect, minVal, maxVal, val);

			float part = (val - minVal) / (maxVal - minVal);
			float emptyLen = gaugeRect.size.x * (1.0f - part);

			emptyRect.size.x = emptyLen;
			emptyRect.size.y = gauge.row(0).minSize;
			emptyRect.toCenter(gaugeRect);
			emptyRect.toRight(gaugeRect);

			gauge.build(emptyRect);
			gauge.toRight(emptyRect);

			float minX = gauge.getMinSize().x;
			leftScissor = minX > emptyLen ? minX - emptyLen : 0.0f;
		}

		void calculateGaugeRect(const Rect& rect, Rect& gaugeRect)
		{
			table.build(rect);

			gaugeRect = table.cellsUnion(0, 2, 0, 0);
			gaugeRect.size.x -= 4.0f;
			gaugeRect.pos.x += 2;
		}

		unigui::IndexedSizeableTable table;
		unigui::IndexedSizeableTable gauge;
		Point2 fieldSize;
	};

	void progressBar(const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val)
	{

		core::checked_begin();

		ProgressBarResource resource;
		resource.init(inputFieldLength);

		Control ctrl(controls::SLIDER, Rect(pos.x, pos.y, (float)length, resource.fieldSize.y));


		float leftScissor;
		Rect gaugeRect, fieldRect, emptyRect;
		resource.build(ctrl, gaugeRect, fieldRect, emptyRect, leftScissor, minVal, maxVal, val);

		unigui::draw::texrect(resource.table, detail::guiAtlas);
		unigui::draw::texrect(resource.gauge, 0, 0, 0, 0, detail::guiAtlas);

		core::checked_end();
	}
}



// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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