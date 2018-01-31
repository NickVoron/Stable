// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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