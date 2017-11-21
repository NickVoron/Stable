#include "profiler.h"

namespace AppModules
{

	void D3DProfilerKernel::init()
	{
		dx9::device->CreateQuery(D3DQUERYTYPE_EVENT, &flushEvent);
	}

	void D3DProfilerKernel::release()
	{
		flushEvent->Release();
		flushEvent = 0;
	}

	void D3DProfilerKernel::start(const char* sampleName)
	{
		flush(); 
		D3DPERF_BeginEvent( D3DCOLOR_ARGB(255, 255, 255, 255),  Base::StrUtils::Convert(sampleName).c_str()); 
	}
	void D3DProfilerKernel::end()
	{ 
		D3DPERF_EndEvent(); 
		flush();
	}

	void D3DProfilerKernel::flush()
	{
		flushEvent->Issue(D3DISSUE_END);
		while(S_FALSE == flushEvent->GetData( NULL, 0, D3DGETDATA_FLUSH ));
	}

	profiler::KernelImplementation<D3DProfilerKernel> kernel;

	void Profiler::init()
	{
		kernel.init();
	}

	void Profiler::process()
	{
		PROFILE_REPORT_RESULT;
	}

	void Profiler::release()
	{
		kernel.release();
	}
	
}
