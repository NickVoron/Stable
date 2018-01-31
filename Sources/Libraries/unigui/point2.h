// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <ostream>

namespace unigui{
namespace math{

template<class T>
class Point2
{
public:
	T x, y;

	inline Point2():x(0), y(0) {}
	inline Point2(T _x, T _y): x(_x), y(_y) {}

	template<class TT>
	inline Point2(TT _x, TT _y): x((T)_x), y((T)_y) {}

	template<class VT>
	inline Point2(const VT& v): x((T)v.x), y((T)v.y) {}

	inline T& operator[] (int i); 
	inline T operator[] (int i) const;

	inline Point2& operator += (const Point2& p);
	inline Point2& operator -= (const Point2& p);
	inline Point2& operator *= (const Point2& p);
	inline Point2& operator /= (const Point2& p);
	inline Point2& operator /= (T f);
	inline Point2& operator *= (T f);

	inline Point2 operator + (const Point2& p) const;
	inline Point2 operator - (const Point2& p) const;
	inline Point2 operator * (const Point2& p) const;
	inline Point2 operator / (const Point2& p) const;
	inline Point2 operator * (T f) const;
	inline Point2 operator / (T f) const;
	inline Point2 operator- () const;

	inline bool operator == (const Point2& p) const; 
	inline bool operator != (const Point2& p) const;

	inline void zero() { *this = ZERO(); }
	inline void invalidate() { *this = INF(); }

	static inline Point2 ZERO(){ return Point2(0, 0); }
	static inline Point2 INF(){ return Point2(1000000, 1000000); }
};




template<class T> T& Point2<T>::operator[] (int i) {return (&x)[i];}
template<class T> T Point2<T>::operator[] (int i) const { return (&x)[i];}

template<class T> Point2<T>& Point2<T>::operator += (const Point2& p){ x += p.x; 	y += p.y; return *this; }
template<class T> Point2<T>& Point2<T>::operator -= (const Point2& p){ x -= p.x;		y -= p.y; return *this; }
template<class T> Point2<T>& Point2<T>::operator *= (const Point2& p){ x *= p.x; 	y *= p.y; return *this; }
template<class T> Point2<T>& Point2<T>::operator /= (const Point2& p){ x /= p.x;	y /= p.y; return *this; }
template<class T> Point2<T>& Point2<T>::operator /= (T f) {	x /= f; y /= f;	return *this;}
template<class T> Point2<T>& Point2<T>::operator *= (T f) {	x *= f; y *= f;	return *this;}

template<class T> Point2<T> Point2<T>::operator + (const Point2& _p) const{	Point2 p(*this);	return p += _p;}
template<class T> Point2<T> Point2<T>::operator - (const Point2& _p) const{	Point2 p(*this);	return p -= _p;}
template<class T> Point2<T> Point2<T>::operator * (const Point2& _p) const{	Point2 p(*this);	return p *= _p;}
template<class T> Point2<T> Point2<T>::operator / (const Point2& _p) const{	Point2 p(*this);	return p /= _p;}
template<class T> Point2<T> Point2<T>::operator * (T f) const{	Point2 p(*this);	return p *= f;}
template<class T> Point2<T> Point2<T>::operator / (T f) const{	Point2 p(*this);	return p /= f;}
template<class T> Point2<T> Point2<T>::operator- () const{	return Point2(-x, -y);}

template<class T> bool Point2<T>::operator == (const Point2& p) const {	return p.x == x && p.y == y;}
template<class T> bool Point2<T>::operator != (const Point2& p) const {	return !operator==(p);}

template<class T> bool compare(const Point2<T>& p0, const Point2<T>& p1);

template<> inline bool compare(const Point2<int>& p0, const Point2<int>& p1) { return p0.x == p1.x && p0.y == p1.y; };
template<> inline bool compare(const Point2<float>& p0, const Point2<float>& p1) 
{ 
	Point2<float> p( p0.x - p1.x, p0.y - p1.y );
	p.x *= p.x;
	p.y *= p.y;

	return (p.x + p.y) < 0.0001f; 
};





template<class T> 
Point2<T> middlePoint(const Point2<T>& p1, const Point2<T>& p2)
{
	Point2<T> minPt(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
	Point2<T> maxPt(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
	return (maxPt - minPt) / 2;
}

template<class Type> 
std::ostream& operator<<(std::ostream& os, const unigui::math::Point2<Type>& point)
{
	return os << "{x = " << point.x << ", y = " << point.y << "}";
}

}

typedef math::Point2<float> Point2;

}



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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