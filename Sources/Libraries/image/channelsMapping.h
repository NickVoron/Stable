// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "type_utils.h"
#include "formats.h"
#include "swizzles.h"

namespace image
{
	template<class T, std::uint8_t numPlanes, class ChSwizzle>
	struct ChannelsMappingBase
	{
		typedef T value_t;
		enum class PlanesCount : std::uint8_t { value = numPlanes	};
		T plane[numPlanes];

		template<class T1, std::uint8_t n1, class S1> void addc (const ChannelsMappingBase<T1, n1, S1>& c)	{ std::uint8_t n = std::min((std::uint8_t)PlanesCount::value, n1); for(std::uint8_t i = 0; i < n; ++i) mappedAdd( plane[ChSwizzle::swizzle(i)], c.plane[S1::swizzle(i)]);  }
		template<class T1, std::uint8_t n1, class S1> void subc (const ChannelsMappingBase<T1, n1, S1>& c)	{ std::uint8_t n = std::min((std::uint8_t)PlanesCount::value, n1); for(std::uint8_t i = 0; i < n; ++i) mappedSub( plane[ChSwizzle::swizzle(i)], c.plane[S1::swizzle(i)]);  }
		template<class T1, std::uint8_t n1, class S1> void mulc (const ChannelsMappingBase<T1, n1, S1>& c)	{ std::uint8_t n = std::min((std::uint8_t)PlanesCount::value, n1); for(std::uint8_t i = 0; i < n; ++i) mappedMul( plane[ChSwizzle::swizzle(i)], c.plane[S1::swizzle(i)]);  }
		template<class T1, std::uint8_t n1, class S1> void divc (const ChannelsMappingBase<T1, n1, S1>& c)	{ std::uint8_t n = std::min((std::uint8_t)PlanesCount::value, n1); for(std::uint8_t i = 0; i < n; ++i) mappedDiv( plane[ChSwizzle::swizzle(i)], c.plane[S1::swizzle(i)]);  }
		template<class T1, std::uint8_t n1, class S1> void setc (const ChannelsMappingBase<T1, n1, S1>& c)	{ std::uint8_t n = std::min((std::uint8_t)PlanesCount::value, n1); for(std::uint8_t i = 0; i < n; ++i) mappedSet( plane[ChSwizzle::swizzle(i)], c.plane[S1::swizzle(i)]);  }

		void sqrtc ()	{ for(std::uint8_t i = 0; i < (std::uint8_t)PlanesCount::value; ++i) mapFromFloat<T>( mapToFloat(plane[i]) );  }

		template<class scalar> void add (scalar s)	{ for(std::uint8_t i = 0; i < (std::uint8_t)PlanesCount::value; ++i) mappedAdd(plane[i], s); }
		template<class scalar> void sub (scalar s)	{ for(std::uint8_t i = 0; i < (std::uint8_t)PlanesCount::value; ++i) mappedSub(plane[i], s); }
		template<class scalar> void mul (scalar s)	{ for(std::uint8_t i = 0; i < (std::uint8_t)PlanesCount::value; ++i) mappedMul(plane[i], s); }
		template<class scalar> void div (scalar s)	{ for(std::uint8_t i = 0; i < (std::uint8_t)PlanesCount::value; ++i) mappedDiv(plane[i], s); }
		
	};

	template<class T, class ChSwizzle, class ColorType>
	struct ChannelsMapping1 : public ChannelsMappingBase<T, 1, ChSwizzle>
	{
		typedef ChannelsMappingBase<T, 1, ChSwizzle> BaseMapping;
		typedef ChannelsMapping1 ChMap;		

		ChannelsMapping1(){}

		template<class T1, std::uint8_t numPlanes, class ChSwizzle1>
		ChannelsMapping1( const ChannelsMappingBase<T1, numPlanes, ChSwizzle1>& c )
		{
			BaseMapping::setc(c);
		}

		ChannelAccessor<T> r()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::r]);	}

		T r() const	{ return BaseMapping::plane[ChSwizzle::r];	}

		template<class T1> T1 r() const	{ return normalizedConvert<T>( r() );	}

		ColorType& saturate_r() { saturate(r()); return (ColorType&)*this; }
	};

	template<class T, class ChSwizzle, class ColorType>
	struct ChannelsMapping2 : public ChannelsMappingBase<T, 2, ChSwizzle>
	{
		typedef ChannelsMappingBase<T, 2, ChSwizzle> BaseMapping;
		typedef ChannelsMapping2 ChMap;		

		ChannelsMapping2(){}

		template<class T1, std::uint8_t numPlanes, class ChSwizzle1>
		ChannelsMapping2( const ChannelsMappingBase<T1, numPlanes, ChSwizzle1>& c )
		{
			BaseMapping::setc(c);
		}

		ChannelAccessor<T> r()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::r]);	}
		ChannelAccessor<T> g()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::g]);	}

		T r() const	{ return BaseMapping::plane[ChSwizzle::r];	}
		T g() const { return BaseMapping::plane[ChSwizzle::g];	}

		template<class T1> T1 r() const	{ return normalizedConvert<T>( r() );	}
		template<class T1> T1 g() const { return normalizedConvert<T>( g() );	}

		ColorType& saturate_r() { saturate(r()); return (ColorType&)*this; }
		ColorType& saturate_g() { saturate(g()); return (ColorType&)*this; }
	};

	template<class T, class ChSwizzle, class ColorType>
	struct ChannelsMapping3 : public ChannelsMappingBase<T, 3, ChSwizzle>
	{
		typedef ChannelsMappingBase<T, 3, ChSwizzle> BaseMapping;	  
		typedef ChannelsMapping3 ChMap;		

		ChannelsMapping3(){}

		template<class T1, std::uint8_t numPlanes, class ChSwizzle1>
		ChannelsMapping3( const ChannelsMappingBase<T1, numPlanes, ChSwizzle1>& c )
		{
			BaseMapping::setc(c);
		}

		ChannelAccessor<T> r()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::r]);	}
		ChannelAccessor<T> g()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::g]);	}
		ChannelAccessor<T> b()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::b]);	}

		T r() const	{ return BaseMapping::plane[ChSwizzle::r];	}
		T g() const { return BaseMapping::plane[ChSwizzle::g];	}
		T b() const { return BaseMapping::plane[ChSwizzle::b];	}

		template<class T1> T1 r() const	{ return normalizedConvert<T>( r() );	}
		template<class T1> T1 g() const { return normalizedConvert<T>( g() );	}
		template<class T1> T1 b() const { return normalizedConvert<T>( b() );	}

		ColorType& saturate_r() { saturate(r()); return (ColorType&)*this; }
		ColorType& saturate_g() { saturate(g()); return (ColorType&)*this; }
		ColorType& saturate_b() { saturate(b()); return (ColorType&)*this; }
	};

	template<class T, class ChSwizzle, class ColorType>
	struct ChannelsMapping4 : public ChannelsMappingBase<T, 4, ChSwizzle>
	{
		typedef ChannelsMappingBase<T, 4, ChSwizzle> BaseMapping;
		typedef ChannelsMapping4 ChMap;		

		ChannelsMapping4(){}

		template<class T1, std::uint8_t numPlanes, class ChSwizzle1>
		ChannelsMapping4( const ChannelsMappingBase<T1, numPlanes, ChSwizzle1>& c )
		{
			BaseMapping::setc(c);
		}

		ChannelAccessor<T> r()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::r]);	}
		ChannelAccessor<T> g()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::g]);	}
		ChannelAccessor<T> b()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::b]);	}
		ChannelAccessor<T> a()	{ return ChannelAccessor<T>(BaseMapping::plane[ChSwizzle::a]);	}

		T r() const	{ return BaseMapping::plane[ChSwizzle::r];	}
		T g() const { return BaseMapping::plane[ChSwizzle::g];	}
		T b() const { return BaseMapping::plane[ChSwizzle::b];	}
		T a() const { return BaseMapping::plane[ChSwizzle::a];	}

		template<class T1> T1 r() const	{ return normalizedConvert<T>( r() );	}
		template<class T1> T1 g() const { return normalizedConvert<T>( g() );	}
		template<class T1> T1 b() const { return normalizedConvert<T>( b() );	}
		template<class T1> T1 a() const { return normalizedConvert<T>( a() );	}

		ColorType& saturate_r() { saturate(r()); return (ColorType&)*this; }
		ColorType& saturate_g() { saturate(g()); return (ColorType&)*this; }
		ColorType& saturate_b() { saturate(b()); return (ColorType&)*this; }
		ColorType& saturate_a() { saturate(a()); return (ColorType&)*this; } 
	};

	template<class T, class ChSwizzle, class ColorType>
	std::ostream& operator<<(std::ostream& os, const ChannelsMapping4<T, ChSwizzle, ColorType>& clr)	
	{ return os << "(" << clr.r() << ", " << clr.g() << ", " << clr.b() << ", " << clr.a() << ")"; }

	template<class T, class ChSwizzle, class ColorType>
	std::ostream& operator<<(std::ostream& os, const ChannelsMapping3<T, ChSwizzle, ColorType>& clr)	
	{ return os << "(" << clr.r() << ", " << clr.g() << ", " << clr.b() << ")"; }

	template<class T, class ChSwizzle, class ColorType>
	std::ostream& operator<<(std::ostream& os, const ChannelsMapping2<T, ChSwizzle, ColorType>& clr)	
	{ return os << "(" << clr.r() << ", " << clr.g() << ")"; }

	template<class T, class ChSwizzle, class ColorType>
	std::ostream& operator<<(std::ostream& os, const ChannelsMapping1<T, ChSwizzle, ColorType>& clr)	
	{ return os << "(" << clr.r() << ")"; }

	template<class T, class ColorType, std::uint8_t channelsCount>
	struct ChannelsMappingSH : public ChannelsMappingBase<T, channelsCount, ChannelsSwizzleDirect>
	{
		typedef ChannelsMappingSH ChMap;		
	};


	template<Format fmt, class ColorType>	struct ChannelsTranslator;

	template<class ColorType> struct ChannelsTranslator<SH4, ColorType> : public ChannelsMappingSH<ChannelsToSwizzle<SH4>::value_t, ColorType, 4>{};
	template<class ColorType> struct ChannelsTranslator<SH9, ColorType> : public ChannelsMappingSH<ChannelsToSwizzle<SH9>::value_t, ColorType, 9>{};
	template<class ColorType> struct ChannelsTranslator<SH16, ColorType> : public ChannelsMappingSH<ChannelsToSwizzle<SH16>::value_t, ColorType, 16>{};
	template<class ColorType> struct ChannelsTranslator<SH25, ColorType> : public ChannelsMappingSH<ChannelsToSwizzle<SH25>::value_t, ColorType, 25>{};
	template<class ColorType> struct ChannelsTranslator<SH36, ColorType> : public ChannelsMappingSH<ChannelsToSwizzle<SH36>::value_t, ColorType, 36>{};



#define DEFINE_CHANNELS_TRANSLATOR_COMMON_PART \
	inline ChannelsTranslator(){} \
	template<Format fmt, class ColorType1> inline ChannelsTranslator(const ChannelsTranslator<fmt, ColorType1>& c):ChannelsTranslator::ChMap(c){} \
	};	

#define DEFINE_CHANNELS_TRANSLATOR1(FORMAT) \
	template<class ColorType>	struct ChannelsTranslator<FORMAT, ColorType> : public ChannelsMapping1<ChannelsToSwizzle<FORMAT>::value_t, ChannelsToSwizzle<FORMAT>::SWIZZLE, ColorType>{ \
	DEFINE_CHANNELS_TRANSLATOR_COMMON_PART

#define DEFINE_CHANNELS_TRANSLATOR2(FORMAT) \
	template<class ColorType>	struct ChannelsTranslator<FORMAT, ColorType> : public ChannelsMapping2<ChannelsToSwizzle<FORMAT>::value_t, ChannelsToSwizzle<FORMAT>::SWIZZLE, ColorType>{ \
	DEFINE_CHANNELS_TRANSLATOR_COMMON_PART

#define DEFINE_CHANNELS_TRANSLATOR3(FORMAT) \
	template<class ColorType>	struct ChannelsTranslator<FORMAT, ColorType> : public ChannelsMapping3<ChannelsToSwizzle<FORMAT>::value_t, ChannelsToSwizzle<FORMAT>::SWIZZLE, ColorType>{ \
	DEFINE_CHANNELS_TRANSLATOR_COMMON_PART

#define DEFINE_CHANNELS_TRANSLATOR4(FORMAT) \
	template<class ColorType>	struct ChannelsTranslator<FORMAT, ColorType> : public ChannelsMapping4<ChannelsToSwizzle<FORMAT>::value_t, ChannelsToSwizzle<FORMAT>::SWIZZLE, ColorType>{ \
	DEFINE_CHANNELS_TRANSLATOR_COMMON_PART

	DEFINE_CHANNELS_TRANSLATOR1(R8);
	DEFINE_CHANNELS_TRANSLATOR1(R16);
	DEFINE_CHANNELS_TRANSLATOR1(R32);
	DEFINE_CHANNELS_TRANSLATOR1(R8S);
	DEFINE_CHANNELS_TRANSLATOR1(R16S);
	DEFINE_CHANNELS_TRANSLATOR1(R32S);
	DEFINE_CHANNELS_TRANSLATOR1(R16F);
	DEFINE_CHANNELS_TRANSLATOR1(R32F);

	DEFINE_CHANNELS_TRANSLATOR2(RG8);
	DEFINE_CHANNELS_TRANSLATOR2(RG16);
	DEFINE_CHANNELS_TRANSLATOR2(RG32);
	DEFINE_CHANNELS_TRANSLATOR2(RG8S);
	DEFINE_CHANNELS_TRANSLATOR2(RG16S);
	DEFINE_CHANNELS_TRANSLATOR2(RG32S);
	DEFINE_CHANNELS_TRANSLATOR2(RG16F);
	DEFINE_CHANNELS_TRANSLATOR2(RG32F);

	DEFINE_CHANNELS_TRANSLATOR2(GR8);
	DEFINE_CHANNELS_TRANSLATOR2(GR16);
	DEFINE_CHANNELS_TRANSLATOR2(GR32);
	DEFINE_CHANNELS_TRANSLATOR2(GR8S);
	DEFINE_CHANNELS_TRANSLATOR2(GR16S);
	DEFINE_CHANNELS_TRANSLATOR2(GR32S);
	DEFINE_CHANNELS_TRANSLATOR2(GR16F);
	DEFINE_CHANNELS_TRANSLATOR2(GR32F);

	DEFINE_CHANNELS_TRANSLATOR3(RGB8);
	DEFINE_CHANNELS_TRANSLATOR3(RGB16);
	DEFINE_CHANNELS_TRANSLATOR3(RGB32);
	DEFINE_CHANNELS_TRANSLATOR3(RGB8S);
	DEFINE_CHANNELS_TRANSLATOR3(RGB16S);
	DEFINE_CHANNELS_TRANSLATOR3(RGB32S);
	DEFINE_CHANNELS_TRANSLATOR3(RGB16F);
	DEFINE_CHANNELS_TRANSLATOR3(RGB32F);

	DEFINE_CHANNELS_TRANSLATOR3(BGR8);
	DEFINE_CHANNELS_TRANSLATOR3(BGR16);
	DEFINE_CHANNELS_TRANSLATOR3(BGR32);
	DEFINE_CHANNELS_TRANSLATOR3(BGR8S);
	DEFINE_CHANNELS_TRANSLATOR3(BGR16S);
	DEFINE_CHANNELS_TRANSLATOR3(BGR32S);
	DEFINE_CHANNELS_TRANSLATOR3(BGR16F);
	DEFINE_CHANNELS_TRANSLATOR3(BGR32F);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA8);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB8);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR8);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA8);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA16);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB16);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR16);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA16);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA32);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB32);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR32);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA32);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA8S);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB8S);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR8S);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA8S);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA16S);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB16S);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR16S);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA16S);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA32S);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB32S);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR32S);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA32S);

	DEFINE_CHANNELS_TRANSLATOR4(RGBA16F);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB16F);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR16F);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA16F);
							  
	DEFINE_CHANNELS_TRANSLATOR4(RGBA32F);
	DEFINE_CHANNELS_TRANSLATOR4(ARGB32F);
	DEFINE_CHANNELS_TRANSLATOR4(ABGR32F);
	DEFINE_CHANNELS_TRANSLATOR4(BGRA32F);
							  
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