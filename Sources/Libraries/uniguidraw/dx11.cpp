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
	
	//
	//
	//
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