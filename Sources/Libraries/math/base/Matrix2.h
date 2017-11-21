/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 21.07.2001
	  Time: 13:53:06

	Abstract:	Standard column 2x2 Matrix class

*********************************************************************/
#pragma once

#include <math.h>

#include "Vector2.h"

// Matrix2
class Matrix2 {
public:

	// Ctors
	inline Matrix2();
	inline Matrix2( const float matrix[] );
	inline Matrix2( float m00, float m01, float m10, float m11 );

	// Load / save
	inline void Load( const float matrix[] );
	inline void Save( float matrix[] ) const;

	// Reset
	inline void Zero();
	inline void Identity();

	static inline Matrix2 GetZero();
	static inline Matrix2 GetIdentity();

	// Access
	inline float& operator [] (size_t index );
	inline float& operator () (size_t r, size_t c );
	inline float operator [] (size_t index ) const;
	inline float operator () (size_t r, size_t c ) const;

	// Scalar
	inline Matrix2& operator += ( float scalar );
	inline Matrix2& operator -= ( float scalar );
	inline Matrix2& operator *= ( float scalar );
	inline Matrix2& operator /= ( float scalar );

	inline Matrix2 operator + ( float scalar ) const;
	inline Matrix2 operator - ( float scalar ) const;
	inline Matrix2 operator * ( float scalar ) const;
	inline Matrix2 operator / ( float scalar ) const;

	// Vector
	inline Vector2 operator * ( const Vector2& vector ) const;
	friend Vector2 operator * ( const Vector2& vector, const Matrix2& matrix );

	// Matrix ops
	inline Matrix2 GetInverse() const;
	inline Matrix2& Invert();

	inline Matrix2 GetTranspose() const;
	inline Matrix2& Transpose();

	inline float GetDeterminant() const;

	// Matrix post-multiplication
	inline void Multiply( const float matrix[] );
	inline void Multiply( const Matrix2& matrix );

	// Affine transform
	// All transforms are concatenated with the current matrix
	// ie newMatrix = oldMatrix * newTransform
	// The transforms construct matrices to pre-multiply vectors
	// ie newVector = oldMatrix * newTransform
	// This means that successive transforms affects the vector
	// in reverse order of addition

	inline void Rotate( float z );
	inline void Scale( float scale );
	inline void Scale( float x, float y );
	inline void Scale( const Vector2& vector );

	// infix
	inline Matrix2& operator += ( const Matrix2& matrix );
	inline Matrix2& operator -= ( const Matrix2& matrix );
	inline Matrix2& operator *= ( const Matrix2& matrix );

	inline Matrix2 operator + ( const Matrix2& matrix ) const;
	inline Matrix2 operator - ( const Matrix2& matrix ) const;
	inline Matrix2 operator * ( const Matrix2& matrix ) const;

	// Logic compare
	inline bool Equal( const Matrix2& matrix, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Matrix2& matrix, float tolerance = 0.000001f) const;

	// Binary compare
	inline bool operator == ( const Matrix2& matrix ) const;
	inline bool operator != ( const Matrix2& matrix ) const;

protected:
	float data[4];	// matrix elements in column major order
};

/////////////////////////////////////////////////////////////////
// Implementation

Matrix2::Matrix2()
{}

Matrix2::Matrix2( const float matrix[] )
{
	Load( matrix );
}

Matrix2::Matrix2( float m00, float m01, float m10, float m11 )
{
	data[0]	= m00;
	data[1]	= m10;
	data[2] = m01;
	data[3]	= m11;
}

void Matrix2::Load( const float matrix[] )
{
	data[0]	= matrix[0];
	data[1]	= matrix[1];
	data[2]	= matrix[2];
	data[3]	= matrix[3];
}

void Matrix2::Save( float matrix[] ) const
{
	matrix[0]	= data[0];
	matrix[1]	= data[1];
	matrix[2]	= data[2];
	matrix[3]	= data[3];
}

void Matrix2::Zero()
{
	data[0]	= 0.0f;
	data[1]	= 0.0f;
	data[2]	= 0.0f;
	data[3]	= 0.0f;
}

void Matrix2::Identity()
{
	data[0]	= 1.0f;
	data[1]	= 0.0f;
	data[2]	= 0.0f;
	data[3]	= 1.0f;
}

Matrix2 Matrix2::GetZero()
{
	return Matrix2( 0.0f, 0.0f, 0.0f, 0.0f );
}

Matrix2 Matrix2::GetIdentity()
{
	return Matrix2( 1.0f, 0.0f, 0.0f, 1.0f );
}

//

float& Matrix2::operator [] (size_t index )
{
	return data[ index ];
}

float Matrix2::operator [] (size_t index ) const
{
	return data[ index ];
}

float& Matrix2::operator () (size_t r, size_t c )
{
	return data[ c * 2 + r ];
}

float Matrix2::operator () (size_t r, size_t c ) const
{
	return data[ c * 2 + r ];
}

//

Matrix2& Matrix2::operator += ( float scalar )
{
	data[0]	+= scalar;
	data[1]	+= scalar;
	data[2]	+= scalar;
	data[3]	+= scalar;
	return *this;
}

Matrix2& Matrix2::operator -= ( float scalar )
{
	data[0]	-= scalar;
	data[1]	-= scalar;
	data[2]	-= scalar;
	data[3]	-= scalar;
	return *this;
}

Matrix2& Matrix2::operator *= ( float scalar )
{
	data[0]	*= scalar;
	data[1]	*= scalar;
	data[2]	*= scalar;
	data[3]	*= scalar;
	return *this;
}

Matrix2& Matrix2::operator /= ( float scalar )
{
	return operator *= ( 1.0f / scalar );
}

Matrix2 Matrix2::operator + ( float scalar ) const
{
	return Matrix2( *this ) += scalar;
}

Matrix2 Matrix2::operator - ( float scalar ) const
{
	return Matrix2( *this ) -= scalar;
}

Matrix2 Matrix2::operator * ( float scalar ) const
{
	return Matrix2( *this ) *= scalar;
}

Matrix2 Matrix2::operator / ( float scalar ) const
{
	return Matrix2( *this ) /= scalar;
}

//

Vector2 Matrix2::operator * ( const Vector2& vector ) const
{
	return Vector2( vector.x * data[0] + vector.y * data[2],
					vector.x * data[1] + vector.y * data[3] );
}

inline Vector2 operator * ( const Vector2& vector, const Matrix2& matrix )
{
	return Vector2( vector.x * matrix.data[0] + vector.y * matrix.data[1],
					vector.x * matrix.data[2] + vector.y * matrix.data[3] );
}

//

Matrix2 Matrix2::GetInverse() const
{
	// build transposed cofactor of matrix
	Matrix2 matrix( data[3], data[1], data[2], data[0] );

	// calc determinant
	float det = data[0] * data[3] - data[2] * data[1];

	// Scale each cofactor element by 1 / |M|
	matrix.Scale( 1.0f / det );

	// return inverse
	return matrix;
}

Matrix2& Matrix2::Invert()
{
	*this = GetInverse();
	return *this;
}

//

Matrix2 Matrix2::GetTranspose() const
{
	return Matrix2( data[0], data[1], data[2], data[3] );
}

Matrix2& Matrix2::Transpose()
{
	*this = GetTranspose();
	return *this;
}

//

float Matrix2::GetDeterminant() const
{
	return data[0] * data[3] - data[2] * data[1];
}

// 
void Matrix2::Multiply( const float matrix[] )
{
	// copy data
	float temp[4];
	temp[0]	= data[0];
	temp[1]	= data[1];
	temp[2]	= data[2];
	temp[3]	= data[3];

	// first col
	data[0] = temp[0] * matrix[0] + temp[2] * matrix[1];
	data[1] = temp[1] * matrix[0] + temp[3] * matrix[1];

	// second col
	data[2] = temp[0] * matrix[2] + temp[2] * matrix[3];
	data[3] = temp[1] * matrix[2] + temp[3] * matrix[3];
}

void Matrix2::Multiply( const Matrix2& matrix )
{
	Multiply( matrix.data );
}

// Affine

void Matrix2::Rotate( float z )
{
	float s = sinf( z );
	float c = cosf( z );

	float m[4] = 
	{
		c, s,
		-s,c
	};

	// concatenate
	Multiply( m );
}

void Matrix2::Scale( float scale )
{
	float m[4] = 
	{
		scale, 0.0f,
		0.0f, scale
	};
	Multiply( m );
}

void Matrix2::Scale( float x, float y )
{
	float m[4] =
	{
		x, 0.0f,
		0.0f, y
	};
	Multiply( m );
}

void Matrix2::Scale( const Vector2& vector )
{
	float m[4] =
	{
		vector.x, 0.0f,
		0.0f, vector.y
	};
	Multiply( m );
}

// Infix
Matrix2& Matrix2::operator += ( const Matrix2& matrix )
{
	data[0]	+= matrix.data[0];
	data[1]	+= matrix.data[1];
	data[2]	+= matrix.data[2];
	data[3]	+= matrix.data[3];
	return *this;
}

Matrix2& Matrix2::operator -= ( const Matrix2& matrix )
{
	data[0]	-= matrix.data[0];
	data[1]	-= matrix.data[1];
	data[2]	-= matrix.data[2];
	data[3]	-= matrix.data[3];
	return *this;
}

Matrix2& Matrix2::operator *= ( const Matrix2& matrix )
{
	Multiply( matrix.data );
	return *this;
}

Matrix2 Matrix2::operator + ( const Matrix2& matrix ) const
{
	return Matrix2( *this ) += matrix;
}

Matrix2 Matrix2::operator - ( const Matrix2& matrix ) const
{
	return Matrix2( *this ) -= matrix;
}

Matrix2 Matrix2::operator * ( const Matrix2& matrix ) const
{
	return Matrix2( *this ) *= matrix;
}

// Compare

bool Matrix2::Equal( const Matrix2& matrix, float tolerance ) const
{
	return (fabsf( data[0] - matrix.data[0] ) <= tolerance &&
			fabsf( data[1] - matrix.data[1] ) <= tolerance &&
			fabsf( data[2] - matrix.data[2] ) <= tolerance &&
			fabsf( data[3] - matrix.data[3] ) <= tolerance );
}

bool Matrix2::NotEqual( const Matrix2& matrix, float tolerance ) const
{
	return !Equal( matrix, tolerance );
}

bool Matrix2::operator == ( const Matrix2& matrix ) const
{
	return (data[0] == matrix.data[0] &&
			data[1] == matrix.data[1] &&
			data[2] == matrix.data[2] &&
			data[3] == matrix.data[3] );
}

bool Matrix2::operator != ( const Matrix2& matrix ) const
{
	return ! operator == ( matrix );
}
