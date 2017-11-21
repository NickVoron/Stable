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