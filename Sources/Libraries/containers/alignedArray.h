#pragma once

template <typename T, unsigned int alignment = 16> 
class AlignedArray
{
public:
	AlignedArray():data(0), sz(0), allocated(0){}
	~AlignedArray(){ clear(); }

	void clear();
	void resize(std::size_t size);
	void reserve(std::size_t size);
	auto size() const { return sz; }
	auto capacity() const { return allocated; }

	T& operator[](std::size_t idx) { return data[idx]; }
	const T& operator[](std::size_t idx) const { return data[idx]; }
	
private:
	T* data;
	std::size_t sz;
	std::size_t allocated;
};

#define NOT_ALIGNED

#ifndef	NOT_ALIGNED
#define al_malloc(size, alignment) _aligned_malloc(size, alignment)
#define al_realloc(base, size, alignment) _aligned_realloc(base, size, alignment)
#define al_free(size) _aligned_free(size)
#else
#define al_malloc(size, alignment) malloc(size)
#define al_realloc(base, size, alignment) realloc(base, size)
#define al_free(size) free(size)
#endif


//
//
//
template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::clear()
{
	if(data)
	{
		al_free(data);
		sz = 0;
		data = 0;
		allocated = 0;
	}
}

template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::resize(std::size_t size)
{
	if(size != sz)
	{
		reserve(size);		
		sz = size;
	}	
}

template <typename T, unsigned int alignment> 
void AlignedArray<T, alignment>::reserve(std::size_t size)
{
	if(size > allocated)
	{
		allocated = size;

		data = (T*)(data ? al_realloc(data, sizeof(T) * allocated, alignment) : al_malloc(sizeof(T) * allocated, alignment));
	}
}
