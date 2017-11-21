//#include "renderTargetsManager/library.include.h"
#include "resourceFont/library.include.h"

#include "dx9.h"
#include "renderers/statesBucket.h"

namespace unigui {
	namespace draw {   
		namespace dx9 {

			namespace
			{ 
				LPD3DXLINE pLine = 0;			
				
			}

//			Resources::Effect* effectFX = 0;
//			RenderTargets::DepthStencil depthStencil;
//			RenderTargets::DepthStencilBackup backup;



			//
			//
			//
			void init()
			{
	//			Resources::load(effectFX, "gui/gui.fx");
				D3DXCreateLine(::dx9::device, &pLine);
//				depthStencil.create();
			}

			//
			//
			//
			void begin(Viewports::Viewport* viewport)
			{
// 				if(effectFX)
// 				{
// 					effectFX->resource([viewport](auto e)
// 					{ 
// 						flush();
// 
// 						if (viewport)
// 						{
// 							D3DXMATRIX proj;
// 							D3DXMatrixOrthoOffCenterLH(&proj, 0, (float) viewport->width(), (float) viewport->height(), 0, 0, 10000.0f);
// 							e->SetMatrix("proj", &proj);
// 						}						
// 					});
// 				}
			}

			void flush()
			{
				renderers().flush();
			}

			void release()
			{
				SAFE_RELEASE(pLine);
   			}
		}
	}
}