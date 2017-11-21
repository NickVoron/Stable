#include "gapi.h"

namespace gapi
{
	bool flags[GAPI_COUNT] = {false, false, false};

	void initialize(Id id)	{	flags[id] = true;	}
	bool initialized(Id id)	{	return flags[id];	}
}
