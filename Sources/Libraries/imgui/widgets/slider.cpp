#include "slider.h"
#include "../focus.h"
#include "../control.h"
#include "../controlTypes.h"
#include "../drawing/drawing.h"
#include "../globals.h"
#include "../imgui.h"

namespace imgui
{		
namespace detail
{

	//
	void sliderLogic(const Control& ctrl, Rect& rect, float startVal, float endVal, float& val)
	{
		if( ctrl.mouseLeftDown || ctrl.mouseLeftDrag )
		{
			float x;
			Point2 mPos = mouse.pos - rect.pos;
			x = mPos.x / rect.size.x;

			if(x >= 0 || ctrl.mouseLeftDrag)
			{
				val = nm::lerp(startVal, endVal, x);
			}			
		}

		val = nm::clamp(val, startVal, endVal);
	}
}
//

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
		for(int i = 0; i < 13; ++i)
		{
			int w = unigui::draw::charWidth(s[i], font);
			if(res < w)
				res = w;
		}

		return res;
	}

	const char* selectFormat(float minVal, float maxVal, int inputFieldLen, Resources::Font* font)
	{
		static const std::size_t formatsCount = 5;
		static const char* formats[formatsCount] = { "%-02.4f", "%-02.3f", "%-02.2f", "%-02.1f", "%-02.0f" };

		char buf[32]; 
		
		int cmw = charMaxWidth(font);
		std::size_t rlen = 1000000;
		std::size_t ri = 0;
		for(std::size_t i = 0; i < formatsCount; ++i)
		{
			sprintf(buf, formats[i], minVal);
			auto minLen = strlen(buf);

			sprintf(buf, formats[i], maxVal);
			auto maxLen = strlen(buf);

			auto len = std::max(minLen, maxLen) * cmw;

			if(len <= inputFieldLen)
				return formats[i];

			if(rlen > len)
			{
				rlen = len;
				ri = i;
			}
		}

		return formats[ri];
	}
}

struct SliderResource
{
	void init(int inFieldLen)
	{
		detail::guiAtlas->resource([this, inFieldLen](auto& a)
		{
			const RectAtlas& atl = a.atlas;
			table.setStructSize(5, 1);

			table.addData[0][0] = atl.index("Slider_El_L");
			table.addData[1][0] = atl.index("Slider_El_M");
			table.addData[2][0] = atl.index("Slider_El_C");
			table.addData[3][0] = atl.index("Slider_El_M");
			table.addData[4][0] = atl.index("Slider_El_R");

			unigui::initIndexedTableFromAtlas(table, atl);

			table.column(1).minSize	= (float)inFieldLen;
			table.column(3).autosize = true;

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

		detail::sliderLogic(ctrl, gaugeRect, minVal, maxVal, val);

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

	void calculateGaugeRect(const Rect& rect,  Rect& gaugeRect)
	{
		table.build(rect);

		gaugeRect = table.cellsUnion(3, 4, 0, 0);
 		gaugeRect.size.x -= 2.0f;
	}

	unigui::IndexedSizeableTable table;
	unigui::IndexedSizeableTable gauge;
	Point2 fieldSize;
};

float slider(const char* caption, const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val)
{
	core::checked_begin();

	SliderResource resource;
	resource.init(inputFieldLength);

	//	Control ctrlInput(controls::SLIDER_INPUT_FIELD, Rect(pos, resource.fieldSize), false);
	Control ctrl(controls::SLIDER, Rect(pos.x, pos.y, (float)length, resource.fieldSize.y));


	float leftScissor;
	Rect gaugeRect, fieldRect, emptyRect;
	resource.build(ctrl, gaugeRect, fieldRect, emptyRect, leftScissor, minVal, maxVal, val);

	unigui::draw::texrect(resource.table, detail::guiAtlas);
	unigui::draw::texrect(resource.gauge, 0, 0, 0, 0, detail::guiAtlas);

	unigui::draw::print(caption, gaugeRect, detail::font, unigui::color(0.0f, 0.0f, 0.0f, 1.0f), false);

	char buf[32]; sprintf(buf, selectFormat(minVal, maxVal, inputFieldLength, detail::font), val);
	int sw = unigui::draw::stringWidth(buf, detail::font);

	unigui::draw::print(buf, fieldRect, detail::font, unigui::color(0.0, 0.0, 0.0, 1.0f), false);

	core::checked_end();

	return val;
}

float slider(const char* caption, int inputFieldLength, int length, float minVal, float maxVal, float& val)
{
	return slider(caption, imgui::Point2(0, 0), inputFieldLength, length, minVal, maxVal, val);
}

float slider(const char* caption, const Point2& pos, int length, float minVal, float maxVal, float& val)
{
	return slider(caption, pos, inputFieldWidth(), length, minVal, maxVal, val);
}

float slider(const char* caption, int length, float minVal, float maxVal, float& val)
{
	return slider(caption, imgui::Point2(0, 0), inputFieldWidth(), length, minVal, maxVal, val);
}

float slider(const char* caption, float minVal, float maxVal, float& val)
{
	return slider(caption, 150, minVal, maxVal, val);
}

float slider(const char* caption, const Base::Interval<float>& interval, float& val)
{
	return slider(caption, interval.minValue, interval.maxValue, val);
}

float slider(const Point2& pos, int length, float minVal, float maxVal, float& val)
{
	return slider("", pos, length, minVal, maxVal, val);
}

float slider(int length, float minVal, float maxVal, float& val)
{
	return slider("", length, minVal, maxVal, val);
}

float slider(float minVal, float maxVal, float& val)
{
	return slider("", minVal, maxVal, val);
}

float slider(const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val)
{
	return slider("", pos, inputFieldLength, length, minVal, maxVal, val);
}

float slider(int inputFieldLength, int length, float minVal, float maxVal, float& val)
{
	return slider("", inputFieldLength, length, minVal, maxVal, val);
}

float slider(const char* caption, const Vector3& pos, float minVal, float maxVal, float& val)
{
 	Point2 p;
 	bool onScreen = detail::worldToScreen(pos, p);
 	return onScreen ? slider(caption, p, 150, minVal, maxVal, val) : val;		
}

float slider(const Vector3& pos, float minVal, float maxVal, float& val)
{
	return slider("", pos, minVal, maxVal, val);
}

float slider_angle(const char* caption, float& val)
{
	return slider(caption, 0.0f, nm::TwoPI, val);
}
}

