// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vertexElements/color.h"
#include "vertexElements/blend.h"

#include "vertexElements/position.h"

#include "vertexElements/tangentSpace.h"
#include "vertexElements/textureCoords.h"

#include "Loki/library.include.h"

namespace Geometry
{

template<class VertexType, class T>
struct IsVertexHas
{
	static const bool value = Loki::TL::ExistType<typename VertexType::ComponentsList, T>::value;
};

template<class Tuple, int size>
struct VertexEqualImpl
{
	static bool equal(const Tuple& t1, const Tuple& t2)
	{
		return !t1.field.equal(t2.field) ? false : VertexEqualImpl<typename Tuple::ChildTuple, size - 1>::equal(t1.tuple, t2.tuple);
	}
};

template<class Tuple>
struct VertexEqualImpl<Tuple, 1>
{
	static bool equal(const Tuple& t1, const Tuple& t2)
	{
		return t1.field.equal(t2.field);
	}
};

template<class Tuple> 
struct VertexEqual
{
	static const int tupleSize = Tuple::size;

	static inline bool equal(const Tuple& t1, const Tuple& t2)
	{
		return VertexEqualImpl<Tuple, tupleSize>::equal(t1, t2);
	}
};

#define VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname()																			{ return this->template get<TNAME>(); }\
	inline const TNAME& fname() const																	{ return this->template get<TNAME>(); }


#define VERTEX_COMPONENT_1(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0)															{ return this->template get<TNAME>()(p0); }


#define VERTEX_COMPONENT_2(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1)										{ return this->template get<TNAME>()(p0, p1); }\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ return this->template get<TNAME>()(v); }

#define VERTEX_COMPONENT_3(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1, TNAME::scalar p2)						{ return this->template get<TNAME>()(p0, p1, p2); }\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ return this->template get<TNAME>()(v); }

#define VERTEX_COMPONENT_4(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1, TNAME::scalar p2, TNAME::scalar p3)	{ return this->template get<TNAME>()(p0, p1, p2, p3);}\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ return this->template get<TNAME>()(v); }


#define VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname() { return this->template get<TNAME<usageIndex> >(); } \
	template<int usageIndex> inline const TNAME<usageIndex>& fname() const { return this->template get<TNAME<usageIndex> >(); }

#define VERTEX_UV_COMPONENT_1(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0)	{ return this->template get<TNAME<usageIndex> >()(p0); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ return this->template get<TNAME<usageIndex> >()(v); }
	

#define VERTEX_UV_COMPONENT_2(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1)	{ return this->template get<TNAME<usageIndex> >()(p0, p1); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ return this->template get<TNAME<usageIndex> >()(v); }

#define VERTEX_UV_COMPONENT_3(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1, typename TNAME<usageIndex>::scalar p2)	{ return this->template get<TNAME<usageIndex> >()(p0, p1, p2); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ return this->template get<TNAME<usageIndex> >()(v); }

#define VERTEX_UV_COMPONENT_4(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1, typename TNAME<usageIndex>::scalar p2, typename TNAME<usageIndex>::scalar p3)	{ return this->template get<TNAME<usageIndex> >()(p0, p1, p2, p3);  }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ return this->template get<TNAME<usageIndex> >()(v); }


template<class... T> struct Vertex;


template<class... T>
struct VertexBase
{
template<class... TT> friend struct VertexBase;
public:
	typedef Loki::MakeTypelist<T...> ComponentsList;
	typedef Loki::TypeTuple<ComponentsList> VertexTuple;

	static const int ELEMENTS_COUNT = Loki::TL::Length<ComponentsList>::value;
	
	template<class SelectedList, class Vertex2>
	inline VertexBase& assign(const Vertex2& v2) { Loki::TupleSelectSet<VertexTuple, typename Vertex2::VertexTuple, SelectedList>::exec(data, v2.data); return *this; }

	template<class Vertex2>
	inline VertexBase& maxAssign(const Vertex2& v2) 
	{
		Loki::TupleMaxSet<VertexTuple, typename Vertex2::VertexTuple>::exec(data, v2.data);
		return *this; 
	}

	template<class Vertex2>	inline VertexBase& operator=(const Vertex2& v2) { return maxAssign(v2); }

	inline VertexBase operator+(const VertexBase& v) const	{ VertexBase res = *this; res += v; return res; }
	inline VertexBase operator-(const VertexBase& v) const	{ VertexBase res = *this; res -= v; return res; }
	inline VertexBase operator*(const VertexBase& v) const	{ VertexBase res = *this; res *= v; return res; }
	inline VertexBase operator/(const VertexBase& v) const	{ VertexBase res = *this; res /= v; return res; }

	template<class... TT> inline void operator+=(const VertexBase<TT...>& v) { Loki::TupleMaxAdd<VertexTuple, typename Vertex<TT...>::VertexTuple>::exec(data, v.data); }
	template<class... TT> inline void operator-=(const VertexBase<TT...>& v) { Loki::TupleMaxSub<VertexTuple, typename Vertex<TT...>::VertexTuple>::exec(data, v.data); }
	template<class... TT> inline void operator*=(const VertexBase<TT...>& v) { Loki::TupleMaxMul<VertexTuple, typename Vertex<TT...>::VertexTuple>::exec(data, v.data); }
	template<class... TT> inline void operator/=(const VertexBase<TT...>& v) { Loki::TupleMaxDiv<VertexTuple, typename Vertex<TT...>::VertexTuple>::exec(data, v.data); }


	inline VertexBase operator*(float s) const { VertexBase res = *this; res *= s; return res; }
	inline VertexBase operator/(float s) const { VertexBase res = *this; res /= s; return res; }

	inline void operator*=(float s)	{ Loki::TupleMul<VertexTuple>::exec(data, s);	   }
	inline void operator/=(float s)	{ Loki::TupleMul<VertexTuple>::exec(data, 1.0f / s); }

	inline bool equal(const VertexBase& v) { return VertexEqual<VertexTuple>::equal(data, v.data); }

	
	template<class M> inline		M& get()		{ static_assert( (IsVertexHas<VertexBase, M>::value), "Vertex hasn't element" ); return Loki::FieldGetter< VertexTuple, Loki::TL::IndexOf<ComponentsList, M>::value >::get(data); }
	template<class M> inline const	M& get() const	{ static_assert( (IsVertexHas<VertexBase, M>::value), "Vertex hasn't element" ); return Loki::FieldGetter< VertexTuple, Loki::TL::IndexOf<ComponentsList, M>::value >::get(data);}

	VertexTuple data;
};


template<class... T>
struct Vertex : public VertexBase<T...>
{
public:
	
	VERTEX_COMPONENT_1(POS1, pos1)
	VERTEX_COMPONENT_2(POS2, pos2)
	VERTEX_COMPONENT_3(POS3, pos3)
	VERTEX_COMPONENT_4(POS4, pos4)
	VERTEX_COMPONENT_4(POST, posT)

	
	VERTEX_COMPONENT_3(NORMAL, normal)
	VERTEX_COMPONENT_3(BINORMAL, binormal)
	VERTEX_COMPONENT_3(TANGENT, tangent)

	
	VERTEX_COMPONENT_4(DIFFUSE, diffuse)
	VERTEX_COMPONENT_4(SPECULAR, specular)
	VERTEX_COMPONENT_4(EMISSION, emission)

	
	VERTEX_UV_COMPONENT_4(BLENDWEIGHT, blendweight)
	VERTEX_UV_COMPONENT_4(BLENDINDICES, blendindices)

	
	VERTEX_UV_COMPONENT_1(UV1, uv1)
	VERTEX_UV_COMPONENT_2(UV2, uv2)
	VERTEX_UV_COMPONENT_3(UV3, uv3)
	VERTEX_UV_COMPONENT_4(UV4, uv4)
};

template<class VertexList>	struct VertexListElementsCounter;
template<>					struct VertexListElementsCounter<Loki::NullType> { static const int count = 0; };

template<class VertexList>	struct VertexListElementsCounter
{
	static const int count = VertexList::Head::ELEMENTS_COUNT + VertexListElementsCounter<typename VertexList::Tail>::count;
};

}

















// typename TL::Length<typename TL::Append< TYPELIST_1(VertexDataStorage<TList>), typename InterfacesExtractor< VertexDataStorage<TList>, NullType, TList>::Result >::Result>::value



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