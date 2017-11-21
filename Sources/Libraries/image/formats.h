#pragma once

#include "type_utils.h"

namespace image
{
	enum Format
	{
		R8		,
		R16		,
		R32		,
		R8S		,
		R16S	,
		R32S	,
		R16F	,
		R32F	,

		RG8		,
		RG16	,
		RG32	,
		RG8S	,
		RG16S	,
		RG32S	,
		RG16F	,
		RG32F	,

		GR8		,
		GR16	,
		GR32	,
		GR8S	,
		GR16S	,
		GR32S	,
		GR16F	,
		GR32F	,

		RGB8	,
		RGB16	,
		RGB32	,
		RGB8S	,
		RGB16S	,
		RGB32S	,
		RGB16F	,
		RGB32F	,

		BGR8	,
		BGR16	,
		BGR32	,
		BGR8S	,
		BGR16S	,
		BGR32S	,
		BGR16F	,
		BGR32F	,

		RGBA8S	,
		ARGB8S	,
		ABGR8S	,
		BGRA8S	,
		RGBA8	,
		ARGB8	,
		ABGR8	,
		BGRA8	,

		RGBA16S	,
		ARGB16S	,
		ABGR16S	,
		BGRA16S	,
		RGBA16	,
		ARGB16	,
		ABGR16	,
		BGRA16	,
		RGBA16F	,
		ARGB16F	,
		ABGR16F	,
		BGRA16F	,

		RGBA32S	,
		ARGB32S	,
		ABGR32S	,
		BGRA32S	,
		RGBA32	,
		ARGB32	,
		ABGR32	,
		BGRA32	,
		RGBA32F	,
		ARGB32F	,
		ABGR32F	,
		BGRA32F	,

		SH4,
		SH9,
		SH16,
		SH25,
		SH36,

	};

	template<Format> struct pixel_info;

	template<> struct pixel_info<R8	>		{ static const char* name() { return "R8";} static const unsigned int size = 1; };
	template<> struct pixel_info<R16>		{ static const char* name() { return "R16";} static const unsigned int size = 2; };
	template<> struct pixel_info<R32>		{ static const char* name() { return "R32";} static const unsigned int size = 4; }; //-V112
	template<> struct pixel_info<R8S>		{ static const char* name() { return "R8S";} static const unsigned int size = 1; };
	template<> struct pixel_info<R16S>		{ static const char* name() { return "R16S";} static const unsigned int size = 2; };
	template<> struct pixel_info<R32S>		{ static const char* name() { return "R32S";} static const unsigned int size = 4; }; //-V112
	template<> struct pixel_info<R16F>		{ static const char* name() { return "R16F";} static const unsigned int size = 2; };
	template<> struct pixel_info<R32F>		{ static const char* name() { return "R32F";} static const unsigned int size = 4; }; //-V112
																				  
	template<> struct pixel_info<RG8	>	{ static const char* name() { return "RG8";} static const unsigned int size = 1*2; };
	template<> struct pixel_info<RG16	>	{ static const char* name() { return "RG16";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<RG32	>	{ static const char* name() { return "RG32";} static const unsigned int size = 4*2; }; //-V112
	template<> struct pixel_info<RG8S	>	{ static const char* name() { return "RG8S";} static const unsigned int size = 1*2; };
	template<> struct pixel_info<RG16S	>	{ static const char* name() { return "RG16S";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<RG32S	>	{ static const char* name() { return "RG32S";} static const unsigned int size = 4*2; }; //-V112
	template<> struct pixel_info<RG16F	>	{ static const char* name() { return "RG16F";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<RG32F	>	{ static const char* name() { return "RG32F";} static const unsigned int size = 4*2; }; //-V112
																				  
	template<> struct pixel_info<GR8	>	{ static const char* name() { return "GR8";} static const unsigned int size = 1*2; };
	template<> struct pixel_info<GR16	>	{ static const char* name() { return "GR16";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<GR32	>	{ static const char* name() { return "GR32";} static const unsigned int size = 4*2; }; //-V112
	template<> struct pixel_info<GR8S	>	{ static const char* name() { return "GR8S";} static const unsigned int size = 1*2; };
	template<> struct pixel_info<GR16S	>	{ static const char* name() { return "GR16S";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<GR32S	>	{ static const char* name() { return "GR32S";} static const unsigned int size = 4*2; }; //-V112
	template<> struct pixel_info<GR16F	>	{ static const char* name() { return "GR16F";} static const unsigned int size = 2*2; };
	template<> struct pixel_info<GR32F	>	{ static const char* name() { return "GR32F";} static const unsigned int size = 4*2; }; //-V112
																				  
	template<> struct pixel_info<RGB8	>	{ static const char* name() { return "RGB8";} static const unsigned int size = 1*3; };
	template<> struct pixel_info<RGB16	>	{ static const char* name() { return "RGB16";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<RGB32	>	{ static const char* name() { return "RGB32";} static const unsigned int size = 4*3; }; //-V112
	template<> struct pixel_info<RGB8S	>	{ static const char* name() { return "RGB8S";} static const unsigned int size = 1*3; };
	template<> struct pixel_info<RGB16S	>	{ static const char* name() { return "RGB16S";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<RGB32S	>	{ static const char* name() { return "RGB32S";} static const unsigned int size = 4*3; }; //-V112
	template<> struct pixel_info<RGB16F	>	{ static const char* name() { return "RGB16F";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<RGB32F	>	{ static const char* name() { return "RGB32F";} static const unsigned int size = 4*3; }; //-V112
																				  
	template<> struct pixel_info<BGR8	>	{ static const char* name() { return "BGR8";} static const unsigned int size = 1*3; };
	template<> struct pixel_info<BGR16	>	{ static const char* name() { return "BGR16";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<BGR32	>	{ static const char* name() { return "BGR32";} static const unsigned int size = 4*3; }; //-V112
	template<> struct pixel_info<BGR8S	>	{ static const char* name() { return "BGR8S";} static const unsigned int size = 1*3; };
	template<> struct pixel_info<BGR16S	>	{ static const char* name() { return "BGR16S";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<BGR32S	>	{ static const char* name() { return "BGR32S";} static const unsigned int size = 4*3; }; //-V112
	template<> struct pixel_info<BGR16F	>	{ static const char* name() { return "BGR16F";} static const unsigned int size = 2*3; };
	template<> struct pixel_info<BGR32F	>	{ static const char* name() { return "BGR32F";} static const unsigned int size = 4*3; }; //-V112
																				  
	template<> struct pixel_info<RGBA8S	>	{ static const char* name() { return "RGBA8S";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<ARGB8S	>	{ static const char* name() { return "ARGB8S";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<ABGR8S	>	{ static const char* name() { return "ABGR8S";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<BGRA8S	>	{ static const char* name() { return "BGRA8S";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<RGBA8	>	{ static const char* name() { return "RGBA8";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<ARGB8	>	{ static const char* name() { return "ARGB8";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<ABGR8	>	{ static const char* name() { return "ABGR8";} static const unsigned int size = 1*4; }; //-V112
	template<> struct pixel_info<BGRA8	>	{ static const char* name() { return "BGRA8";} static const unsigned int size = 1*4; }; //-V112
																				  
	template<> struct pixel_info<RGBA16S>	{ static const char* name() { return "RGBA16S";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ARGB16S>	{ static const char* name() { return "ARGB16S";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ABGR16S>	{ static const char* name() { return "ABGR16S";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<BGRA16S>	{ static const char* name() { return "BGRA16S";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<RGBA16	>	{ static const char* name() { return "RGBA16";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ARGB16	>	{ static const char* name() { return "ARGB16";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ABGR16	>	{ static const char* name() { return "ABGR16";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<BGRA16	>	{ static const char* name() { return "BGRA16";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<RGBA16F>	{ static const char* name() { return "RGBA16F";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ARGB16F>	{ static const char* name() { return "ARGB16F";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<ABGR16F>	{ static const char* name() { return "ABGR16F";} static const unsigned int size = 2*4; }; //-V112
	template<> struct pixel_info<BGRA16F>	{ static const char* name() { return "BGRA16F";} static const unsigned int size = 2*4; }; //-V112
																				  
	template<> struct pixel_info<RGBA32S>	{ static const char* name() { return "RGBA32S";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ARGB32S>	{ static const char* name() { return "ARGB32S";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ABGR32S>	{ static const char* name() { return "ABGR32S";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<BGRA32S>	{ static const char* name() { return "BGRA32S";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<RGBA32	>	{ static const char* name() { return "RGBA32";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ARGB32	>	{ static const char* name() { return "ARGB32";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ABGR32	>	{ static const char* name() { return "ABGR32";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<BGRA32	>	{ static const char* name() { return "BGRA32";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<RGBA32F>	{ static const char* name() { return "RGBA32F";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ARGB32F>	{ static const char* name() { return "ARGB32F";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<ABGR32F>	{ static const char* name() { return "ABGR32F";} static const unsigned int size = 4*4; }; //-V112
	template<> struct pixel_info<BGRA32F>	{ static const char* name() { return "BGRA32F";} static const unsigned int size = 4*4; }; //-V112
																				  
																				  	
	template<> struct pixel_info<SH4>		{ static const char* name() { return "SH4";} static const unsigned int size = 4*4;  }; //-V112
	template<> struct pixel_info<SH9>		{ static const char* name() { return "SH9";} static const unsigned int size = 9*4;  }; //-V112
	template<> struct pixel_info<SH16>		{ static const char* name() { return "SH16";} static const unsigned int size = 16*4; }; //-V112
	template<> struct pixel_info<SH25>		{ static const char* name() { return "SH25";} static const unsigned int size = 25*4; }; //-V112
	template<> struct pixel_info<SH36>		{ static const char* name() { return "SH36";} static const unsigned int size = 36*4; }; //-V112


	unsigned int pixelSize(Format format);
}