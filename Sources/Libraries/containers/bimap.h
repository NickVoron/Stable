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

	auto size() const { return map0.size(); }

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