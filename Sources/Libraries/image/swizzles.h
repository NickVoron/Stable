// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "type_utils.h"
#include "formats.h"
#include "stuff/enforce.h"

namespace image
{

	typedef Vector4 (*RGBA_Func)(const void*, int x, int y, int pitch);

	struct swizzle_desc
	{
		swizzle_desc(int cnt, int channelSz, RGBA_Func f, int r = 0, int g = 0, int b = 0, int a = 0)
			:count(cnt), channelSize(channelSz), pixel(f)
		{
			s[0] = r;
			s[1] = g;
			s[2] = b;
			s[3] = a;
		}

		int swizzle(int i) const { ENFORCE(i >= 0 && i < count); return s[i]; }
		int sizeOf() const { return channelSize * count; }
		
		int s[4];
		const int count;
		const int channelSize;		
		const RGBA_Func pixel;
	};

	template<Format fmt> struct ChannelsToSwizzle {};

	struct ChannelsSwizzle1
	{
		static const int r = 0;

		static int swizzle(int i){ ENFORCE(i == 0); static const int s[1] = {r}; return s[i]; }
		static swizzle_desc desc(int channelSize, RGBA_Func func) { return swizzle_desc(1, channelSize, func, r); }

		typedef ChannelsSwizzle1 Reverse;
	};

	template<int ri, int gi>
	struct ChannelsSwizzle2
	{
		static const int r = ri;
		static const int g = gi;

		static int swizzle(int i){ ENFORCE(i >= 0 && i < 2); static const int s[2] = {r, g}; return s[i]; }		
		static swizzle_desc desc(int channelSize, RGBA_Func func) { return swizzle_desc(2, channelSize, func, r, g); }

		typedef ChannelsSwizzle2<g, r> Reverse;
	};

	template<int ri, int gi, int bi>
	struct ChannelsSwizzle3
	{
		static const int r = ri;
		static const int g = gi;
		static const int b = bi;

		static int swizzle(int i){ ENFORCE(i >= 0 && i < 3); static const int s[3] = {r, g, b}; return s[i]; }		
		static swizzle_desc desc(int channelSize, RGBA_Func func) { return swizzle_desc(3, channelSize, func, r, g, b); }

		typedef ChannelsSwizzle3<b, g, r> Reverse;
	};

	template<int ri, int gi, int bi, int ai>
	struct ChannelsSwizzle4
	{
		static const int r = ri;
		static const int g = gi;
		static const int b = bi;
		static const int a = ai;

		static int swizzle(int i){ ENFORCE(i >= 0 && i < 4); static const int s[4] = {r, g, b, a}; return s[i]; }
		static swizzle_desc desc(int channelSize, RGBA_Func func) { return swizzle_desc(4, channelSize, func, r, g, b, a); }

		typedef ChannelsSwizzle4<a, b, g, r> Reverse;
	};



	struct ChannelsSwizzleDirect
	{
		static int swizzle(int i){ ENFORCE(i >= 0); return i; }
	};

	typedef ChannelsSwizzle1 SWIZZLE_R;

	typedef ChannelsSwizzle2<0, 1> SWIZZLE_RG;
	typedef ChannelsSwizzle2<1, 0> SWIZZLE_GR;

	typedef ChannelsSwizzle3<0, 1, 2> SWIZZLE_RGB;
	typedef ChannelsSwizzle3<2, 1, 0> SWIZZLE_BGR;

	typedef ChannelsSwizzle4<0, 1, 2, 3> SWIZZLE_RGBA;
	typedef ChannelsSwizzle4<2, 1, 0, 3> SWIZZLE_ARGB;
	typedef ChannelsSwizzle4<0, 1, 2, 3> SWIZZLE_ABGR;
	typedef ChannelsSwizzle4<2, 1, 0, 3> SWIZZLE_BGRA;

	template<class ValueType, class SwT>
	struct SwizzleDesc
	{
		typedef ValueType value_t; 
		typedef SwT SWIZZLE;		
		static swizzle_desc desc() { return SWIZZLE::desc(sizeof(value_t), &rgba); } 

		static Vector4 rgba(const void* data, int x, int y, int pitch)
		{
			value_t* tpd = (value_t*)data;
			value_t* td = &tpd[ y * pitch + x ];
			swizzle_desc dsc = desc();
			
			Vector4 result(0, 0, 0, 1);
			float* fd = &result.x;

			for (int i = 0; i < dsc.count; ++i)
			{
				fd[i] = mapToFloat<value_t>( td[ dsc.s[i] ] );
			}

			return result;
		}
	};

	template<class ValueType> struct R_T : public SwizzleDesc<ValueType, SWIZZLE_R>			{ static const char* name() { return "R"; } };

	template<class ValueType> struct RG_T : public SwizzleDesc<ValueType, SWIZZLE_RG>		{ static const char* name() { return "RG"; } };
	template<class ValueType> struct GR_T : public SwizzleDesc<ValueType, SWIZZLE_GR>		{ static const char* name() { return "GR"; } };

	template<class ValueType> struct RGB_T : public SwizzleDesc<ValueType, SWIZZLE_RGB>		{ static const char* name() { return "RGB";	} };
	template<class ValueType> struct BGR_T : public SwizzleDesc<ValueType, SWIZZLE_BGR>		{ static const char* name() { return "BGR";	} };

	template<class ValueType> struct RGBA_T : public SwizzleDesc<ValueType, SWIZZLE_RGBA>	{ static const char* name() { return "RGBA"; } };
	template<class ValueType> struct ARGB_T : public SwizzleDesc<ValueType, SWIZZLE_ARGB>	{ static const char* name() { return "ARGB"; } };
	template<class ValueType> struct ABGR_T : public SwizzleDesc<ValueType, SWIZZLE_ABGR>	{ static const char* name() { return "ABGR"; } };
	template<class ValueType> struct BGRA_T : public SwizzleDesc<ValueType, SWIZZLE_BGRA>	{ static const char* name() { return "BGRA"; } };









	template<> struct ChannelsToSwizzle<R8>			: public R_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<R16>		: public R_T<unsigned short >{ };
	template<> struct ChannelsToSwizzle<R32>		: public R_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<R8S>		: public R_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<R16S>		: public R_T<signed short 	>{ };
	template<> struct ChannelsToSwizzle<R32S>		: public R_T<signed int		>{ };
	template<> struct ChannelsToSwizzle<R16F>		: public R_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<R32F>		: public R_T<float			>{ };

	template<> struct ChannelsToSwizzle<RG8>		: public RG_T< unsigned char>{ };
	template<> struct ChannelsToSwizzle<RG16>		: public RG_T< unsigned short>{ };
	template<> struct ChannelsToSwizzle<RG32>		: public RG_T< unsigned int	>{ };
	template<> struct ChannelsToSwizzle<RG8S>		: public RG_T< signed char	>{ };
	template<> struct ChannelsToSwizzle<RG16S>		: public RG_T< signed short >{ };
	template<> struct ChannelsToSwizzle<RG32S>		: public RG_T< signed int	>{ };
	template<> struct ChannelsToSwizzle<RG16F>		: public RG_T< half_float	>{ };
	template<> struct ChannelsToSwizzle<RG32F>		: public RG_T< float		>{ };

	template<> struct ChannelsToSwizzle<GR8>		: public GR_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<GR16>		: public GR_T<unsigned short>{ };
	template<> struct ChannelsToSwizzle<GR32>		: public GR_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<GR8S>		: public GR_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<GR16S>		: public GR_T<signed short 	>{ };
	template<> struct ChannelsToSwizzle<GR32S>		: public GR_T<signed int	>{};
	template<> struct ChannelsToSwizzle<GR16F>		: public GR_T<half_float	>{};
	template<> struct ChannelsToSwizzle<GR32F>		: public GR_T<float			>{ };

	template<> struct ChannelsToSwizzle<RGB8>		: public RGB_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<RGB16>		: public RGB_T<unsigned short 	>{ };
	template<> struct ChannelsToSwizzle<RGB32>		: public RGB_T<unsigned int		>{ };
	template<> struct ChannelsToSwizzle<RGB8S>		: public RGB_T<signed char		>{ };
	template<> struct ChannelsToSwizzle<RGB16S>		: public RGB_T<signed short 	>{ };
	template<> struct ChannelsToSwizzle<RGB32S>		: public RGB_T<signed int		>{ };
	template<> struct ChannelsToSwizzle<RGB16F>		: public RGB_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<RGB32F>		: public RGB_T<float			>{ };

	template<> struct ChannelsToSwizzle<BGR8>		: public BGR_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<BGR16>		: public BGR_T<unsigned short 	>{ };
	template<> struct ChannelsToSwizzle<BGR32>		: public BGR_T<unsigned int		>{ };
	template<> struct ChannelsToSwizzle<BGR8S>		: public BGR_T<signed char		>{ };
	template<> struct ChannelsToSwizzle<BGR16S>		: public BGR_T<signed short 	>{ };
	template<> struct ChannelsToSwizzle<BGR32S>		: public BGR_T<signed int		>{ };
	template<> struct ChannelsToSwizzle<BGR16F>		: public BGR_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<BGR32F>		: public BGR_T<float			>{ };

	template<> struct ChannelsToSwizzle<RGBA8>		: public RGBA_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<RGBA16>		: public RGBA_T<unsigned short	>{ };
	template<> struct ChannelsToSwizzle<RGBA32>		: public RGBA_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<RGBA16F>	: public RGBA_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<RGBA32F>	: public RGBA_T<float			>{ };
																   
	template<> struct ChannelsToSwizzle<ARGB8>		: public ARGB_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<ARGB16>		: public ARGB_T<unsigned short	>{ };
	template<> struct ChannelsToSwizzle<ARGB32>		: public ARGB_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<ARGB16F>	: public ARGB_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<ARGB32F>	: public ARGB_T<float			>{ };

	template<> struct ChannelsToSwizzle<ABGR8>		: public ABGR_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<ABGR16>		: public ABGR_T<unsigned short	>{ };
	template<> struct ChannelsToSwizzle<ABGR32>		: public ABGR_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<ABGR16F>	: public ABGR_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<ABGR32F>	: public ABGR_T<float			>{ };

	template<> struct ChannelsToSwizzle<BGRA8>		: public BGRA_T<unsigned char	>{ };
	template<> struct ChannelsToSwizzle<BGRA16>		: public BGRA_T<unsigned short	>{ };
	template<> struct ChannelsToSwizzle<BGRA32>		: public BGRA_T<unsigned int	>{ };
	template<> struct ChannelsToSwizzle<BGRA16F>	: public BGRA_T<half_float		>{ };
	template<> struct ChannelsToSwizzle<BGRA32F>	: public BGRA_T<float			>{ };

	template<> struct ChannelsToSwizzle<RGBA8S>		: public RGBA_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<RGBA16S>	: public RGBA_T<signed short>{ };
	template<> struct ChannelsToSwizzle<RGBA32S>	: public RGBA_T<signed int	>{ };

	template<> struct ChannelsToSwizzle<ARGB8S>		: public ARGB_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<ARGB16S>	: public ARGB_T<signed short>{ };
	template<> struct ChannelsToSwizzle<ARGB32S>	: public ARGB_T<signed int	>{ };

	template<> struct ChannelsToSwizzle<ABGR8S>		: public ABGR_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<ABGR16S>	: public ABGR_T<signed short>{ };
	template<> struct ChannelsToSwizzle<ABGR32S>	: public ABGR_T<signed int	>{ };

	template<> struct ChannelsToSwizzle<BGRA8S>		: public BGRA_T<signed char	>{ };
	template<> struct ChannelsToSwizzle<BGRA16S>	: public BGRA_T<signed short>{ };
	template<> struct ChannelsToSwizzle<BGRA32S>	: public BGRA_T<signed int	>{ };

	template<> struct ChannelsToSwizzle<SH4>	: public ChannelsSwizzleDirect{ typedef float	value_t;	};
	template<> struct ChannelsToSwizzle<SH9>	: public ChannelsSwizzleDirect{ typedef float	value_t;	};
	template<> struct ChannelsToSwizzle<SH16>	: public ChannelsSwizzleDirect{ typedef float	value_t;	};
	template<> struct ChannelsToSwizzle<SH25>	: public ChannelsSwizzleDirect{ typedef float	value_t;	};
	template<> struct ChannelsToSwizzle<SH36>	: public ChannelsSwizzleDirect{ typedef float	value_t;	};

}



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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