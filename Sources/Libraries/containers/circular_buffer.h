// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/enforce.h"

template<class T, unsigned int maxElements>
class circular_buffer
{
public:
	static const unsigned int BUFFER_SIZE = maxElements;

	T& operator[](unsigned int index) { return buffer[cidx(index)]; }
	const T& operator[](unsigned int index) const { return buffer[cidx(index)]; }

	T& current() { return operator[](cursor); }
	const T& current() const { return operator[](cursor); }

	T& step()
	{
		if (count < BUFFER_SIZE)
			++count;

		return next();
	}

	int size() const { return count; }

	int begin_index() const { return cursor + 1; }
	int end_index() const { return begin_index() + size(); }

private:

	unsigned int cidx(unsigned int index) const
	{
		ENFORCE(count > 0 && count <= BUFFER_SIZE);
		return index % count;
	}

	T& next()
	{
		++cursor;
		if (cursor == count)
			cursor = 0;

		return current();
	}

	int count = 0;
	int cursor = -1;
	T buffer[BUFFER_SIZE];
};



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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