/*********************************************************************

	Nikolay Voronetskiy Copyright (C) 2005

	Author: Cool Ace
	  Date: 20.1.2005
	  Time: 16:49

	Description: ������ �������������� �������

*********************************************************************/
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


// realization:
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

