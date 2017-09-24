// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "baseRendererAPI.h"
#include "textureSetter.h"

namespace unigui 
{
	namespace draw 
	{

		
		struct StatesBucketBase;
		struct Renderers : public std::vector<StatesBucketBase*>
		{
			void flush();
		};

		Renderers& renderers();
		
		
		template<class InstVtx>
		class BaseRectRenderer : public BaseRendererAPI<InstVtx, MeshVtx, unsigned short, 4096>
		{
		protected:
			virtual void init()
			{
				if (!this->inited)
				{
					VertexStream<MeshVtx> vs;
					IndexStream<unsigned short> is;

					Private::initQuadPrimitive(vs, is);
					BaseRendererAPI<InstVtx, MeshVtx, unsigned short, 4096>::init(vs, is);
				}			
			}
		};

		
		class TexRectRenderer : public BaseRectRenderer<TRInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const math::Rect<float>& tr, const Color& color);
		};

		
		class SolidRectRenderer : public BaseRectRenderer<RInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const Color& color);
		};

		
		class LineRenderer : public BaseRectRenderer<LineInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const math::Point2<float>& p0, const math::Point2<float>& p1, float thickness, const Color& color);
		};

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