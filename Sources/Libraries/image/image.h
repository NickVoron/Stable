#pragma once

#include "color.h"
#include "intcolor.h"
#include "line.h"
#include "plane.h"
#include "volume.h"
#include "cubemap.h"
#include "planeMips.h"
#include "type_utils.h"
#include "channelsMapping.h"

#include "filter/sobel.h"
#include "filter/resize.h"
#include "filter/kernelsHelpers.h"

#include "runtime_plane.h"

#include "io.h"

namespace image
{
/*
	template<class SrcChannelType, class DstChannelType, int numChannels>
	inline void accumulate(const Plane< Color<SrcChannelType, numChannels> >& img, Color<DstChannelType, numChannels>& res)
	{
		typedef accumulation_type<SrcChannelType>::result SummChT;
		typedef Color<SummChT, numChannels> SummColor;
		typedef Color<SrcChannelType, numChannels> IColor;
		
		SummColor summ; summ.zero();
		nm::index2 i;
		for(i.y=0; i.y < img.sizeY; ++i.y)
		{
			for(i.x=0; i.x < img.sizeX; ++i.x)
			{
				for (int j = 0; j < numChannels; ++j)
				{
					summ.plane[j] += img(i.x, i.y).plane[j];
				}
			}
		}

		res = summ;
	}*/


	template<class CT0, class CT1>
	inline void blend(Plane<CT0>& trg, const Plane<CT1>& bld)
	{
		for(int j = 0; j < bld.sizeY; ++j)
		{
			for(int i = 0; i < bld.sizeX; ++i)
			{
				trg(i, j) *= bld(i, j);
			}
		} 		
	}

	template<class CT0, class CT1>
	inline void add(Plane<CT0>& trg, const Plane<CT1>& bld)
	{
		for(int j = 0; j < bld.sizeY; ++j)
		{
			for(int i = 0; i < bld.sizeX; ++i)
			{
				trg(i, j) += bld(i, j);
			}
		} 		
	}

	namespace Private
	{
		struct copy_tool_r_to_r	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).r() = src(x, y).r(); } };
		struct copy_tool_r_to_g	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).g() = src(x, y).r(); } };
		struct copy_tool_r_to_b	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).b() = src(x, y).r(); } };
		struct copy_tool_r_to_a	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).a() = src(x, y).r(); } };

		struct copy_tool_g_to_r	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).r() = src(x, y).g(); } };
		struct copy_tool_g_to_g	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).g() = src(x, y).g(); } };
		struct copy_tool_g_to_b	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).b() = src(x, y).g(); } };
		struct copy_tool_g_to_a	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).a() = src(x, y).g(); } };

		struct copy_tool_b_to_r	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).r() = src(x, y).b(); } };
		struct copy_tool_b_to_g	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).g() = src(x, y).b(); } };
		struct copy_tool_b_to_b	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).b() = src(x, y).b(); } };
		struct copy_tool_b_to_a	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).a() = src(x, y).b(); } };

		struct copy_tool_a_to_r	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).r() = src(x, y).a(); } };
		struct copy_tool_a_to_g	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).g() = src(x, y).a(); } };
		struct copy_tool_a_to_b	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).b() = src(x, y).a(); } };
		struct copy_tool_a_to_a	{ template<Format f0, Format f1> static void copy(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst, int x, int y) { dst(x, y).a() = src(x, y).a(); } };

		template<class CopyTool, Format f0, Format f1>
		void copy_c_to_c(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst)
		{
			for (int y = 0; y < src.sizeY; ++y)
			{
				for (int x = 0; x < src.sizeX; ++x)
				{
					CopyTool::copy(src, dst, x, y);
				}
			}
		}
	}

	template<Format f0, Format f1> void copy_r_to_r(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_r_to_r>(src, dst); }
	template<Format f0, Format f1> void copy_r_to_g(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_r_to_g>(src, dst); }
	template<Format f0, Format f1> void copy_r_to_b(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_r_to_b>(src, dst); }
	template<Format f0, Format f1> void copy_r_to_a(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_r_to_a>(src, dst); }

	template<Format f0, Format f1> void copy_g_to_r(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_g_to_r>(src, dst); }
	template<Format f0, Format f1> void copy_g_to_g(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_g_to_g>(src, dst); }
	template<Format f0, Format f1> void copy_g_to_b(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_g_to_b>(src, dst); }
	template<Format f0, Format f1> void copy_g_to_a(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_g_to_a>(src, dst); }

	template<Format f0, Format f1> void copy_b_to_r(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_b_to_r>(src, dst); }
	template<Format f0, Format f1> void copy_b_to_g(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_b_to_g>(src, dst); }
	template<Format f0, Format f1> void copy_b_to_b(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_b_to_b>(src, dst); }
	template<Format f0, Format f1> void copy_b_to_a(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_b_to_a>(src, dst); }

	template<Format f0, Format f1> void copy_a_to_r(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_a_to_r>(src, dst); }
	template<Format f0, Format f1> void copy_a_to_g(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_a_to_g>(src, dst); }
	template<Format f0, Format f1> void copy_a_to_b(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_a_to_b>(src, dst); }
	template<Format f0, Format f1> void copy_a_to_a(const Plane< Color<f0> >& src, Plane< Color<f1> >& dst) {	Private::copy_c_to_c<Private::copy_tool_a_to_a>(src, dst); }

	template<class ColorType0, class ColorType1, class ColorType2>
	void blend_replace(const ColorType0& dest, const ColorType1& src, ColorType2& result)
	{
		result = src;
//		image::rgba(src.a(), src.a(), src.a(), src.a(), result);
	}

	template<class ColorType0, class ColorType1, class ColorType2>
	void blend_alpha(const ColorType0& dst, const ColorType1& src, ColorType2& result)
	{
		image::Color<image::RGBA32F> fdst = dst;
		image::Color<image::RGBA32F> fsrc = src;
		result = nm::lerp(fdst, fsrc, (float)fsrc.a());
		//result = fsrcfsrc
	}

	template<class ColorType0, class ColorType1, class ColorType2, class PointType, class RectType, class BlendFunc>
	void blend_rect(const Plane<ColorType0>& srcImage0, const Plane<ColorType1>& srcImage1, const PointType& destStartPos, const RectType& srcRect, Plane<ColorType2>& dstImage, BlendFunc blender)
	{
		Plane<ColorType1> dstT;
		Plane<ColorType1>& dst = (srcImage0.bmp == dstImage.bmp) ? dstT : dstImage;
		
		if (dst.empty())
		{
			dst.copy(srcImage0);
		}

		if (dst.empty())
		{
			dst.copy(srcImage1);
		}
		else
		{
			nm::index2 si, di;
			nm::index2 end(srcRect.pos.x + srcRect.size.x, srcRect.pos.y + srcRect.size.y);

			for (si.y = srcRect.pos.y, di.y = destStartPos.y; si.y < end.y; ++si.y, ++di.y)
			{
				for (si.x = srcRect.pos.x, di.x = destStartPos.x; si.x < end.x; ++si.x, ++di.x)
				{
					ColorType2 res;
					blender(srcImage0(di.x, di.y), srcImage1(si.x, si.y), res);
					dst(di.x, di.y) = res;
				}
			}
		}		

		if (dst.bmp != dstImage.bmp)
		{
			dstImage.copy(dst);
		}
	}

	template<class ColorType0, class ColorType1, class RectType>
	void copy_rect(const Plane<ColorType0>& srcImage, Plane<ColorType1>& dstImage, const RectType& srcRect, const RectType& dstRect)
	{
		ENFORCE(srcRect.size == dstRect.size);

		Plane<ColorType1> dstT;
		Plane<ColorType1>& dst = (srcImage.bmp == dstImage.bmp) ? dstT : dstImage;
		const Plane<ColorType0>& src = srcImage;

		if (dst.empty())
		{
			dst.allocate(dstRect.size.x, dstRect.size.y);
		}

		nm::index2 si, di;
		for (si.y = srcRect.pos.y, di.y = dstRect.pos.y; si.y < srcRect.pos.y + srcRect.size.y; ++si.y, ++di.y)
		{
			for (si.x = srcRect.pos.x, di.x = dstRect.pos.x; si.x < srcRect.pos.x + srcRect.size.x; ++si.x, ++di.x)
			{
				dst(di.x, di.y) = srcImage(si.x, si.y);
			}
		}			

		if (dst.bmp != dstImage.bmp)
		{
			dstImage.copy(dst);
		}
	}

	template<class ColorType0, class ColorType1, class RectType>
	void sub_rect(const Plane<ColorType0>& srcImage, Plane<ColorType1>& dstImage, const RectType& subRect)
	{
		ENFORCE(
			(srcImage.sizeX >= (subRect.pos.x + subRect.size.x)) && 
			(srcImage.sizeY >= (subRect.pos.y + subRect.size.y))
			);

		RectType dstRect;

		dstRect.pos.x = 0;
		dstRect.pos.y = 0;
		dstRect.size = subRect.size;

		copy_rect(srcImage, dstImage, subRect, dstRect);
	}

	template<class ColorType0, class ColorType1>
	void flip_horizontal(const Plane<ColorType0>& srcImage, Plane<ColorType1>& dstImage)
	{
		Plane<ColorType1> dstT;
		Plane<ColorType1>& dst = (srcImage.bmp == dstImage.bmp) ? dstT : dstImage;
		const Plane<ColorType0>& src = srcImage;		

		if (dst.empty())
		{
			dst.allocate(src.sizeX, src.sizeY);
		}
		
		for (int y = 0; y < src.sizeY; ++y)
		{
			for (int x = 0; x < src.sizeX; ++x)
			{
				dst(x, y) = src(src.sizeX - x - 1, y);
			}
		}

		if (dst.bmp != dstImage.bmp)
		{
			dstImage.copy(dst);
		}
	}

	template<class ColorType0, class ColorType1>
	void flip_vertical(const Plane<ColorType0>& srcImage, Plane<ColorType1>& dstImage)
	{
		Plane<ColorType1> dstT;
		Plane<ColorType1>& dst = (srcImage.bmp == dstImage.bmp) ? dstT : dstImage;
		const Plane<ColorType0>& src = srcImage;

		if (dst.empty())
		{
			dst.allocate(src.sizeX, src.sizeY);
		}

		for (int y = 0; y < src.sizeY; ++y)
		{
			for (int x = 0; x < src.sizeX; ++x)
			{
				dst(x, y) = src(x, src.sizeY - y - 1);
			}
		}

		if (dst.bmp != dstImage.bmp)
		{
			dstImage.copy(dst);
		}
	}

	template<class ColorType>
	bool has_alpha(const Plane<ColorType>& img)
	{
		for (int y = 0; y < img.sizeY; ++y)
		{
			for (int x = 0; x < img.sizeX; ++x)
			{
				if (has_alpha_value(img(x, y)))
					return true;
			}
		}

		return false;
	}
}