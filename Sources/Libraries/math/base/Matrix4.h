// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once
#include <math.h>

#include "Vector4.h"
#include "Matrix3.h"



class Vector3;
class Vector4;


class Matrix4 {
public:
	inline Matrix4();
	inline Matrix4( const float matrix[] );
	inline Matrix4( float m00, float m01, float m02, float m03,
					float m10, float m11, float m12, float m13,
					float m20, float m21, float m22, float m23,
					float m30, float m31, float m32, float m33 );

	
	inline void Load( const float matrix[] );
	inline void Save( float matrix[] ) const;

	
	inline void Zero();
	inline void Identity();
	static inline Matrix4 GetZero();
	static inline Matrix4 GetIdentity();

	
	inline float& operator [] (size_t index );
	inline float operator [] (size_t index ) const;
	inline float& operator () (size_t r, size_t c );
	inline float operator () (size_t r, size_t c ) const;

	
	inline const void GetMatrix3( Matrix3& matrix ) const;

	
	inline Matrix4& operator += ( float scalar );
	inline Matrix4& operator -= ( float scalar );
	inline Matrix4& operator *= ( float scalar );
	inline Matrix4& operator /= ( float scalar );

	inline Matrix4 operator + ( float scalar ) const;
	inline Matrix4 operator - ( float scalar ) const;
	inline Matrix4 operator * ( float scalar ) const;
	inline Matrix4 operator / ( float scalar ) const;

	
	
	inline Vector3 operator * ( const Vector3& vector ) const;
	friend Vector3 operator * ( const Vector3& vector, const Matrix4& matrix );

	inline Vector4 operator * ( const Vector4& vector ) const;
	friend Vector4 operator * ( const Vector4& vector, const Matrix4& matrix );

	
	inline Matrix4 GetInverse() const;
	Matrix4& Invert();

	inline Matrix4 GetTranspose() const;
	inline Matrix4& Transpose();

	 float GetDeterminant() const;

	const Vector3& getTranslate() const;

	
	inline void Multiply( const float matrix[] );
	inline void Multiply( const Matrix4& matrix );

	
	inline void Rotate( float x, float y, float z );
	inline void Rotate( const Vector3& vector );
	inline void RotateX( float angle );
	inline void RotateY( float angle );
	inline void RotateZ( float angle );
	inline void Scale( float scale );
	inline void Scale( float x, float y, float z );
	inline void Scale( const Vector3& scale );
	inline void Translate( float x, float y, float z );
	inline void Translate( const Vector3& vector );

	
	inline Matrix4& operator += ( const Matrix4& matrix );
	inline Matrix4& operator -= ( const Matrix4& matrix );
	inline Matrix4& operator *= ( const Matrix4& matrix );

	inline Matrix4 operator + ( const Matrix4& matrix ) const;
	inline Matrix4 operator - ( const Matrix4& matrix ) const;
	inline Matrix4 operator * ( const Matrix4& matrix ) const;

	
	inline bool Equal( const Matrix4& matrix, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Matrix4& matrix, float tolerance = 0.000001f) const;

	
	inline bool operator == ( const Matrix4& matrix ) const;
	inline bool operator != ( const Matrix4& matrix ) const;

	float data[16];
};




Matrix4::Matrix4()
{}

Matrix4::Matrix4( const float matrix[] )
{
	Load( matrix );
}

Matrix4::Matrix4( float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33 )
{
	data[0]		= m00;
	data[1]		= m10;
	data[2]		= m20;
	data[3]		= m30;
	data[4]		= m01;
	data[5]		= m11;
	data[6]		= m21;
	data[7]		= m31;
	data[8]		= m02;
	data[9]		= m12;
	data[10]	= m22;
	data[11]	= m32;
	data[12]	= m03;
	data[13]	= m13;
	data[14]	= m23;
	data[15]	= m33;
}


void Matrix4::Load( const float matrix[] )
{
	data[0]		= matrix[0];
	data[1]		= matrix[1];
	data[2]		= matrix[2];
	data[3]		= matrix[3];
	data[4]		= matrix[4];
	data[5]		= matrix[5];
	data[6]		= matrix[6];
	data[7]		= matrix[7];
	data[8]		= matrix[8];
	data[9]		= matrix[9];
	data[10]	= matrix[10];
	data[11]	= matrix[11];
	data[12]	= matrix[12];
	data[13]	= matrix[13];
	data[14]	= matrix[14];
	data[15]	= matrix[15];
}

void Matrix4::Save( float matrix[] ) const
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
	matrix[9]	= data[9];
	matrix[10]	= data[10];
	matrix[11]	= data[11];
	matrix[12]	= data[12];
	matrix[13]	= data[13];
	matrix[14]	= data[14];
	matrix[15]	= data[15];
}

void Matrix4::Zero()
{
	data[0]		= 0.0f;
	data[1]		= 0.0f;
	data[2]		= 0.0f;
	data[3]		= 0.0f;
	data[4]		= 0.0f;
	data[5]		= 0.0f;
	data[6]		= 0.0f;
	data[7]		= 0.0f;
	data[8]		= 0.0f;
	data[9]		= 0.0f;
	data[10]	= 0.0f;
	data[11]	= 0.0f;
	data[12]	= 0.0f;
	data[13]	= 0.0f;
	data[14]	= 0.0f;
	data[15]	= 0.0f;
}

void Matrix4::Identity()
{
	data[0]		= 1.0f;
	data[1]		= 0.0f;
	data[2]		= 0.0f;
	data[3]		= 0.0f;
	data[4]		= 0.0f;
	data[5]		= 1.0f;
	data[6]		= 0.0f;
	data[7]		= 0.0f;
	data[8]		= 0.0f;
	data[9]		= 0.0f;
	data[10]	= 1.0f;
	data[11]	= 0.0f;
	data[12]	= 0.0f;
	data[13]	= 0.0f;
	data[14]	= 0.0f;
	data[15]	= 1.0f;
}

Matrix4 Matrix4::GetZero()
{
	return Matrix4( 0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f );
}

Matrix4 Matrix4::GetIdentity()
{
	return Matrix4( 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f );
}


float& Matrix4::operator [] (size_t index )
{
	return data[ index ];
}

float Matrix4::operator [] (size_t index ) const
{
	return data[ index ];
}

float& Matrix4::operator () (size_t r, size_t c )
{
	return data[ c * 4 + r ];
}

float Matrix4::operator () (size_t r, size_t c ) const
{
	return data[ c * 4 + r ];
}


const void Matrix4::GetMatrix3( Matrix3& matrix ) const
{
	matrix.data[0] = data[0]; matrix.data[3] = data[4]; matrix.data[6] = data[8];
	matrix.data[1] = data[1]; matrix.data[4] = data[5]; matrix.data[7] = data[9];
	matrix.data[2] = data[2]; matrix.data[5] = data[6]; matrix.data[8] = data[10];
}



Matrix4& Matrix4::operator += ( float scalar )
{
	data[0]		+= scalar;
	data[1]		+= scalar;
	data[2]		+= scalar;
	data[3]		+= scalar;
	data[4]		+= scalar;
	data[5]		+= scalar;
	data[6]		+= scalar;
	data[7]		+= scalar;
	data[8]		+= scalar;
	data[9]		+= scalar;
	data[10]	+= scalar;
	data[11]	+= scalar;
	data[12]	+= scalar;
	data[13]	+= scalar;
	data[14]	+= scalar;
	data[15]	+= scalar;
	return *this;
}

Matrix4& Matrix4::operator -= ( float scalar )
{
	data[0]		-= scalar;
	data[1]		-= scalar;
	data[2]		-= scalar;
	data[3]		-= scalar;
	data[4]		-= scalar;
	data[5]		-= scalar;
	data[6]		-= scalar;
	data[7]		-= scalar;
	data[8]		-= scalar;
	data[9]		-= scalar;
	data[10]	-= scalar;
	data[11]	-= scalar;
	data[12]	-= scalar;
	data[13]	-= scalar;
	data[14]	-= scalar;
	data[15]	-= scalar;
	return *this;
}

Matrix4& Matrix4::operator *= ( float scalar )
{
	data[0]		*= scalar;
	data[1]		*= scalar;
	data[2]		*= scalar;
	data[3]		*= scalar;
	data[4]		*= scalar;
	data[5]		*= scalar;
	data[6]		*= scalar;
	data[7]		*= scalar;
	data[8]		*= scalar;
	data[9]		*= scalar;
	data[10]	*= scalar;
	data[11]	*= scalar;
	data[12]	*= scalar;
	data[13]	*= scalar;
	data[14]	*= scalar;
	data[15]	*= scalar;
	return *this;
}

Matrix4& Matrix4::operator /= ( float scalar )
{
	
	
	
	
	
	
	
	
	
	
	return operator *= ( 1.0f / scalar );
}

Matrix4 Matrix4::operator + ( float scalar ) const
{
	return Matrix4( *this ) += scalar;
}

Matrix4 Matrix4::operator - ( float scalar ) const
{
	return Matrix4( *this ) -= scalar;
}

Matrix4 Matrix4::operator * ( float scalar ) const
{
	return Matrix4( *this ) *= scalar;
}

Matrix4 Matrix4::operator / ( float scalar ) const
{
	return Matrix4( *this ) /= scalar;
}


Vector3 Matrix4::operator * ( const Vector3& vector ) const
{
	Vector4 vector4( vector );
	vector4 = *this * vector4;
	vector4.Homogenize();
	return vector4.GetVector3();
}

inline Vector3 operator * ( const Vector3& vector, const Matrix4& matrix )
{
	Vector4 vector4( vector );
	vector4 = vector4 * matrix;
	vector4.Homogenize();
	return vector4.GetVector3();
}


Vector4 Matrix4::operator * ( const Vector4& vector ) const
{
	return Vector4(
		vector.x * data[0]  + vector.y * data[4]  + vector.z * data[8]  + vector.w * data[12],
		vector.x * data[1]  + vector.y * data[5]  + vector.z * data[9]  + vector.w * data[13],
		vector.x * data[2]  + vector.y * data[6]  + vector.z * data[10] + vector.w * data[14],
		vector.x * data[3]  + vector.y * data[7]  + vector.z * data[11] + vector.w * data[15] );
}

inline Vector4 operator * ( const Vector4& vector, const Matrix4& matrix )
{
	return Vector4(
		vector.x * matrix.data[0]  + vector.y * matrix.data[1]  + vector.z * matrix.data[2]  + vector.w * matrix.data[3],
		vector.x * matrix.data[4]  + vector.y * matrix.data[5]  + vector.z * matrix.data[6]  + vector.w * matrix.data[7],
		vector.x * matrix.data[8]  + vector.y * matrix.data[9]  + vector.z * matrix.data[10] + vector.w * matrix.data[11],
		vector.x * matrix.data[12] + vector.y * matrix.data[13] + vector.z * matrix.data[14] + vector.w * matrix.data[15] );
}

Matrix4 Matrix4::GetInverse() const
{
	return Matrix4( *this ).Invert();
}

Matrix4 Matrix4::GetTranspose() const
{
	return Matrix4( data[0],  data[1],  data[2],  data[3],
					data[4],  data[5],  data[6],  data[7],
					data[8],  data[9],  data[10], data[11],
					data[12], data[13], data[14], data[15] );
}

Matrix4& Matrix4::Transpose()
{
	*this = GetTranspose();
	return *this;
}

void Matrix4::Multiply( const float matrix[] )
{
	
	float temp[16];
	memcpy(temp, data, sizeof(float) * 16);

	
	data[0]   = temp[0] * matrix[0]  + temp[4] * matrix[1]  + temp[8]  * matrix[2]  + temp[12] * matrix[3];
	data[1]   = temp[1] * matrix[0]  + temp[5] * matrix[1]  + temp[9]  * matrix[2]  + temp[13] * matrix[3];
	data[2]   = temp[2] * matrix[0]  + temp[6] * matrix[1]  + temp[10] * matrix[2]  + temp[14] * matrix[3];
	data[3]   = temp[3] * matrix[0]  + temp[7] * matrix[1]  + temp[11] * matrix[2]  + temp[15] * matrix[3];

	
	data[4]   = temp[0] * matrix[4]  + temp[4] * matrix[5]  + temp[8]  * matrix[6]  + temp[12] * matrix[7];
	data[5]   = temp[1] * matrix[4]  + temp[5] * matrix[5]  + temp[9]  * matrix[6]  + temp[13] * matrix[7];
	data[6]   = temp[2] * matrix[4]  + temp[6] * matrix[5]  + temp[10] * matrix[6]  + temp[14] * matrix[7];
	data[7]   = temp[3] * matrix[4]  + temp[7] * matrix[5]  + temp[11] * matrix[6]  + temp[15] * matrix[7];

	
	data[8]   = temp[0] * matrix[8]  + temp[4] * matrix[9]  + temp[8]  * matrix[10] + temp[12] * matrix[11];
	data[9]   = temp[1] * matrix[8]  + temp[5] * matrix[9]  + temp[9]  * matrix[10] + temp[13] * matrix[11];
	data[10]  = temp[2] * matrix[8]  + temp[6] * matrix[9]  + temp[10] * matrix[10] + temp[14] * matrix[11];
	data[11]  = temp[3] * matrix[8]  + temp[7] * matrix[9]  + temp[11] * matrix[10] + temp[15] * matrix[11];

	
	data[12]  = temp[0] * matrix[12] + temp[4] * matrix[13] + temp[8]  * matrix[14] + temp[12] * matrix[15];
	data[13]  = temp[1] * matrix[12] + temp[5] * matrix[13] + temp[9]  * matrix[14] + temp[13] * matrix[15];
	data[14]  = temp[2] * matrix[12] + temp[6] * matrix[13] + temp[10] * matrix[14] + temp[14] * matrix[15];
	data[15]  = temp[3] * matrix[12] + temp[7] * matrix[13] + temp[11] * matrix[14] + temp[15] * matrix[15];
}

void Matrix4::Multiply( const Matrix4& matrix )
{
	Multiply( matrix.data );
}


void Matrix4::Rotate( float x, float y, float z )
{
	RotateX( x );
	RotateY( y );
	RotateZ( z );
}

void Matrix4::Rotate( const Vector3& vector )
{
	Rotate( vector.x, vector.y, vector.z );
}

void Matrix4::RotateX( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	float m[16] = 
	{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	   c,	   s,	0.0f,
		0.0f,	  -s,	   c,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	};

	Multiply( m );
}

void Matrix4::RotateY( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	float m[16] =
	{

		c,  0.0f,    -s,  0.0f,
	 0.0f,  1.0f,  0.0f,  0.0f,
		s,  0.0f,     c,  0.0f,
	 0.0f,  0.0f,  0.0f,  1.0f
	};

	Multiply( m );
}

void Matrix4::RotateZ( float angle )
{
	float	s = sinf( angle );
	float	c = cosf( angle );

	float m[16] =
	{

		c,     s,  0.0f,  0.0f,
	   -s,     c,  0.0f,  0.0f,
	 0.0f,  0.0f,  1.0f,  0.0f,
	 0.0f,  0.0f,  0.0f,  1.0f
	};

	Multiply( m );
}

void Matrix4::Scale( float scale )
{
	float m[16] = 
	{
		scale,	0.0f,	0.0f,	0.0f,
		 0.0f,	scale,	0.0f,	0.0f,
		 0.0f,	0.0f,	scale,	0.0f,
		 0.0f,	0.0f,	0.0f,	1.0f
	};

	Multiply( m );
}

void Matrix4::Scale( float x, float y, float z )
{
	float m[16] = 
	{
			x,	0.0f,	0.0f,	0.0f,
		 0.0f,		y,	0.0f,	0.0f,
		 0.0f,	0.0f,		z,	0.0f,
		 0.0f,	0.0f,	0.0f,	1.0f
	};

	Multiply( m );
}

void Matrix4::Scale( const Vector3& vector )
{
	Scale( vector.x, vector.y, vector.z );
}

void Matrix4::Translate( float x, float y, float z )
{
	float m[16] =
	{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		   x,	   y,	   z,	1.0f
	};

	Multiply( m );
}

void Matrix4::Translate( const Vector3& vector )
{
	Translate( vector.x, vector.y, vector.z );
}



Matrix4& Matrix4::operator += ( const Matrix4& matrix )
{
	data[0]		+= matrix.data[0];
	data[1]		+= matrix.data[1];
	data[2]		+= matrix.data[2];
	data[3]		+= matrix.data[3];
	data[4]		+= matrix.data[4];
	data[5]		+= matrix.data[5];
	data[6]		+= matrix.data[6];
	data[7]		+= matrix.data[7];
	data[8]		+= matrix.data[8];
	data[9]		+= matrix.data[9];
	data[10]	+= matrix.data[10];
	data[11]	+= matrix.data[11];
	data[12]	+= matrix.data[12];
	data[13]	+= matrix.data[13];
	data[14]	+= matrix.data[14];
	data[15]	+= matrix.data[15];
	return *this;
}

Matrix4& Matrix4::operator -= ( const Matrix4& matrix )
{
	data[0]		-= matrix.data[0];
	data[1]		-= matrix.data[1];
	data[2]		-= matrix.data[2];
	data[3]		-= matrix.data[3];
	data[4]		-= matrix.data[4];
	data[5]		-= matrix.data[5];
	data[6]		-= matrix.data[6];
	data[7]		-= matrix.data[7];
	data[8]		-= matrix.data[8];
	data[9]		-= matrix.data[9];
	data[10]	-= matrix.data[10];
	data[11]	-= matrix.data[11];
	data[12]	-= matrix.data[12];
	data[13]	-= matrix.data[13];
	data[14]	-= matrix.data[14];
	data[15]	-= matrix.data[15];
	return *this;
}

Matrix4& Matrix4::operator *= ( const Matrix4& matrix )
{
	Multiply( matrix.data );
	return *this;
}

Matrix4 Matrix4::operator + ( const Matrix4& matrix ) const
{
	return Matrix4( *this ) += matrix;
}

Matrix4 Matrix4::operator - ( const Matrix4& matrix ) const
{
	return Matrix4( *this ) -= matrix;
}

Matrix4 Matrix4::operator * ( const Matrix4& matrix ) const
{
	return Matrix4( *this ) *= matrix;
}


bool Matrix4::Equal( const Matrix4& matrix, float tolerance ) const
{
	return  (fabsf( data[0]	- matrix.data[0])	<= tolerance) &&
			(fabsf( data[1]	- matrix.data[1])	<= tolerance) &&
			(fabsf( data[2]	- matrix.data[2])	<= tolerance) &&
			(fabsf( data[3]	- matrix.data[3])	<= tolerance) &&
			(fabsf( data[4]	- matrix.data[4])	<= tolerance) &&
			(fabsf( data[5]	- matrix.data[5])	<= tolerance) &&
			(fabsf( data[6]	- matrix.data[6])	<= tolerance) &&
			(fabsf( data[7]	- matrix.data[7])	<= tolerance) &&
			(fabsf( data[8]	- matrix.data[8])	<= tolerance) &&
			(fabsf( data[9]	- matrix.data[9])	<= tolerance) &&
			(fabsf( data[10]- matrix.data[10])	<= tolerance) &&
			(fabsf( data[11]- matrix.data[11])	<= tolerance) &&
			(fabsf( data[12]- matrix.data[12])	<= tolerance) &&
			(fabsf( data[13]- matrix.data[13])	<= tolerance) &&
			(fabsf( data[14]- matrix.data[14])	<= tolerance) &&
			(fabsf( data[15]- matrix.data[15])	<= tolerance) ;
}

bool Matrix4::NotEqual( const Matrix4& matrix, float tolerance ) const
{
	return !Equal( matrix, tolerance );
}



bool Matrix4::operator == ( const Matrix4& matrix ) const
{
	return	data[0]	== matrix.data[0]	&&
			data[1]	== matrix.data[2]	&&
			data[3]	== matrix.data[3]	&&
			data[4]	== matrix.data[4]	&&
			data[5]	== matrix.data[5]	&&
			data[6]	== matrix.data[6]	&&
			data[7]	== matrix.data[7]	&&
			data[8]	== matrix.data[8]	&&
			data[9]	== matrix.data[9]	&&
			data[10]== matrix.data[10]	&&
			data[11]== matrix.data[11]	&&
			data[12]== matrix.data[12]	&&
			data[13]== matrix.data[13]	&&
			data[14]== matrix.data[14]	&&
			data[15]== matrix.data[15];
}

bool Matrix4::operator != ( const Matrix4& matrix ) const
{
	return ! operator == ( matrix );
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