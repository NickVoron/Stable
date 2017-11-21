#pragma once
#include "color_ext.h"

namespace imgui
{

#define RGBA_GETALPHA(rgb)      ((rgb) >> 24)
#define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)

	void color(const char* caption, Vector4& c)		{	color_rgba(caption, c.x, c.y, c.z, c.w);	}
	void color(const char* caption, Vector3& c)		{	color_rgb(caption, c.x, c.y, c.z);			}

	void color(Vector4& c)	{	color("", c);	}
	void color(Vector3& c)	{	color("", c);	}

	void color_d3d(const char* caption, D3DXCOLOR& c)
	{
		color_rgba(caption, c.r, c.g, c.b, c.a);
	}

	void color_d3d(const char* caption, D3DCOLOR& c)
	{
		float r = RGBA_GETRED(c)	/ 255.0f;
		float g = RGBA_GETGREEN(c)	/ 255.0f;
		float b = RGBA_GETBLUE(c)	/ 255.0f;
		float a = RGBA_GETALPHA(c)	/ 255.0f;

		color_rgba(caption, r, g, b, a);

		c = D3DCOLOR_COLORVALUE(r, g, b, a); 
	}

	void color_d3d(D3DXCOLOR& c)
	{
		color_d3d("", c);
	}

	void color_d3d(D3DCOLOR& c)
	{
		color_d3d("", c);
	}

}//