
#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1 
#define ENABLE_OPENGL 1
#define ENABLE_OPENGL_ES 0

#ifdef _DEBUG
						  
#else	  

	// no use DEBUG STL version
	#ifdef _SECURE_SCL
	#undef _SECURE_SCL
	#endif

	#ifdef _HAS_ITERATOR_DEBUGGING
	#undef _HAS_ITERATOR_DEBUGGING
	#endif

	#define _SECURE_SCL 0
	#define _HAS_ITERATOR_DEBUGGING 0
	#define DIRECTINPUT_VERSION 0x0800

#endif



