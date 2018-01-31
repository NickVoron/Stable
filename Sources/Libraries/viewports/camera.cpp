// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "camera.h"
#include "debugDrawing/library.include.h"
#include "newmath/quaternion.h"

namespace Viewports
{
	nm::matrix4& MatrixPerspectiveFovLH(nm::matrix4& out, float fovy, float aspect, float zn, float zf)
	{
		out.identity();
		out.m[0][0] = 1.0f / (aspect * tan(fovy / 2.0f));
		out.m[1][1] = 1.0f / tan(fovy / 2.0f);
		out.m[2][2] = zf / (zf - zn);
		out.m[2][3] = 1.0f;
		out.m[3][2] = (zf * zn) / (zn - zf);
		out.m[3][3] = 0.0f;
		return out;
	}

	nm::matrix4& MatrixOrthoLH(nm::matrix4& out, float w, float h, float zn, float zf)
	{
		out.identity();
		out.m[0][0] = 2.0f / w;
		out.m[1][1] = 2.0f / h;
		out.m[2][2] = 1.0f / (zf - zn);
		out.m[3][2] = zn / (zn - zf);
		return out;
	}

	nm::matrix4& MatrixAffineTransformation(nm::matrix4& out, float scaling, const nm::Vector3* rotationcenter, const nm::Quaternion* rotation, const nm::Vector3 *translation)
	{
		out.identity();

		if (rotation)
		{
			float temp00, temp01, temp02, temp10, temp11, temp12, temp20, temp21, temp22;

			temp00 = 1.0f - 2.0f * (rotation->v.y * rotation->v.y + rotation->v.z * rotation->v.z);
			temp01 = 2.0f * (rotation->v.x * rotation->v.y + rotation->v.z * rotation->w);
			temp02 = 2.0f * (rotation->v.x * rotation->v.z - rotation->v.y * rotation->w);
			temp10 = 2.0f * (rotation->v.x * rotation->v.y - rotation->v.z * rotation->w);
			temp11 = 1.0f - 2.0f * (rotation->v.x * rotation->v.x + rotation->v.z * rotation->v.z);
			temp12 = 2.0f * (rotation->v.y * rotation->v.z + rotation->v.x * rotation->w);
			temp20 = 2.0f * (rotation->v.x * rotation->v.z + rotation->v.y * rotation->w);
			temp21 = 2.0f * (rotation->v.y * rotation->v.z - rotation->v.x * rotation->w);
			temp22 = 1.0f - 2.0f * (rotation->v.x * rotation->v.x + rotation->v.y * rotation->v.y);

			out.m[0][0] = scaling * temp00;
			out.m[0][1] = scaling * temp01;
			out.m[0][2] = scaling * temp02;
			out.m[1][0] = scaling * temp10;
			out.m[1][1] = scaling * temp11;
			out.m[1][2] = scaling * temp12;
			out.m[2][0] = scaling * temp20;
			out.m[2][1] = scaling * temp21;
			out.m[2][2] = scaling * temp22;

			if (rotationcenter)
			{
				out.m[3][0] = rotationcenter->x * (1.0f - temp00) - rotationcenter->y * temp10 - rotationcenter->z * temp20;
				out.m[3][1] = rotationcenter->y * (1.0f - temp11) - rotationcenter->x * temp01 - rotationcenter->z * temp21;
				out.m[3][2] = rotationcenter->z * (1.0f - temp22) - rotationcenter->x * temp02 - rotationcenter->y * temp12;
			}
		}
		else
		{
			out.m[0][0] = scaling;
			out.m[1][1] = scaling;
			out.m[2][2] = scaling;
		}

		if (translation)
		{
			out.m[3][0] += translation->x;
			out.m[3][1] += translation->y;
			out.m[3][2] += translation->z;
		}

		return out;
	}

	nm::matrix4* MatrixInverse(nm::matrix4& out, float *pdeterminant, const nm::matrix4& pm)
	{
		float det, t[3], v[16];
		unsigned int i, j;

		t[0] = pm.m[2][2] * pm.m[3][3] - pm.m[2][3] * pm.m[3][2];
		t[1] = pm.m[1][2] * pm.m[3][3] - pm.m[1][3] * pm.m[3][2];
		t[2] = pm.m[1][2] * pm.m[2][3] - pm.m[1][3] * pm.m[2][2];
		v[0] = pm.m[1][1] * t[0] - pm.m[2][1] * t[1] + pm.m[3][1] * t[2];
		v[4] = -pm.m[1][0] * t[0] + pm.m[2][0] * t[1] - pm.m[3][0] * t[2];

		t[0] = pm.m[1][0] * pm.m[2][1] - pm.m[2][0] * pm.m[1][1];
		t[1] = pm.m[1][0] * pm.m[3][1] - pm.m[3][0] * pm.m[1][1];
		t[2] = pm.m[2][0] * pm.m[3][1] - pm.m[3][0] * pm.m[2][1];
		v[8] = pm.m[3][3] * t[0] - pm.m[2][3] * t[1] + pm.m[1][3] * t[2];
		v[12] = -pm.m[3][2] * t[0] + pm.m[2][2] * t[1] - pm.m[1][2] * t[2];

		det = pm.m[0][0] * v[0] + pm.m[0][1] * v[4] + pm.m[0][2] * v[8] + pm.m[0][3] * v[12];
		if (det == 0.0f)
			return nullptr;

		if (pdeterminant)
			*pdeterminant = det;

		t[0] = pm.m[2][2] * pm.m[3][3] - pm.m[2][3] * pm.m[3][2];
		t[1] = pm.m[0][2] * pm.m[3][3] - pm.m[0][3] * pm.m[3][2];
		t[2] = pm.m[0][2] * pm.m[2][3] - pm.m[0][3] * pm.m[2][2];
		v[1] = -pm.m[0][1] * t[0] + pm.m[2][1] * t[1] - pm.m[3][1] * t[2];
		v[5] = pm.m[0][0] * t[0] - pm.m[2][0] * t[1] + pm.m[3][0] * t[2];

		t[0] = pm.m[0][0] * pm.m[2][1] - pm.m[2][0] * pm.m[0][1];
		t[1] = pm.m[3][0] * pm.m[0][1] - pm.m[0][0] * pm.m[3][1];
		t[2] = pm.m[2][0] * pm.m[3][1] - pm.m[3][0] * pm.m[2][1];
		v[9] = -pm.m[3][3] * t[0] - pm.m[2][3] * t[1] - pm.m[0][3] * t[2];
		v[13] = pm.m[3][2] * t[0] + pm.m[2][2] * t[1] + pm.m[0][2] * t[2];

		t[0] = pm.m[1][2] * pm.m[3][3] - pm.m[1][3] * pm.m[3][2];
		t[1] = pm.m[0][2] * pm.m[3][3] - pm.m[0][3] * pm.m[3][2];
		t[2] = pm.m[0][2] * pm.m[1][3] - pm.m[0][3] * pm.m[1][2];
		v[2] = pm.m[0][1] * t[0] - pm.m[1][1] * t[1] + pm.m[3][1] * t[2];
		v[6] = -pm.m[0][0] * t[0] + pm.m[1][0] * t[1] - pm.m[3][0] * t[2];

		t[0] = pm.m[0][0] * pm.m[1][1] - pm.m[1][0] * pm.m[0][1];
		t[1] = pm.m[3][0] * pm.m[0][1] - pm.m[0][0] * pm.m[3][1];
		t[2] = pm.m[1][0] * pm.m[3][1] - pm.m[3][0] * pm.m[1][1];
		v[10] = pm.m[3][3] * t[0] + pm.m[1][3] * t[1] + pm.m[0][3] * t[2];
		v[14] = -pm.m[3][2] * t[0] - pm.m[1][2] * t[1] - pm.m[0][2] * t[2];

		t[0] = pm.m[1][2] * pm.m[2][3] - pm.m[1][3] * pm.m[2][2];
		t[1] = pm.m[0][2] * pm.m[2][3] - pm.m[0][3] * pm.m[2][2];
		t[2] = pm.m[0][2] * pm.m[1][3] - pm.m[0][3] * pm.m[1][2];
		v[3] = -pm.m[0][1] * t[0] + pm.m[1][1] * t[1] - pm.m[2][1] * t[2];
		v[7] = pm.m[0][0] * t[0] - pm.m[1][0] * t[1] + pm.m[2][0] * t[2];

		v[11] = -pm.m[0][0] * (pm.m[1][1] * pm.m[2][3] - pm.m[1][3] * pm.m[2][1]) +
			pm.m[1][0] * (pm.m[0][1] * pm.m[2][3] - pm.m[0][3] * pm.m[2][1]) -
			pm.m[2][0] * (pm.m[0][1] * pm.m[1][3] - pm.m[0][3] * pm.m[1][1]);

		v[15] = pm.m[0][0] * (pm.m[1][1] * pm.m[2][2] - pm.m[1][2] * pm.m[2][1]) -
			pm.m[1][0] * (pm.m[0][1] * pm.m[2][2] - pm.m[0][2] * pm.m[2][1]) +
			pm.m[2][0] * (pm.m[0][1] * pm.m[1][2] - pm.m[0][2] * pm.m[1][1]);

		det = 1.0f / det;

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				out.m[i][j] = v[4 * i + j] * det;

		return &out;
	}



	nm::Vector3* Vec3TransformCoord(nm::Vector3 *pout, const nm::Vector3 *pv, const nm::matrix4 *pm)
	{
		nm::Vector3 out;
		
		float norm = 1.0f / (pm->m[0][3] * pv->x + pm->m[1][3] * pv->y + pm->m[2][3] * pv->z + pm->m[3][3]);

		out.x = (pm->m[0][0] * pv->x + pm->m[1][0] * pv->y + pm->m[2][0] * pv->z + pm->m[3][0]) * norm;
		out.y = (pm->m[0][1] * pv->x + pm->m[1][1] * pv->y + pm->m[2][1] * pv->z + pm->m[3][1]) * norm;
		out.z = (pm->m[0][2] * pv->x + pm->m[1][2] * pv->y + pm->m[2][2] * pv->z + pm->m[3][2]) * norm;

		*pout = out;

		return pout;
	}


	void GetMatricesFromCameraParams(const CameraSystem::Params& params, nm::matrix4& projection, nm::matrix4& view, nm::matrix4& invView)
	{
		const CameraSystem::Params::BaseData& data = params.getData();
		const State& s = data.state;

		if (!data.ortho)
		{
			MatrixPerspectiveFovLH(projection, data.fovV, data.aspect, data.planes.minValue, data.planes.maxValue);
		}
		else
		{
			MatrixOrthoLH(projection, 1.0f, 1.0f, data.planes.minValue, data.planes.maxValue);
		}


		
		MatrixAffineTransformation(invView,
			1.0f,	 
			nullptr, 
			(const nm::Quaternion*) (&s.orientation),	
			(const nm::Vector3*) (&s.position) 
		);

		MatrixInverse(view, 0, invView);
	}

	
	
	
	CalculatedCamera::CalculatedCamera() :state(params.getStateRef()), position((nm::Vector3&)params.getStateRef().position)
	{
		calculate();
	}

	CalculatedCamera::CalculatedCamera(const CalculatedCamera& camera) : state(params.getStateRef()), position((nm::Vector3&)params.getStateRef().position)
	{
		operator=(camera);
	}

	CalculatedCamera& CalculatedCamera::operator=(const CalculatedCamera& camera)
	{
		params = camera.params;
		viewportSize = camera.viewportSize;

		calculate();

		return *this;
	}

	void CalculatedCamera::calculate(const CameraSystem::Params& prms)
	{
		params = prms;
		calculate();
	}

	void CalculatedCamera::calculate()
	{
		direction = state.orientation.GetZAxis();

		world.identity();
		GetMatricesFromCameraParams(params, projection, view, invView);
		viewProjection = view * projection;
	}

	bool CalculatedCamera::screenPos(const nm::Vector3& worldCoord, nm::Vector3& screenCoord) const
	{
		Vec3TransformCoord((nm::Vector3*) &screenCoord, (nm::Vector3*) &worldCoord, &viewProjection);
		return screenCoord.z < 1.0f;
	}

	bool CalculatedCamera::screenPos(const nm::Vector3& worldCoord, float& screenX, float& screenY) const
	{
		nm::Vector3 p;
		bool visible = screenPos(worldCoord, p);

		float hw = viewportSize.x  * 0.5f;
		float hh = viewportSize.y * 0.5f;
		screenX = ((+p.x * hw) + hw);
		screenY = ((-p.y * hh) + hh);

		return visible;
	}

	bool CalculatedCamera::screenPos(const nm::Vector3& worldCoord, nm::index2& screenCoord) const
	{
		float x, y;
		bool visible = screenPos(worldCoord, x, y);

		screenCoord.x = (int) x;
		screenCoord.y = (int) y;

		return visible;
	}

	template<class Vector3T>
	inline float WorldRay(float vx, float vy, float nearPlaneDist, const nm::matrix4& invView, Vector3T& pos, Vector3T& dir)
	{
		const nm::matrix4& m = invView;
		
		dir.x = vx*m._11 + vy*m._21 + m._31;
		dir.y = vx*m._12 + vy*m._22 + m._32;
		dir.z = vx*m._13 + vy*m._23 + m._33;
		nm::normalize(dir);

		Vector3T normal;  
		normal.x = m._31;
		normal.y = m._32;
		normal.z = m._33;
		nm::normalize(normal);

		float t = 1.0f / nm::dot(normal, dir);


		pos.x = dir.x*nearPlaneDist*t + m._41;
		pos.y = dir.y*nearPlaneDist*t + m._42;
		pos.z = dir.z*nearPlaneDist*t + m._43;

		return t;
	}

	template<class Vector3T>
	inline void WorldRay(const nm::Vector2& v, float nearPlaneDist, const nm::matrix4& invView, Vector3T& pos, Vector3T& dir)
	{
		WorldRay(v.x, v.y, nearPlaneDist, invView, pos, dir);
	}

	void CalculatedCamera::worldPos(const nm::index2& screenCoord, nm::Vector3& pos) const
	{
		nm::Vector3 dir;
		worldRay(screenCoord, pos, dir);
	}

	void CalculatedCamera::worldPos(const nm::Vector2& screenPos, nm::Vector3& pos) const
	{
		nm::Vector3 dir;
		worldRay(screenPos, pos, dir);
	}

	void CalculatedCamera::worldRay(const nm::index2& screenCoord, nm::Vector3& pos, nm::Vector3& dir) const
	{
		const Vector2 pt((2.0f * screenCoord.x / viewportSize.x - 1.0f), -(2.0f * screenCoord.y / viewportSize.y - 1.0f));
		worldRay(pt, pos, dir);
	}

	void CalculatedCamera::worldRay(const nm::Vector2& screenPos, nm::Vector3& pos, nm::Vector3& dir) const
	{
		float nearDist = -(projection._43 / projection._33);
		nm::Vector2 v(screenPos.x / projection._11, screenPos.y / projection._22);
		WorldRay(v, nearDist, (nm::matrix4&)invView, pos, dir);
	}

	intersections::Ray CalculatedCamera::worldRay(const nm::index2& screenCoord) const
	{
		nm::Vector3 pos;
		nm::Vector3 dir;
		worldRay(screenCoord, pos, dir);

		intersections::Ray res;
		res.origin = pos;
		res.direction = dir;

		return res;
	}

	intersections::Ray CalculatedCamera::worldRay(const nm::Vector2& screenPos) const
	{
		nm::Vector3 pos;
		nm::Vector3 dir;
		worldRay(screenPos, pos, dir);

		intersections::Ray res;
		res.origin = pos;
		res.direction = dir;

		return res;
	}

	void CalculatedCamera::worldFrustum(const unigui::Rect& screenRect, intersections::Frustum& frustum) const
	{
		float invP11 = 1.0f / projection._11;
		float invP22 = 1.0f / projection._22;
		float sx = 2.0f / viewportSize.x;
		float sy = 2.0f / viewportSize.y;

		float fnLeft = +(sx * screenRect.left() - 1.0f) * invP11;
		float fnRight = +(sx * screenRect.right() - 1.0f) * invP11;
		float fnTop = -(sy * screenRect.top() - 1.0f) * invP22;
		float fnBottom = -(sy * screenRect.bottom() - 1.0f) * invP22;

		float nearDist = -(projection._43 / projection._33);
		float farDist = projection._33 * nearDist / (projection._33 - 1.0f);

		Vector3 points[8], dir;
		float t = 0;
		t = WorldRay(fnLeft, fnTop, nearDist, (nm::matrix4&)invView, points[0], dir);		points[4] = points[0] + dir * (farDist - nearDist)*t;
		t = WorldRay(fnRight, fnTop, nearDist, (nm::matrix4&)invView, points[1], dir);		points[5] = points[1] + dir * (farDist - nearDist)*t;
		t = WorldRay(fnRight, fnBottom, nearDist, (nm::matrix4&)invView, points[2], dir);		points[6] = points[2] + dir * (farDist - nearDist)*t;
		t = WorldRay(fnLeft, fnBottom, nearDist, (nm::matrix4&)invView, points[3], dir);		points[7] = points[3] + dir * (farDist - nearDist)*t;

		frustum.planes[0].build(points[0], points[4], points[3]);
		frustum.planes[1].build(points[1], points[2], points[5]);

		frustum.planes[2].build(points[0], points[1], points[5]);
		frustum.planes[3].build(points[3], points[6], points[2]);

		frustum.planes[4].build(points[0], points[2], points[1]);
		frustum.planes[5].build(points[4], points[5], points[6]);

		for (int i = 0; i < 6; ++i)
		{
			frustum.planes[i].normalize();
		}

		
		
		

		
	}

	intersections::Frustum CalculatedCamera::worldFrustum(const unigui::Rect& screenRect) const
	{
		intersections::Frustum frustum;
		worldFrustum(screenRect, frustum);
		return frustum;
	}

	intersections::Frustum CalculatedCamera::worldFrustum(const nm::index2& screenCoord0, const nm::index2& screenCoord1) const
	{
		unigui::Point2 minCorner(std::min(screenCoord0.x, screenCoord1.x), std::min(screenCoord0.y, screenCoord1.y));
		unigui::Point2 maxCorner(std::max(screenCoord0.x, screenCoord1.x), std::max(screenCoord0.y, screenCoord1.y));
		unigui::Rect screenRect(minCorner, maxCorner - minCorner);

		return worldFrustum(screenRect);
	}

	void CalculatedCamera::worldFrustum(intersections::Frustum& frustum) const
	{

		unigui::Rect rect(0, 0, viewportSize.x, viewportSize.y);
		worldFrustum(rect, frustum);

	}

	intersections::Frustum CalculatedCamera::worldFrustum() const
	{
		intersections::Frustum result;
		worldFrustum(result);
		return result;
	}
}




// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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