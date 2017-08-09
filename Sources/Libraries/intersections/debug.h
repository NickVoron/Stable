// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "shapes/shapes.h"
#include "shapes/obb.h"
#include "math/library.include.h"
#include "algorithm/ray2shapes.h"

namespace intersections
{
	namespace debug
	{
		template<class BV>
		struct BoundVolume : public BV
		{
			typedef BV BaseType;
			typedef BoundVolume Parent;

			BoundVolume(){mark(false);};
			BoundVolume(const BV& bv):BV(bv){mark(false);}

			bool mark(bool f)	{ color = f ? Vector3( 0.4f, 1.0f, 0.5f ) : Vector3( 1.0f, 0.0f, 0.2f ); return f; }
						
			Vector3 color;
		};

		class DebugOBB : public BoundVolume<OBB> 
		{
		public:
			DebugOBB(){};
			DebugOBB(const OBB& obj):BoundVolume<OBB>(obj){};
		}; 

		class DebugSphere : public BoundVolume<Sphere> 
		{
		public:
			DebugSphere(){};
			DebugSphere(const Sphere& obj):BoundVolume<Sphere>(obj){};
		}; 

		class DebugTriangle : public BoundVolume<Triangle> 
		{
		public:
			DebugTriangle(){};
			DebugTriangle(const Triangle& obj):BoundVolume<Triangle>(obj){};
		}; 

		template<int vcount>
		class DebugPrysm : public BoundVolume< Prysm<vcount> > 
		{
		public:
			DebugPrysm(){};
			DebugPrysm(const Prysm<vcount>& obj):BoundVolume< Prysm<vcount> >(obj){};
		}; 

		template<class BV>
		bool test( const Ray& ray, BoundVolume<BV>& obj )
		{
			return obj.mark( intersections::test(ray, obj) );
		}				

		void draw( const OBB& obj );
		void draw( const Sphere& obj );
		void draw( const Triangle& obj );
		void draw( const Segment3& obj );

		void draw( const DebugOBB& obj );
		void draw( const DebugSphere& obj );
		void draw( const DebugTriangle& obj );
		void draw( const Capsule3& caps);

		void draw( const State& state);

		template<int vcount>
		void draw( const Prysm<vcount>& obj )
		{
			draw(DebugPrysm<vcount>(obj));
		}

		void draw( const Vector3& point);
		void draw(const Ray& ray);










	}
}



// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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