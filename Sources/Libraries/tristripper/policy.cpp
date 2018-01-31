








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
