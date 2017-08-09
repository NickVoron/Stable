// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once

#include "Vector3.h"

#include <math.h>
#include <string.h> 


class Vector3;
class Matrix4;


class Matrix3 {
public:

	
	inline Matrix3();
	inline Matrix3( const float matrix[] );
	inline Matrix3( float m00, float m01, float m02, 
					float m10, float m11, float m12,
					float m20, float m21, float m22 );

	
	inline void Load( const float matrix[] );
	inline void Save( float matrix[] ) const;

	
	inline void Zero();
	inline void Identity();
	static inline Matrix3 GetZero();
	static inline Matrix3 GetIdentity();

	
	inline float& operator [] (size_t index );
	inline float operator [] (size_t index ) const;

	inline float& operator () (size_t r, size_t c );
	inline float operator () (size_t r, size_t c ) const;

	
	inline Matrix3& operator += ( float scalar );
	inline Matrix3& operator -= ( float scalar );
	inline Matrix3& operator *= ( float scalar );
	inline Matrix3& operator /= ( float scalar );

	inline Matrix3 operator + ( float scalar ) const;
	inline Matrix3 operator - ( float scalar ) const;
	inline Matrix3 operator * ( float scalar ) const;
	inline Matrix3 operator / ( float scalar ) const;

	
	inline Vector3 operator * ( const Vector3& vector ) const;
	friend Vector3 operator * ( const Vector3& vector, const Matrix3& matrix );
	friend Vector3& operator *= ( Vector3& vector, const Matrix3& matrix );

	
	inline Matrix3 GetInverse() const;
	inline Matrix3& Invert();

	inline Matrix3 GetTranspose() const;
	inline Matrix3& Transpose();

	inline float GetDeterminant() const;

	
	inline void Multiply( const float matrix[] );
	inline void Multiply( const Matrix3& matrix );

	
	
	
	
	
	
	
	inline void Rotate( float x, float y, float z );
	inline void Rotate( const Vector3& vector );
	inline void RotateX( float angle );
	inline void RotateY( float angle );
	inline void RotateZ( float angle );
	inline void Scale( float scale );
	inline void Scale( float x, float y, float z );
	inline void Scale( const Vector3& vector );

	
	inline Matrix3& operator += ( const Matrix3& matrix );
	inline Matrix3& operator -= ( const Matrix3& matrix );
	inline Matrix3& operator *= ( const Matrix3& matrix );

	inline Matrix3 operator + ( const Matrix3& matrix ) const;
	inline Matrix3 operator - ( const Matrix3& matrix ) const;
	inline Matrix3 operator * ( const Matrix3& matrix ) const;

	
	inline bool Equal( const Matrix3& matrix, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Matrix3& matrix, float tolerance = 0.000001f) const;

	
	inline bool operator == ( const Matrix3& matrix ) const;
	inline bool operator != ( const Matrix3& matrix ) const;

protected:
	
	float data[9];

	friend class Matrix4;
};




Matrix3::Matrix3()
{}

Matrix3::Matrix3( const float matrix[] )
{
	Load( matrix );
}

Matrix3::Matrix3(float m00, float m01, float m02,
				 float m10, float m11, float m12,
				 float m20, float m21, float m22 )
{
	data[0]	= m00;
	data[1]	= m10;
	data[2]	= m20;
	data[3]	= m01;
	data[4]	= m11;
	data[5]	= m21;
	data[6]	= m02;
	data[7]	= m12;
	data[8]	= m22;
}

void Matrix3::Load( const float matrix[] )
{
	data[0]	= matrix[0];
	data[1]	= matrix[1];
	data[2]	= matrix[2];
	data[3]	= matrix[3];
	data[4]	= matrix[4];
	data[5]	= matrix[5];
	data[6]	= matrix[6];
	data[7]	= matrix[7];
	data[8]	= matrix[8];
}

void Matrix3::Save( float matrix[] ) const
{
	matrix[0]	= data[0];
	matrix[1]	= data[1];
	matrix[2]	= data[2];
	matrix[3]	= data[3];
	matrix[4]	= data[4];
	matrix[5]	= data[5];
	matrix[6]	= data[6];
	matrix[7]	= data[7];
	matrix[8]	= data[8];
}

void Matrix3::Zero()
{
	data[0]	= 0.0f;
	data[1]	= 0.0f;
	data[2]	= 0.0f;
	data[3]	= 0.0f;
	data[4]	= 0.0f;
	data[5]	= 0.0f;
	data[6]	= 0.0f;
	data[7]	= 0.0f;
	data[8]	= 0.0f;
}

void Matrix3::Identity()
{
	data[0]	= 1.0f;
	data[1]	= 0.0f;
	data[2]	= 0.0f;
	data[3]	= 0.0f;
	data[4]	= 1.0f;
	data[5]	= 0.0f;
	data[6]	= 0.0f;
	data[7]	= 0.0f;
	data[8]	= 1.0f;
}

Matrix3 Matrix3::GetZero()
{
	return Matrix3( 0.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 0.0f );
}

Matrix3 Matrix3::GetIdentity()
{
	return Matrix3( 1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f );
}


float& Matrix3::operator [] (size_t index )
{
	return data[ index ];
}

float Matrix3::operator [] (size_t index ) const
{
	return data[ index ];
}

float& Matrix3::operator () (size_t r, size_t c )
{
	return data[ c * 3 + r ];
}

float Matrix3::operator () (size_t r, size_t c ) const
{
	return data[ c * 3 + r ];
}


Matrix3& Matrix3::operator += ( float scalar )
{
	data[0]	+= scalar;
	data[1]	+= scalar;
	data[2]	+= scalar;
	data[3]	+= scalar;
	data[4]	+= scalar;
	data[5]	+= scalar;
	data[6]	+= scalar;
	data[7]	+= scalar;
	data[8]	+= scalar;
	return *this;
}

Matrix3& Matrix3::operator -= ( float scalar )
{
	data[0]	-= scalar;
	data[1]	-= scalar;
	data[2]	-= scalar;
	data[3]	-= scalar;
	data[4]	-= scalar;
	data[5]	-= scalar;
	data[6]	-= scalar;
	data[7]	-= scalar;
	data[8]	-= scalar;
	return *this;
}

Matrix3& Matrix3::operator *= ( float scalar )
{
	data[0]	*= scalar;
	data[1]	*= scalar;
	data[2]	*= scalar;
	data[3]	*= scalar;
	data[4]	*= scalar;
	data[5]	*= scalar;
	data[6]	*= scalar;
	data[7]	*= scalar;
	data[8]	*= scalar;
	return *this;
}

Matrix3& Matrix3::operator /= ( float scalar )
{
	return operator *= ( 1.0f / scalar );
}

Matrix3 Matrix3::operator + ( float scalar ) const
{
	return Matrix3( *this ) += scalar;
}

Matrix3 Matrix3::operator - ( float scalar ) const
{
	return Matrix3( *this ) -= scalar;
}

Matrix3 Matrix3::operator * ( float scalar ) const
{
	return Matrix3( *this ) *= scalar;
}

Matrix3 Matrix3::operator / ( float scalar ) const
{
	return Matrix3( *this ) /= scalar;
}



Vector3 Matrix3::operator * ( const Vector3& vector ) const
{
	return Vector3( vector.x * data[0] + vector.y * data[3] + vector.z * data[6],
					vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
					vector.x * data[2] + vector.y * data[5] + vector.z * data[8] );
}

inline Vector3 operator * ( const Vector3& vector, const Matrix3& matrix )
{
	return Vector3( vector.x * matrix.data[0] + vector.y * matrix.data[1] + vector.z * matrix.data[2],
					vector.x * matrix.data[3] + vector.y * matrix.data[4] + vector.z * matrix.data[5],
					vector.x * matrix.data[6] + vector.y * matrix.data[7] + vector.z * matrix.data[8] );
}

inline Vector3& operator *= ( Vector3& vector, const Matrix3& matrix )
{
	vector = vector * matrix;
	return vector;
}


Matrix3 Matrix3::GetInverse() const
{
	return Matrix3( *this ).Invert();
}

Matrix3& Matrix3::Invert()
{
	
	Matrix3 matrix( +(data[4] * data[8] - data[7] * data[5]), -(data[3] * data[8] - data[6] * data[5]), +(data[3] * data[7] - data[6] * data[4]),
					-(data[1] * data[8] - data[7] * data[2]), +(data[0] * data[8] - data[6] * data[2]), -(data[0] * data[7] - data[6] * data[1]),
					+(data[1] * data[5] - data[4] * data[2]), -(data[0] * data[5] - data[3] * data[2]), +(data[0] * data[4] - data[3] * data[1]) );

	
	float det = matrix.data[0] * data[0] + matrix.data[1] * data[3] + matrix.data[2] * data[6];
	
	matrix.Scale( 1.0f / det );

	*this = matrix;
	
	return *this;
}

Matrix3 Matrix3::GetTranspose() const
{
	return Matrix3( data[0], data[1], data[2],
					data[3], data[4], data[5],
					data[6], data[7], data[8] );
}

Matrix3& Matrix3::Transpose()
{
	*this = GetTranspose();
	return *this;
}

float Matrix3::GetDeterminant() const
{
	float m00_cofactor	= +1.0f * ( data[4] * data[8] - data[7] * data[5] );
	float m10_cofactor	= -1.0f * ( data[1] * data[8] - data[7] * data[2] );
	float m20_cofactor	= +1.0f * ( data[1] * data[5] - data[4] * data[2] );

	return data[0] * m00_cofactor + data[3] * m10_cofactor + data[6] * m20_cofactor;
}


void Matrix3::Multiply( const float matrix[] )
{
	
	float temp[9];
	memcpy(temp, data, 9 * sizeof(float));

	
	data[0] = temp[0] * matrix[0] + temp[3] * matrix[1] + temp[6] * matrix[2];
	data[1] = temp[1] * matrix[0] + temp[4] * matrix[1] + temp[7] * matrix[2];
	data[2] = temp[2] * matrix[0] + temp[5] * matrix[1] + temp[8] * matrix[2];

	
	data[3] = temp[0] * matrix[3] + temp[3] * matrix[4] + temp[6] * matrix[5];
	data[4] = temp[1] * matrix[3] + temp[4] * matrix[4] + temp[7] * matrix[5];
	data[5] = temp[2] * matrix[3] + temp[5] * matrix[4] + temp[8] * matrix[5];

	
	data[6] = temp[0] * matrix[6] + temp[3] * matrix[7] + temp[6] * matrix[8];
	data[7] = temp[1] * matrix[6] + temp[4] * matrix[7] + temp[7] * matrix[8];
	data[8] = temp[2] * matrix[6] + temp[5] * matrix[7] + temp[8] * matrix[8];
}

void Matrix3::Multiply( const Matrix3& matrix )
{
	Multiply( matrix.data );
}



void Matrix3::Rotate( float x, float y, float z )
{
	RotateX( x );
	RotateY( y );
	RotateZ( z );
}

void Matrix3::Rotate( const Vector3& vector )
{
	Rotate( vector.x, vector.y, vector.z );
}

void Matrix3::RotateX( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	
	float m[9]	= 
	{
		1.0f,	0.0f,	0.0f,
		0.0f,	c,		s,
		0.0f,	-s,		c
	};

	Multiply( m );
}

void Matrix3::RotateY( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	
	float m[9]	= 
	{
		c,		0.0f,	-s,
		0.0f,	1.0f,	0.0f,
		s,		0.0f,		c
	};

	Multiply( m );
}


void Matrix3::RotateZ( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	
	float m[9]	= 
	{
		c,		s,		0.0f,
		-s,		c,		0.0f,
		0.0f,	0.0f,	1.0f
	};

	Multiply( m );
}

void Matrix3::Scale( float scale )
{
	float m[9]	= 
	{
		scale,	0.0f,	0.0f,
		0.0f,	scale,	0.0f,
		0.0f,	0.0f,	scale
	};

	Multiply( m );
}

void Matrix3::Scale( float x, float y, float z )
{
	float m[9]	= 
	{
		x,		0.0f,	0.0f,
		0.0f,	y,		0.0f,
		0.0f,	0.0f,	z
	};

	Multiply( m );
}

void Matrix3::Scale( const Vector3& vector )
{
	Scale( vector.x, vector.y, vector.z );
}


Matrix3& Matrix3::operator += ( const Matrix3& matrix )
{
	data[0]	+= matrix.data[0];
	data[1]	+= matrix.data[1];
	data[2]	+= matrix.data[2];
	data[3]	+= matrix.data[3];
	data[4]	+= matrix.data[4];
	data[5]	+= matrix.data[5];
	data[6]	+= matrix.data[6];
	data[7]	+= matrix.data[7];
	data[8]	+= matrix.data[8];
	return *this;
}

Matrix3& Matrix3::operator -= ( const Matrix3& matrix )
{
	data[0]	-= matrix.data[0];
	data[1]	-= matrix.data[1];
	data[2]	-= matrix.data[2];
	data[3]	-= matrix.data[3];
	data[4]	-= matrix.data[4];
	data[5]	-= matrix.data[5];
	data[6]	-= matrix.data[6];
	data[7]	-= matrix.data[7];
	data[8]	-= matrix.data[8];
	return *this;
}

Matrix3& Matrix3::operator *= ( const Matrix3& matrix )
{
	Multiply( matrix.data );
	return *this;
}

Matrix3 Matrix3::operator + ( const Matrix3& matrix ) const
{
	return Matrix3( *this ) += matrix;
}

Matrix3 Matrix3::operator - ( const Matrix3& matrix ) const
{
	return Matrix3( *this ) -= matrix;
}

Matrix3 Matrix3::operator * ( const Matrix3& matrix ) const
{
	return Matrix3( *this ) *= matrix;
}


bool Matrix3::Equal( const Matrix3& matrix, float tolerance ) const
{
	return  (fabsf(data[0] - matrix.data[0]) <= tolerance) &&
			(fabsf(data[1] - matrix.data[1]) <= tolerance) &&
			(fabsf(data[2] - matrix.data[2]) <= tolerance) &&
			(fabsf(data[3] - matrix.data[3]) <= tolerance) &&
			(fabsf(data[4] - matrix.data[4]) <= tolerance) &&
			(fabsf(data[5] - matrix.data[5]) <= tolerance) &&
			(fabsf(data[6] - matrix.data[6]) <= tolerance) &&
			(fabsf(data[7] - matrix.data[7]) <= tolerance) &&
			(fabsf(data[8] - matrix.data[8]) <= tolerance);
}

bool Matrix3::NotEqual( const Matrix3& matrix, float tolerance ) const
{
	return !Equal( matrix, tolerance );
}


bool Matrix3::operator == ( const Matrix3& matrix ) const
{
	return	data[0]	== matrix.data[0] &&
			data[1]	== matrix.data[1] &&
			data[2]	== matrix.data[2] &&
			data[3]	== matrix.data[3] &&
			data[4]	== matrix.data[4] &&
			data[5]	== matrix.data[5] &&
			data[6]	== matrix.data[6] &&
			data[7]	== matrix.data[7] &&
			data[8]	== matrix.data[8];
}

bool Matrix3::operator != ( const Matrix3& matrix ) const
{
	return !operator == ( matrix );
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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