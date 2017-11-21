#pragma once

namespace image
{
	void hsv_to_rgb(float hue, float saturation, float value, float& red, float& green, float& blue);
	void rgb_to_hsv(float red, float green, float blue, float& hue, float& saturation, float& value);

	void hsl_to_rgb(float hue, float saturation, float lightness, float& red, float& green, float& blue);
	void rgb_to_hsl(float red, float green, float blue, float& hue, float& saturation, float& lightness);

	void cmyk_to_rgb(float cyan, float magenta, float yellow, float key, float& red, float& green, float& blue);
	void rgb_to_cmyk(float red, float green, float blue, float& cyan, float& magenta, float& yellow, float& key);

	void yuv_to_rgb(float y, float u, float v, float& red, float& green, float& blue);
	void rgb_to_yuv(float red, float green, float blue, float& y, float& u, float& v);
}