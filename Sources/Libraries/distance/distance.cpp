// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "distance.h"

namespace intersections
{
	float distanceSq(const Segment3& s0, const Segment3& s1, Segment3& res, DistParams<Segment3, Segment3>& params)
	{
		float EPS = 0.00000001f;

		Vector3 delta21 = s0.direction();
		Vector3 delta41 = s1.direction();
		Vector3 delta13 = s0.p0 - s1.p0;

		float a = nm::dot(delta21, delta21);
		float b = nm::dot(delta21, delta41);
		float c = nm::dot(delta41, delta41);
		float d = nm::dot(delta21, delta13);
		float e = nm::dot(delta41, delta13);
		float D = a * c - b * b;

		float sc, sN, sD = D;
		float tc, tN, tD = D;

		if (D < EPS)
		{
			sN = 0.0;
			sD = 1.0;
			tN = e;
			tD = c;
		}
		else
		{
			sN = (b * e - c * d);
			tN = (a * e - b * d);
			if (sN < 0.0)
			{
				sN = 0.0;
				tN = e;
				tD = c;
			}
			else if (sN > sD)
			{
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if (tN < 0.0)
		{
			tN = 0.0;

			if (-d < 0.0)
				sN = 0.0;
			else if (-d > a)
				sN = sD;
			else
			{
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD)
		{
			tN = tD;
			if ((-d + b) < 0.0)
				sN = 0;
			else if ((-d + b) > a)
				sN = sD;
			else
			{
				sN = (-d + b);
				sD = a;
			}
		}

		sc = (fabsf(sN) < EPS) ? 0.0f : sN / sD;
		tc = (fabsf(tN) < EPS) ? 0.0f : tN / tD;

		Vector3 dP = delta13 + (sc * delta21) - (tc * delta41);

		res.p0 = s0.p0 + (sc * delta21);
		res.p1 = s1.p0 + (tc * delta41);
		return nm::magnitudeSq(dP);
	}	 


	float distanceSq( const nm::Vector3& point, const Segment3& segment, Segment3& res, DistParams<Vector3, Segment3>& params)
	{
		nm::Vector3 c = point - segment.p0;	
		nm::Vector3 v = segment.direction(); 
		float d = nm::magnitude(v);	
		v /= d;
		float t = nm::dot(v, c);	

		
		
		res.p0 = point;
		res.p1 = (t <= 0) ? segment.p0 : ( (t >= d) ? segment.p1 : segment.p0 + v * t);

		return nm::magnitudeSq(res.direction());
	}

	float distanceSq( const nm::Vector3& point, const intersections::Triangle& triangle, Segment3& res, DistParams<nm::Vector3, Triangle>&	params)
	{
		nm::Vector3 kDiff	= triangle.vertices[0] - point;
		nm::Vector3 kEdge0	= triangle.vertices[1] - triangle.vertices[0];
		nm::Vector3 kEdge1	= triangle.vertices[2] - triangle.vertices[0];

		float fA00 = nm::magnitudeSq(kEdge0);
		float fA01 = nm::dot(kEdge0, kEdge1);
		float fA11 = nm::magnitudeSq(kEdge1);
		float fB0 = nm::dot(kDiff, kEdge0);
		float fB1 = nm::dot(kDiff, kEdge1);
		float fC = nm::magnitudeSq(kDiff);
		float fDet = fabsf(fA00*fA11-fA01*fA01);
		float fS = fA01*fB1-fA11*fB0;
		float fT = fA01*fB0-fA00*fB1;
		float fSqrDistance;

		if (fS + fT <= fDet)
		{
			if (fS < 0.0f)
			{
				if (fT < 0.0f)  
				{
					if (fB0 < 0.0f)
					{
						fT = 0.0f;
						if (-fB0 >= fA00)
						{
							fS = 1.0f;
							fSqrDistance = fA00+(2.0f)*fB0+fC;
						}
						else
						{
							fS = -fB0/fA00;
							fSqrDistance = fB0*fS+fC;
						}
					}
					else
					{
						fS = 0.0f;
						if (fB1 >= 0.0f)
						{
							fT = 0.0f;
							fSqrDistance = fC;
						}
						else if (-fB1 >= fA11)
						{
							fT = 1.0f;
							fSqrDistance = fA11+(2.0f)*fB1+fC;
						}
						else
						{
							fT = -fB1/fA11;
							fSqrDistance = fB1*fT+fC;
						}
					}
				}
				else  
				{
					fS = 0.0f;
					if (fB1 >= 0.0f)
					{
						fT = 0.0f;
						fSqrDistance = fC;
					}
					else if (-fB1 >= fA11)
					{
						fT = 1.0f;
						fSqrDistance = fA11+(2.0f)*fB1+fC;
					}
					else
					{
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			}
			else if (fT < 0.0f)  
			{
				fT = 0.0f;
				if (fB0 >= 0.0f)
				{
					fS = 0.0f;
					fSqrDistance = fC;
				}
				else if (-fB0 >= fA00)
				{
					fS = 1.0f;
					fSqrDistance = fA00+(2.0f)*fB0+fC;
				}
				else
				{
					fS = -fB0/fA00;
					fSqrDistance = fB0*fS+fC;
				}
			}
			else  
			{
				
				float fInvDet = (1.0f)/fDet;
				fS *= fInvDet;
				fT *= fInvDet;
				fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
			}
		}
		else
		{
			float fTmp0, fTmp1, fNumer, fDenom;

			if (fS < 0.0f)  
			{
				fTmp0 = fA01 + fB0;
				fTmp1 = fA11 + fB1;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-2.0f*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fS = 1.0f;
						fT = 0.0f;
						fSqrDistance = fA00+(2.0f)*fB0+fC;
					}
					else
					{
						fS = fNumer/fDenom;
						fT = 1.0f - fS;
						fSqrDistance = fS*(fA00*fS+fA01*fT+2.0f*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
					}
				}
				else
				{
					fS = 0.0f;
					if (fTmp1 <= 0.0f)
					{
						fT = 1.0f;
						fSqrDistance = fA11+(2.0f)*fB1+fC;
					}
					else if (fB1 >= 0.0f)
					{
						fT = 0.0f;
						fSqrDistance = fC;
					}
					else
					{
						fT = -fB1/fA11;
						fSqrDistance = fB1*fT+fC;
					}
				}
			}
			else if (fT < 0.0f)  
			{
				fTmp0 = fA01 + fB1;
				fTmp1 = fA00 + fB0;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-(2.0f)*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fT = 1.0f;
						fS = 0.0f;
						fSqrDistance = fA11+(2.0f)*fB1+fC;
					}
					else
					{
						fT = fNumer/fDenom;
						fS = 1.0f - fT;
						fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
					}
				}
				else
				{
					fT = 0.0f;
					if (fTmp1 <= 0.0f)
					{
						fS = 1.0f;
						fSqrDistance = fA00+(2.0f)*fB0+fC;
					}
					else if (fB0 >= 0.0f)
					{
						fS = 0.0f;
						fSqrDistance = fC;
					}
					else
					{
						fS = -fB0/fA00;
						fSqrDistance = fB0*fS+fC;
					}
				}
			}
			else  
			{
				fNumer = fA11 + fB1 - fA01 - fB0;
				if (fNumer <= 0.0f)
				{
					fS = 0.0f;
					fT = 1.0f;
					fSqrDistance = fA11+(2.0f)*fB1+fC;
				}
				else
				{
					fDenom = fA00-2.0f*fA01+fA11;
					if (fNumer >= fDenom)
					{
						fS = 1.0f;
						fT = 0.0f;
						fSqrDistance = fA00+(2.0f)*fB0+fC;
					}
					else
					{
						fS = fNumer/fDenom;
						fT = 1.0f - fS;
						fSqrDistance = fS*(fA00*fS+fA01*fT+(2.0f)*fB0) + fT*(fA01*fS+fA11*fT+(2.0f)*fB1)+fC;
					}
				}
			}
		}

		
		if (fSqrDistance < 0.0f)
		{
			fSqrDistance = 0.0f;
		}

		res.p0 = point;
		res.p1 = triangle.vertices[0] + fS*kEdge0 + fT*kEdge1;
		params.info1.barycentric[0] = 1.0f - fS - fT;
		params.info1.barycentric[1] = fS;
		params.info1.barycentric[2] = fT;
		
		
		
		return fSqrDistance;
	};

	static const float ZERO_TOLERANCE = 0.000001f;

	float distanceSq(const Line& line, const Segment3& segment, Segment3& res, DistParams<Line, Segment3>&	params)
	{
		Vector3 extent = segment.direction()*0.5f;
		float extLen = nm::magnitude(extent);
		Vector3 center = segment.p0 + extent;
		Vector3 direction = segment.direction();
		Vector3 lineDirection = line.direction;
		nm::normalize(direction);
		nm::normalize(lineDirection);

		Vector3 diff = line.origin - center;
		float a01 = nm::dot(-lineDirection, direction);
		float b0 = nm::dot(diff, lineDirection);
		float c = nm::magnitudeSq(diff);
		float det = fabsf(1.0f - a01*a01);
		float b1, s0, s1, sqrDist, extDet;

		if (det >= ZERO_TOLERANCE)
		{
			
			b1 = nm::dot(-diff, direction);
			s1 = a01*b0 - b1;
			extDet = extLen*det;

			if (s1 >= -extDet)
			{
				if (s1 <= extDet)
				{
					
					
					float invDet = 1.0f / det;
					s0 = (a01*b1 - b0)*invDet;
					s1 *= invDet;
					sqrDist = s0*(s0 + a01*s1 + 2.0f*b0) +	s1*(a01*s0 + s1 + 2.0f*b1) + c;
				}
				else
				{
					
					
					s1 = extLen;
					s0 = -(a01*s1 + b0);
					sqrDist = -s0*s0 + s1*(s1 + 2.0f*b1) + c;
				}
			}
			else
			{
				
				
				s1 = -extLen;
				s0 = -(a01*s1 + b0);
				sqrDist = -s0*s0 + s1*(s1 + 2.0f*b1) + c;
			}
		}
		else
		{
			
			
			s1 = 0.0f;
			s0 = -b0;
			sqrDist = b0*s0 + c;
		}

		res.p0 = line.origin + s0*lineDirection;
		res.p1 = center + s1*direction;

		params.info0.lineParam = s0;
		params.info1.segmentParam = s1;



		
		return sqrDist > 0 ? sqrDist : 0.0f;
	}

	
	void complementBasis(Vector3& u, Vector3& v, const Vector3& w)
	{
		float invLength;
		
		if (fabsf(w.x) >= fabsf(w.y))
		{
			
			invLength = 1.0f / sqrtf(w.x*w.x + w.z*w.z);
			u.x = -w.z*invLength;
			u.y = 0.0f;
			u.z = +w.x*invLength;
			v.x = w.y*u.z;
			v.y = w.z*u.x - w.x*u.z;
			v.z = -w.y*u.x;
		}
		else
		{
			
			invLength = 1.0f / sqrtf(w.y*w.y + w.z*w.z);
			u.x = 0.0f;
			u.y = +w.z*invLength;
			u.z = -w.y*invLength;
			v.x = w.y*u.z - w.z*u.y;
			v.y = -w.x*u.z;
			v.z = w.x*u.y;
		}
	}


	float distanceSq(const Line& line, const Triangle& triangle, Segment3& res, DistParams<Line, Triangle>&	params)
	{
		
		Vector3 edge0 = triangle.ab();
		Vector3 edge1 = triangle.ac();
		Vector3 normal;
		nm::cross(edge0, edge1, normal);
		nm::normalize(normal);
		Vector3 lineDirection = line.direction;
		nm::normalize(lineDirection);

		float NdD = fabsf( nm::dot(lineDirection, normal) );
		if (NdD > ZERO_TOLERANCE)
		{
			
			
			Vector3 diff = line.origin - triangle.vertices[0];
			Vector3 U, V;
			complementBasis(U, V, lineDirection);
			float UdE0 = nm::dot(U, edge0);
			float UdE1 = nm::dot(U, edge1);
			float UdDiff = nm::dot(U, diff);
			float VdE0 = nm::dot(V, edge0);
			float VdE1 = nm::dot(V, edge1);
			float VdDiff = nm::dot(V, diff);
			float invDet = 1.0f/(UdE0*VdE1 - UdE1*VdE0);

			
			float b1 = (VdE1*UdDiff - UdE1*VdDiff)*invDet;
			float b2 = (UdE0*VdDiff - VdE0*UdDiff)*invDet;
			float b0 = 1.0f - b1 - b2;

			if (b0 >= 0.0f && b1 >= 0.0f && b2 >= 0.0f)
			{
				
				float DdE0 = nm::dot(line.direction, edge0);
				float DdE1 = nm::dot(line.direction, edge1);
				float DdDiff = nm::dot(line.direction, diff);
				float mLineParameter = b1*DdE0 + b2*DdE1 - DdDiff;

				




				res.p0 = line.origin + mLineParameter * lineDirection;

				
				res.p1 = triangle.vertices[0] + b1*edge0 + b2*edge1;

				params.info0.lineParam = mLineParameter;
				params.info1.barycentric[0] = b0;
				params.info1.barycentric[1] = b1;
				params.info1.barycentric[2] = b2;

				return 0.0f;
			}
		}

		
		
		
		
		
		float dist = FLT_MAX;
		for (int i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
		{
			Segment3 segment;
			segment.p0 = triangle.vertices[i0];
			segment.p1 = triangle.vertices[i1];






			DistParams<Line, Segment3> tparams;
			Segment3 lres;
			float distTmp = distanceSq(line, segment, lres, tparams);
			if (distTmp < dist)
			{


				dist = distTmp;
				res = lres;







				float ratio = tparams.info1.segmentParam / ( nm::magnitude(segment.direction())*0.5f );
				params.info0.lineParam = tparams.info0.lineParam;
				params.info1.barycentric[i0] = 0.5f * (1.0f - ratio);
				params.info1.barycentric[i1] = 1.0f - params.info1.barycentric[i0];
				params.info1.barycentric[3 - i0 - i1] = 0.0f;
			}
		}

		return dist;
	}

	float distanceSq(const Segment3& s0, const Triangle& triangle, Segment3& res, DistParams<Segment3, Triangle>& params)
	{
		Line line;
		line.origin = s0.p0 + s0.direction()*0.5f;
		line.direction = s0.direction();
		nm::normalize(line.direction);

		float extent = nm::magnitude(s0.direction()) * 0.5f;
		DistParams<Line, Triangle> ltParams;
		Segment3 ltRes;
		float sqrDist = distanceSq(line, triangle, ltRes, ltParams);
		float mSegmentParameter = ltParams.info0.lineParam;

		if (mSegmentParameter >= -extent)
		{
			if (mSegmentParameter <= extent)
			{
				res = ltRes;
				params.info0.segmentParam = mSegmentParameter;
				params.info1 = ltParams.info1;
				



			}
			else
			{
				
				res.p0 = s0.p1;
				
				Segment3 ptRes;
				DistParams<Vector3, Triangle> ptParams;
				sqrDist = distanceSq(res.p0, triangle, ptRes, ptParams);
				res.p1 = ptRes.p1;
				mSegmentParameter = extent;
				params.info0.segmentParam = mSegmentParameter;
				params.info1 = ptParams.info1;



			}
		}
		else
		{
			
			res.p0 = s0.p1;

			Segment3 ptRes;
			DistParams<Vector3, Triangle> ptParams;
			sqrDist = distanceSq(res.p0, triangle, ptRes, ptParams);
			res.p1 = ptRes.p1;

			
			mSegmentParameter = -extent;
			params.info0.segmentParam = mSegmentParameter;
			params.info1 = ptParams.info1;



		}

		return sqrDist;
	}

	float distanceSq(const Triangle& t0, const Triangle& t1, Segment3& res, DistParams<Triangle, Triangle>& params)
	{
		
		float sqrDist = FLT_MAX; 
		float sqrDistTmp;

		Segment3 edge;
		
		int i0, i1;
		for (i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
		{
			edge.p0 = t0.vertices[i0];
			edge.p1 = t0.vertices[i1];







			Segment3 stRes;
			DistParams<Segment3, Triangle> stParams;
			sqrDistTmp = distanceSq(edge, t1, stRes, stParams);
			if (sqrDistTmp < sqrDist)
			{
				res = stRes;
				sqrDist = sqrDistTmp;


				









				if (sqrDist <= ZERO_TOLERANCE)
				{
					return 0.0f;
				}
			}
		}

		
		for (i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
		{
			edge.p0 = t1.vertices[i0];
			edge.p1 = t1.vertices[i1];





			Segment3 stRes;
			DistParams<Segment3, Triangle> stParams;
			sqrDistTmp = distanceSq(edge, t0, stRes, stParams);
			if (sqrDistTmp < sqrDist)
			{
				res = stRes;
				sqrDist = sqrDistTmp;











				if (sqrDist <= ZERO_TOLERANCE)
				{
					return 0.0f;
				}
			}
		}

		return sqrDist;
	}
}




// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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