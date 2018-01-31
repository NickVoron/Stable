// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "libcfg.h"

namespace nm
{




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

	
	template<typename vec_t> vec_t		operator +	(const vec_t& v) const;
	template<typename vec_t> vec_t		operator -	(const vec_t& v) const;


	void operator*=(scalar s);
	void operator/=(scalar s);

	self_t operator-() const;

	operator scalar* () { return &x; }
	operator const scalar* () const { return &x; }

	scalar x, y;
};




template<typename scalar>
struct vector3_t 
{
	typedef vector3_t self_t;

	vector3_t();
	vector3_t(scalar _x, scalar _y, scalar _z);

	template<typename vec_t> vector3_t(const vec_t& v);

	
	template<typename vec_t> self_t&	operator =	(const vec_t& v);
	template<typename vec_t> void		operator +=	(const vec_t& v);
	template<typename vec_t> void		operator -=	(const vec_t& v);
	template<typename vec_t> void		operator *=	(const vec_t& v);
	template<typename vec_t> void		operator /=	(const vec_t& v);

	
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



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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