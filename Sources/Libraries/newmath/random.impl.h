// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "random.h"
#include "operations.h"
#include "vector.h"

namespace nm
{
	
	
	
	
	class Random
	{
	public:
		
		
		
		inline Random();

		
		
		
		inline void seed(unsigned int seed) const;

		
		
		
		inline unsigned int seed() const;

		
		inline operator unsigned int() const;

		
		inline float urnd() const;

		
		inline float srnd() const;

	private:
		#define MATRIX_A 0x9908b0dfUL   
		#define UMASK 0x80000000UL 
		#define LMASK 0x7fffffffUL 
		#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
		#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))

		static const int RANDN = 624;
		static const int RANDM = 397;
		mutable unsigned int state[RANDN];
		mutable int left;
		mutable int initf;
		mutable unsigned int *next;

		inline void next_state() const
		{
			unsigned int* p = (unsigned int*)state;

			
			
			if (initf == 0) seed(5489UL);

			left = RANDN;
			next = (unsigned int*) state;

			int j;
			for (j = RANDN - RANDM + 1; --j; p++)
				*p = p[RANDM] ^ TWIST(p[0], p[1]);

			for (j = RANDM; --j; p++)
				*p = p[RANDM - RANDN] ^ TWIST(p[0], p[1]);

			*p = p[RANDM - RANDN] ^ TWIST(p[0], state[0]);
		}

#undef MATRIX_A
#undef UMASK
#undef LMASK
#undef MIXBITS
#undef TWIST
	};

	
	
	
	inline Random::Random()
	{
		left = 1;
		initf = 0;
		seed(0);
	}

	
	
	
	inline void Random::seed(unsigned int s) const
	{
		state[0] = s & 0xffffffffUL;
		for (int j = 1; j < RANDN; j++)
		{
			state[j] = (1812433253UL * (state[j - 1] ^ (state[j - 1] >> 30)) + j);
			
			
			
			
			state[j] &= 0xffffffffUL;  
		}
		left = 1; initf = 1;

	}

	
	
	
	inline unsigned int Random::seed() const { return *this; }

	
	inline Random::operator unsigned int() const
	{
		if (--left == 0) next_state();
		unsigned int y = *next++;

		
		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);

		return y;
	}

	
	inline float Random::urnd() const
	{
		unsigned int a = static_cast<unsigned int>(*this) >> 5;
		unsigned int b = static_cast<unsigned int>(*this) >> 6;
		return(a*67108864.0f + b)*(1.0f / 9007199254740992.0f);
	}

	
	inline float Random::srnd() const
	{
		return urnd()*2.0f - 1.0f;
	}


	
	
	
	namespace
	{
		static Random random_gen;
	}
	
 	template<> int random<int>() { return (int)(unsigned int)random_gen; }
 	template<> int random<int>(int v1, int v2) 
 	{ 
 		if(v1 < v2) std::swap(v1, v2); 
 		return random<int>() % (v2 - v1) + v1; 
 	}
 
 	template<> float random<float>() { return (float)(unsigned int)random_gen; }
 
 	template<> float random<float>(float v1, float v2) 
 	{ 
 		if(v1 < v2) std::swap(v1, v2); 
 		return random_urnd() * (v2 - v1) + v1; 
 	}
 
 
 	template<class T> T random(const T& v1, const T& v2);

	
	
	
	
	
	
	
	
	Vector3 hemisphereCosineRand(const Vector3& v, float cosPow);
	void hemisphereCosineRandAngles(float& phi, float& theta, float cosPow);

	void hemisphereUniformRandAngles(float& phi, float& theta);
	Vector3 hemisphereUniformRand(const Vector3 &norm);

	void sphereUniformRand(Vector3 &v);

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