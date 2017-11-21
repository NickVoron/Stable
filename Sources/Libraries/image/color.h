#pragma once
#include "intcolor.h"
#include "type_utils.h"
#include "channelsMapping.h"
#include "colorSpaces/library.include.h"

#pragma pack(push,1)

namespace image
{

template<image::Format fmt> 
struct Color : public ChannelsTranslator<fmt, Color<fmt> >
{
	typedef ChannelsTranslator<fmt, Color<fmt> > ChTrans;
	typedef typename ChTrans::value_t value_t;
	const static Format FMT = fmt;

	static int pixelSize() { return pixel_info<fmt>::size; }
	static const char* formatName() { return pixel_info<fmt>::name(); }
	inline static Format format() { return fmt; }

	Color() {}

	template<Format fmt0>
	Color(const Color<fmt0>& c):ChTrans(c){}

	Color& zero() { mem::memzero(ChTrans::plane); return *this; }

	template<Format fmt0> void operator += (const Color<fmt0>& c) { this->addc(c); }
	template<Format fmt0> void operator -= (const Color<fmt0>& c) { this->subc(c); }
	template<Format fmt0> void operator *= (const Color<fmt0>& c) { this->mulc(c); }
	template<Format fmt0> void operator /= (const Color<fmt0>& c) { this->divc(c); }

	template<Format fmt0> Color operator + (const Color<fmt0>& c) const { Color cc(*this); cc += c; return cc; }
	template<Format fmt0> Color operator - (const Color<fmt0>& c) const { Color cc(*this); cc -= c; return cc; }
	template<Format fmt0> Color operator * (const Color<fmt0>& c) const { Color cc(*this); cc *= c; return cc; }
	template<Format fmt0> Color operator / (const Color<fmt0>& c) const { Color cc(*this); cc /= c; return cc; }

	template<typename scalar> void operator += (scalar f)	{ this->add(f); }
	template<typename scalar> void operator -= (scalar f)	{ this->sub(f); }
	template<typename scalar> void operator *= (scalar f)	{ this->mul(f); }
	template<typename scalar> void operator /= (scalar f)	{ this->div(f); }

	template<typename scalar> Color operator + (scalar f) const	{ Color cc(*this); cc += f; return cc; }
	template<typename scalar> Color operator - (scalar f) const	{ Color cc(*this); cc -= f; return cc; }
	template<typename scalar> Color operator * (scalar f) const	{ Color cc(*this); cc *= f; return cc; }
	template<typename scalar> Color operator / (scalar f) const	{ Color cc(*this); cc /= f; return cc; }
};

template<Format fmt> 
void rgba(float r, float g, float b, float a, Color<fmt>& dst)
{
	dst.r() = r;
	dst.g() = g;
	dst.b() = b;
	dst.a() = a;
}

template<Format fmt> 
void rgb(float r, float g, float b, Color<fmt>& dst)
{
	dst.r() = r;
	dst.g() = g;
	dst.b() = b;
}

template<Format fmt> 
Color<fmt> rgba(float r, float g, float b, float a)
{
	Color<fmt> cc;
	rgba(r, g, b, a, cc);
	return cc;
}

template<Format fmt> 
Color<fmt> rgb(float r, float g, float b)
{
	Color<fmt> cc;
	rgb(r, g, b, cc);
	return cc;
}

//
//
//
template<Format fmt> 
void hsva(float h, float s, float v, float a, Color<fmt>& dst)
{
	float r, g, b;
	image::hsv_to_rgb(h, s, v, r, g, b);

	dst.r() = r;
	dst.g() = g;
	dst.b() = b;
	dst.a() = a;
}

template<Format fmt> 
void hsv(float h, float s, float v, Color<fmt>& dst)
{
	float r, g, b;
	image::hsv_to_rgb(h, s, v, r, g, b);

	dst.r() = r;
	dst.g() = g;
	dst.b() = b;
}

template<Format fmt> 
Color<fmt> hsva(float h, float s, float v, float a)
{
	Color<fmt> cc;
	hsva(h, s, v, a, cc);
	return cc;
}

template<Format fmt> 
Color<fmt> hsv(float h, float s, float v)
{
	Color<fmt> cc;
	hsv(h, s, v, cc);
	return cc;
}


//
//
//
template<Format fmt>
bool has_alpha_value(const Color<fmt>& color)
{
	return (max_value<typename Color<fmt>::value_t>::tvalue() - color.a() ) > 0;
}

};//namespace Base

#pragma pack(pop)
