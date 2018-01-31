// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#if defined(USE_WINDOWS)

#include "dx11.h"
#include "dx9.h"
#include "dx11/library.include.h"

#include "renderers/statesBucket.h"

namespace unigui {
namespace draw {
namespace dx11 {

	::dx11::Sampler& sampler()		{	static ::dx11::Sampler s;			return s;	}
	::dx11::ConstantBuffer& cb()	{	static ::dx11::ConstantBuffer s;	return s;	}

	Resources::VertexPixelShader* shaders[PRIMITIVE_TYPES];
	
	
	
	
	void init()
	{
		Resources::load(shaders[PRT_LINE], "gui/line11.fx");
		Resources::load(shaders[PRT_SOLID_RECT], "gui/rect11.fx");
		Resources::load(shaders[PRT_TEXTURED_RECT], "gui/texrect11.fx");
		Resources::load(shaders[PRT_FONT], "gui/font11DistanceProcessing.fx");

		sampler().samplerDesc.filter = gapi::MIN_MAG_MIP_POINT;
		sampler().create();
	}

	void release()
	{
		sampler().release();
	}

	Viewports::ViewportHandle currentViewport;

	void begin(Viewports::Viewport* viewport)
	{
		currentViewport.bind(viewport);

		flush();

		if (viewport)
		{
			D3DXMATRIX proj;
			D3DXMatrixOrthoOffCenterLH(&proj, 0, (float) viewport->width(), (float) viewport->height(), 0, 0, 10000.0f);

			cb().upload(proj);
			cb().vertex(0);
			cb().pixel(0);
			cb().execute(viewport->deviceIndex());
		}
 		
	}

	void flush()
	{
		if (currentViewport.viewport())
		{
			renderers().flush();
		}		
	}

	void end()
	{
		flush();
		currentViewport.remove();
	}

}
}
}
#endif



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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