#pragma once

#include "math/library.include.h"
#include "newmath/library.include.h"
#include "stream/library.include.h"

#include <iostream>
#include <vector>
		
namespace UserStruct
{
	typedef  std::vector<Vector3> Vector3List;
	typedef  std::vector<std::string> StringList;
	struct Index2List :		public std::vector<nm::index2>{};
	struct IntList :		public std::vector<int>{};
	struct FloatList :		public std::vector<float>{};

	template<class T>
	std::ostream& streamOut(std::ostream& os, const std::vector<T>& list)
	{
		os << "{ ";
		int last = (int)list.size() - 1;
		int c = 0;
		for(const auto& val : list)
		{
			os << val;
			if(c++ < last)
			{
				os << ", ";
			}
		}

		return os << " }" << std::endl;
	}

	template<class T> stream::ostream& stream_out(stream::ostream& os, const std::vector<T>& list)	{ return os << list; }
	template<class T> stream::istream& stream_in(stream::istream& is, std::vector<T>& list)			{ return is >> list; }

	inline std::ostream& operator<<(std::ostream& os, const Index2List& list)	{ return streamOut(os, list); }
	inline std::ostream& operator<<(std::ostream& os, const IntList& list)		{ return streamOut(os, list); }
	inline std::ostream& operator<<(std::ostream& os, const FloatList& list)	{ return streamOut(os, list); }

	inline stream::ostream& operator<<(stream::ostream& os, const Index2List& list)		{ return stream_out(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const IntList& list)		{ return stream_out(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const FloatList& list)		{ return stream_out(os, list); }

	inline stream::istream& operator>>(stream::istream& is, Index2List& list)		{ return stream_in(is, list); }
	inline stream::istream& operator>>(stream::istream& is, IntList& list)			{ return stream_in(is, list); }
	inline stream::istream& operator>>(stream::istream& is, FloatList& list)		{ return stream_in(is, list); }
}