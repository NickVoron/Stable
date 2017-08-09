// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#include "detail/policy.h"




namespace triangle_stripper {

	namespace detail {




void policy::Challenge(strip Strip, size_t Degree, size_t CacheHits)
{
	if (Strip.Size() < m_MinStripSize)
		return;

	
	if (! m_Cache) {

		if (Strip.Size() > m_Strip.Size())
			m_Strip = Strip;

	
	} else {

		
		if (CacheHits > m_CacheHits) {
			m_Strip = Strip;
			m_Degree = Degree;
			m_CacheHits = CacheHits;

		} else if (CacheHits == m_CacheHits) {

			
			if ((m_Strip.Size() != 0) && (Degree < m_Degree)) {
				m_Strip = Strip;
				m_Degree = Degree;

			
			} else if (Strip.Size() > m_Strip.Size()) {
				m_Strip = Strip;
				m_Degree = Degree;
			}
		}
	}
}




	} 

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