// Copyright (C) 2015-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.h"


namespace nm 
{

struct index2 {

	
	index2() { operator=( outIndex ); }
	index2( int sx, int sy ) : x(sx), y(sy) {}

	
	int x, y;
	
	static const index2 outIndex;
	static const index2 zeroIndex;
	static const index2 oneIndex;

	
	bool operator==( const index2& i ) const { return ( i.x == x ) && ( i.y == y ); }
	bool operator!=( const index2& i ) const { return ! operator==( i ); }

	index2 operator+( const index2& i ) const	{ index2 res(*this); res += i; return res; }
	index2 operator-( const index2& i ) const	{ index2 res(*this); res -= i; return res; }
	index2 operator-() const					{ index2 res( -x, -y ); return res; }


	index2& operator+=( const index2& i ) { x += i.x; y += i.y; return *this;}
	index2& operator-=( const index2& i ) { x -= i.x; y -= i.y; return *this;}

	index2 operator*( int d ) const	{ index2 res(*this); res *= d; return res; }
	index2 operator/( int d ) const	{ index2 res(*this); res /= d; return res; }
	index2 operator%( int d ) const	{ index2 res(*this); res %= d; return res; }

	index2& operator*=( int d ) { x *= d; y *= d; return *this;}
	index2& operator/=( int d ) { x /= d; y /= d; return *this;}
	index2& operator%=( int d ) { x %= d; y %= d; return *this;}

	index2 operator*( const index2& i ) const	{ index2 res(*this); res *= i; return res; }
	index2 operator/( const index2& i ) const	{ index2 res(*this); res /= i; return res; }
	index2 operator%( const index2& i ) const	{ index2 res(*this); res %= i; return res; }
	index2& operator*=( const index2& i )		{ x *= i.x; y *= i.y; return *this;}
	index2& operator/=( const index2& i )		{ x /= i.x; y /= i.y; return *this;}
	index2& operator%=( const index2& i )		{ x %= i.x; y %= i.y; return *this;}



	
	bool IsCorrect() const { return operator!=( outIndex ); }
	inline int Square() const { return abs(x*y); }
	inline int magnitudeSq() const	{ return x*x + y*y; }

};


struct index2Iterator	{
	index2Iterator(const index2& b, const index2& e) : val(b), begin(b), end(e) {}
	
	bool Forward()
	{ 
		if(++val.x >= end.x)
		if(++val.y >= end.y)
			return false;
		return true;
	}

	bool Backward()
	{ 
		if(--val.x <= begin.x)
		if(--val.y <= begin.y)
			return false;
		return true;
	}

	const index2& operator()() const { return val; }

	index2 val;
	const index2 begin;
	const index2 end;
};

inline Vector2 floatize(const index2& idx) { return Vector2((float) idx.x, (float) idx.y); }
  

}





// Copyright (C) 2015-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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