// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "ray2shapes.h"
#include "../debug.h"
#include "debugDrawing/library.include.h"


namespace intersections
{

	int squareEquation(float a, float b, float c, float& x1, float& x2)
	{
		float D = b*b - 4 * a*c;

		if (D < 0.0f)
		{
			return 0;
		}

		if (D == 0.0f)
		{
			x1 = -b / (2.0f * a);
			x2 = x1;
			return 1;
		}

		if (D > 0.0f)
		{
			float inv = 0.5f / a;
			x1 = (-b + sqrtf(D)) * inv;
			x2 = (-b - sqrtf(D)) * inv;

			return 2;
		}

		return 0;
	}

	bool test(const intersections::Ray& ray, const intersections::Sphere& sphere, Vector3& point)
	{
		Vector3 diff = ray.origin - sphere.center;
		float b = 2.0f * nm::dot(diff, ray.direction);
		float c = nm::magnitudeSq(diff) - nm::sqr(sphere.radius);

		float x1, x2;
		int rc = squareEquation(1.0f, b, c, x1, x2);
		if (rc > 0)
		{
			if (x1 < 0.0f && x2 < 0.0f)
			{
				return false;
			}

			float x;
			if (x1 >= 0.0f && x2 >= 0.0f)
			{
				x = std::min(x1, x2);
			}
			else if (x1 >= 0.0f)
			{
				x = x1;
			}
			else
			{
				x = x2;
			}

			point = ray.origin + ray.direction*x;
			return true;
		}

		return false;
	}

	bool test(const Ray& ray, const OBB& box)
	{
		float WdU[3], AWdU[3], DdU[3], ADdU[3], AWxDdU[3], RHS;

		Vector3 diff = ray.origin - box.center;

		WdU[0] = nm::dot(ray.direction, box.axis[0]);
		AWdU[0] = fabs(WdU[0]);
		DdU[0] = nm::dot(diff, box.axis[0]);
		ADdU[0] = fabs(DdU[0]);
		if (ADdU[0] > box.extent[0] && DdU[0] * WdU[0] >= 0) return false;

		WdU[1] = nm::dot(ray.direction, box.axis[1]);
		AWdU[1] = fabs(WdU[1]);
		DdU[1] = nm::dot(diff, box.axis[1]);
		ADdU[1] = fabs(DdU[1]);
		if (ADdU[1] > box.extent[1] && DdU[1] * WdU[1] >= 0) return false;

		WdU[2] = nm::dot(ray.direction, box.axis[2]);
		AWdU[2] = fabs(WdU[2]);
		DdU[2] = nm::dot(diff, box.axis[2]);
		ADdU[2] = fabs(DdU[2]);
		if (ADdU[2] > box.extent[2] && DdU[2] * WdU[2] >= 0) return false;

		Vector3 WxD;
		nm::cross(ray.direction, diff, WxD);

		AWxDdU[0] = fabs(nm::dot(WxD, box.axis[0]));
		RHS = box.extent[1] * AWdU[2] + box.extent[2] * AWdU[1];
		if (AWxDdU[0] > RHS) return false;

		AWxDdU[1] = fabs(nm::dot(WxD, box.axis[1]));
		RHS = box.extent[0] * AWdU[2] + box.extent[2] * AWdU[0];
		if (AWxDdU[1] > RHS) return false;


		AWxDdU[2] = fabs(nm::dot(WxD, box.axis[2]));
		RHS = box.extent[0] * AWdU[1] + box.extent[1] * AWdU[0];
		if (AWxDdU[2] > RHS) return false;

		return true;
	}

	bool test(const Ray& ray, const OBB& bbox, Vector3& point)
	{
		Quad quads[6] =
		{
			bbox.quadXp(),
			bbox.quadXn(),
			bbox.quadYp(),
			bbox.quadYn(),
			bbox.quadZp(),
			bbox.quadZn(),
		};

		float minAlpha = FLT_MAX;
		Vector3 res;
		int minDistIdx = -1;
		for (int i = 0; i < 6; ++i)
		{
			Vector3 pos;
			if (test(ray, quads[i], pos))
			{
				float alpha = nm::magnitudeSq(pos - ray.origin);
				if (minAlpha > alpha)
				{
					res = pos;
					minAlpha = alpha;
					minDistIdx = i;
				}
			}
		}

		bool result = minDistIdx >= 0;
		if (result)
		{
			point = res;
		}

		return result;

	}

	bool test(const Ray& ray, const AABB& bbox, Vector3& point)
	{
		Quad quads[6] =
		{
			bbox.quadXp(),
			bbox.quadXn(),
			bbox.quadYp(),
			bbox.quadYn(),
			bbox.quadZp(),
			bbox.quadZn(),
		};

		float minAlpha = FLT_MAX;
		Vector3 res;
		int minDistIdx = -1;
		for (int i = 0; i < 6; ++i)
		{
			Vector3 pos;
			if (test(ray, quads[i], pos))
			{
				float alpha = nm::magnitudeSq(pos - ray.origin);
				if (minAlpha > alpha)
				{
					res = pos;
					minAlpha = alpha;
					minDistIdx = i;
				}
			}
		}

		bool result = minDistIdx >= 0;
		if (result)
		{
			point = res;
		}

		return result;
	}

	bool test(const Ray& ray, const Sphere& sphere)
	{
		Vector3 diff = ray.origin - sphere.center;
		float a0 = nm::magnitudeSq(diff) - nm::sqr(sphere.radius);
		if (a0 <= 0.0f) return true; 

		float a1 = nm::dot(ray.direction, diff);
		if (a1 >= 0.0f) return false;

		return a1*a1 >= a0;
	}

	bool test(const Ray& ray, const Triangle& trg)
	{
		Segment3 seg;
		seg.p0 = ray.origin;
		seg.p1 = ray.origin + ray.direction*10000.0f;
		return test(seg, trg);
	}

	bool test(const Ray& ray, const Triangle& trg, Vector3& point)
	{
		
		if (test(ray, trg))
		{
			Vector3 c = ray.origin - trg.vertices[0];
			Vector3 ve = ray.direction;

			Vector3 a = trg.vertices[1] - trg.vertices[0];
			Vector3 b = trg.vertices[2] - trg.vertices[0];

			Vector3 pvec;
			nm::cross(ve, b, pvec);
			float det = nm::dot(a, pvec);
			float invdet = 1.0f / det;

			float u = nm::dot(c, pvec)*invdet;
			float v = nm::dot(ve, nm::cross<Vector3>(c, a))*invdet;

			point = trg.vertices[0] + a*u + b*v;

			return true;
		}

		return false;
	}

	bool test(const Ray& ray, const Quad& quad)
	{
		Triangle trg;
		trg.vertices[0] = quad.vertices[0];
		trg.vertices[1] = quad.vertices[1];
		trg.vertices[2] = quad.vertices[2];

		bool res = test(ray, trg);

		
		if (!res)
		{
			trg.vertices[0] = quad.vertices[0];
			trg.vertices[1] = quad.vertices[2];
			trg.vertices[2] = quad.vertices[3];
			
			res = test(ray, trg);
		}

		return res;
	}

	bool test(const Segment3& seg, const Triangle& trg)
	{
		Vector3 end = seg.p1;

		Vector3 e0, e1, e2, norm, point, v, av, vb, vc;
		e0 = trg.vertices[1] - trg.vertices[0];
		e1 = trg.vertices[2] - trg.vertices[0];
		nm::cross(e0, e1, norm);

		float pd = nm::dot(norm, trg.vertices[0]);

		float signSrc = nm::dot(norm, seg.p0) - pd;
		float signDst = nm::dot(norm, end) - pd;

		if (signSrc*signDst > 0.0)
			return false;

		float d = signSrc / (signSrc - signDst);

		point = seg.p0 + d * (end - seg.p0);
		v = point - trg.vertices[0];
		nm::cross(e0, v, av);
		nm::cross(v, e1, vb);

		if (nm::dot(av, vb) >= 0.0)
		{
			e2 = trg.vertices[1] - trg.vertices[2];
			v = point - trg.vertices[1];
			nm::cross(v, e2, vc);
			if (nm::dot(av, vc) >= 0.0)
				return true;
		}

		return false;

	}


	bool test(const Segment3& seg, const Triangle& trg, Vector3& point)
	{
		if (test(seg, trg))
		{
			Vector3 c = seg.p0 - trg.vertices[0];
			Vector3 ve = seg.direction();

			Vector3 a = trg.vertices[1] - trg.vertices[0];
			Vector3 b = trg.vertices[2] - trg.vertices[0];

			Vector3 pvec;
			nm::cross(ve, b, pvec);
			float det = nm::dot(a, pvec);
			float invdet = 1.0f / det;

			float u = nm::dot(c, pvec)*invdet;
			float v = nm::dot(ve, nm::cross<Vector3>(c, a))*invdet;

			point = trg.vertices[0] + a*u + b*v;

			return true;
		}

		return false;
	}

	bool test(const Ray& ray, const Quad& quad, Vector3& point)
	{
		Triangle trg;
		trg.vertices[0] = quad.vertices[0];
		trg.vertices[1] = quad.vertices[1];
		trg.vertices[2] = quad.vertices[2];

		bool res = test(ray, trg, point);

		

		if (!res)
		{
			trg.vertices[0] = quad.vertices[0];
			trg.vertices[1] = quad.vertices[2];
			trg.vertices[2] = quad.vertices[3];
			
			res = test(ray, trg, point);
		}

		return res;
	}

	bool test(const AABB& aabb0, const AABB& aabb1)
	{
		auto& a0 = aabb0.bmin;
		auto& a1 = aabb0.bmax;
		auto& b0 = aabb1.bmin;
		auto& b1 = aabb1.bmax;
		return (a0.x <= b1.x && b0.x <= a1.x) && (a0.y <= b1.y && b0.y <= a1.y) && (a0.z <= b1.z && b0.z <= a1.z);
	}
}



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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