// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef TRI_STRIPPER_HEADER_GUARD_POLICY_H
#define TRI_STRIPPER_HEADER_GUARD_POLICY_H

#include "public_types.h"
#include "types.h"




namespace triangle_stripper {

	namespace detail {




class policy
{
public:
	policy(size_t MinStripSize, bool Cache);

	strip BestStrip() const;
	void Challenge(strip Strip, size_t Degree, size_t CacheHits);

private:
	strip	m_Strip;
	size_t	m_Degree;
	size_t	m_CacheHits;

	const size_t	m_MinStripSize;
	const bool		m_Cache;
};





inline policy::policy(size_t MinStripSize, bool Cache)
: m_Degree(0), m_CacheHits(0), m_MinStripSize(MinStripSize), m_Cache(Cache) { }


inline strip policy::BestStrip() const
{
	return m_Strip;
}




	} 

} 




#endif 




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