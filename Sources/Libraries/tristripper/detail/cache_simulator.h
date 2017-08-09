// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef TRI_STRIPPER_HEADER_GUARD_CACHE_SIMULATOR_H
#define TRI_STRIPPER_HEADER_GUARD_CACHE_SIMULATOR_H

#include <algorithm>
#include <limits>
#include <deque>




namespace triangle_stripper {

	namespace detail {




class cache_simulator
{
public:
	cache_simulator();

	void clear();
	void resize(size_t Size);
	void reset();
	void push_cache_hits(bool Enabled = true);
	size_t size() const;

	void push(index i, bool CountCacheHit = false);
	void merge(const cache_simulator & Backward, size_t PossibleOverlap);

	void reset_hitcount();
	size_t hitcount() const;

protected:
	typedef std::deque<index> indices_deque;

	indices_deque	m_Cache;
	size_t			m_NbHits;
	bool			m_PushHits;
};









inline cache_simulator::cache_simulator()
	: m_NbHits(0),
	  m_PushHits(true)
{

}


inline void cache_simulator::clear()
{
	reset_hitcount();
	m_Cache.clear();
}


inline void cache_simulator::resize(const size_t Size)
{
	m_Cache.resize(Size, std::numeric_limits<index>::max());
}


inline void cache_simulator::reset()
{
	std::fill(m_Cache.begin(), m_Cache.end(), std::numeric_limits<index>::max());
	reset_hitcount();
}


inline void cache_simulator::push_cache_hits(bool Enabled)
{
	m_PushHits = Enabled;
}


inline size_t cache_simulator::size() const
{
	return m_Cache.size();
}


inline void cache_simulator::push(const index i, const bool CountCacheHit)
{
	if (CountCacheHit || m_PushHits) {

		if (std::find(m_Cache.begin(), m_Cache.end(), i) != m_Cache.end()) {

			
			if (CountCacheHit)
				++m_NbHits;
			
			
			if (! m_PushHits)
				return;
		}
	}
	    
	
	m_Cache.push_front(i);
	m_Cache.pop_back();
}


inline void cache_simulator::merge(const cache_simulator & Backward, const size_t PossibleOverlap)
{
	const size_t Overlap = std::min(PossibleOverlap, size());

	for (size_t i = 0; i < Overlap; ++i)
		push(Backward.m_Cache[i], true);

	m_NbHits += Backward.m_NbHits;
}


inline void cache_simulator::reset_hitcount()
{
	m_NbHits = 0;
}


inline size_t cache_simulator::hitcount() const
{
	return m_NbHits;
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