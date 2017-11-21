#pragma once

#include "renderers.h"
#include "statesBucket.h"

namespace unigui {
namespace draw {

	void Renderers::flush()
	{
		for(StatesBucketBase* bucket : *this)
		{
			bucket->flush();
		}
	}

	Renderers& renderers()
	{
		static Renderers res;
		return res;

	}


	struct RectAlgo 
	{

		bool add(const math::Rect<float>& rect)
		{
			for (const auto& r : data)
			{
				if (r.intersected_neq(rect))
				{
					data.clear();
					data.push_back(rect);
					return false;
				}
			}

			data.push_back(rect);
			return true;
		}

		std::vector<math::Rect<float>> data;
	};

	RectAlgo rectAlgo;

	//
	//
	//
	void TexRectRenderer::add(const math::Rect<float>& r, const math::Rect<float>& tr, const Color& color)
	{
		if (!rectAlgo.add(r))
		{
			checkedFlush();
		}

		checkedBegin();

		TRInstVtx& v = instances()[cursor()++];
		v.uv4<0>(r.pos.x, r.pos.y, r.size.x, r.size.y);

		bool halfPixelCorrection = gapi::initialized(gapi::DX9);
		float correctionP = halfPixelCorrection ? 0.5f : 0.0f;
		float correctionS = 0.5f;

		float ix = invsize().x;
		float iy = invsize().y;

		v.uv4<1>((tr.pos.x + correctionP) * ix,	(tr.pos.y + correctionP) * iy, (tr.size.x - correctionS) * ix, (tr.size.y - correctionS) * iy);
		v.diffuse(color.r(), color.g(), color.b(), color.a());
	}

	void SolidRectRenderer::add(const math::Rect<float>& r, const Color& color)
	{
		if (!rectAlgo.add(r))
		{
			checkedFlush();
		}

		checkedBegin();

		RInstVtx& v = instances()[cursor()++];
		v.uv4<0>(r.pos.x, r.pos.y, r.size.x, r.size.y);
		v.diffuse(color.r(), color.g(), color.b(), color.a());
	}

	
	void LineRenderer::add(const math::Rect<float>& rr, const math::Point2<float>& p0, const math::Point2<float>& p1, float thickness, const Color& color)
	{
		math::Rect<float> r = rr;
		r.addLeft(thickness);
		r.addRight(thickness);
		r.addTop(thickness);
		r.addBottom(thickness);

		if (!rectAlgo.add(r))
		{
			checkedFlush();
		}

		checkedBegin();

		LineInstVtx& v = instances()[cursor()++];


		v.uv4<0>(r.pos.x, r.pos.y, r.size.x, r.size.y);
		v.uv4<1>(p0.x, p0.y, p1.x, p1.y);
		v.uv4<2>(thickness, thickness, thickness, thickness);
		v.diffuse(color.r(), color.g(), color.b(), color.a());
	}

}
}
