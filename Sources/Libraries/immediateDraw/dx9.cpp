// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dx9.h"

namespace dx9 
{
	namespace im 
	{

		void DrawFSQuad(int quadSize)
		{
			DrawFSQuad(quadSize, quadSize);
		}

		void DrawFSQuad(int w, int h)
		{
			float fw = (float)w - 0.5f;
			float fh = (float)h - 0.5f;

			typedef Geometry::Vertex<Geometry::POST, Geometry::UV2<0>> ScreenQuadVertex;
			ScreenQuadVertex quad[4];

			quad[0].posT(-0.5f, -0.5f, 0.0f, 1.0f);
			quad[0].uv2<0>(0.0f, 0.0f);

			quad[1].posT(fw, -0.5f, 0.0f, 1.0f);
			quad[1].uv2<0>(1.0f, 0.0f);

			quad[2].posT(-0.5f, fh, 0.0f, 1.0f);
			quad[2].uv2<0>(0.0f, 1.0f);

			quad[3].posT(fw, fh, 0.0f, 1.0f);
			quad[3].uv2<0>(1.0f, 1.0f);

			DrawTriangleStrip(&quad[0], 4);
		}
	}
}



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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