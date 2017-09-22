// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
	inline stream::istream& operator>>(stream::istream& is, IntList& list)		{ return stream_in(is, list); }
	inline stream::istream& operator>>(stream::istream& is, FloatList& list)		{ return stream_in(is, list); }
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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