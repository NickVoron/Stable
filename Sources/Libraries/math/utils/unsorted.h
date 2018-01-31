// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include <stdlib.h>

#include "../base/Vector2.h"
#include "../base/Vector3.h"
#include "../base/Matrix3.h"

#include "../base/Quaternion.h"


class Quaternion;

namespace Base {
namespace MathUtils {



		inline float GetRandomThetha() { return static_cast<float>( rand() % 16384 ) * 0.00006103288176f; }	

		
		bool GetRandomTrigger( float probablity );

		inline int modulus( int a, int b )
		{
			b = abs(b);
			if ( a >= 0 ) return a % b;

			return (a + (((-a)/b)+1) * b) % b;
		}

		
		inline float GetRandomFromRange( float a, float b )
		{
			float th = GetRandomThetha();
			return a + th * (b-a);
		}

		inline int GetRandomFromRange( int a, int b )
		{
			int diff = b - a + 1;
			return a + (rand( ) % diff);
		}

		
		template<class T> struct Interval;
		template<class T>
		inline T GetRandomFromInterval( const Interval<T>& i)
		{
			return GetRandomFromRange(i.minValue, i.maxValue);
		}

		
		float Egy( float x , float p, float h );

		
		float Remnants( float x, float m );

		
		float Signum( float x );

		
		float SegmentDistanceFlat( const Vector3& sectionStart, const Vector3& sectionEnd, float lengthCashed, const Vector3& point );

		
		float HalfPlane( const Vector2& sectionStart, const Vector2& sectionEnd, const Vector2& point );
		float HalfPlane( const Vector3& sectionStart, const Vector3& sectionEnd, const Vector3& point );

		
		int LogInt2( int x );

		
		template < class T >
		T Zero( const T& dummy );

		
		inline int PowerOfTwo( int power );
		inline int GreaterPowerOf2(int v);
		inline int LowerPowerOf2(int v);
		inline int LogarithmicNearestP2(int v);

		Vector3 Interpolate(const Vector3& v1, const Vector3& v2, float coef);
		Vector2 Interpolate(const Vector2& v1, const Vector2& v2, float coef);

		
		float GetAzimuthNeg( const Vector3& point, const Vector3& targ );
		float GetAzimuthNeg( const Vector2& point, const Vector2& targ );
		float GetVertical( const Vector3& point, const Vector3& targ );

		
		float GetAzimuthPos( const Vector3& point, const Vector3& targ );
		float GetAzimuthPos( const Vector2& point, const Vector2& targ );


		void SplitFloat( float input, float desiredSize, float& result_size, int& splices );

		Vector3 postRotate(Vector3& v, const Quaternion& q);
		Vector3 preRotate(Vector3& v, const Quaternion& q);


		
		inline int PowerOfTwo( int lod )
		{
			static int table[] = {	
				1,
				2,
				4,
				8,
				16,
				32,
				64,
				128,
				256,
				512,
				1024,
				2048,
				4096,
				8192,
				16384,
				32768,
				65536,
				131072,
				262144,
				524288,
				1048576,
				2097152,
				4194304,
				8388608,
				16777216,
				33554432,
				67108864,
				134217728,
				268435456,
				536870912,
				1073741824
			};
			return table[lod];
		}

		inline int GreaterPowerOf2(int v)
		{
			v--;
			v |= v >> 1;
			v |= v >> 2;
			v |= v >> 4;
			v |= v >> 8;
			v |= v >> 16;
			v++;
			return v;
		}

		inline int LowerPowerOf2(int v)
		{
			v |= v >> 1;
			v |= v >> 2;
			v |= v >> 4;
			v |= v >> 8;
			v |= v >> 16;
			v++;
			return v >> 1;
		}

		inline int LogarithmicNearestP2(int v)
		{
			
			
			
			
			return PowerOfTwo(LowerPowerOf2(v));
		}

		template<class V1, class V2> 
		inline void ConvertVectors3(V1 &v1, const V2 &v2)
		{
			v1.x = v2.x;
			v1.y = v2.y;
			v1.z = v2.z;
		}

		float QuaternionHeading(const Quaternion& q);
		float QuaternionPitch(const Quaternion& q);
		float QuaternionBank(const Quaternion& q);

		Vector3 QuaterionToHPB(Quaternion q);
		Quaternion HPBtoQuaternion(Vector3 hpb);

		inline bool CompareFloat(float f1, float f2, float tolerance = 0.0001)
		{		
			return fabsf(f1-f2) < tolerance;
		}

		
		void buildPlaneBasis(const Vector3& n, Vector3& p, Vector3& q);
		Matrix3 changeBasisMatrix(const Vector3& srcNormal, const Vector3& dstNormal);

	} 
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