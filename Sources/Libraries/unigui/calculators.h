#pragma once

#include "point2.h"
#include "rect.h"

namespace unigui
{

template<class T>
struct Margins
{
	inline Margins():bottom(0), left(0), right(0), top(0){}
	T left, right, top, bottom;
};

inline bool hasFlag(int mask, int flags){return (mask & flags) == flags;}

struct Align
{
	enum Mask
	{
		CENTER	= 0x00000001,
		LEFT	= 0x00000010,
		RIGHT	= 0x00000100,
		TOP		= 0x00001000,
		BOTTOM	= 0x00010000,
		CLIENT_X= LEFT | RIGHT,
		CLIENT_Y= TOP | BOTTOM,
		CLIENT	= CLIENT_X | CLIENT_Y,
	};

	inline Align(): c(false), l(false), r(false), t(false), b(false){}
	inline Align(bool center, bool left, bool right, bool top, bool bottom): c(center), l(left), r(right), t(top), b(bottom){}
	inline int mask() const;
	bool c, l, r, t, b;

	inline Align& operator += (const Align& a)
	{
		c	= a.c || c;
		l	= a.l || l;
		r	= a.r || r;
		t	= a.t || t;
		b	= a.b || b;

		return *this;
	}

	inline Align& operator -= (const Align& a)
	{
		if(a.c) c = false;
		if(a.l) l = false;
		if(a.r) r = false;
		if(a.t) t = false;
		if(a.b) b = false;

		return *this;
	}

	inline Align operator + (const Align& a){ Align r = *this; r += a; return r; }
	inline Align operator - (const Align& a){ Align r = *this; r -= a; return r; }

	inline Align& operator += (int mask) { return operator +=( construct(mask) ); }
	inline Align& operator -= (int mask) { return operator -=( construct(mask) ); }

	inline Align operator + (int mask) const { Align r = *this; r += mask; return r; }
	inline Align operator - (int mask) const { Align r = *this; r -= mask; return r; }

	static inline Align construct(int mask) 
	{
		Align a;
		a.c	= hasFlag(mask, CENTER);
		a.l	= hasFlag(mask, LEFT);
		a.r	= hasFlag(mask, RIGHT);
		a.t	= hasFlag(mask, TOP);
		a.b	= hasFlag(mask, BOTTOM);

		return a;
	}

	static inline Align center()	{ return Align(true, false, false, false, false);	}
	static inline Align left()		{ return Align(false, true, false, false, false);	}
	static inline Align right()		{ return Align(false, false, true, false, false);	}
	static inline Align top()		{ return Align(false, false, false, true, false);	}
	static inline Align bottom()	{ return Align(false, false, false, false, true);	}
	static inline Align client()	{ return Align(false, true, true, true, true);		}
	static inline Align clientX()	{ return Align(false, true, true, false, false);	}
	static inline Align clientY()	{ return Align(false, false, false, true, true);	}
};

template<class T>
struct Constraints 
{
	inline Constraints():bottom(0), left(0), right(0), top(0), canBeOutside(true){}
	inline void normalize() const;

	T left, right, top, bottom;
	bool canBeOutside;
};

struct RelativeSize
{
	bool use = false;
	float coef = 0.0f;
};


template<class T>
struct RelativePlaceParams 
{
	math::Rect<T> wishfulRect;

	RelativeSize relativeX;
	RelativeSize relativeY;

	Align align;
	Margins<T> margins;
	Constraints<T> constrains;
};

template<class T>
inline void Constraints<T>::normalize() const
{
	if(left > right) std::swap(left, right);
	if(top > bottom) std::swap(top, bottom);
}

inline int Align::mask() const
{
	int res = 0;
	res |= c ? CENTER : 0;
	res |= l ? LEFT	  : 0;
	res |= r ? RIGHT  : 0;
	res |= t ? TOP	  : 0;
	res |= b ? BOTTOM : 0;

	return res;	
}

template<class T>
void alignRect(math::Rect<T>& rect, const math::Rect<T>& parentRect, const Align& align)
{
	int mask = align.mask();

	if(mask == Align::CLIENT)	
	{ 
		rect = parentRect; 
	}
	else if( hasFlag(mask, Align::CLIENT_X) )
	{
		rect.pos.x = parentRect.pos.x; 
		rect.size.x = parentRect.size.x;

			 if( hasFlag(mask, Align::TOP) )	rect.pos.y = parentRect.top();
		else if( hasFlag(mask, Align::BOTTOM) ) rect.pos.y = parentRect.bottom() - rect.size.y;
		else if( hasFlag(mask, Align::CENTER) ) rect.pos.y = parentRect.centerV() - rect.halfHeight();
	}
	else if( hasFlag(mask, Align::CLIENT_Y) )
	{
		rect.pos.y = parentRect.pos.y; 
		rect.size.y = parentRect.size.y;

			 if( hasFlag(mask, Align::LEFT) )	rect.pos.x = parentRect.left();
		else if( hasFlag(mask, Align::RIGHT) )	rect.pos.x = parentRect.right() - rect.size.x;
		else if( hasFlag(mask, Align::CENTER) ) rect.pos.x = parentRect.centerH() - rect.halfWidth();
	}
	else
	{
		if(hasFlag(mask, Align::CENTER))	{ rect.toCenter(parentRect); }
		if(hasFlag(mask, Align::LEFT))		{ rect.toLeft(parentRect); }
		if(hasFlag(mask, Align::RIGHT))		{ rect.toRight(parentRect); }
		if(hasFlag(mask, Align::TOP))		{ rect.toTop(parentRect); }
		if(hasFlag(mask, Align::BOTTOM))	{ rect.toBottom(parentRect); }
	}
}

template<class RectT>
inline RectT alignedRect(const RectT& rect, const RectT& parentRect, const Align& align)
{
	RectT res = rect;
	alignRect(res, parentRect, align);
	return res;
}

template<class T>
void marginsRect(math::Rect<T>& parentRect, const Margins<T>& margins)
{
	parentRect.pos.x += margins.left;
	parentRect.pos.y += margins.top;
	parentRect.size.x -= margins.right;
	parentRect.size.y -= margins.bottom;
}

template<class T>
void calcRelativePlace(math::Rect<T>& rect, const math::Rect<T>& parentRect, const RelativePlaceParams<T>& params)
{
	rect = params.wishfulRect;
	auto marginedRect = parentRect;
	marginsRect(marginedRect, params.margins);
	alignRect(rect, marginedRect, params.align);
}

}

