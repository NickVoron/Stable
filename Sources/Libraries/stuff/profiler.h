#pragma once

#include "timer.h"
#include "containers/library.include.h"
#include "meyersSingleton.h"


namespace Base {

struct ScopeProfiler
{
	ScopeProfiler(const char* text, int mode);
	~ScopeProfiler();

	Base::Timer t;
	int mode;
	std::string text;
};

#define PROFILE_THIS_SCOPE_MSG Base::ScopeProfiler p(__FUNCTION__, 0);
#define PROFILE_THIS_SCOPE_REPORT Base::ScopeProfiler p(__FUNCTION__, 1);

struct IncrementalProfiler : public Base::IntrusiveList<IncrementalProfiler>::Node
{
	IncrementalProfiler(const char* text);

	Base::Timer t;
	std::string text;
};

struct IncrementalProfilers : public Base::IntrusiveList<IncrementalProfiler>, public Base::MeyersSingleton<IncrementalProfilers>
{
	void report();
};



}
