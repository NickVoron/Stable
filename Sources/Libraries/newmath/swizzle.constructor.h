#pragma once

#include "swizzle.h" 

namespace nm { 
template<class scalar, class vec_t> swizzle2_t<scalar> xx(vec_t& v) { return swizzle2_t<scalar>(v.x, v.x); }
template<class scalar, class vec_t> swizzle2_t<scalar> xy(vec_t& v) { return swizzle2_t<scalar>(v.x, v.y); }
template<class scalar, class vec_t> swizzle2_t<scalar> xz(vec_t& v) { return swizzle2_t<scalar>(v.x, v.z); }
template<class scalar, class vec_t> swizzle2_t<scalar> xw(vec_t& v) { return swizzle2_t<scalar>(v.x, v.w); }
template<class scalar, class vec_t> swizzle2_t<scalar> yx(vec_t& v) { return swizzle2_t<scalar>(v.y, v.x); }
template<class scalar, class vec_t> swizzle2_t<scalar> yy(vec_t& v) { return swizzle2_t<scalar>(v.y, v.y); }
template<class scalar, class vec_t> swizzle2_t<scalar> yz(vec_t& v) { return swizzle2_t<scalar>(v.y, v.z); }
template<class scalar, class vec_t> swizzle2_t<scalar> yw(vec_t& v) { return swizzle2_t<scalar>(v.y, v.w); }
template<class scalar, class vec_t> swizzle2_t<scalar> zx(vec_t& v) { return swizzle2_t<scalar>(v.z, v.x); }
template<class scalar, class vec_t> swizzle2_t<scalar> zy(vec_t& v) { return swizzle2_t<scalar>(v.z, v.y); }
template<class scalar, class vec_t> swizzle2_t<scalar> zz(vec_t& v) { return swizzle2_t<scalar>(v.z, v.z); }
template<class scalar, class vec_t> swizzle2_t<scalar> zw(vec_t& v) { return swizzle2_t<scalar>(v.z, v.w); }
template<class scalar, class vec_t> swizzle2_t<scalar> wx(vec_t& v) { return swizzle2_t<scalar>(v.w, v.x); }
template<class scalar, class vec_t> swizzle2_t<scalar> wy(vec_t& v) { return swizzle2_t<scalar>(v.w, v.y); }
template<class scalar, class vec_t> swizzle2_t<scalar> wz(vec_t& v) { return swizzle2_t<scalar>(v.w, v.z); }
template<class scalar, class vec_t> swizzle2_t<scalar> ww(vec_t& v) { return swizzle2_t<scalar>(v.w, v.w); }
}