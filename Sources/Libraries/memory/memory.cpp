#include "memory.h"
#include <stdlib.h>
#include <string.h>

namespace mem
{
	//
	void memset(void* dst, char val, std::size_t len)
	{
		::memset(dst, val, len);
	}

	//
	void memcpy(void* dst, const void* src, std::size_t len)
	{	
		::memcpy(dst, src, len);
	}	

	void mem_desc::copy(const void* src, std::size_t len)
	{
		allocate(*this, len);
		::memcpy(data, src, len);
	}

	void mem_desc::copy(const mem_desc& src)
	{
		copy(src.data, src.len);
	}

	bool mem_desc::includeAddress(void* d) const
	{
		ptrdiff_t idata = (char*)d - (char*) data;
		
		return (idata >= 0) && (idata < len);
	}

	void mem_desc::memset(char val)
	{
		::memset(data, val, len);
	}

	//
	void allocate(mem_desc& md, std::size_t size)
	{
		md.data = md.data ? realloc(md.data, size) : malloc(size);
		md.len = size;
	}

	//
	void deallocate(mem_desc& md)
	{
		md.len = 0;
		free(md.data);
		md.data = nullptr;
	}

}