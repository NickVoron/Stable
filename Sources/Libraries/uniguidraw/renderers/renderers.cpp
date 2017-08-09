// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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