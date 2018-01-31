// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <stream/library.include.h>

namespace nm
{
	struct index3
	{
		
		index3( int sx, int sy, int sz ) : x(sx), y(sy), z(sz) {}
		index3() { operator=( outIndex ); }

		
		int x, y, z;
		static const index3 outIndex;

		
		const index3& operator=( const index3& i ) { x = i.x; y = i.y; z=i.z; return *this; }

		
		bool operator==( const index3& i ) const	{ return (i.x == x) && (i.y == y) && (i.z==z); }
		bool operator!=( const index3& i ) const	{ return ! operator==( i ); }
		bool operator<(const index3& i) const		{ return (i.x > x) || ((i.x == x) && (i.y > y)) || ((i.x == x) && (i.y == y) && (i.z > z)); }

		bool operator<=(const index3& i) const		{ return operator<(i) || operator==(i); }


		index3 operator+( const index3& i ) const	{ index3 res(*this); res += i; return res; }
		index3 operator-( const index3& i ) const	{ index3 res(*this); res -= i; return res; }
		index3 operator-() const					{ index3 res( -x, -y, -z ); return res; }


		index3& operator+=( const index3& i ) { x += i.x; y += i.y; z+=i.z; return *this;}
		index3& operator-=( const index3& i ) { x -= i.x; y -= i.y; z-=i.z; return *this;}

		index3 operator*( int d ) const	{ index3 res(*this); res *= d; return res; }
		index3 operator/( int d ) const	{ index3 res(*this); res /= d; return res; }
		index3 operator%( int d ) const	{ index3 res(*this); res %= d; return res; }


		index3& operator*=( int d ) { x *= d; y *= d; z*=d; return *this;}
		index3& operator/=( int d ) { x /= d; y /= d; z/=d; return *this;}
		index3& operator%=( int d ) { x %= d; y %= d; z%=d; return *this;}


		index3 operator*( const index3& i ) const	{ index3 res(*this); res *= i; return res; }
		index3 operator/( const index3& i ) const	{ index3 res(*this); res /= i; return res; }
		index3 operator%( const index3& i ) const	{ index3 res(*this); res %= i; return res; }

		index3& operator*=( const index3& i )		{ x *= i.x; y *= i.y; z*=i.z; return *this;}
		index3& operator/=( const index3& i )		{ x /= i.x; y /= i.y; z/=i.z; return *this;}
		index3& operator%=( const index3& i )		{ x %= i.x; y %= i.y; z%=i.z; return *this;}



		bool IsCorrect() const { return operator!=( outIndex ); }
		inline int Square() const { return abs(x*y*z); }
		inline int magnitudeSq() const { return x*x + y*y + z*z; }
	};

	stream::ostream& operator<<(stream::ostream& os, const index3& index);
	stream::istream& operator >> (stream::istream& is, index3& index);

}



// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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