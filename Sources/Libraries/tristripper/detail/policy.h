








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
