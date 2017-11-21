#pragma once

#include "vector.h"

namespace nm
{
	template<class scalar>
	struct swizzle2_t
	{
		typedef swizzle2_t self_t;

		swizzle2_t(scalar& x, scalar& y);

		template<typename vec_t> self_t&	operator =	(const vec_t& v);
		template<typename vec_t> void		operator +=	(const vec_t& v);
		template<typename vec_t> void		operator -=	(const vec_t& v);
		template<typename vec_t> void		operator *=	(const vec_t& v);
		template<typename vec_t> void		operator /=	(const vec_t& v);
		template<class T>					operator T	() const;

		void operator*=(scalar s);
		void operator/=(scalar s);

		union {	struct	{ scalar& x; scalar& y;	}; };
	};

}

