// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <algorithm>
#include <vector>

#include "nvtristrip/NvTriStrip.h"
#include "containers/alignedArray.h"

namespace Geometry
{

template<class IndexType>
struct Face
{
	Face(IndexType a_, IndexType b_, IndexType c_) : a(a_), b(b_), c(c_){}

	union
	{
		struct
		{
			IndexType a;
			IndexType b;
			IndexType c;
		};

		IndexType vertex[3];
	};
	
};

template<class IndexType>
struct FaceRef
{
	FaceRef(IndexType& a_, IndexType& b_, IndexType& c_) : a(a_), b(b_), c(c_){}
	IndexType& a;
	IndexType& b;
	IndexType& c;
};


template<class Type_>
class IndexStream
{
public:
	typedef Type_ Type;
											  
	IndexStream(){}
	~IndexStream(){ clear(); }

	IndexStream(const IndexStream<Type>& is) : maxIndexValue(-1), indices(0) 	{ operator=(is); }

	void update() { indices = data.size() > 0 ? &data[0] : 0; }

	void clear()
	{
		data.clear();
		update();
	}

	IndexStream<Type>& operator=(const IndexStream<Type>& is)
	{
		resize(is.size());
		memcpy( indices, is.indices, memory());
		maxIndexValue = is.maxIndexValue;
		return *this;
	}

	Type* begin()				{ return &data[0]; }
	const Type* begin() const	{ return &data[0]; }
	Type* end()				{ return &data[size()]; }
	const Type* end() const { return &data[size()]; }

	Type& operator[](std::size_t i){return indices[i];}
	const Type& operator[](std::size_t i) const{return indices[i];}

	Type getMaxIndexValue() const { return maxIndexValue; }
	Type getNextIndexValue() const { return maxIndexValue + 1; }

	std::size_t size() const { return data.size(); }

	void resize(std::size_t s)
	{
		data.resize(s);
		update();
		if(s == 0) maxIndexValue = -1;
	}

	template<class IStream>
	void assign(const IStream& is)
	{
		resize(is.size());
		for(std::size_t i = 0; i < size(); ++i)
		{
			indices[i] = is[i];			
		}

		updateMaxValue();
	}

	Face<Type> getFace(std::size_t faceIdx) const
	{
		std::size_t start = faceIdx * 3;
		return Face<Type>( indices[start + 0], indices[start + 1], indices[start + 2]);
	}

	FaceRef<Type> getFaceRef(std::size_t faceIdx)
	{
		std::size_t start = faceIdx * 3;
		return FaceRef<Type>( indices[start + 0], indices[start + 1], indices[start + 2]);
	}

	void swapFaces(std::size_t idx0, std::size_t idx1)
	{
		std::size_t s0 = idx0 * 3;
		std::size_t s1 = idx1 * 3;
		std::swap(indices[s0 + 0], indices[s1 + 0]);
		std::swap(indices[s0 + 1], indices[s1 + 1]);
		std::swap(indices[s0 + 2], indices[s1 + 2]);
	}

	void updateMaxValue()
	{
		int maxVal = -1;
		for(std::size_t i = 0; i < size(); ++i)
		{
			int val = indices[i];
			if(val > maxVal) maxVal = val;
		}

		maxIndexValue = maxVal;
	}

	void setMaxValue(int val)
	{
		maxIndexValue = val;
	}

	std::size_t memory() const {return sizeof(Type) * size();}

	void optimize()
	{		 








	}


private:
	AlignedArray<Type> data;
	Type* indices = nullptr;
	int maxIndexValue = -1;
};

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