#pragma once
#include "../types.h"

namespace imgui
{
	bool checkbox(const char* caption, const Point2& pos, bool& value);
	bool checkbox(const char* caption, bool& value);

	bool checkbox(const char* caption, const Vector3& pos, bool& value);
	bool checkbox(const Vector3& pos, bool& value);

	template<class PropertyGET, class PropertySET>
	bool checkbox(const char* caption, const PropertyGET& get, const PropertySET& set)
	{
		bool val = get();
		check(caption, val);
		set(val);
		return val;
	}
}