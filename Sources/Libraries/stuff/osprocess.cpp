#include "osprocess.h"

#ifdef WIN32
#include <process.h>
#endif

namespace Base
{	 
	void spawn_process(const char* processName, const char* params)
	{
#ifdef WIN32
		_spawnlp(_P_NOWAIT, processName, processName, params, NULL);
#endif
	}
}