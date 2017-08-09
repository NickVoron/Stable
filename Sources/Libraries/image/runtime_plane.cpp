// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "runtime_plane.h"
#include "plane.h"

#include "stuff/enforce.h"
#include "imageD3D/library.include.h"

namespace image
{
	ImageInfo::ImageInfo()
		:data(0), 
		dataOwner(false),
		sizeX(0),
		sizeY(0),
		pitchInBytes(0)
	{
	}

	ImageInfo::ImageInfo(Format f, unsigned int sX, unsigned int sY)
		:data(0), dataOwner(false)
	{
		init(f, sX, sY, false);
		format = f;
		sizeX = sX;
		sizeY = sY;
		pitchInBytes = 0;
	}

	ImageInfo::ImageInfo(void* d, bool dO, Format f, unsigned int sX, unsigned int sY, unsigned int pitch)
	{
		init(d, dO, f, sX, sY, pitch);
	}

	void ImageInfo::init(Format f, unsigned int sX, unsigned int sY, bool allocateInternalBuffer)
	{
		clear();

		format = f;
		sizeX = sX;
		sizeY = sY;
		if(allocateInternalBuffer)
		{
			allocate_plane_image(*this);
			dataOwner = true;
		}		
	}

	void ImageInfo::init(void* d, bool dO, Format f, unsigned int sX, unsigned int sY, unsigned int pitch)
	{
		clear();

		data = d;
		dataOwner = dO;
		format = f;
		sizeX = sX;
		sizeY = sY;
		pitchInBytes = pitch;
	}

	void ImageInfo::freemem()
	{
		if(data && dataOwner)
			free(data);

		data = 0;
		dataOwner = 0;
	}

	ImageInfo::~ImageInfo()
	{
		freemem();
	}

	void ImageInfo::clear()
	{
		freemem();
		sizeX = 0;
		sizeY = 0;
		pitchInBytes = 0;
	}

	bool ImageInfo::valid() const
	{
		return data != 0 && sizeX != 0 && sizeY != 0 && pitchInBytes != 0;
	}
										  
	void allocate_plane_image(ImageInfo& imgInfo)
	{
		imgInfo.freemem();
		imgInfo.pitchInBytes = pixelSize(imgInfo.format) * imgInfo.sizeX;
		int memSize = imgInfo.pitchInBytes * imgInfo.sizeY;
		imgInfo.data = malloc(memSize);
		mem::memset(imgInfo.data, 0, memSize);
		imgInfo.dataOwner = (imgInfo.data != 0);
	}

	template<Format srcFormat, Format dstFormat>
	void convert(void* src, void* dst, int sizeX, int sizeY, int srcPitch, int dstPitch)
	{
		Plane< Color<srcFormat> > srcImg(src, srcPitch, sizeX, sizeY);
		Plane< Color<dstFormat> > dstImg(dst, dstPitch, sizeX, sizeY);

		dstImg.copy(srcImg);
	}

	template<Format srcFormat>
	void convert(void* src, void* dst, Format dstFormat, int sizeX, int sizeY, int srcPitch, int dstPitch)
	{

		switch(dstFormat)
		{
		case R8		: convert<srcFormat, R8		>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16	: convert<srcFormat, R16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32	: convert<srcFormat, R32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R8S	: convert<srcFormat, R8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16S	: convert<srcFormat, R16S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32S	: convert<srcFormat, R32S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16F	: convert<srcFormat, R16F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32F	: convert<srcFormat, R32F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG8	: convert<srcFormat, RG8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16	: convert<srcFormat, RG16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32	: convert<srcFormat, RG32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG8S	: convert<srcFormat, RG8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16S	: convert<srcFormat, RG16S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32S	: convert<srcFormat, RG32S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16F	: convert<srcFormat, RG16F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32F	: convert<srcFormat, RG32F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR8	: convert<srcFormat, GR8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16	: convert<srcFormat, GR16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32	: convert<srcFormat, GR32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR8S	: convert<srcFormat, GR8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16S	: convert<srcFormat, GR16S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32S	: convert<srcFormat, GR32S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16F	: convert<srcFormat, GR16F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32F	: convert<srcFormat, GR32F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB8	: convert<srcFormat, RGB8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16	: convert<srcFormat, RGB16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32	: convert<srcFormat, RGB32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB8S	: convert<srcFormat, RGB8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16S	: convert<srcFormat, RGB16S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32S	: convert<srcFormat, RGB32S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16F	: convert<srcFormat, RGB16F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32F	: convert<srcFormat, RGB32F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR8	: convert<srcFormat, BGR8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16	: convert<srcFormat, BGR16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32	: convert<srcFormat, BGR32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR8S	: convert<srcFormat, BGR8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16S	: convert<srcFormat, BGR16S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32S	: convert<srcFormat, BGR32S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16F	: convert<srcFormat, BGR16F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32F	: convert<srcFormat, BGR32F	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA8S	: convert<srcFormat, RGBA8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB8S	: convert<srcFormat, ARGB8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR8S	: convert<srcFormat, ABGR8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA8S	: convert<srcFormat, BGRA8S	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA8	: convert<srcFormat, RGBA8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB8	: convert<srcFormat, ARGB8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR8	: convert<srcFormat, ABGR8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA8	: convert<srcFormat, BGRA8	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16S: convert<srcFormat, RGBA16S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16S: convert<srcFormat, ARGB16S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16S: convert<srcFormat, ABGR16S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16S: convert<srcFormat, BGRA16S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16	: convert<srcFormat, RGBA16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16	: convert<srcFormat, ARGB16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16	: convert<srcFormat, ABGR16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16	: convert<srcFormat, BGRA16	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16F: convert<srcFormat, RGBA16F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16F: convert<srcFormat, ARGB16F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16F: convert<srcFormat, ABGR16F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16F: convert<srcFormat, BGRA16F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32S: convert<srcFormat, RGBA32S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32S: convert<srcFormat, ARGB32S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32S: convert<srcFormat, ABGR32S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32S: convert<srcFormat, BGRA32S>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32	: convert<srcFormat, RGBA32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32	: convert<srcFormat, ARGB32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32	: convert<srcFormat, ABGR32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32	: convert<srcFormat, BGRA32	>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32F: convert<srcFormat, RGBA32F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32F: convert<srcFormat, ARGB32F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32F: convert<srcFormat, ABGR32F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32F: convert<srcFormat, BGRA32F>(src, dst, sizeX, sizeY, srcPitch, dstPitch); break;
		}
	}

	void convert(void* src, void* dst, Format srcFormat, Format dstFormat, int sizeX, int sizeY, int srcPitch, int dstPitch)
	{
		switch(srcFormat)
		{
		case R8		: convert<R8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16	: convert<R16	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32	: convert<R32	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R8S	: convert<R8S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16S	: convert<R16S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32S	: convert<R32S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R16F	: convert<R16F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case R32F	: convert<R32F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG8	: convert<RG8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16	: convert<RG16	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32	: convert<RG32	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG8S	: convert<RG8S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16S	: convert<RG16S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32S	: convert<RG32S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG16F	: convert<RG16F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RG32F	: convert<RG32F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR8	: convert<GR8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16	: convert<GR16	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32	: convert<GR32	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR8S	: convert<GR8S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16S	: convert<GR16S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32S	: convert<GR32S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR16F	: convert<GR16F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case GR32F	: convert<GR32F	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB8	: convert<RGB8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16	: convert<RGB16	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32	: convert<RGB32	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB8S	: convert<RGB8S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16S	: convert<RGB16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32S	: convert<RGB32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB16F	: convert<RGB16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGB32F	: convert<RGB32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR8	: convert<BGR8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16	: convert<BGR16	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32	: convert<BGR32	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR8S	: convert<BGR8S	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16S	: convert<BGR16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32S	: convert<BGR32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR16F	: convert<BGR16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGR32F	: convert<BGR32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA8S	: convert<RGBA8S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB8S	: convert<ARGB8S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR8S	: convert<ABGR8S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA8S	: convert<BGRA8S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA8	: convert<RGBA8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB8	: convert<ARGB8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR8	: convert<ABGR8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA8	: convert<BGRA8	>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16S: convert<RGBA16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16S: convert<ARGB16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16S: convert<ABGR16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16S: convert<BGRA16S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16	: convert<RGBA16>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16	: convert<ARGB16>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16	: convert<ABGR16>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16	: convert<BGRA16>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA16F: convert<RGBA16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB16F: convert<ARGB16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR16F: convert<ABGR16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA16F: convert<BGRA16F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32S: convert<RGBA32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32S: convert<ARGB32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32S: convert<ABGR32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32S: convert<BGRA32S>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32	: convert<RGBA32>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32	: convert<ARGB32>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32	: convert<ABGR32>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32	: convert<BGRA32>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case RGBA32F: convert<RGBA32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ARGB32F: convert<ARGB32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case ABGR32F: convert<ABGR32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		case BGRA32F: convert<BGRA32F>(src, dst, dstFormat, sizeX, sizeY, srcPitch, dstPitch); break;
		}
	}

	swizzle_desc swizzle(Format f)
	{
		switch(f)
		{
		case R8		: return ChannelsToSwizzle<R8	>	::desc();
		case R16	: return ChannelsToSwizzle<R16	>	::desc();
		case R32	: return ChannelsToSwizzle<R32	>	::desc();
		case R8S	: return ChannelsToSwizzle<R8S	>	::desc();
		case R16S	: return ChannelsToSwizzle<R16S	>	::desc();
		case R32S	: return ChannelsToSwizzle<R32S	>	::desc();
		case R16F	: return ChannelsToSwizzle<R16F	>	::desc();
		case R32F	: return ChannelsToSwizzle<R32F	>	::desc();
		case RG8	: return ChannelsToSwizzle<RG8	>	::desc();
		case RG16	: return ChannelsToSwizzle<RG16	>	::desc();
		case RG32	: return ChannelsToSwizzle<RG32	>	::desc();
		case RG8S	: return ChannelsToSwizzle<RG8S	>	::desc();
		case RG16S	: return ChannelsToSwizzle<RG16S>	::desc();
		case RG32S	: return ChannelsToSwizzle<RG32S>	::desc();
		case RG16F	: return ChannelsToSwizzle<RG16F>	::desc();
		case RG32F	: return ChannelsToSwizzle<RG32F>	::desc();
		case GR8	: return ChannelsToSwizzle<GR8	>	::desc();
		case GR16	: return ChannelsToSwizzle<GR16	>	::desc();
		case GR32	: return ChannelsToSwizzle<GR32	>	::desc();
		case GR8S	: return ChannelsToSwizzle<GR8S	>	::desc();
		case GR16S	: return ChannelsToSwizzle<GR16S>	::desc();
		case GR32S	: return ChannelsToSwizzle<GR32S>	::desc();
		case GR16F	: return ChannelsToSwizzle<GR16F>	::desc();
		case GR32F	: return ChannelsToSwizzle<GR32F>	::desc();
		case RGB8	: return ChannelsToSwizzle<RGB8	>	::desc();
		case RGB16	: return ChannelsToSwizzle<RGB16>	::desc();
		case RGB32	: return ChannelsToSwizzle<RGB32>	::desc();
		case RGB8S	: return ChannelsToSwizzle<RGB8S>	::desc();
		case RGB16S	: return ChannelsToSwizzle<RGB16S>	::desc();
		case RGB32S	: return ChannelsToSwizzle<RGB32S>	::desc();
		case RGB16F	: return ChannelsToSwizzle<RGB16F>	::desc();
		case RGB32F	: return ChannelsToSwizzle<RGB32F>	::desc();
		case BGR8	: return ChannelsToSwizzle<BGR8	>	::desc();
		case BGR16	: return ChannelsToSwizzle<BGR16>	::desc();
		case BGR32	: return ChannelsToSwizzle<BGR32>	::desc();
		case BGR8S	: return ChannelsToSwizzle<BGR8S>	::desc();
		case BGR16S	: return ChannelsToSwizzle<BGR16S>	::desc();
		case BGR32S	: return ChannelsToSwizzle<BGR32S>	::desc();
		case BGR16F	: return ChannelsToSwizzle<BGR16F>	::desc();
		case BGR32F	: return ChannelsToSwizzle<BGR32F>	::desc();
		case RGBA8S	: return ChannelsToSwizzle<RGBA8S>	::desc();
		case ARGB8S	: return ChannelsToSwizzle<ARGB8S>	::desc();
		case ABGR8S	: return ChannelsToSwizzle<ABGR8S>	::desc();
		case BGRA8S	: return ChannelsToSwizzle<BGRA8S>	::desc();
		case RGBA8	: return ChannelsToSwizzle<RGBA8>	::desc();
		case ARGB8	: return ChannelsToSwizzle<ARGB8>	::desc();
		case ABGR8	: return ChannelsToSwizzle<ABGR8>	::desc();
		case BGRA8	: return ChannelsToSwizzle<BGRA8>	::desc();
		case RGBA16S: return ChannelsToSwizzle<RGBA16S>	::desc();
		case ARGB16S: return ChannelsToSwizzle<ARGB16S>	::desc();
		case ABGR16S: return ChannelsToSwizzle<ABGR16S>	::desc();
		case BGRA16S: return ChannelsToSwizzle<BGRA16S>	::desc();
		case RGBA16	: return ChannelsToSwizzle<RGBA16>	::desc();
		case ARGB16	: return ChannelsToSwizzle<ARGB16>	::desc();
		case ABGR16	: return ChannelsToSwizzle<ABGR16>	::desc();
		case BGRA16	: return ChannelsToSwizzle<BGRA16>	::desc();
		case RGBA16F: return ChannelsToSwizzle<RGBA16F>	::desc();
		case ARGB16F: return ChannelsToSwizzle<ARGB16F>	::desc();
		case ABGR16F: return ChannelsToSwizzle<ABGR16F>	::desc();
		case BGRA16F: return ChannelsToSwizzle<BGRA16F>	::desc();
		case RGBA32S: return ChannelsToSwizzle<RGBA32S>	::desc();
		case ARGB32S: return ChannelsToSwizzle<ARGB32S>	::desc();
		case ABGR32S: return ChannelsToSwizzle<ABGR32S>	::desc();
		case BGRA32S: return ChannelsToSwizzle<BGRA32S>	::desc();
		case RGBA32	: return ChannelsToSwizzle<RGBA32>	::desc();
		case ARGB32	: return ChannelsToSwizzle<ARGB32>	::desc();
		case ABGR32	: return ChannelsToSwizzle<ABGR32>	::desc();
		case BGRA32	: return ChannelsToSwizzle<BGRA32>	::desc();
		case RGBA32F: return ChannelsToSwizzle<RGBA32F>	::desc();
		case ARGB32F: return ChannelsToSwizzle<ARGB32F>	::desc();
		case ABGR32F: return ChannelsToSwizzle<ABGR32F>	::desc();
		case BGRA32F: return ChannelsToSwizzle<BGRA32F>	::desc();
		}

		throw std::runtime_error("unknown color format");
	}

	bool alpha_channel_used(const ImageInfo& ii)
	{
		swizzle_desc sd = swizzle(ii.format);
		for(unsigned int yy = 0; yy < ii.sizeY; ++yy)
		{
			for(unsigned int xx = 0; xx < ii.sizeX; ++xx)
			{
				Vector4 color = sd.pixel(ii.data, xx, yy, ii.pitchInBytes);
				if(color.w < 1.0f)
				{
					return true;
				}
			}
		}

		return false;
	}

	void convert_plane_image(const ImageInfo& src, ImageInfo& dst)
	{
		if(	(dst.sizeX != src.sizeX && 
			dst.sizeY != src.sizeY && 
			dst.pitchInBytes != src.pitchInBytes) || !dst.data)
		{
			dst.sizeX = src.sizeX;
			dst.sizeY = src.sizeY;
			allocate_plane_image(dst);
		}

		convert(src.data, dst.data, src.format, dst.format, src.sizeX, src.sizeY, src.pitchInBytes, dst.pitchInBytes);

	}

};





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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