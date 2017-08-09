// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

template <typename T, unsigned int alignment = 16> 
class AlignedArray
{
public:
	AlignedArray():data(0), sz(0), allocated(0){}
	~AlignedArray(){ clear(); }

	void clear();
	void resize(std::size_t size);
	void reserve(std::size_t size);
	auto size() const { return sz; }
	auto capacity() const { return allocated; }

	T& operator[](std::size_t idx) { return data[idx]; }
	const T& operator[](std::size_t idx) const { return data[idx]; }
	
private:
	T* data;
	std::size_t sz;
	std::size_t allocated;
};

#define NOT_ALIGNED

#ifndef	NOT_ALIGNED
#define al_malloc(size, alignment) _aligned_malloc(size, alignment)
#define al_realloc(base, size, alignment) _aligned_realloc(base, size, alignment)
#define al_free(size) _aligned_free(size)
#else
#define al_malloc(size, alignment) malloc(size)
#define al_realloc(base, size, alignment) realloc(base, size)
#define al_free(size) free(size)
#endif





template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::clear()
{
	if(data)
	{
		al_free(data);
		sz = 0;
		data = 0;
		allocated = 0;
	}
}

template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::resize(std::size_t size)
{
	if(size != sz)
	{
		reserve(size);		
		sz = size;
	}	
}

template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::reserve(std::size_t size)
{
	if(size > allocated)
	{
		allocated = size;

		data = (T*)(data ? al_realloc(data, sizeof(T) * allocated, alignment) : al_malloc(sizeof(T) * allocated, alignment));
	}
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