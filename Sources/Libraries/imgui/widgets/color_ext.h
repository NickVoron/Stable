#pragma once
#include "color.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "image/library.include.h"

namespace imgui
{

	template<image::Format format>
	void color_rgba(const char* caption, image::Color<format>& c)
	{
		image::Color<image::RGBA32F> oc = c;
		color_rgba(caption, oc.r(), oc.g(), oc.b(), oc.a());
		c = oc;
	}

	template<image::Format format>
	void color_rgba(image::Color<format>& c)
	{
		color_rgba("", c);
	}

	void color_d3d(const char* caption, D3DCOLOR& c);
	void color_d3d(const char* caption, D3DXCOLOR& c);
	void color_d3d(D3DCOLOR& c);
	void color_d3d(D3DXCOLOR& c);

	void color(const char* caption, Vector4& c);
	void color(const char* caption, Vector3& c);
	void color(Vector4& c);
	void color(Vector3& c);

}//