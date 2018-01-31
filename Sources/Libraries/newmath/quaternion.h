// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.h"
#include "vector.operations.h"

namespace nm
{
		  
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	class Quaternion {
	public:

		
		Vector3	v;					
		float	w;					

		
		inline Quaternion();

		
		inline Quaternion( float r, float p, float y );
		inline Quaternion( const Vector3& euler );

		
		void Set( float roll, float pitch, float yaw  );
		inline void Set( const Vector3& euler );

		
		Quaternion& Set( float angle, const Vector3& axis, bool normalizeAxis = false );

		inline Quaternion& SetXAxis( float angle );
		inline Quaternion& SetYAxis( float angle );
		inline Quaternion& SetZAxis( float angle );

		
		inline void Identity();
		inline bool IsIdentity() const;
		
		
		Vector3 GetEulerAngles() const;

		
		inline Vector3 GetXAxis() const;
		inline void GetXAxis( Vector3& axis ) const;

		
		inline Vector3 GetYAxis() const;
		inline void GetYAxis( Vector3& axis ) const;

		
		inline Vector3 GetZAxis() const;
		inline void GetZAxis( Vector3& axis ) const;

		
		
		
		
		
		static Quaternion Slerp( const Quaternion& start, const Quaternion& end, float time, bool shortest = true );

		
		inline Quaternion& operator *= ( float s );
		inline Quaternion& operator /= ( float s );
		inline Quaternion operator * ( float s ) const;
		inline Quaternion operator / ( float s ) const;

		
		inline Quaternion GetInverse() const;
		inline Quaternion& Invert();

		
		inline float SDot( const Quaternion& q ) const;

		
		inline float Magnitude() const;
		inline float MagnitudeSquared() const;

		
		inline Quaternion GetConjugate() const;
		inline Quaternion& Conjugate();

		
		inline Quaternion GetNormalized() const;
		inline Quaternion& Normalize();

		
		inline Quaternion&	operator = ( const Quaternion& q );
		inline Quaternion&	operator += ( const Quaternion& q );
		inline Quaternion&	operator -= ( const Quaternion& q );

		inline Quaternion&	operator *= ( const Quaternion& q );
		inline Quaternion&	operator /= ( const Quaternion& q );

		inline Quaternion operator + ( const Quaternion& q ) const;
		inline Quaternion operator - ( const Quaternion& q ) const;
		inline Quaternion operator * ( const Quaternion& q ) const;
		inline Quaternion operator / ( const Quaternion& q ) const;
	};


	
	

	Quaternion::Quaternion()
	{}

	Quaternion::Quaternion( float r, float p, float y )
	{
		Set( r, p ,y );
	}

	Quaternion::Quaternion( const Vector3& euler )
	{
		Set( euler.x, euler.y, euler.z );
	}

	

	void Quaternion::Set( const Vector3& euler )
	{
		Set( euler.x, euler.y, euler.z );
	}

	
	Quaternion& Quaternion::SetXAxis( float angle )
	{
		w	= cosf( 0.5f * angle );
		v.x	= sinf( 0.5f * angle );
		v.y	= 0.0f;
		v.z	= 0.0f;
		return *this;
	}

	Quaternion& Quaternion::SetYAxis( float angle )
	{
		w	= cosf( 0.5f * angle );
		v.x	= 0.0f;
		v.y	= sinf( 0.5f * angle );
		v.z	= 0.0f;
		return *this;
	}

	Quaternion& Quaternion::SetZAxis( float angle )
	{
		w	= cosf( 0.5f * angle );
		v.x	= 0.0f;
		v.y	= 0.0f;
		v.z	= sinf( 0.5f * angle );
		return *this;
	}




	
	void Quaternion::Identity()
	{
		v.x = v.y = v.z;
		w = 1.0f;
	}

	bool Quaternion::IsIdentity() const
	{
		return w == 1.0f;
	}

	
	Vector3 Quaternion::GetXAxis() const
	{
		Vector3 v;
		GetXAxis( v );
		return v;
	}

	void Quaternion::GetXAxis( Vector3& axis ) const
	{
		axis.x = 1.0f -	( v.y * ( v.y + v.y ) + v.z * ( v.z + v.z ));
		axis.y = v.x *	( v.y + v.y ) + w * ( v.z + v.z );
		axis.z = v.x *	( v.z + v.z ) - w * ( v.y + v.y );
	}

	Vector3 Quaternion::GetYAxis() const
	{
		Vector3 v;
		GetYAxis( v );
		return v;
	}

	void Quaternion::GetYAxis( Vector3& axis ) const
	{
		axis.x = v.x *	( v.y + v.y ) - w * ( v.z + v.z );
		axis.y = 1.0f -	( v.x * ( v.x + v.x ) + v.z * ( v.z + v.z ));
		axis.z = v.y *	( v.z + v.z ) + w * ( v.x + v.x );
	}

	Vector3 Quaternion::GetZAxis() const
	{
		Vector3 v;
		GetZAxis( v );
		return v;
	}

	void Quaternion::GetZAxis( Vector3& axis ) const
	{
		axis.x = v.x *	( v.z + v.z ) + w * ( v.y + v.y );
		axis.y = v.y *	( v.z + v.z ) - w * ( v.x + v.x );
		axis.z = 1.0f -	( v.x * ( v.x + v.x ) + v.y * ( v.y + v.y ));
	}

   	
	Quaternion& Quaternion::operator *= ( float s )
	{
		v *= s;
		w *= s;
		return *this;
	}

	Quaternion& Quaternion::operator /= ( float s )
	{
		return operator *=(1.0f / s);
	}

	Quaternion Quaternion::operator * ( float s ) const
	{
		return Quaternion( *this ) *= s;
	}

	Quaternion Quaternion::operator / ( float s ) const
	{
		return Quaternion( *this ) /= s;
	}

	inline Quaternion operator * ( float s, const Quaternion& q )
	{
		return Quaternion( q ) *= s;
	}

	

	Quaternion Quaternion::GetInverse() const
	{
		return Quaternion( *this ).Invert();
	}

	Quaternion&  Quaternion::Invert()
	{
		float mag = MagnitudeSquared();
		Conjugate();
		return operator /= (mag);
	}

	
	float Quaternion::SDot( const Quaternion& q ) const
	{
		return dot(v, q.v) + w * q.w;
	}

	
	float Quaternion::Magnitude() const
	{
		return sqrtf( MagnitudeSquared() );
	}

	float Quaternion::MagnitudeSquared() const
	{
		return magnitudeSq(v) + w * w;
	}

	
	Quaternion Quaternion::GetConjugate() const
	{
		return Quaternion( *this ).Conjugate();
	}

	Quaternion& Quaternion::Conjugate()
	{
		v = -v;
		return *this;
	}

	
	Quaternion Quaternion::GetNormalized() const
	{
		return Quaternion( *this ).Normalize();
	}

	Quaternion& Quaternion::Normalize() 
	{
		return operator /= (Magnitude());
	}

	
	Quaternion& Quaternion::operator = ( const Quaternion& q )
	{
		v = q.v;
		w = q.w;
		return *this;
	}

	Quaternion& Quaternion::operator += ( const Quaternion& q )
	{
		v += q.v;
		w += q.w;
		return *this;
	}

	Quaternion& Quaternion::operator -= ( const Quaternion& q )
	{
		v -= q.v;
		w -= q.w;
		return *this;
	}

	Quaternion& Quaternion::operator *= ( const Quaternion& q )
	{
		Quaternion temp = *this;
		cross(temp.v, q.v, v);
		v += temp.w * q.v + q.w * temp.v;
		w = temp.w * q.w - dot(temp.v, q.v);
		return *this;
	}

	Quaternion& Quaternion::operator /= ( const Quaternion& q )
	{
		(*this) *= q.GetInverse();
		return *this;
	}

	Quaternion Quaternion::operator + ( const Quaternion& q ) const
	{
		return Quaternion( *this ) += q;
	}

	Quaternion Quaternion::operator - ( const Quaternion& q ) const
	{
		return Quaternion( *this ) -= q;
	}

	Quaternion Quaternion::operator * ( const Quaternion& q ) const
	{
		Quaternion res(*this);
		res *= q;
		return res;
	}

	Quaternion Quaternion::operator / ( const Quaternion& q ) const
	{
		return (*this) * q.GetInverse();
	}

	void hpb(const Quaternion &q, Vector3 &result);
	float heading(const Quaternion& q);
	float pitch(const Quaternion& q);
	float bank(const Quaternion& q);
}





// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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