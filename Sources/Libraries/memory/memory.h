#pragma once

#include <cstddef>

namespace mem
{
	struct mem_desc
	{
		mem_desc() : data(nullptr), len(0){}
		mem_desc(void* d, std::size_t l) : data(d), len(l){}

		void copy(const void* src, std::size_t len);
		void copy(const mem_desc& src);
		void memset(char val);
		bool includeAddress(void* d) const;

		void* data;
		std::size_t len;
	};

	void allocate(mem_desc& md, std::size_t size);
	void deallocate(mem_desc& md);

 	void memset(void* dst, char val, std::size_t len);
	void memcpy(void* dst, const void* src, std::size_t len);

	template<class T>
	void memzero(T& dst)
	{
		memset((void*)&dst, 0, sizeof(T));
	}
}