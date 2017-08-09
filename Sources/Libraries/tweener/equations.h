// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <math.h>

#define TWEENER_INLINE __forceinline

namespace tween
{
	typedef float(*EasingFunc)(float t, float d);

	static float Back_easeIn(float t, float d);
	static float Back_easeOut(float t, float d);
	static float Back_easeInOut(float t, float d);

	static float Bounce_easeIn(float t, float d);
	static float Bounce_easeOut(float t, float d);
	static float Bounce_easeInOut(float t, float d);

	static float Circ_easeIn(float t, float d);
	static float Circ_easeOut(float t, float d);
	static float Circ_easeInOut(float t, float d);

	static float Cubic_easeIn(float t, float d);
	static float Cubic_easeOut(float t, float d);
	static float Cubic_easeInOut(float t, float d);

	static float Elastic_easeIn(float t, float d);
	static float Elastic_easeOut(float t, float d);
	static float Elastic_easeInOut(float t, float d);

	static float Expo_easeIn(float t, float d);
	static float Expo_easeOut(float t, float d);
	static float Expo_easeInOut(float t, float d);

	static float Quad_easeIn(float t, float d);
	static float Quad_easeOut(float t, float d);
	static float Quad_easeInOut(float t, float d);

	static float Quart_easeIn(float t, float d);
	static float Quart_easeOut(float t, float d);
	static float Quart_easeInOut(float t, float d);

	static float Quint_easeIn(float t, float d);
	static float Quint_easeOut(float t, float d);
	static float Quint_easeInOut(float t, float d);

	static float Sine_easeIn(float t, float d);
	static float Sine_easeOut(float t, float d);
	static float Sine_easeInOut(float t, float d);

	static float Linear_easeIn(float t, float d);
	static float Linear_easeOut(float t, float d);
	static float Linear_easeInOut(float t, float d);

	
	
	
	
	
	static const float PI = 3.1415926535897932384626433832795f;

	
	TWEENER_INLINE  float Linear_easeIn(float t, float d)
	{
		return t / d;
	}

	TWEENER_INLINE  float Linear_easeOut(float t, float d)
	{
		return t / d;
	}

	TWEENER_INLINE  float Linear_easeInOut(float t, float d)
	{
		return t / d;
	}

	
	TWEENER_INLINE  float Quad_easeIn(float t, float d)
	{
		t = t / d;
		return t*t;
	}

	TWEENER_INLINE  float Quad_easeOut(float t, float d)
	{
		t = t / d;
		return -t*(t - 2);
	}

	TWEENER_INLINE  float Quad_easeInOut(float t, float d)
	{
		t = (t / d) * 2.0f;
		return (t < 1) ? 0.5f*t*t : -0.5f * ((t - 3)*(t - 1) - 1);
	}

	
	TWEENER_INLINE  float Cubic_easeIn(float t, float d)
	{
		t = t / d;
		return t*t*t;
	}

	TWEENER_INLINE  float Cubic_easeOut(float t, float d)
	{
		t = t / d - 1;
		return t*t*t + 1;
	}

	TWEENER_INLINE  float Cubic_easeInOut(float t, float d)
	{
		t = (t / d) * 2.0f;

		if (t < 1)
		{
			return 0.5f*t*t*t;
		}
		else
		{
			t -= 2.0f;
			return 0.5f*(t*t*t + 2);
		}
	}

	
	TWEENER_INLINE  float Quart_easeIn(float t, float d)
	{
		t = t / d;
		return t*t*t*t;
	}

	TWEENER_INLINE  float Quart_easeOut(float t, float d)
	{
		t = t / d - 1;
		return -(t*t*t*t - 1);
	}

	TWEENER_INLINE  float Quart_easeInOut(float t, float d)
	{
		if ((t /= d / 2) < 1) return 0.5f*t*t*t*t;
		t -= 2.0f;
		return -0.5f * (t*t*t*t - 2);
	}

	

	TWEENER_INLINE  float Quint_easeIn(float t, float d)
	{
		t = t / d;
		return t*t*t*t*t;
	}

	TWEENER_INLINE  float Quint_easeOut(float t, float d)
	{
		t = t / d - 1;
		return (t*t*t*t*t + 1);
	}

	TWEENER_INLINE  float Quint_easeInOut(float t, float d)
	{
		if ((t /= d / 2) < 1) return 0.5f*t*t*t*t*t;
		t -= 2.0f;
		return 0.5f*(t*t*t*t*t + 2);
	}

	

	TWEENER_INLINE  float Expo_easeIn(float t, float d)
	{
		return (t == 0) ? 0 : powf(2, 10 * (t / d - 1));
	}

	TWEENER_INLINE  float Expo_easeOut(float t, float d)
	{
		return (t == d) ? 1.0f : (-powf(2, -10 * t / d) + 1);
	}

	TWEENER_INLINE  float Expo_easeInOut(float t, float d)
	{
		if (t == 0) return 0.0f;
		if (t == d) return 1.0f;
		if ((t /= d / 2) < 1) return 0.5f * powf(2, 10 * (t - 1));
		return 0.5f * (-powf(2, -10 * (t - 1)) + 2);
	}

	
	
	

	

	TWEENER_INLINE  float Sine_easeIn(float t, float d)
	{
		t = t / d;
		return -cosf(t * (PI / 2)) + 1.0f;
	}

	TWEENER_INLINE  float Sine_easeOut(float t, float d)
	{
		t = t / d;
		return sinf(t * (PI / 2));
	}

	TWEENER_INLINE  float Sine_easeInOut(float t, float d)
	{
		t = t / d;
		return -0.5f * (cosf(PI*t) - 1);
	}


	

	TWEENER_INLINE  float Elastic_easeIn(float t, float d)
	{
		if (t == 0) return 0.0f;
		if ((t /= d) == 1) return 1.0f;

		float p = d*.3f;
		float s = p / 4;
		float postFix = powf(2, 10 * (t -= 1)); 
		return -(postFix * sinf((t*d - s)*(2 * PI) / p));
	}

	TWEENER_INLINE  float Elastic_easeOut(float t, float d)
	{
		if (t == 0) return 0.0f;
		if ((t /= d) == 1) return 1.0f;
		float p = d*.3f;
		float s = p / 4;
		return powf(2, -10 * t) * sinf((t*d - s)*(2 * PI) / p) + 1.0f;
	}

	TWEENER_INLINE  float Elastic_easeInOut(float t, float d)
	{
		if (t == 0) return 0.0f;
		if ((t /= d / 2) == 2) return 1.0f;
		float p = d*(.3f*1.5f);
		float s = p / 4;

		if (t < 1)
		{
			float postFix = powf(2, 10 * (t -= 1)); 
			return -0.5f*(postFix* sinf((t*d - s)*(2 * PI) / p));
		}
		float postFix = powf(2, -10 * (t -= 1)); 
		return postFix * sinf((t*d - s)*(2 * PI) / p)*.5f + 1.0f;
	}

	

	TWEENER_INLINE  float Circ_easeIn(float t, float d)
	{
		t /= d;
		return -(sqrtf(1 - t*t) - 1);
	}

	TWEENER_INLINE  float Circ_easeOut(float t, float d)
	{
		return sqrtf(1 - (t = t / d - 1)*t);
	}

	TWEENER_INLINE  float Circ_easeInOut(float t, float d)
	{
		if ((t /= d / 2) < 1) return -0.5f * (sqrtf(1 - t*t) - 1);
		return 0.5f * (sqrtf(1 - t*(t -= 2)) + 1);
	}

	
	TWEENER_INLINE  float Bounce_easeIn(float t, float d)
	{
		return 1.0f - Bounce_easeOut(d - t, d);
	}

	TWEENER_INLINE  float Bounce_easeOut(float t, float d)
	{
		t = t / d;

		if (t < (1.0f / 2.75f))
		{
			return (7.5625f*t*t);
		}
		else if (t < (2.0f / 2.75f))
		{
			t -= (1.5f / 2.75f);
			return (7.5625f*t*t + 0.75f);
		}
		else if (t < (2.5f / 2.75f))
		{
			t -= (2.25f / 2.75f);
			return (7.5625f*t*t + 0.9375f);
		}
		else
		{
			t -= (2.625f / 2.75f);
			return (7.5625f*t*t + 0.984375f);
		}
	}

	TWEENER_INLINE  float Bounce_easeInOut(float t, float d)
	{
		if (t < d / 2) return Bounce_easeIn(t * 2, d) * 0.5f;
		else return Bounce_easeOut(t * 2 - d, d) * 0.5f + 0.5f;
	}

	
	
	
	TWEENER_INLINE  float Back_easeIn(float t, float d)
	{
		t /= d;
		float s = 1.70158f;
		return t*t*((s + 1)*t - s);
	}

	TWEENER_INLINE  float Back_easeOut(float t, float d)
	{
		float s = 1.70158f;
		t = t / d - 1.0f;
		return (t*t*((s + 1)*t + s) + 1);
	}

	TWEENER_INLINE  float Back_easeInOut(float t, float d)
	{
		float s = 1.70158f;
		if ((t /= d / 2) < 1) return 0.5f*(t*t*(((s *= (1.525f)) + 1)*t - s));

		t -= 2;
		return 0.5f*(t*t*(((s *= (1.525f)) + 1)*t + s) + 2);
	}

}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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