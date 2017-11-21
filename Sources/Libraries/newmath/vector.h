#pragma once

#include "libcfg.h"

namespace nm
{

//
//
//
template<typename scalar>
struct vector2_t 
{
	typedef vector2_t self_t;

	vector2_t();
	vector2_t(scalar _x, scalar _y);

	template<typename vec_t> vector2_t(const vec_t& v);

	template<typename vec_t> self_t&	operator =	(const vec_t& v);
	template<typename vec_t> void		operator +=	(const vec_t& v);
	template<typename vec_t> void		operator -=	(const vec_t& v);
	template<typename vec_t> void		operator *=	(const vec_t& v);
	template<typename vec_t> void		operator /=	(const vec_t& v);
 	template<class T>					operator T	() const;

	//binary
	template<typename vec_t> vec_t		operator +	(const vec_t& v) const;
	template<typename vec_t> vec_t		operator -	(const vec_t& v) const;


	void operator*=(scalar s);
	void operator/=(scalar s);

	self_t operator-() const;

	operator scalar* () { return &x; }
	operator const scalar* () const { return &x; }

	scalar x, y;
};

//
//
//
template<typename scalar>
struct vector3_t 
{
	typedef vector3_t self_t;

	vector3_t();
	vector3_t(scalar _x, scalar _y, scalar _z);

	template<typename vec_t> vector3_t(const vec_t& v);

	//unary
	template<typename vec_t> self_t&	operator =	(const vec_t& v);
	template<typename vec_t> void		operator +=	(const vec_t& v);
	template<typename vec_t> void		operator -=	(const vec_t& v);
	template<typename vec_t> void		operator *=	(const vec_t& v);
	template<typename vec_t> void		operator /=	(const vec_t& v);

	//binary
	template<typename vec_t> vec_t		operator +	(const vec_t& v) const;
	template<typename vec_t> vec_t		operator -	(const vec_t& v) const;
	template<typename vec_t>			operator vec_t() { return vec_t(*this); }

	void operator*=(scalar s);
	void operator/=(scalar s);

	self_t operator-() const;

	operator scalar* () { return &x; }
	operator const scalar* () const { return &x; }

	scalar x, y, z;
};

template<typename scalar>
struct vector4_t 
{
	typedef vector4_t self_t;

	vector4_t();
	vector4_t(scalar _x, scalar _y, scalar _z, scalar _w);

	template<typename vec_t> vector4_t(const vec_t& v);

	template<typename vec_t> self_t&	operator =	(const vec_t& v);
	template<typename vec_t> void		operator +=	(const vec_t& v);
	template<typename vec_t> void		operator -=	(const vec_t& v);
	template<typename vec_t> void		operator *=	(const vec_t& v);
	template<typename vec_t> void		operator /=	(const vec_t& v);
	template<class T>					operator T	() const;

	void operator*=(scalar s);
	void operator/=(scalar s);

	self_t operator-() const;

	operator scalar* () { return &x; }
	operator const scalar* () const { return &x; }

	scalar x, y, z, w;
};

typedef vector2_t<float> Vector2;
typedef vector3_t<float> Vector3;
typedef vector4_t<float> Vector4;

}