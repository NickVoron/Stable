#include "query.h"
#include "device.h"

#include "dx9/interfaces.h"

namespace dx9 
{
	int OcclusionQuery::occludedCounter = 0;

	void OcclusionQuery::resetCounter()
	{
		occludedCounter = 0;
	}

	OcclusionQuery::OcclusionQuery():query(0)
	{

	}

	void OcclusionQuery::init()
	{
		device->CreateQuery( D3DQUERYTYPE_OCCLUSION, &query);
	}

	void OcclusionQuery::deinit()
	{
		SAFE_RELEASE(query);
	}

	void OcclusionQuery::begin()
	{
		query->Issue(D3DISSUE_BEGIN);
	}

	void OcclusionQuery::end()
	{
		query->Issue(D3DISSUE_END);
	}

	unsigned int OcclusionQuery::result()
	{
		DWORD occluded = 0;
		//while(S_FALSE == query->GetData( &occluded, sizeof(DWORD), D3DGETDATA_FLUSH )){}

		//query->GetData( &occluded, sizeof(DWORD), 0 );

  		if( S_FALSE == query->GetData( &occluded, sizeof(DWORD), 0 ) )
  		{
  			return 1;
  		}
// 
// 		if(occluded == 0)
// 			++occludedCounter;

		return occluded;
	}
	
} //