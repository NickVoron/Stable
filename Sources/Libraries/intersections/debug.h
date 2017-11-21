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

// 		template<int vcount>
// 		void draw( const DebugPrysm<vcount>& obj )
// 		{
// 			DebugDrawing::color(obj.color);
// 			DebugDrawing::wirePrysm(obj);
// 		}



	}
}