#include "dx9/library.include.h"
#include "dx11/library.include.h"
#include "unigui/library.include.h"

//#include "renderTargetsManager/library.include.h"

#include "dx11.h"
#include "types.h"

namespace unigui {
namespace draw {
namespace dx9 {

// 	extern RenderTargets::DepthStencil depthStencil;
// 	extern RenderTargets::DepthStencilBackup backup;
//	extern Resources::Effect* effectFX;

	void init();
	void release();
	void begin(Viewports::Viewport* viewport);
	void flush();

}				   
}
}