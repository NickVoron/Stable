#pragma once

#include "../types.h"

namespace imgui
{
	bool button(const char* caption, const Point2& pos, int length);
	bool button(const char* caption);

	template<class Func> bool button(const char* caption, const Point2& pos, int length, Func&& func) { bool res = button(caption, pos, length); if (res) func(); return res; }
	template<class Func> bool button(const char* caption, Func&& func) { bool res = button(caption); if (res) func(); return res; }

	template<class T> bool button(const T& val) { return button(str::stringize(val).c_str()); }
	template<class T, class Func> bool button(const T& val, Func&& func) { return button(str::stringize(val).c_str(), func); }
	template<class T, class Func> bool button(const T& val, const Point2& pos, int length, Func&& func) { return button(str::stringize(val).c_str(), pos, length, std::forward<Func>(func)); }

	inline bool button_if(bool condition, const char* caption, const Point2& pos, int length) { return condition ? button(caption, pos, length) : false; }
	inline bool button_if(bool condition, const char* caption) { return condition ? button(caption) : false; }

	template<class Func> bool button_if(bool condition, const char* caption, const Point2& pos, int length, Func&& func) { return condition ? button(caption, pos, length, std::forward<Func>(func)) : false; }
	template<class Func> bool button_if(bool condition, const char* caption, Func&& func) { return condition ? button(caption, std::forward<Func>(func)) : false; }

	template<class T> bool button_if(bool condition, const T& val) { return button_if(condition, str::stringize(val).c_str()); }
	template<class T, class Func> bool button_if(bool condition, const T& val, Func&& func) { return button_if(condition, str::stringize(val).c_str(), std::forward<Func>(func)); }
	template<class T, class Func> bool button_if(bool condition, const T& val, const Point2& pos, int length, Func&& func) { return button_if(condition, str::stringize(val).c_str(), pos, length, std::forward<Func>(func)); }
}