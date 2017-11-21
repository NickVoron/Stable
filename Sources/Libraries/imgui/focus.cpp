#include "focus.h"

namespace imgui
{

namespace focus
{

	Id current;
	
	void set(const Id& id) { current = id; }
	bool check(const Id& id) { return current == id;}
}

namespace capture {

	Id current;
	bool captured;

	void set(const Id& id) { current = id; current.empty = false;}
	bool check(const Id& id) { return current == id && !current.empty;}
	bool empty() { return current.empty; }
	void reset() { current.empty = true; }
}

}
