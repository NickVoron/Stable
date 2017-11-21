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