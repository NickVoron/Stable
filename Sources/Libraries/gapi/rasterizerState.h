#include "samplers.h"

namespace gapi
{
	enum FillMode
	{
		SOLID,
		WIREFRAME
	};

	enum CullMode
	{
		NONE,
		FRONT,
		BACK
	};

	struct RasterizerState
	{
		RasterizerState();

		FillMode fillMode;
		CullMode cullMode;
		bool frontCounterClockwise;
		int depthBias;
		float depthBiasClamp;
		float slopeScaledDepthBias;
		bool depthClipEnable;
		bool scissorEnable;
		bool multisampleEnable;
		bool antialiasedLineEnable;
	};

	//
	//
	//
	enum Blend
	{	
		BLEND_ZERO				= 1,
		BLEND_ONE				= 2,
		BLEND_SRC_COLOR			= 3,
		BLEND_INV_SRC_COLOR		= 4,
		BLEND_SRC_ALPHA			= 5,
		BLEND_INV_SRC_ALPHA		= 6,
		BLEND_DEST_ALPHA		= 7,
		BLEND_INV_DEST_ALPHA	= 8,
		BLEND_DEST_COLOR		= 9,
		BLEND_INV_DEST_COLOR	= 10,
		BLEND_SRC_ALPHA_SAT		= 11,
		BLEND_BLEND_FACTOR		= 14,
		BLEND_INV_BLEND_FACTOR	= 15,
		BLEND_SRC1_COLOR		= 16,
		BLEND_INV_SRC1_COLOR	= 17,
		BLEND_SRC1_ALPHA		= 18,
		BLEND_INV_SRC1_ALPHA	= 19
	}; 	

	enum BlendOp
	{	
		BLEND_OP_ADD			= 1,
		BLEND_OP_SUBTRACT		= 2,
		BLEND_OP_REV_SUBTRACT	= 3,
		BLEND_OP_MIN			= 4,
		BLEND_OP_MAX			= 5
	};

	enum ColorWriteEnable
	{	
		COLOR_WRITE_ENABLE_RED		= 1,
		COLOR_WRITE_ENABLE_GREEN	= 2,
		COLOR_WRITE_ENABLE_BLUE		= 4,
		COLOR_WRITE_ENABLE_ALPHA	= 8,
		COLOR_WRITE_ENABLE_ALL		= ( ( ( COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN )  | COLOR_WRITE_ENABLE_BLUE )  | COLOR_WRITE_ENABLE_ALPHA ) 
	};

	struct RenderTargetBlend
	{
		RenderTargetBlend();

		bool			blendEnable;
		Blend			srcBlend;
		Blend			destBlend;
		BlendOp			blendOp;
		Blend			srcBlendAlpha;
		Blend			destBlendAlpha;
		BlendOp			blendOpAlpha;
		unsigned char	renderTargetWriteMask;
	};

	struct BlendState
	{
		BlendState();
		bool alphaToCoverageEnable;
		bool independentBlendEnable;
		RenderTargetBlend renderTarget[8];
	};

	enum DepthWriteMask
	{
		MASK_ZERO,
		MASK_ALL,
	};

	enum StencilOp
	{
		STENCIL_OP_KEEP = 1,
		STENCIL_OP_ZERO = 2,
		STENCIL_OP_REPLACE = 3,
		STENCIL_OP_INCR_SAT = 4,
		STENCIL_OP_DECR_SAT = 5,
		STENCIL_OP_INVERT = 6,
		STENCIL_OP_INCR = 7,
		STENCIL_OP_DECR = 8
	};

	struct DepthStencilOpDesc
	{
		DepthStencilOpDesc();

		StencilOp stencilFailOp;
		StencilOp stencilDepthFailOp;
		StencilOp stencilPassOp;
		ComparsionFunc stencilFunc;
	};
								   
 	struct DepthStencilState
 	{
		DepthStencilState();

 		bool				depthEnable;
 		DepthWriteMask		depthWriteMask;
		ComparsionFunc		depthFunc;
 		bool				stencilEnable;
 		unsigned char		stencilReadMask;
 		unsigned char		stencilWriteMask;
		DepthStencilOpDesc	frontFace;
		DepthStencilOpDesc	backFace;
		unsigned int		stencilRef;
 	};
}


