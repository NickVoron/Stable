// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.