#pragma once
#include "dx11/library.include.h"
#include "resourceShader/library.include.h"

#include "types.h"

namespace unigui {
namespace draw {
namespace dx11 {

	extern Resources::VertexPixelShader* shaders[PRIMITIVE_TYPES];
	::dx11::Sampler& sampler();
	::dx11::ConstantBuffer& cb();

	void init();
	void release();
	void begin(Viewports::Viewport* viewport);
	void end();
	void flush();
	
}
}
}