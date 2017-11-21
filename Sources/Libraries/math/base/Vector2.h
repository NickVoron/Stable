/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 20.07.2001
	  Time: 5:10:04

	Abstract:	Standard 2x1 column vector class

*********************************************************************/
#pragma once

#include <math.h>
#include <float.h>
#include <stdlib.h>

#undef Vector2
#undef SDot
#undef SCross

// The Vector2 class implements a 2x1 column vector or 1x2 row vector
// ( dep on the context )
// This class and the associated MatrixN and Quaternion classes are
// provided to simplify vector and math in code.

class Vector2
{
public:

	// Public data
	float x, y;

	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 xAxis;
	static const Vector2 yAxis;
	static const Vector2 xNegAxis;
	static const Vector2 yNegAxis;

	// Ctor / dtor
	inline Vector2();
	inline Vector2( float x, float y );
	inline Vector2( const float vector[] );

	// Load / save
	inline void Load( const float vector[] );
	inline void Save( float vector[] ) const;

	// Reset to zero
	inline void Zero();
	static inline Vector2 GetZero();

	// Access
	inline float& operator[]( size_t index );
	inline const float& operator [] (size_t index ) const;

	// Scalar

	// infix
	inline Vector2& operator += ( float scalar );
	inline Vector2& operator -= ( float scalar );
	inline Vector2& operator *= ( float scalar );
	inline Vector2& operator /= ( float scalar );

	inline Vector2 operator + ( float scalar ) const;
	inline Vector2 operator - ( float scalar ) const;
	inline Vector2 operator * ( float scalar ) const;
	inline Vector2 operator / ( float scalar ) const;

	friend Vector2 operator + ( float scalar, const Vector2& vector );
	friend Vector2 operator - ( float scalar, const Vector2& vector );
	friend Vector2 operator * ( float scalar, const Vector2& vector );

	// Vector ops

	// SDot product
	inline float SDot( const Vector2& vector ) const;

	// Returns length of given vector projected onto this
	inline float Projection( const Vector2& vector ) const;

	// Exact magnitude with the square root
	inline float Magnitude() const;
	inline float MagnitudeSquared() const;

	// Exact inverse magnitude
	inline float InvMagnitude() const;
	inline float InvMagnitudeSquared() const;

	// Returns normalized vector
	inline Vector2 GetNormalized() const;
	inline Vector2& Normalize();

	// Faster magnitude approx
	inline float ApproxMagnitude() const;

	// Faster inverse magnitude approximation
	inline float ApproxInvMagnitude() const;

	// Returns normalized version of this vector using approximate magnitude
	inline Vector2 GetApproxNormalized() const;
	inline Vector2& ApproxNormalized();

	// Unary
	friend Vector2 operator + ( const Vector2& vector );
	friend Vector2 operator - ( const Vector2& vector );

	// Infix
	inline Vector2& operator += ( const Vector2& vector );
	inline Vector2& operator -= ( const Vector2& vector );

	inline Vector2 operator + ( const Vector2& vector ) const;
	inline Vector2 operator - ( const Vector2& vector ) const;

	inline Vector2& mul( const Vector2& vector );

	// vector logical compare
	// ! with an given tolerance!
	inline bool Equal( const Vector2& vector, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Vector2& vector, float tolerance = 0.000001f) const;
	
	// Vector binary compare
	inline bool operator == ( const Vector2& vector ) const;
	inline bool operator != ( const Vector2& vector ) const;

protected:
	// Approximate the magnitude of a 2d vector using Taylor series 
	// expansion. Accuracy of result is between 0.99946 and 1.0 dependin'
	// on the length of the different components
	inline float ApproximateMagnitude( float x1, float y1 ) const;
};


///////////////////////////////////////////////////////////////////
/// Implementation

Vector2::Vector2()
{}

Vector2::Vector2( float ax, float ay )
{
	x = ax; y = ay;
}

Vector2::Vector2( const float vector[] )
{
	x = vector[0];
	y = vector[1];
}

//

void Vector2::Load( const float vector[] )
{
	x = vector[0];
	y = vector[1];
}

void Vector2::Save( float vector[] ) const
{
	vector[0] = x;
	vector[1] = y;
}

//

void Vector2::Zero()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2 Vector2::GetZero() 
{
	return Vector2( 0.0f, 0.0f );
}

//

float& Vector2::operator [] (size_t index )
{
	return *((&x) + index );
}

const float& Vector2::operator [] (size_t index ) const
{
	return *((&x) + index );
}

// 

Vector2& Vector2::operator += ( float scalar )
{
	x += scalar;
	y += scalar;
	return *this;
}

Vector2& Vector2::operator -= ( float scalar )
{
	x -= scalar;
	y -= scalar;
	return *this;
}

Vector2& Vector2::operator *= ( float scalar )
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2& Vector2::operator /= ( float scalar )
{
	float invScalar = 1.0f / scalar;
	x *= invScalar;
	y *= invScalar;
	return *this;
}

Vector2& Vector2::mul( const Vector2& vector )
{
	x *= vector.x;
	y *= vector.y;
	return *this;
}

Vector2 Vector2::operator + ( float scalar ) const
{
	return Vector2( *this ) += scalar;
}

Vector2 Vector2::operator - ( float scalar ) const
{
	return Vector2( *this ) -= scalar;
}

Vector2 Vector2::operator * ( float scalar ) const
{
	return Vector2( *this ) *= scalar;
}

Vector2 Vector2::operator / ( float scalar ) const
{
	return Vector2( *this ) /= scalar;
}

//

inline Vector2 operator + ( float scalar, const Vector2& vector )
{
	return vector + scalar;
}

inline Vector2 operator - ( float scalar, const Vector2& vector )
{
	return vector - scalar;
}

inline Vector2 operator * ( float scalar, const Vector2& vector )
{
	return vector * scalar;
}

// 

float Vector2::SDot( const Vector2& vector ) const
{
	return x * vector.x + y * vector.y;
}

float Vector2::Projection( const Vector2& vector ) const
{
	return SDot( vector.GetNormalized() );
}

float Vector2::Magnitude() const
{
	return sqrtf( x * x + y * y );
}

float Vector2::MagnitudeSquared() const
{
	return x * x + y * y;
}

float Vector2::InvMagnitude() const
{
	float mag = Magnitude();
	return 1.0f / mag;
}

float Vector2::InvMagnitudeSquared() const
{
	float mag = MagnitudeSquared();
	return 1.0f / mag;
}

Vector2 Vector2::GetNormalized() const
{
	float inv_mag = InvMagnitude();
	
	return Vector2( x * inv_mag, y * inv_mag );
}

Vector2& Vector2::Normalize() 
{
	float inv_mag = InvMagnitude();
	
	x *= inv_mag;
	y *= inv_mag;
	return *this;
}

//

float Vector2::ApproximateMagnitude( float x1, float y1 ) const
{
	x1 = fabsf(x1);
	y1 = fabsf(y1);

	if ( x1 < 0.414213f * y1 ) return y1 + 0.483608f * x1 * x1 / y1;
	if ( y1 < 0.414213f * x1 ) return x1 + 0.483608f * y1 * y1 / x1;

	// function returns infinite float then zero length used
	float sum = x1 + y1;
	if ( sum ) return 1.04907f * sum - 1.36785f * x1 * y1 / sum;
	else 
		return 0.0f;
}

float Vector2::ApproxMagnitude() const
{
	return ApproximateMagnitude( x, y );
}

float Vector2::ApproxInvMagnitude() const
{
	float mag = ApproximateMagnitude( x, y );
	return 1.0f / mag;
}

Vector2 Vector2::GetApproxNormalized() const
{
	float mag = ApproximateMagnitude( x, y );

	float inv = 1.0f / mag;
	
	return Vector2( x * inv, y * inv );
}

Vector2& Vector2::ApproxNormalized()
{
	float mag = ApproximateMagnitude( x, y );

	float inv = 1.0f / mag;
	
	x *= inv;
	y *= inv;
	
	return *this;
}

// unary

inline Vector2 operator - ( const Vector2& vector )
{
	return Vector2( -vector.x, -vector.y );
}

inline Vector2 operator + ( const Vector2& vector )
{
	return vector;
}

//
Vector2& Vector2::operator += ( const Vector2& vector )
{
	x += vector.x;
	y += vector.y;
	return *this;
}

Vector2& Vector2::operator -= ( const Vector2& vector )
{
	x -= vector.x;
	y -= vector.y;
	return *this;
}

Vector2 Vector2::operator + ( const Vector2& vector ) const
{
	return Vector2( *this ) += vector;
}

Vector2 Vector2::operator - ( const Vector2& vector ) const
{
	return Vector2( *this ) -= vector;
}

//

bool Vector2::Equal( const Vector2& vector, float tolerance ) const
{
	return ( fabsf( x - vector.x ) <= tolerance 
		&& fabsf( y - vector.y ) <= tolerance );
}

bool Vector2::NotEqual( const Vector2& vector, float tolerance ) const
{
	return !Equal( vector, tolerance );
}

//

bool Vector2::operator == ( const Vector2& vector ) const
{
	return ( x == vector.x && y == vector.y );
}

bool Vector2::operator != ( const Vector2& vector ) const
{
	return ! operator == ( vector );
}
