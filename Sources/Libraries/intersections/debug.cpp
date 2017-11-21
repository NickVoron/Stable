#include "debug.h"

#include "debugDrawing/library.include.h"
#include "defaultLogs/library.include.h"
#include "newmath/library.include.h"

namespace intersections
{
	namespace debug
	{

		void draw(const DebugOBB& obj)
		{
			DebugDrawing::color(obj.color);
			DebugDrawing::wireBox(obj);
		}

		void draw(const DebugSphere& obj)
		{
			DebugDrawing::color(obj.color);
			DebugDrawing::sphere(obj.center, obj.radius);
		}

		void draw(const DebugTriangle& obj)
		{
			DebugDrawing::color(obj.color);
			DebugDrawing::line(obj.vertices[0], obj.vertices[1]);
			DebugDrawing::line(obj.vertices[1], obj.vertices[2]);
			DebugDrawing::line(obj.vertices[0], obj.vertices[2]);
		}

		void draw( const Segment3& obj )
		{
			DebugDrawing::line(obj.p0, obj.p1);
		}

		void draw(const OBB& obj)		{	draw(DebugOBB(obj));		}
		void draw(const Sphere& obj)	{	draw(DebugSphere(obj));		}
		void draw(const Triangle& obj)	{	draw(DebugTriangle(obj));	}



		void draw(const Capsule3& caps)
		{
			Vector3 p0 = caps.segment.p0;
			Vector3 p1 = caps.segment.p1;

			State state;
			//state.position = p0 + (p1-p0)/2;
			state.position = p1;


			Vector3 axis(1,1,1); 
			state.LookToPoint(p0);

			Quaternion rotX90;
			rotX90.SetXAxis(nm::PI_2);

			state.orientation = state.orientation * rotX90;

			
			
			DebugDrawing::cylinder(state, caps.radius, nm::magnitude(caps.segment.direction()) );

			DebugDrawing::color(Vector3(0,1,1));
			DebugDrawing::sphere(caps.segment.p0, caps.radius);
			DebugDrawing::sphere(caps.segment.p1, caps.radius);

		}


		void draw( const State& state)
		{
			Vector3 xAxis = state.orientation.Transform(Vector3(1, 0, 0));
			Vector3 yAxis = state.orientation.Transform(Vector3(0, 1, 0));
			Vector3 zAxis = state.orientation.Transform(Vector3(0, 0, 1));

			DebugDrawing::color(Vector3(1,1,1));
			DebugDrawing::box(state, Vector3(0.5, 0.5, 0.5));

			DebugDrawing::color(Vector3(1, 0, 0));
			DebugDrawing::line(state.position, state.position + xAxis);

			DebugDrawing::color(Vector3(0, 1, 0));
			DebugDrawing::line(state.position, state.position + yAxis);

			DebugDrawing::color(Vector3(0, 0, 1));
			DebugDrawing::line(state.position, state.position + zAxis);
		}
		

		void draw( const Vector3& point)
		{
			DebugDrawing::sphere(point, 0.1f);
		}

		void draw(const Ray& ray)
		{
			DebugDrawing::line(ray.origin, ray.origin + ray.direction * 1000.0f);
		}

	}
}