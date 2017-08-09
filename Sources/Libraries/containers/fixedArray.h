// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once

namespace Base
{

template<class T, const std::size_t maxCount>
class FixedArray
{
public:
	static const std::size_t maxSize = maxCount;

	T& addEntry() {	return cont[cursor++];	}
	T& addEntry(const T& t)	{	return addEntry() = t;	}

	T* begin() { return &cont[0]; }
	T* end() { return &cont[cursor]; }

	void eraseChain(std::size_t starts, std::size_t count)
	{
		if(starts + count != cursor)
		{
			for(std::size_t i = 0; i < count; ++i)
			{
				cont[i + starts] = cont[i + cursor - count];
			}
		}	

		cursor -= count;
	}

	bool eraseEntry(std::size_t idx)
	{
		if(!isContainIdx(idx)) 
			return false; 

		--cursor;
		if(idx != cursor)
		{
			cont[idx] = cont[cursor];		
		}
		return true;
	}

	template< class T1 >
	bool eraseByValue( const T1& t )
	{
		bool flag = false;
		while (true)
		{
			auto idx = findIdx(t);
			if (idx.second)
			{
				flag |= eraseEntry(idx.first);
			}
			else
			{
				break;
			}
		}

		return flag;
	}

	std::pair<std::size_t, bool> findIdx(const T& t) const
	{
		for(std::size_t i = 0; i < cursor; ++i)
		{
			if(cont[i] == t) 
				return std::make_pair(i, true);
		}

		return std::make_pair(-1, false);
	}

	std::pair<std::size_t, bool> findIdx( T const* t ) const;

	T& operator[](std::size_t idx)	{		return cont[idx];	}

	const T& operator[](std::size_t idx) const	{	return cont[idx];	}

	bool empty() const {return cursor == 0;}
	std::size_t size() const {return cursor;}
	std::size_t capacity() const {return maxCount;}
	void clear(){cursor = 0;}
	void resize(std::size_t ns)
	{
		ENFORCE_MSG(isValidCursor(ns), "FixedArray::addEntry() : limit extended!");
		cursor = ns;
	}

	bool isContain(const T& val) const { return findIdx(val) != -1; }
	bool isFull() const {return cursor == maxSize;}
	bool isContainIdx(std::size_t idx) const { return idx < cursor; }

private:
	
	bool isValidCursor(int cur) const {	return cur < maxCount; } 

	T cont[maxCount];
	std::size_t cursor = 0;
};



template < class T, const std::size_t maxCount >
std::pair<std::size_t, bool>  FixedArray<T, maxCount >::findIdx( T const* t ) const
{
	auto base = &cont[0];
	auto last = &cont[cursor-1];
	if ( (t < base) || (t > last) ) 
		return std::make_pair(-1, false);

	return std::make_pair(t - base, false);
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