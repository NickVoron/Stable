#pragma once

#include "libcfg.h"

namespace nm
{
	constexpr float PHI		= 1.61803398874989484820f;
	constexpr float E       = 2.71828182845904523536f;
	constexpr float LOG2E   = 1.44269504088896340736f;
	constexpr float LOG10E  = 0.434294481903251827651f;
	constexpr float LN2     = 0.693147180559945309417f;
	constexpr float LN10    = 2.30258509299404568402f;
	constexpr float PI      = 3.14159265358979323846f;
	constexpr float TwoPI   = 6.28318530717958647692f;
	constexpr float PI_2    = 1.57079632679489661923f;
	constexpr float PI_4    = 0.785398163397448309616f;
	constexpr float PI_180  = 0.017453292519943295769f;
	constexpr float SQRT2   = 1.41421356237309504880f;
	constexpr float SQRT1_2 = 0.707106781186547524401f;

	template<class T> struct one;
	template<> struct one<float>	{ static NM_INLINE float	value()	{ return 1.0f;	}	};
	template<> struct one<double>	{ static NM_INLINE double	value()	{ return 1.0;	}	};
	template<> struct one<int>		{ static NM_INLINE int		value()	{ return 1;		}	};

	template<class T> struct half;
	template<> struct half<float>	{ static NM_INLINE float	value()	{ return 0.5f;	}	};
	template<> struct half<double>	{ static NM_INLINE double	value()	{ return 0.5;	}	};

	template<class T> struct zero;
	template<> struct zero<float>	{ static NM_INLINE float	value()	{ return 0.0f;	}	};
	template<> struct zero<double>	{ static NM_INLINE double	value()	{ return 0.0;	}	};
	template<> struct zero<int>		{ static NM_INLINE int		value()	{ return 0;		}	};



	//���������� ���������� ��������
	enum RotationOrder
	{
		XYZ,
		YZX,
		ZXY,
		XZY,
		YXZ,
		ZYX
	};
}