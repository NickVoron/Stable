// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "equationType.h"
#include "equations.h"

namespace tween
{

	template<EasingEquation equation> struct SelectEquation;

	template<> struct SelectEquation<LINEAR_EASE_IN>		{ static TWEENER_INLINE float ease(float t, float d) { return Linear_easeIn(t, d);		} };
	template<> struct SelectEquation<QUAD_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Quad_easeIn(t, d);			} };
	template<> struct SelectEquation<CUBIC_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Cubic_easeIn(t, d); 		} };
	template<> struct SelectEquation<QUART_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Quart_easeIn(t, d); 		} };
	template<> struct SelectEquation<QUINT_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Quint_easeIn(t, d); 		} };
	template<> struct SelectEquation<EXPO_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Expo_easeIn(t, d); 		} };
	template<> struct SelectEquation<SINE_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Sine_easeIn(t, d); 		} };
	template<> struct SelectEquation<CIRC_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Circ_easeIn(t, d); 		} };
	template<> struct SelectEquation<ELASTIC_EASE_IN>		{ static TWEENER_INLINE float ease(float t, float d) { return Elastic_easeIn(t, d);		} };
	template<> struct SelectEquation<BOUNCE_EASE_IN>		{ static TWEENER_INLINE float ease(float t, float d) { return Bounce_easeIn(t, d);		} };
	template<> struct SelectEquation<BACK_EASE_IN>			{ static TWEENER_INLINE float ease(float t, float d) { return Back_easeIn(t, d);			} };

	template<> struct SelectEquation<LINEAR_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Linear_easeOut(t, d);		} };
	template<> struct SelectEquation<QUAD_EASE_OUT>			{ static TWEENER_INLINE float ease(float t, float d) { return Quad_easeOut(t, d);		} };
	template<> struct SelectEquation<CUBIC_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Cubic_easeOut(t, d); 		} };
	template<> struct SelectEquation<QUART_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Quart_easeOut(t, d); 		} };
	template<> struct SelectEquation<QUINT_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Quint_easeOut(t, d); 		} };
	template<> struct SelectEquation<EXPO_EASE_OUT>			{ static TWEENER_INLINE float ease(float t, float d) { return Expo_easeOut(t, d); 		} };
	template<> struct SelectEquation<SINE_EASE_OUT>			{ static TWEENER_INLINE float ease(float t, float d) { return Sine_easeOut(t, d); 		} };
	template<> struct SelectEquation<CIRC_EASE_OUT>			{ static TWEENER_INLINE float ease(float t, float d) { return Circ_easeOut(t, d); 		} };
	template<> struct SelectEquation<ELASTIC_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Elastic_easeOut(t, d);		} };
	template<> struct SelectEquation<BOUNCE_EASE_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Bounce_easeOut(t, d);		} };
	template<> struct SelectEquation<BACK_EASE_OUT>			{ static TWEENER_INLINE float ease(float t, float d) { return Back_easeOut(t, d);		} };

	template<> struct SelectEquation<LINEAR_EASE_IN_OUT>	{ static TWEENER_INLINE float ease(float t, float d) { return Linear_easeInOut(t, d);	} };
	template<> struct SelectEquation<QUAD_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Quad_easeInOut(t, d);		} };
	template<> struct SelectEquation<CUBIC_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Cubic_easeInOut(t, d); 	} };
	template<> struct SelectEquation<QUART_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Quart_easeInOut(t, d); 	} };
	template<> struct SelectEquation<QUINT_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Quint_easeInOut(t, d); 	} };
	template<> struct SelectEquation<EXPO_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Expo_easeInOut(t, d); 		} };
	template<> struct SelectEquation<SINE_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Sine_easeInOut(t, d); 		} };
	template<> struct SelectEquation<CIRC_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Circ_easeInOut(t, d); 		} };
	template<> struct SelectEquation<ELASTIC_EASE_IN_OUT>	{ static TWEENER_INLINE float ease(float t, float d) { return Elastic_easeInOut(t, d);	} };
	template<> struct SelectEquation<BOUNCE_EASE_IN_OUT>	{ static TWEENER_INLINE float ease(float t, float d) { return Bounce_easeInOut(t, d);	} };
	template<> struct SelectEquation<BACK_EASE_IN_OUT>		{ static TWEENER_INLINE float ease(float t, float d) { return Back_easeInOut(t, d);		} };

}



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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