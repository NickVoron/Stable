// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "point2.h"

namespace unigui{
namespace math{


template<class T>
class Rect
{
public:
	typedef Point2<T> PointType;

	inline Rect() {}
	inline Rect(T x, T y, T w, T h): pos(x, y), size(w, h) {}
	inline Rect(const PointType& pos_, const PointType& size_): pos(pos_), size(size_) {}

	template<class VT>
	inline Rect(const VT& p_, const VT& s_): pos(p_), size(s_) {}

	T left()	const;
	T right()	const;
	T top()		const;
	T bottom()	const;

	T centerH() const;
	T centerV() const;

	PointType leftTop()			const;
	PointType centerTop()		const;
	PointType rightTop()		const;
	PointType leftBottom()		const;
	PointType centerBottom()	const;
	PointType rightBottom()		const;
	PointType leftCenter()		const;
	PointType rightCenter()		const;
	PointType center()			const;

	T halfWidth()	const;
	T halfHeight()	const;
	T square() const { return size.x * size.y; }

	bool collapsed() const;
	void collapse();
	void normalize();

	
	void toCenter(const Rect& parentRect);

	void toLeft(T l);
	void toRight(T r);
	void toTop(T t);
	void toBottom(T b);

	void toLeft(const Rect& parentRect);
	void toRight(const Rect& parentRect);
	void toTop(const Rect& parentRect);
	void toBottom(const Rect& parentRect);

	bool contain(const PointType& p) const;
	
	bool contain(const Rect& r) const;
	bool canContain(const Rect& r) const;
	bool intersected(const Rect& r) const;
	bool intersected_neq(const Rect& r) const;
	

	void cutLeft(T size);
	void cutRight(T size);
	void cutTop(T size);
	void cutBottom(T size);

	void addLeft(T size);
	void addRight(T size);
	void addTop(T size);
	void addBottom(T size);

	inline bool operator == (const Rect& p) const { return pos == p.pos && size == p.size; }
	inline bool operator != (const Rect& p) const { return !operator==(p); }


	PointType pos;
	PointType size;

	static Rect COLLAPSED(){ return Rect(PointType::ZERO(), PointType::ZERO()); }
};

template<class T> T Rect<T>::halfWidth() const {return size.x / 2;}
template<class T> T Rect<T>::halfHeight() const {return size.y / 2;}
template<class T> T Rect<T>::centerH() const {return pos.x + halfWidth();}
template<class T> T Rect<T>::centerV() const {return pos.y + halfHeight();}

template<class T> T Rect<T>::left()		const	{ return pos.x; }
template<class T> T Rect<T>::top()		const	{ return pos.y;}
template<class T> T Rect<T>::right()	const	{ return pos.x + size.x;}
template<class T> T Rect<T>::bottom()	const	{ return pos.y + size.y;}

template<class T> bool Rect<T>::collapsed() const{	return compare(size, PointType::ZERO());}
template<class T> void Rect<T>::collapse(){	pos.x = pos.y = size.x = size.y = 0;}

template<class T> void Rect<T>::normalize()
{
	if(size.x < 0) { size.x = -size.x; pos.x -= size.x; }
	if(size.y < 0) { size.y = -size.y; pos.y -= size.y; }
}

template<class T> Point2<T> Rect<T>::leftTop()		const {	return pos;}
template<class T> Point2<T> Rect<T>::centerTop()	const {	return PointType(centerH(), top() );}
template<class T> Point2<T> Rect<T>::rightTop()		const {	return PointType(right(), top());}
template<class T> Point2<T> Rect<T>::leftBottom()	const {	return PointType(left(), bottom());}
template<class T> Point2<T> Rect<T>::centerBottom()	const {	return PointType(centerH(), bottom() );}
template<class T> Point2<T> Rect<T>::rightBottom()	const {	return PointType(right(), bottom());}
template<class T> Point2<T> Rect<T>::leftCenter()	const {	return PointType(left(), centerV());}
template<class T> Point2<T> Rect<T>::rightCenter()	const {	return PointType(right(), centerV());}
template<class T> Point2<T> Rect<T>::center()		const {	return PointType(centerH(), centerV());}

template<class T> void Rect<T>::toLeft(T l){pos.x = l;}
template<class T> void Rect<T>::toTop(T t){pos.y = t;}
template<class T> void Rect<T>::toRight(T r){pos.x = r - size.x;}
template<class T> void Rect<T>::toBottom(T b){pos.y = b - size.y;}

template<class T> void Rect<T>::toLeft(const Rect& parentRect){ toLeft(parentRect.pos.x); }
template<class T> void Rect<T>::toTop(const Rect& parentRect){ toTop(parentRect.pos.y);}
template<class T> void Rect<T>::toRight(const Rect& parentRect){ toRight(parentRect.right()); }
template<class T> void Rect<T>::toBottom(const Rect& parentRect){ toBottom(parentRect.bottom());}
template<class T> void Rect<T>::toCenter(const Rect& parentRect)
{
	pos.x = parentRect.centerH() - halfWidth();
	pos.y = parentRect.centerV() - halfHeight();
}

template<class T> void Rect<T>::cutLeft(T sz)
{
	T s = std::min(sz, size.x);
	pos.x += s;
	size.x -= s;
}

template<class T> void Rect<T>::cutRight(T sz)
{
	size.x -= std::min(sz, size.x);
}

template<class T> void Rect<T>::cutTop(T sz)
{
	T s = std::min(sz, size.y);
	pos.y += s;
	size.y -= s;
}

template<class T> void Rect<T>::cutBottom(T sz)
{
	size.y -= std::min(sz, size.y);
}

template<class T> void Rect<T>::addLeft(T sz)
{
	pos.x -= sz;
	size.x += sz;
}

template<class T> void Rect<T>::addRight(T sz)
{
	size.x += sz;
}

template<class T> void Rect<T>::addTop(T sz)
{
	pos.y -= sz;
	size.y += sz;
}

template<class T> void Rect<T>::addBottom(T sz)
{
	size.y += sz;
}


template<class T> bool Rect<T>::contain(const Point2<T>& p) const{ return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom(); }
template<class T> bool Rect<T>::contain(const Rect<T> &r) const { return r.left() >= left() && r.top() >= top() && r.right() <= right() && r.bottom() <= bottom(); }






template<class T> bool Rect<T>::intersected(const Rect<T> &r) const 
{ 
	return r.right() >= left() && r.left() <= right() && r.bottom() >= top() && r.top() <= bottom(); 
}

template<class T> bool Rect<T>::intersected_neq(const Rect<T> &r) const
{
	return r.right() > left() && r.left() < right() && r.bottom() > top() && r.top() < bottom();
}




template<class T> 
T get_middle(T i1, T i2, T i3)
{
	T max1 = std::max( i1, i2);
	T max2 = std::max( i2, i3);
	return std::min(max1, max2);
}

template<class T> 
Rect<T> conjunction(const Rect<T>& r1, const Rect<T>& r2)
{
	if( !r1.intersected(r2) )
	{
		return Rect<T>::COLLAPSED();
	}
	else
	{
		T middle_x1 = get_middle(r1.left(), r2.left(), r2.right());
		T middle_x2 = get_middle(r2.right(), r1.left(), r1.right());
		T middle_y1 = get_middle(r1.top(), r2.top(), r2.bottom());
		T middle_y2 = get_middle(r2.bottom(), r1.top(), r1.bottom());

		Point2<T> minPt(std::min(middle_x1, middle_x2), std::min(middle_y1, middle_y2));
		Point2<T> maxPt(std::max(middle_x1, middle_x2), std::max(middle_y1, middle_y2));
	
		return Rect<T>(minPt, maxPt - minPt);
	}
}

template<class T> 
inline Rect<T> convexHull(const Rect<T>& r1, const Rect<T>& r2)
{
	Rect<T> r;
	r.pos.x = std::min(r1.pos.x, r2.pos.x);
	r.pos.y = std::min(r1.pos.y, r2.pos.y);
	r.size = r1.rightBottom();
	Point2<T> br2 = r2.rightBottom();
	r.size.x = std::max(r.size.x, br2.x); 
	r.size.y = std::max(r.size.y, br2.y);
	r.size -= r.pos; 
	return r;
}

template<class T> 
inline Point2<T> clamp(const Point2<T>& pt, const Rect<T>& rect)
{
	Point2<T> res;
	res.x = std::min(pt.x, rect.right());
	res.x = std::max(pt.x, rect.left());
	res.y = std::min(pt.y, rect.bottom());
	res.y = std::max(pt.y, rect.top());
	return res;
}

template<class Type>
std::ostream& operator<<(std::ostream& os, const Rect<Type>& rect)
{
	return os << "{pos = " << rect.pos << "}" << "{size = " << rect.size << "}";
}


}

typedef math::Rect<float> Rect;
}



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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