#include "random.h"
#include "operations.h"
#include "vector.h"

namespace nm
{
	// Random - генератор псевдослучайных чисел. На практике показал отличное распределение и большую длинну последовательности.
	// Например, утилита освещения может просчитать 1 миллиард фотонов, причём на каждый фотон генерируется более
	// 20-ти случайных чисел. В таких условиях не было достигнуто зацикливание последовательности, тогда как другой
	// генератор дал в освещении характерные полосы.
	class Random
	{
	public:
		// Генератор инициализируется случайным числом, полученным, к примеру, как время системы.
		// такая мера позволяет не заботиться о псевдослучайности генератора.
		// Генератор, инициализтрованный одним и тем же числом будет выдавать одинаковую последовательность чисел.
		inline Random();

		// Инициализация генератора одним и тем-же числом позволяет получать одинаковую
		// псевдослучайную последовательность.
		// param seed новое значение генератора
		inline void seed(unsigned int seed) const;

		// получить текущее внутреннее значение генератора
		// Это значение может быть установлено новому генератору и он продолжит псевдослучайную последовательность.
		// return текущее значение генератора
		inline unsigned int seed() const;

		//генерирует беззнаковое целое 32 бита
		inline operator unsigned int() const;

		//генерирует беззнаковое случайное число в диапазоне [0..1] включительно
		inline float urnd() const;

		//генерирует знаковое случайное число в диапазоне [-1..1] включительно
		inline float srnd() const;

	private:
		#define MATRIX_A 0x9908b0dfUL   // constant vector a
		#define UMASK 0x80000000UL // most significant w-r bits
		#define LMASK 0x7fffffffUL // least significant r bits
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

			// if setSeed() has not been called, 
			// a default initial seed is used         
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

	//
	//
	//
	inline Random::Random()
	{
		left = 1;
		initf = 0;
		seed(0);
	}

	// Инициализация генератора одним и тем-же числом позволяет получать одинаковую
	// псевдослучайную последовательность.
	// param seed новое значение генератора
	inline void Random::seed(unsigned int s) const
	{
		state[0] = s & 0xffffffffUL;
		for (int j = 1; j < RANDN; j++)
		{
			state[j] = (1812433253UL * (state[j - 1] ^ (state[j - 1] >> 30)) + j);
			// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. 
			// In the previous versions, MSBs of the seed affect   
			// only MSBs of the array state[].                        
			// 2002/01/09 modified by Makoto Matsumoto             
			state[j] &= 0xffffffffUL;  // for >32 bit machines 
		}
		left = 1; initf = 1;

	}

	// получить текущее внутреннее значение генератора
	// Это значение может быть установлено новому генератору и он продолжит псевдослучайную последовательность.
	// return текущее значение генератора
	inline unsigned int Random::seed() const { return *this; }

	//генерирует беззнаковое целое 32 бита
	inline Random::operator unsigned int() const
	{
		if (--left == 0) next_state();
		unsigned int y = *next++;

		// Tempering 
		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);

		return y;
	}

	//генерирует беззнаковое случайное число в диапазоне [0..1] включительно
	inline float Random::urnd() const
	{
		unsigned int a = static_cast<unsigned int>(*this) >> 5;
		unsigned int b = static_cast<unsigned int>(*this) >> 6;
		return(a*67108864.0f + b)*(1.0f / 9007199254740992.0f);
	}

	//генерирует знаковое случайное число в диапазоне [-1..1] включительно
	inline float Random::srnd() const
	{
		return urnd()*2.0f - 1.0f;
	}


	//
	//
	//
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

	//Генерирует отклонённый от первоначального вектор с распределением по 
	//степени косинуса угла, т.е. чем выше показатель степени, тем более узкая
	//диаграмма направленности получится. Значение 1.0f - минимальное значение
	//степени, пр этом генерируются направления равномерно по полусфере.
	//Значения степени более 100 000 даст практически параллельный исходному вектор.
	//param v нормализованный вектор направления
	//param cosPow значение степени при косинусе угла раствора конуса.
	//return новое значение вектора
	Vector3 hemisphereCosineRand(const Vector3& v, float cosPow);
	void hemisphereCosineRandAngles(float& phi, float& theta, float cosPow);

	void hemisphereUniformRandAngles(float& phi, float& theta);
	Vector3 hemisphereUniformRand(const Vector3 &norm);

	void sphereUniformRand(Vector3 &v);

}