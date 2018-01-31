// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "equationsTable.h"

namespace tween
{

	EasingFunc funcs[EASING_EQUATIONS_COUNT] = {  &Linear_easeIn,		
												  &Quad_easeIn,			
												  &Cubic_easeIn, 		
												  &Quart_easeIn, 		
												  &Quint_easeIn, 		
												  &Expo_easeIn, 		
												  &Sine_easeIn, 		
												  &Circ_easeIn, 		
												  &Elastic_easeIn,		
												  &Bounce_easeIn,		
												  &Back_easeIn,			
												  
												  &Linear_easeOut,		
												  &Quad_easeOut,		
												  &Cubic_easeOut, 		
												  &Quart_easeOut, 		
												  &Quint_easeOut, 		
												  &Expo_easeOut, 		
												  &Sine_easeOut, 		
												  &Circ_easeOut, 		
												  &Elastic_easeOut,		
												  &Bounce_easeOut,		
												  &Back_easeOut,		
												  
												  &Linear_easeInOut,	
												  &Quad_easeInOut,		
												  &Cubic_easeInOut, 	
												  &Quart_easeInOut, 	
												  &Quint_easeInOut, 	
												  &Expo_easeInOut, 		
												  &Sine_easeInOut, 		
												  &Circ_easeInOut, 		
												  &Elastic_easeInOut,	
												  &Bounce_easeInOut,	
												  &Back_easeInOut
				};
}



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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