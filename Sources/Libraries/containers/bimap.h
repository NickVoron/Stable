// Copyright (C) 2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <map>

namespace Base 
{

template < class A, class T >
class bimap
{
public:
	typedef std::map< A, T > Container0;
	typedef std::map< T, A > Container1;

	T& operator[](const A& index)
	{
		T& res = map0[index];
		map1[res] = index;

		return res;
	}

	A& operator[](const T& index)
	{
		A& res = map1[index];
		map0[res] = index;

		return res;
	}

	auto find(const T& index) { return map1.find(index); }
	auto find(const A& index) { return map0.find(index); }
	auto find(const T& index) const { return map1.find(index); }
	auto find(const A& index) const { return map0.find(index); }

	int size() const { return map0.size(); }

	void clear() 
	{ 
		map0.clear();
		map1.clear();
	}

	const Container0& container0() const { return map0; }
	const Container1& container1() const { return map1; }

protected:
	Container0 map0;
	Container1 map1;
};

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