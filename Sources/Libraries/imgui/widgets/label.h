#pragma once
#include "../types.h"


namespace imgui
{
	void label(const Point2& pos, const char* text);
	void label(const Point2& pos, const std::string& text);
	void label(const char* text);
	void label(const std::string& text);

	template<class T> void label(const Point2& pos, const char* text, const T& val)			{ label(pos, str::stringize(text, val) );	}
	template<class T> void label(const Point2& pos, const std::string& text, const T& val)	{ label(pos, str::stringize(text, val) );	}
	template<class T> void label(const Point2& pos, const T& val)							{ label(pos, str::stringize(val));			}

	template<class T> void label(const char* text, const T& val)		{ label(str::stringize(text, val) );	}
	template<class T> void label(const std::string& text, const T& val)	{ label(str::stringize(text, val) );	}
	template<class T> void label(const T& val)							{ label(str::stringize(val));			}
}