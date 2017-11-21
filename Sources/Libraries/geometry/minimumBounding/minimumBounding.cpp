#include "minimumBounding.h"

namespace Geometry
{
	// computes the OBB for this set of points relative to this transform matrix.
	void computeOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, const float *matrix)
	{
		INCOMPLETE;
// 		const char *src = (const char *) points;
// 
// 		float bmin[3] = { 1e9, 1e9, 1e9 };
// 		float bmax[3] = { -1e9, -1e9, -1e9 };
// 
// 		for (unsigned int i = 0; i < vcount; i++)
// 		{
// 			const float *p = (const float *) src;
// 			float t[3];
// 
// 			if (t[0] < bmin[0]) bmin[0] = t[0];
// 			if (t[1] < bmin[1]) bmin[1] = t[1];
// 			if (t[2] < bmin[2]) bmin[2] = t[2];
// 
// 			if (t[0] > bmax[0]) bmax[0] = t[0];
// 			if (t[1] > bmax[1]) bmax[1] = t[1];
// 			if (t[2] > bmax[2]) bmax[2] = t[2];
// 
// 			src += pstride;
// 		}
// 
// 
// 		sides[0] = bmax[0];
// 		sides[1] = bmax[1];
// 		sides[2] = bmax[2];
// 
// 		if (fabsf(bmin[0]) > sides[0]) sides[0] = fabsf(bmin[0]);
// 		if (fabsf(bmin[1]) > sides[1]) sides[1] = fabsf(bmin[1]);
// 		if (fabsf(bmin[2]) > sides[2]) sides[2] = fabsf(bmin[2]);
// 
// 		sides[0] *= 2.0f;
// 		sides[1] *= 2.0f;
// 		sides[2] *= 2.0f;

	}

	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *matrix)
	{
		INCOMPLETE;
// 
// 		float bmin[3];
// 		float bmax[3];
// 
// 		float center[3];
// 
// 		center[0] = (bmax[0] - bmin[0])*0.5f + bmin[0];
// 		center[1] = (bmax[1] - bmin[1])*0.5f + bmin[1];
// 		center[2] = (bmax[2] - bmin[2])*0.5f + bmin[2];
// 
// 		float ax = 0;
// 		float ay = 0;
// 		float az = 0;
// 
// 		float sweep = 45.0f; // 180 degree sweep on all three axes.
// 		float steps = 8.0f; // 16 steps on each axis.
// 
// 		float bestVolume = 1e9;
// 		float angle[3] = { 0.f,0.f,0.f };
// 
// 		while (true)
// 		{
// 			bool found = false;
// 
// 			float stepsize = sweep / steps;
// 
// 			for (float x = ax - sweep; x <= ax + sweep; x += stepsize)
// 			{
// 				for (float y = ay - sweep; y <= ay + sweep; y += stepsize)
// 				{
// 					for (float z = az - sweep; z <= az + sweep; z += stepsize)
// 					{
// 						float pmatrix[16];
// 
// 						pmatrix[3 * 4 + 0] = center[0];
// 						pmatrix[3 * 4 + 1] = center[1];
// 						pmatrix[3 * 4 + 2] = center[2];
// 
// 						float psides[3];
// 
// 						computeOBB(vcount, points, pstride, psides, pmatrix);
// 
// 						float volume = psides[0] * psides[1] * psides[2]; // the volume of the cube
// 
// 						if (volume <= bestVolume)
// 						{
// 							bestVolume = volume;
// 
// 							sides[0] = psides[0];
// 							sides[1] = psides[1];
// 							sides[2] = psides[2];
// 
// 							angle[0] = x;
// 							angle[1] = y;
// 							angle[2] = z;
// 
// 							memcpy(matrix, pmatrix, sizeof(float) * 16);
// 							found = true; // yes, we found an improvement.
// 						}
// 					}
// 				}
// 			}
// 
// 			if (found)
// 			{
// 				ax = angle[0];
// 				ay = angle[1];
// 				az = angle[2];
// 
// 				sweep *= 0.5f; // sweep 1/2 the distance as the last time.
// 			}
// 			else
// 			{
// 				break; // no improvement, so just
// 			}
// 		}
// 
// 		float diff[3];
// 		diff[0] = sides[0];
// 		diff[1] = sides[1];
// 		diff[2] = sides[2];
// 
// 		float c[3];
// 		c[0] = 0;
// 		c[1] = 0;
// 		c[2] = 0;
// 
// 		while (diff[0] > 0.001f || diff[1] > 0.001f || diff[2] > 0.001f)
// 		{
// 			bool found = false;
// 
// 			float stepsize[3] = { diff[0] / steps, diff[1] / steps, diff[2] / steps };
// 
// 			for (float x = -diff[0]; x <= +diff[0]; x += stepsize[0])
// 			{
// 				for (float y = -diff[1]; y <= +diff[1]; y += stepsize[1])
// 				{
// 					for (float z = -diff[2]; z <= +diff[2]; z += stepsize[2])
// 					{
// 						float pmatrix[16];
// 
// 						pmatrix[3 * 4 + 0] = center[0] + x;
// 						pmatrix[3 * 4 + 1] = center[1] + y;
// 						pmatrix[3 * 4 + 2] = center[2] + z;
// 
// 						float psides[3];
// 
// 						computeOBB(vcount, points, pstride, psides, pmatrix);
// 
// 						float volume = psides[0] * psides[1] * psides[2]; // the volume of the cube
// 
// 						if (volume <= bestVolume)
// 						{
// 							bestVolume = volume;
// 
// 							sides[0] = psides[0];
// 							sides[1] = psides[1];
// 							sides[2] = psides[2];
// 
// 							memcpy(matrix, pmatrix, sizeof(float) * 16);
// 							found = true; // yes, we found an improvement.
// 						}
// 					}
// 				}
// 			}
// 
// 			diff[0] *= 0.5;
// 			diff[1] *= 0.5;
// 			diff[2] *= 0.5;
// 		}
	}

	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos, float *quat)
	{
		INCOMPLETE;
// 
// 		float matrix[16];
// 
// 		computeBestFitOBB(vcount, points, pstride, sides, matrix);
	}


	void computeBestFitABB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos)
	{
		INCOMPLETE;

// 		float bmin[3];
// 		float bmax[3];
// 
// 		bmin[0] = points[0];
// 		bmin[1] = points[1];
// 		bmin[2] = points[2];
// 
// 		bmax[0] = points[0];
// 		bmax[1] = points[1];
// 		bmax[2] = points[2];
// 
// 		const char *cp = (const char *) points;
// 		for (unsigned int i = 0; i < vcount; i++)
// 		{
// 			const float *p = (const float *) cp;
// 
// 			if (p[0] < bmin[0]) bmin[0] = p[0];
// 			if (p[1] < bmin[1]) bmin[1] = p[1];
// 			if (p[2] < bmin[2]) bmin[2] = p[2];
// 
// 			if (p[0] > bmax[0]) bmax[0] = p[0];
// 			if (p[1] > bmax[1]) bmax[1] = p[1];
// 			if (p[2] > bmax[2]) bmax[2] = p[2];
// 
// 			cp += pstride;
// 		}
// 
// 		sides[0] = bmax[0] - bmin[0];
// 		sides[1] = bmax[1] - bmin[1];
// 		sides[2] = bmax[2] - bmin[2];
// 
// 		pos[0] = bmin[0] + sides[0] * 0.5f;
// 		pos[1] = bmin[1] + sides[1] * 0.5f;
// 		pos[2] = bmin[2] + sides[2] * 0.5f;

	}

}
