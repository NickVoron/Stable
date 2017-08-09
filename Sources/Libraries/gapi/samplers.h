// Copyright (C) 2013-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace gapi
{
	enum TextureAddressMode
	{
		WRAP			= 1,
		MIRROR			= 2,
		CLAMP			= 3,
		BORDER			= 4,
		MIRROR_ONCE		= 5
	};


	enum TextureFilter
	{
		MIN_MAG_MIP_POINT							= 0,
		MIN_MAG_POINT_MIP_LINEAR					= 0x1,
		MIN_POINT_MAG_LINEAR_MIP_POINT				= 0x4,
		MIN_POINT_MAG_MIP_LINEAR					= 0x5,
		MIN_LINEAR_MAG_MIP_POINT					= 0x10,
		MIN_LINEAR_MAG_POINT_MIP_LINEAR				= 0x11,
		MIN_MAG_LINEAR_MIP_POINT					= 0x14,
		MIN_MAG_MIP_LINEAR							= 0x15,
		ANISOTROPIC									= 0x55,
		COMPARISON_MIN_MAG_MIP_POINT				= 0x80,
		COMPARISON_MIN_MAG_POINT_MIP_LINEAR			= 0x81,
		COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x84,
		COMPARISON_MIN_POINT_MAG_MIP_LINEAR			= 0x85,
		COMPARISON_MIN_LINEAR_MAG_MIP_POINT			= 0x90,
		COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x91,
		COMPARISON_MIN_MAG_LINEAR_MIP_POINT			= 0x94,
		COMPARISON_MIN_MAG_MIP_LINEAR				= 0x95,
		COMPARISON_ANISOTROPIC						= 0xd5
	};

	enum ComparsionFunc
	{
		COMPARISON_NEVER			= 1,
		COMPARISON_LESS				= 2,
		COMPARISON_EQUAL			= 3,
		COMPARISON_LESS_EQUAL		= 4,
		COMPARISON_GREATER			= 5,
		COMPARISON_NOT_EQUAL		= 6,
		COMPARISON_GREATER_EQUAL	= 7,
		COMPARISON_ALWAYS			= 8
	};

	struct SamplerDesc
	{
		SamplerDesc();

		TextureFilter filter;
		TextureAddressMode addressU;
		TextureAddressMode addressV;
		TextureAddressMode addressW;

		ComparsionFunc comparisonFunc;

		unsigned int maxAnisotropy;

		float BorderColor[4];
		float maxLod;
		float minLod;
		float mipLODBias;
	};
}








// Copyright (C) 2013-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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