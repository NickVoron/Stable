#pragma once

#include "vector.h"


namespace nm 
{

struct index2 {

	// Construction:
	index2() { operator=( outIndex ); }
	index2( int sx, int sy ) : x(sx), y(sy) {}

	// data:
	int x, y;
	// static data:
	static const index2 outIndex;
	static const index2 zeroIndex;
	static const index2 oneIndex;

	// Operations:
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



	// Utility operations:
	bool IsCorrect() const { return operator!=( outIndex ); }
	inline int Square() const { return abs(x*y); }
	inline int magnitudeSq() const	{ return x*x + y*y; }
// 	inline int magnitude() const	{ return sqrt(x*x + y*y); }
};

// 
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

