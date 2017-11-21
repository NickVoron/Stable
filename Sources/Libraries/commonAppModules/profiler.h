#pragma once

#include "appModule/library.include.h"
#include "profiler/library.include.h"

#include "d3d9/direct3d9.h"

namespace AppModules
{

struct D3DProfilerKernel
{
	void init();
	void release();
	void start(const char* sampleName);
	void end();
	void flush();

	IDirect3DQuery9* flushEvent;
};

class Profiler : public ModuleBase<Profiler, Loki::MakeTypelist<Direct3D9>>
{
public:
	template<class Params>  void init(Params& p) { init(); }
	template<class Params>  void process(Params& p) { process(); }
	template<class Params>  void release(Params& p) { release(); }

	void init();
	void process();	
	void release();
};

}