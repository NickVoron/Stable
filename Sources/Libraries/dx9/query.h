#pragma once

#include <d3d9.h>

namespace dx9 
{

class OcclusionQuery
{
public:
	OcclusionQuery();
	void init();
	void deinit();

	void begin();
	void end();
	unsigned int result();

	static int occludedCounter;
	static void resetCounter();
private:
	LPDIRECT3DQUERY9 query;
};

} //

