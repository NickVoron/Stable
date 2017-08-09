// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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