// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace Geometry
{

template<class scalar_>
struct VectorDataContainer_1
{
	typedef scalar_ scalar;
	static const int elementsCount = 1;

	union
	{
		struct { scalar x; };
		struct { scalar u; };
		struct { scalar r; };
		struct { scalar data[elementsCount]; };
	};
};

template<class scalar_>
struct VectorDataContainer_2
{
	typedef scalar_ scalar;
	static const int elementsCount = 2;

	union
	{
		struct { scalar x, y; };
		struct { scalar u, v; };
		struct { scalar r, g; };
		struct { scalar data[elementsCount];}; 
	};
};

template<class scalar_>
struct VectorDataContainer_3
{
	typedef scalar_ scalar;
	static const int elementsCount = 3;

	union
	{
		struct { scalar x, y, z; };
		struct { scalar u, v, s; };
		struct { scalar r, g, b; };
		struct { scalar data[elementsCount]; };
	};
};

template<class scalar_>
struct VectorDataContainer_4
{
	typedef scalar_ scalar;
	static const int elementsCount = 4;

	union
	{
		struct { scalar x, y, z, w; };
		struct { scalar u, v, s, q; };
		struct { scalar r, g, b, a; };
		struct { scalar data[elementsCount]; };
	};
};

template<class scalar_, unsigned int N>
struct VectorDataContainer_N
{
	typedef scalar_ scalar;
	static const int elementsCount = N;

	scalar data[elementsCount];
};

template<int count>	
struct dotEval
{
	template<int count>	struct evaluator	{	template<typename scalar> static inline float eval(const scalar* data0, const scalar* data1) { return data0[0]*data1[0] + evaluator<count-1>::eval(&data0[1], &data1[1]);	}	};
	template<>			struct evaluator<1>	{	template<typename scalar> static inline float eval(const scalar* data0, const scalar* data1) { return data0[0]*data1[0]; }	};	

	template<typename scalar> static inline float eval(const scalar* data0, const scalar* data1) { return evaluator<count>::eval(data0, data1);	}
};

template<int count>	
struct magnitudeEval
{
	template<typename scalar> static inline float evalSq(const scalar* data)	{	return dotEval<count>::eval(data, data);	}
	template<typename scalar> static inline float eval(const scalar* data)		{	return sqrtf( evalSq(data) );	}
};

#define DEFINE_MAGNITUDE_OPERATIONS  \
	inline float magnitude()	const	{ return magnitudeEval<elementsCount>::eval(data);		}\
	inline float magnitudeSq()	const	{ return magnitudeEval<elementsCount>::evalSq(data);	}

#define DEFINE_NORMALIZE_OPERATION \
	inline void		normalize()			{ (*this) /= magnitude(); }

#define DEFINE_EQUAL_BY_DIST(VECTOR, DIST_TYPE, EQ_DIST)  \
	static inline DIST_TYPE equalDistance() { return EQ_DIST; } \
	static inline DIST_TYPE equalDistanceSq() { return EQ_DIST * EQ_DIST; } \
	inline bool equal(const VECTOR& v)	const \
		{ return magnitudeEval<elementsCount>::evalSq( (*this - v).data) <= equalDistanceSq(); }

#define DEFINE_EQUAL_BY_DIR(VECTOR, EQ_ANGLE)  \
	static inline float equalCosine() { return cosf(EQ_ANGLE * 3.14159265358979323846f / 180.0f); } \
	inline bool equal(const VECTOR& v)	const \
		{ \
			float mag = magnitudeEval<elementsCount>::evalSq((*this - v).data); \
			float lp = magnitude() * v.magnitude(); \
			return mag / lp <= equalCosine(); \
		}


#define DEFINE_VECTOR_COMMON_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	typedef SCALAR ScalarType; \
	inline VECTOR()										{}\
	inline VECTOR	operator-(const VECTOR& p) const		{ VECTOR res(*this); res -= p; return res;	}\
	inline VECTOR	operator+(const VECTOR& p) const	{ VECTOR res(*this); res += p; return res;	}\
	inline VECTOR	operator/(float d) const			{ VECTOR res(*this); res /= d; return res;	}\
	inline VECTOR	operator*(float d) const			{ VECTOR res(*this); res *= d; return res;	}\
	inline void		operator/=(float d)					{ float invd = 1.0f / d; (*this) *= invd;	}\
	inline SCALAR	operator[](int idx) const			{ return data[idx];}\
	inline SCALAR&	operator[](int idx)					{ return data[idx];}\
	static const char* name()							{ return #VECTOR; }\
	static const char* semanticName()					{ return #SEMANTIC_NAME; }\
	static const wchar_t* semanticNameW()				{ return L#SEMANTIC_NAME; }\
	static const unsigned int semanticIndex()			{ return SEMANTIC_INDEX; }

#define DEFINE_VECTOR_1_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	DEFINE_VECTOR_COMMON_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	template<class VectorType> inline VECTOR(const VectorType& v){x = v.x;}\
	template<class VectorType> inline VECTOR& operator =(const VectorType& v)	{ x = v.x; return *this; }\
	inline VECTOR(SCALAR x_){ x = x_;}\
	inline void operator-=(const VECTOR& p)	{	x -= p.x;	}\
	inline void operator+=(const VECTOR& p)	{	x += p.x;	}\
	inline void operator*=(float d)			{	x *= d;		}\
	inline VECTOR operator-() const			{ return VECTOR(-x);	}\
	inline VECTOR& operator()(SCALAR x_)	{ x = x_; return *this;	}\
	template<class VectorType> inline VECTOR& operator()(const VectorType& v)	{ x = v.x; return *this; }\
	static const int dimensions = 1;

#define DEFINE_VECTOR_2_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	DEFINE_VECTOR_COMMON_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	template<class VectorType>	inline VECTOR(const VectorType& v){x = v.x; y = v.y;}\
	template<class VectorType>	inline VECTOR& operator =(const VectorType& v)	{ x = v.x; y = v.y; return *this; }\
	inline VECTOR(SCALAR x_, SCALAR y_){ x = x_; y = y_;}\
	inline void operator-=(const VECTOR& p)	{	x -= p.x;	y -= p.y;	}\
	inline void operator+=(const VECTOR& p)	{	x += p.x;	y += p.y;	}\
	inline void operator*=(float d)			{	x *= d;		y *= d;		}\
	inline VECTOR operator-() const	{ return VECTOR(-x, -y);	}\
	inline VECTOR& operator()(SCALAR x_, SCALAR y_)	{ x = x_; y = y_; return *this;	}\
	template<class VectorType>	inline VECTOR& operator()(const VectorType& v)	{ x = v.x; y = v.y; return *this; }\
	static const int dimensions = 2;


#define DEFINE_VECTOR_3_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	DEFINE_VECTOR_COMMON_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	template<class VectorType>	inline VECTOR(const VectorType& v){x = v.x; y = v.y; z = v.z;}\
	template<class VectorType>	inline VECTOR& operator =(const VectorType& v)	{ x = v.x; y = v.y; z = v.z; return *this; }\
	inline VECTOR(SCALAR x_, SCALAR y_, SCALAR z_){ x = x_; y = y_; z = z_;}\
	inline void operator-=(const VECTOR& p)	{	x -= p.x;	y -= p.y;	z -= p.z;	}\
	inline void operator+=(const VECTOR& p)	{	x += p.x;	y += p.y;	z += p.z;	}\
	inline void operator*=(float d)			{	x *= d;		y *= d;		z *= d;		}\
	inline VECTOR operator-() const	{ return VECTOR(-x, -y, -z);	}\
	inline VECTOR& operator()(SCALAR x_, SCALAR y_, SCALAR z_)	{ x = x_; y = y_; z = z_; return *this;	}\
	template<class VectorType>	inline VECTOR& operator()(const VectorType& v)	{ x = v.x; y = v.y; z = v.z; return *this; }\
	static const int dimensions = 3;

#define DEFINE_VECTOR_4_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	DEFINE_VECTOR_COMMON_INTERFACE(VECTOR, SCALAR, SEMANTIC_NAME, SEMANTIC_INDEX) \
	template<class VectorType>	inline VECTOR(const VectorType& v){x = v.x; y = v.y; z = v.z; w = v.w;}\
	template<class VectorType>	inline VECTOR& operator =(const VectorType& v)	{ x = v.x; y = v.y; z = v.z; w = v.w; return *this; }\
	inline VECTOR(SCALAR x_, SCALAR y_, SCALAR z_, SCALAR w_){ x = x_; y = y_; z = z_; w = w_;}\
	inline void operator-=(const VECTOR& p)	{	x -= p.x;	y -= p.y;	z -= p.z;	w -= p.w;	}\
	inline void operator+=(const VECTOR& p)	{	x += p.x;	y += p.y;	z += p.z;	w += p.w;	}\
	inline void operator*=(float d)			{	x *= d;		y *= d;		z *= d;		w *= d;		}\
	inline VECTOR operator-() const	{ return VECTOR(-x, -y, -z, -w);	}\
	inline VECTOR& operator()(SCALAR x_, SCALAR y_, SCALAR z_, SCALAR w_)	{ x = x_; y = y_; z = z_; w = w_; return *this;	}\
	template<class VectorType>	inline VECTOR& operator()(const VectorType& v)	{ x = v.x; y = v.y; z = v.z; w = v.w; return *this; }\
	static const int dimensions = 4;

#define VECTOR1_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	struct VCTYPE : public VectorDataContainer_1<SCALAR>{ DEFINE_VECTOR_1_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_NORMALIZE_OPERATION; DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define VECTOR2_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	struct VCTYPE : public VectorDataContainer_2<SCALAR>{ DEFINE_VECTOR_2_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_NORMALIZE_OPERATION; DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define VECTOR3_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	struct VCTYPE : public VectorDataContainer_3<SCALAR>{ DEFINE_VECTOR_3_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_NORMALIZE_OPERATION; DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define VECTOR4_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	struct VCTYPE : public VectorDataContainer_4<SCALAR>{ DEFINE_VECTOR_4_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_NORMALIZE_OPERATION; DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};

#define BASIS_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQ_ANGLE)	struct VCTYPE : public VectorDataContainer_3<SCALAR>{ DEFINE_VECTOR_3_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_NORMALIZE_OPERATION; DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIR(VCTYPE, EQ_ANGLE);};

#define COLOR1_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)		struct VCTYPE : public VectorDataContainer_1<SCALAR>{ DEFINE_VECTOR_1_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_MAGNITUDE_OPERATIONS; };
#define COLOR2_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)		struct VCTYPE : public VectorDataContainer_2<SCALAR>{ DEFINE_VECTOR_2_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_MAGNITUDE_OPERATIONS; };
#define COLOR3_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)		struct VCTYPE : public VectorDataContainer_3<SCALAR>{ DEFINE_VECTOR_3_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_MAGNITUDE_OPERATIONS; };
#define COLOR4_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)		struct VCTYPE : public VectorDataContainer_4<SCALAR>{ DEFINE_VECTOR_4_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, 0); DEFINE_MAGNITUDE_OPERATIONS; };

#define UV1_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	template<int usageIndex_>	struct VCTYPE : public VectorDataContainer_1<SCALAR>{ static const int usageIndex = usageIndex_; DEFINE_VECTOR_1_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, usageIndex); DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define UV2_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	template<int usageIndex_>	struct VCTYPE : public VectorDataContainer_2<SCALAR>{ static const int usageIndex = usageIndex_; DEFINE_VECTOR_2_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, usageIndex); DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define UV3_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	template<int usageIndex_>	struct VCTYPE : public VectorDataContainer_3<SCALAR>{ static const int usageIndex = usageIndex_; DEFINE_VECTOR_3_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, usageIndex); DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};
#define UV4_COMPONENT_TYPE(VCTYPE, SEMANTIC_NAME, SCALAR, EQUAL_DISTANCE)	template<int usageIndex_>	struct VCTYPE : public VectorDataContainer_4<SCALAR>{ static const int usageIndex = usageIndex_; DEFINE_VECTOR_4_INTERFACE(VCTYPE, SCALAR, SEMANTIC_NAME, usageIndex); DEFINE_MAGNITUDE_OPERATIONS; DEFINE_EQUAL_BY_DIST(VCTYPE, SCALAR, EQUAL_DISTANCE);};


template<class VectorType>
inline bool vectorEqual(const VectorType& v1, const VectorType& v2, float thresholdSq = STDMATH_TOLERANCE)
{
	float magSq = (v1-v2).magnitudeSq();
	return thresholdSq >= magSq;
}

}



// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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