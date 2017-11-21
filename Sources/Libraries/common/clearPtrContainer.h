#pragma once

#include <algorithm>
#include <utility>

namespace Base
{

struct FDeleteObject
{
	template<typename T> inline void operator()(const T* ptr) const { delete ptr;}
	template<typename K, typename T> inline void operator() (std::pair<K,T*> &temp) const { delete temp.second;}
};

template<class Cont>
inline
void deleteObjects(Cont& cont)
{
	if(!cont.empty())
	{
		std::for_each(cont.begin(), cont.end(), FDeleteObject());
	}
}

template<class Cont>
inline
void clearPtrContainer(Cont& cont)
{
	if(!cont.empty())
	{
		deleteObjects(cont);
		cont.clear();
	}
}

}
