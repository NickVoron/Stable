// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <iostream>

#include "stream/library.include.h"

#include "bimap.h"

namespace Base 
{
	template <class T1, class T2>
	stream::istream& operator>>(stream::istream& is, bimap<T1, T2>& map)
	{
		map.clear();
		int size; is >> size;
		for (int i = 0; i < size; ++i)
		{
			T1 key;
			is >> key;
			is >> map[key];
		}
		return is;
	}

	template <class T1, class T2>
	stream::ostream& operator<<(stream::ostream& os, const bimap<T1, T2>& map)
	{
		os << map.size();
		for(const auto& v : map.container0())
		{
			os << v.first << v.second;
		}
		return os;
	}

	template <class T1, class T2>
	std::ostream& operator<<(std::ostream& os, const bimap<T1, T2>& map)
	{
		for (const auto& v : map.container0())
		{
			os << v.first << " : " << v.second << std::endl;
		}
		return os;
	}

}



// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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