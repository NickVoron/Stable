#pragma once

#include "vector.h"
#include "vector.operations.h"

namespace nm
{
		  
	// The Quaternion class provides implementation for quaternions ( also
	// known as Hamiltionians ). Quaternions are used to describe orientations
	// in 3D space. Internally, the Quaternion class stores a vector component v,
	// and a scalar component w. Quaternions have a number of advantages
	// over the more commonly used Euler angles and transformation matrices.
	// They do not suffer from the gimbal-lock problem when orientations
	// approach +/- Pi/2 radians.
	// Additionally, quaternions multiplication	is significaly more efficient
	// than comparable matrix multiplication.
	//
	//	The quaternion class is used for performing computations in quaternion
	//	space. A quaternion is a 4-vector which is most often used to describe
	//	the orientation of rigid reference frame. 
	//	A unit quaternion is an axis (v) and an angle (w) around that axis.
	//	Actually, the more precise definition would be 
	//	[(x,y,z), w] = [ v sin a, cos a ] represents a rotation 2a around the
	//	axis given by the unit vector v.
	//
	//	A quaternion is a 4 element 'vector'  [w,x,y,z] where:
	//
	//	q = w + xi + yj + zk
	//	i*i = -1
	//	j*j = -1
	//	k*k = -1
	//	i*j = -j*i = k
	//	j*k = -k*j = i
	//	k*i = -i*k = j
	//	q' (conjugate) = w - xi - yj - zk
	//	||q|| (magnitude) = sqrt(q*q') = sqrt(w*w + x*x + y*y + z*z)
	//	unit quaternion ||q|| == 1; this implies  q' == qinverse 
	//	quaternions are associative (q1*q2)*q3 == q1*(q2*q3)
	//	quaternions are not commutative  q1*q2 != q2*q1
	//	qinverse (inverse (1/q) ) = q'/(q*q')
	//	
	//	q can be expressed by w + xi + yj + zk or [w,x,y,z] 
	//	or as in this implementation (s,v) where s=w, and v=[x,y,z]
	//
	//	quaternions can represent a rotation.  The rotation is an angle t, around a 
	//	unit vector u.   q=(s,v);  s= cos(t/2);   v= u*sin(t/2).
	//
	//	quaternions can apply the rotation to a point.  let the point be p [px,py,pz],
	//	and let P be a quaternion(0,p).  Protated = q*P*qinverse 
	//	( Protated = q*P*q' if q is a unit quaternion)
	//
	//	concatenation rotations is similar to matrix concatenation.  given two rotations
	//	q1 and q2,  to rotate by q1, then q2:  let qc = (q2*q1), then the combined 
	//	rotation is given by qc*P*qcinverse (= qc*P*qc' if q is a unit quaternion)
	//
	//	multiplication: 
	//	q1 = w1 + x1i + y1j + z1k
	//	q2 = w2 + x2i + y2j + z2k
	//	q1*q2 = q3 =
	//			(w1*w2 - x1*x2 - y1*y2 - z1*z2)     {w3}
	//	        (w1*x2 + x1*w2 + y1*z2 - z1*y2)i	{x3}
	//			(w1*y2 - x1*z2 + y1*w2 + z1*x2)j    {y3}
	//			(w1*z2 + x1*y2 + y1*x2 + z1*w2)k	{z3}
	//
	//	also, 
	//	q1 = (s1,v1) = [s1,(x1,y1,z1)]
	//	q2 = (s2,v2) = [s2,(x2,y2,z2)]
	//	q1*q2 = q3	=	(s1*s2 - dot_product(v1,v2),			{s3}
	//					(s1*v2 + s2*v1 + cross_product(v1,v2))	{v3}
	//
	//
	//	interpolation - it is possible (and sometimes reasonable) to interpolate between
	//	two quaternions by interpolating each component.  This does not quarantee a 
	//	resulting unit quaternion, and will result in an animation with non-linear 
	//	rotational velocity.
	//
	//	spherical interpolation: (slerp) treat the quaternions as vectors 
	//	find the angle between them (w = arccos(q1 dot q2) ).
	//	given 0<=t<=1,  q(t) = q1*(sin((1-t)*w)/sin(w) + q2 * sin(t*w)/sin(w).
	//	since q == -q, care must be taken to rotate the proper way.  
	//
	//	this implementation uses the notation quaternion q = (quatS,quatV) 
	//	  where quatS is a scalar, and quatV is a 3 element vector.
	//
	//

	class Quaternion {
	public:

		// Public data
		Vector3	v;					// Vector part
		float	w;					// Scalar part

		// Ctors
		inline Quaternion();

		// Construct from euler angles
		inline Quaternion( float r, float p, float y );
		inline Quaternion( const Vector3& euler );

		// Set from Euler angles
		void Set( float roll, float pitch, float yaw  );
		inline void Set( const Vector3& euler );

		// Sets the rotation on angle about given axis
		Quaternion& Set( float angle, const Vector3& axis, bool normalizeAxis = false );

		inline Quaternion& SetXAxis( float angle );
		inline Quaternion& SetYAxis( float angle );
		inline Quaternion& SetZAxis( float angle );

		// Reset
		inline void Identity();
		inline bool IsIdentity() const;
		
		// Get the equivalent euler angles ( roll, pitch and yaw ) from quat
		Vector3 GetEulerAngles() const;

		// Get the X axis from the coordiante system ( matrix ) that quat represents
		inline Vector3 GetXAxis() const;
		inline void GetXAxis( Vector3& axis ) const;

		// Get the Y axis from the coordiante system ( matrix ) that quat represents
		inline Vector3 GetYAxis() const;
		inline void GetYAxis( Vector3& axis ) const;

		// Get the Z axis from the coordiante system ( matrix ) that quat represents
		inline Vector3 GetZAxis() const;
		inline void GetZAxis( Vector3& axis ) const;

		// Interpolation
		// Compute the unique interpolated quaternion that lies on
		// the shortest arc an a four-dimensional unit sphere
		// from start ( 0.0 ) to end ( 1.0 ) time.
		// Modifies and returns quaternion
		static Quaternion Slerp( const Quaternion& start, const Quaternion& end, float time, bool shortest = true );

		// Scalar
		inline Quaternion& operator *= ( float s );
		inline Quaternion& operator /= ( float s );
		inline Quaternion operator * ( float s ) const;
		inline Quaternion operator / ( float s ) const;

		// Quaternion ops
		inline Quaternion GetInverse() const;
		inline Quaternion& Invert();

		// SDot product 
		inline float SDot( const Quaternion& q ) const;

		// Exact magnitude
		inline float Magnitude() const;
		inline float MagnitudeSquared() const;

		// Return the conjugate of this quaternion.
		inline Quaternion GetConjugate() const;
		inline Quaternion& Conjugate();

		// Return normalized version
		inline Quaternion GetNormalized() const;
		inline Quaternion& Normalize();

		// Infix
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


	//////////////////////////////////////////////////////////////////////
	// Implementation

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

	//

	void Quaternion::Set( const Vector3& euler )
	{
		Set( euler.x, euler.y, euler.z );
	}

	//
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




	// Reset
	void Quaternion::Identity()
	{
		v.x = v.y = v.z;
		w = 1.0f;
	}

	bool Quaternion::IsIdentity() const
	{
		return w == 1.0f;
	}

	//
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

   	//
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

	// 

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

	// SDot
	float Quaternion::SDot( const Quaternion& q ) const
	{
		return dot(v, q.v) + w * q.w;
	}

	// Magnitude
	float Quaternion::Magnitude() const
	{
		return sqrtf( MagnitudeSquared() );
	}

	float Quaternion::MagnitudeSquared() const
	{
		return magnitudeSq(v) + w * w;
	}

	// Conjugate
	Quaternion Quaternion::GetConjugate() const
	{
		return Quaternion( *this ).Conjugate();
	}

	Quaternion& Quaternion::Conjugate()
	{
		v = -v;
		return *this;
	}

	// Normalization
	Quaternion Quaternion::GetNormalized() const
	{
		return Quaternion( *this ).Normalize();
	}

	Quaternion& Quaternion::Normalize() 
	{
		return operator /= (Magnitude());
	}

	// Quaternion ops
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

