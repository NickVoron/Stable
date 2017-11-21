#pragma once
#include <stream/library.include.h>

namespace nm
{
	struct index3
	{
		// Constructors:
		index3( int sx, int sy, int sz ) : x(sx), y(sy), z(sz) {}
		index3() { operator=( outIndex ); }

		//data:
		int x, y, z;
		static const index3 outIndex;

		// Operators:
		const index3& operator=( const index3& i ) { x = i.x; y = i.y; z=i.z; return *this; }

		// Operations:
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