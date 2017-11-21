#pragma once
#include "types.h"

namespace imgui
{

namespace focus	
{
	void set(const Id& id);
	bool check(const Id& id);
}

namespace capture 
{
	void set(const Id& id);
	bool check(const Id& id);
	bool empty();
	void reset();
}

}