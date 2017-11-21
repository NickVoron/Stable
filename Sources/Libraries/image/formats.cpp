#include "formats.h"

namespace image
{
	unsigned int pixelSize(Format format)
	{
		switch (format)
		{
		case R8		: return pixel_info<R8		>::size;
		case R16	: return pixel_info<R16		>::size;
		case R32	: return pixel_info<R32		>::size;
		case R8S	: return pixel_info<R8S		>::size;
		case R16S	: return pixel_info<R16S	>::size;
		case R32S	: return pixel_info<R32S	>::size;
		case R16F	: return pixel_info<R16F	>::size;
		case R32F	: return pixel_info<R32F	>::size;
		case RG8	: return pixel_info<RG8		>::size;
		case RG16	: return pixel_info<RG16	>::size;
		case RG32	: return pixel_info<RG32	>::size;
		case RG8S	: return pixel_info<RG8S	>::size;
		case RG16S	: return pixel_info<RG16S	>::size;
		case RG32S	: return pixel_info<RG32S	>::size;
		case RG16F	: return pixel_info<RG16F	>::size;
		case RG32F	: return pixel_info<RG32F	>::size;
		case GR8	: return pixel_info<GR8		>::size;
		case GR16	: return pixel_info<GR16	>::size;
		case GR32	: return pixel_info<GR32	>::size;
		case GR8S	: return pixel_info<GR8S	>::size;
		case GR16S	: return pixel_info<GR16S	>::size;
		case GR32S	: return pixel_info<GR32S	>::size;
		case GR16F	: return pixel_info<GR16F	>::size;
		case GR32F	: return pixel_info<GR32F	>::size;
		case RGB8	: return pixel_info<RGB8	>::size;
		case RGB16	: return pixel_info<RGB16	>::size;
		case RGB32	: return pixel_info<RGB32	>::size;
		case RGB8S	: return pixel_info<RGB8S	>::size;
		case RGB16S	: return pixel_info<RGB16S	>::size;
		case RGB32S	: return pixel_info<RGB32S	>::size;
		case RGB16F	: return pixel_info<RGB16F	>::size;
		case RGB32F	: return pixel_info<RGB32F	>::size;
		case BGR8	: return pixel_info<BGR8	>::size;
		case BGR16	: return pixel_info<BGR16	>::size;
		case BGR32	: return pixel_info<BGR32	>::size;
		case BGR8S	: return pixel_info<BGR8S	>::size;
		case BGR16S	: return pixel_info<BGR16S	>::size;
		case BGR32S	: return pixel_info<BGR32S	>::size;
		case BGR16F	: return pixel_info<BGR16F	>::size;
		case BGR32F	: return pixel_info<BGR32F	>::size;
		case RGBA8S	: return pixel_info<RGBA8S	>::size;
		case ARGB8S	: return pixel_info<ARGB8S	>::size;
		case ABGR8S	: return pixel_info<ABGR8S	>::size;
		case BGRA8S	: return pixel_info<BGRA8S	>::size;
		case RGBA8	: return pixel_info<RGBA8	>::size;
		case ARGB8	: return pixel_info<ARGB8	>::size;
		case ABGR8	: return pixel_info<ABGR8	>::size;
		case BGRA8	: return pixel_info<BGRA8	>::size;
		case RGBA16S: return pixel_info<RGBA16S	>::size;
		case ARGB16S: return pixel_info<ARGB16S	>::size;
		case ABGR16S: return pixel_info<ABGR16S	>::size;
		case BGRA16S: return pixel_info<BGRA16S	>::size;
		case RGBA16	: return pixel_info<RGBA16	>::size;
		case ARGB16	: return pixel_info<ARGB16	>::size;
		case ABGR16	: return pixel_info<ABGR16	>::size;
		case BGRA16	: return pixel_info<BGRA16	>::size;
		case RGBA16F: return pixel_info<RGBA16F	>::size;
		case ARGB16F: return pixel_info<ARGB16F	>::size;
		case ABGR16F: return pixel_info<ABGR16F	>::size;
		case BGRA16F: return pixel_info<BGRA16F	>::size;
		case RGBA32S: return pixel_info<RGBA32S	>::size;
		case ARGB32S: return pixel_info<ARGB32S	>::size;
		case ABGR32S: return pixel_info<ABGR32S	>::size;
		case BGRA32S: return pixel_info<BGRA32S	>::size;
		case RGBA32	: return pixel_info<RGBA32	>::size;
		case ARGB32	: return pixel_info<ARGB32	>::size;
		case ABGR32	: return pixel_info<ABGR32	>::size;
		case BGRA32	: return pixel_info<BGRA32	>::size;
		case RGBA32F: return pixel_info<RGBA32F	>::size;
		case ARGB32F: return pixel_info<ARGB32F	>::size;
		case ABGR32F: return pixel_info<ABGR32F	>::size;
		case BGRA32F: return pixel_info<BGRA32F	>::size;
		}

		return 0;
	}
}

