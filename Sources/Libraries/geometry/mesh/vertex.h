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
	static const bool value = Loki::TL::ExistType<VertexType::ComponentsList, T>::value;
};

template<class Tuple> 
struct VertexEqual
{
	static const int tupleSize = Tuple::size;

	template<class Tuple, int size> 
	struct Impl
	{
		static inline bool equal(const Tuple& t1, const Tuple& t2)
		{
			return !t1.field.equal(t2.field) ? false : Impl<typename Tuple::ChildTuple, size - 1>::equal(t1.tuple, t2.tuple);			
		}
	};

	template<class Tuple> 
	struct Impl<Tuple, 1>
	{
		static inline bool equal(const Tuple& t1, const Tuple& t2)
		{
			return t1.field.equal(t2.field);
		}
	};

	static inline bool equal(const Tuple& t1, const Tuple& t2)
	{
		return Impl<Tuple, tupleSize>::equal(t1, t2);
	}
};

#define VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname()																			{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>(); }\
	inline const TNAME& fname() const																	{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>(); }


#define VERTEX_COMPONENT_1(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0)															{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>()(p0); }


#define VERTEX_COMPONENT_2(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1)										{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>()(p0, p1); }\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>()(v); }

#define VERTEX_COMPONENT_3(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1, TNAME::scalar p2)						{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>()(p0, p1, p2); }\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ static_assert((IsVertexHas<Vertex, TNAME>::value), #TNAME); return this->get<TNAME>()(v); }

#define VERTEX_COMPONENT_4(TNAME, fname) \
	VERTEX_COMPONENT_COMMON(TNAME, fname) \
	inline 		 TNAME& fname(TNAME::scalar p0, TNAME::scalar p1, TNAME::scalar p2, TNAME::scalar p3)	{ static_assert( ( IsVertexHas<Vertex, TNAME>::value ), #TNAME ); return this->get<TNAME>()(p0, p1, p2, p3);}\
	template<class VectorT> inline TNAME& fname(const VectorT& v)										{ static_assert((IsVertexHas<Vertex, TNAME>::value), #TNAME); return this->get<TNAME>()(v); }


#define VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname() { static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value), #TNAME ); return this->get<TNAME<usageIndex> >(); } \
	template<int usageIndex> inline const TNAME<usageIndex>& fname() const { static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME); return this->get<TNAME<usageIndex> >(); }

#define VERTEX_UV_COMPONENT_1(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(p0); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(v); }
	

#define VERTEX_UV_COMPONENT_2(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(p0, p1); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(v); }

#define VERTEX_UV_COMPONENT_3(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1, typename TNAME<usageIndex>::scalar p2)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(p0, p1, p2); }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(v); }

#define VERTEX_UV_COMPONENT_4(TNAME, fname) \
	VERTEX_UV_COMPONENT_COMMON(TNAME, fname) \
	template<int usageIndex> inline TNAME<usageIndex>& fname(typename TNAME<usageIndex>::scalar p0, typename TNAME<usageIndex>::scalar p1, typename TNAME<usageIndex>::scalar p2, typename TNAME<usageIndex>::scalar p3)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(p0, p1, p2, p3);  }\
	template<int usageIndex, class VectorT> inline TNAME<usageIndex>& fname( const VectorT& v)	{ static_assert( ( IsVertexHas<Vertex, TNAME<usageIndex> >::value ), #TNAME ); return this->get<TNAME<usageIndex> >()(v); }


//
template<class... T>
struct VertexBase
{
template<class... T> friend struct VertexBase;
public:
	typedef Loki::MakeTypelist<T...> ComponentsList;
	typedef Loki::TypeTuple<ComponentsList> VertexTuple;

	static const int ELEMENTS_COUNT = Loki::TL::Length<ComponentsList>::value;
	
	template<class SelectedList, class Vertex2>
	inline VertexBase& assign(const Vertex2& v2) { Loki::TupleSelectSet<VertexTuple, Vertex2::VertexTuple, SelectedList>::exec(data, v2.data); return *this; }

	template<class Vertex2>
	inline VertexBase& maxAssign(const Vertex2& v2) 
	{
		Loki::TupleMaxSet<VertexTuple, Vertex2::VertexTuple>::exec(data, v2.data); 
		return *this; 
	}

	template<class Vertex2>	inline VertexBase& operator=(const Vertex2& v2) { return maxAssign(v2); }

	inline VertexBase operator+(const VertexBase& v) const	{ VertexBase res = *this; res += v; return res; }
	inline VertexBase operator-(const VertexBase& v) const	{ VertexBase res = *this; res -= v; return res; }
	inline VertexBase operator*(const VertexBase& v) const	{ VertexBase res = *this; res *= v; return res; }
	inline VertexBase operator/(const VertexBase& v) const	{ VertexBase res = *this; res /= v; return res; }

	template<class TT> inline void operator+=(const VertexBase<TT>& v) { Loki::TupleMaxAdd<VertexTuple, Vertex<TT>::VertexTuple>::exec(data, v.data); }
	template<class TT> inline void operator-=(const VertexBase<TT>& v) { Loki::TupleMaxSub<VertexTuple, Vertex<TT>::VertexTuple>::exec(data, v.data); }
	template<class TT> inline void operator*=(const VertexBase<TT>& v) { Loki::TupleMaxMul<VertexTuple, Vertex<TT>::VertexTuple>::exec(data, v.data); }
	template<class TT> inline void operator/=(const VertexBase<TT>& v) { Loki::TupleMaxDiv<VertexTuple, Vertex<TT>::VertexTuple>::exec(data, v.data); }


	inline VertexBase operator*(float s) const { VertexBase res = *this; res *= s; return res; }
	inline VertexBase operator/(float s) const { VertexBase res = *this; res /= s; return res; }

	inline void operator*=(float s)	{ Loki::TupleMul<VertexTuple>::exec(data, s);	   }
	inline void operator/=(float s)	{ Loki::TupleMul<VertexTuple>::exec(data, 1.0f / s); }

	inline bool equal(const VertexBase& v) { return VertexEqual<VertexTuple>::equal(data, v.data); }

	//получатель данных по типу												   
	template<class M> inline		M& get()		{ static_assert( (IsVertexHas<VertexBase, M>::value), "Vertex hasn't element" ); return Loki::FieldGetter< VertexTuple, Loki::TL::IndexOf<ComponentsList, M>::value >::get(data); }
	template<class M> inline const	M& get() const	{ static_assert( (IsVertexHas<VertexBase, M>::value), "Vertex hasn't element" ); return Loki::FieldGetter< VertexTuple, Loki::TL::IndexOf<ComponentsList, M>::value >::get(data);}

	VertexTuple data;
};

//
template<class... T>
struct Vertex : public VertexBase<T...>
{
public:
	// позиция
	VERTEX_COMPONENT_1(POS1, pos1)
	VERTEX_COMPONENT_2(POS2, pos2)
	VERTEX_COMPONENT_3(POS3, pos3)
	VERTEX_COMPONENT_4(POS4, pos4)
	VERTEX_COMPONENT_4(POST, posT)

	// tangent space
	VERTEX_COMPONENT_3(NORMAL, normal)
	VERTEX_COMPONENT_3(BINORMAL, binormal)
	VERTEX_COMPONENT_3(TANGENT, tangent)

	// colors
	VERTEX_COMPONENT_4(DIFFUSE, diffuse)
	VERTEX_COMPONENT_4(SPECULAR, specular)
	VERTEX_COMPONENT_4(EMISSION, emission)

	// animation
	VERTEX_UV_COMPONENT_4(BLENDWEIGHT, blendweight)
	VERTEX_UV_COMPONENT_4(BLENDINDICES, blendindices)

	//texture coords
	VERTEX_UV_COMPONENT_1(UV1, uv1)
	VERTEX_UV_COMPONENT_2(UV2, uv2)
	VERTEX_UV_COMPONENT_3(UV3, uv3)
	VERTEX_UV_COMPONENT_4(UV4, uv4)
};

template<class VertexList>	struct VertexListElementsCounter;
template<>					struct VertexListElementsCounter<Loki::NullType> { static const int count = 0; };

template<class VertexList>	struct VertexListElementsCounter
{
	static const int count = VertexList::Head::ELEMENTS_COUNT + VertexListElementsCounter<VertexList::Tail>::count;
};

}















/*





//
template<class T>
struct VertexDataStorage
{
	static const int ELEMENTS_COUNT = TL::Length<T>::value;

	typedef typename TypeTuple<T, TL::Length<T>::value > VertexTuple;
	VertexTuple data;

	//получатель данных по типу
	template<class M>		M& get()		{ return FieldGetter< VertexTuple, TL::IndexOf<T, M>::value >::get(data); }
	template<class M> const M& get() const	{ return FieldGetter< VertexTuple, TL::IndexOf<T, M>::value >::get(data); }
};

																						  
template <class TList, int count> class GenLinearInheritance;

template <class TList, int count> 
class GenLinearInheritance : public TList::Head, public GenLinearInheritance<typename TList::Tail, count - 1>{};

template <class TList> 
class GenLinearInheritance<TList, 1> : public TList::Head {};

template< class Storage, class TListRes, class TList> struct InterfacesExtractor;

template< class Storage, class TListRes, class TList>
struct InterfacesExtractor
{
	typedef typename InterfacesExtractor<Storage, typename TL::Append<TListRes, typename TList::Head::Interface<Storage> >::Result, typename TList::Tail >::Result Result;
};

template< class Storage, class TListRes>
struct InterfacesExtractor<Storage, TListRes, NullType>
{
	typedef TListRes Result;
};


template<class TList>
struct Vertex : public VertexDataStorage<TList>, public GenLinearInheritance< typename InterfacesExtractor< Vertex<TList>, NullType, TList >::Result, TL::Length<TList>::value >
{
};
*/

// typename TL::Length<typename TL::Append< TYPELIST_1(VertexDataStorage<TList>), typename InterfacesExtractor< VertexDataStorage<TList>, NullType, TList>::Result >::Result>::value