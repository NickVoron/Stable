// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "config.h"

#include <type_traits>
											  
inline void* cm_malloc(std::size_t len) { return malloc(len); }
inline void* cm_realloc(void* data, std::size_t len) { return realloc(data, len); }
inline void cm_free(void* data) { free(data); }

inline ptrdiff_t pointers_distance(void* p1, void* p2)
{
	return (char*)p1 - (char*)p2;
}

template<class T>
class cm_array;

template<class T0, bool isPod> struct destructor_caller_impl;
template<class T0> struct destructor_caller_impl<T0, false> { static inline void call(T0* data, std::size_t count) { for (std::size_t  i = 0; i < count; ++i)	data[i].~T0(); } };
template<class T0> struct destructor_caller_impl<T0, true> { static inline void call(T0* data, std::size_t count) {} };

template<class T0, bool isPod> struct constructor_caller_impl;
template<class T0> struct constructor_caller_impl<T0, false> { static inline void call(T0* data, std::size_t count) { for (std::size_t  i = 0; i < count; ++i) new (&data[i]) T0(); } };
template<class T0> struct constructor_caller_impl<T0, true> { static inline void call(T0* data, std::size_t count) {} };

template<class T0, bool isPod> struct load_caller_impl;

template<class T0> struct load_caller_impl<T0, false>
{
	static inline void call(cm_array<T0>& a, stream::istream& is)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
			a[i].load(is);
	}
};

template<class T0> struct load_caller_impl<T0, true>
{
	static inline void call(cm_array<T0>& a, stream::istream& is)
	{
		is.read(a.data, a.size() * sizeof(T0));
	}
};


template<class T0, bool isPod> struct save_caller_impl;

template<class T0> struct save_caller_impl<T0, false>
{
	static inline void call(const cm_array<T0>& a, stream::ostream& os)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
			a[i].save(os);
	}
};

template<class T0> struct save_caller_impl<T0, true>
{
	static inline void call(const cm_array<T0>& a, stream::ostream& os)
	{
		os.write(a.data, a.size() * sizeof(T0));
	}
};


template<class T0, bool isPod> struct assign_caller_impl;

template<class T0> struct assign_caller_impl<T0, false>
{
	static inline decltype(auto) call(cm_array<T0>& d, const cm_array<T0>& a)
	{
		for (std::size_t i = 0; i < a.size(); ++i)
			d[i] = a[i];

		return d;
	}
};

template<class T0> struct assign_caller_impl<T0, true>
{
	static inline decltype(auto) call(cm_array<T0>& d, const cm_array<T0>& a)
	{
		mem::memcpy(d.data, a.data, a.size() * sizeof(T0));
		return d;
	}
};

template<class T>
class cm_array
{
template<class T0, bool isPod> friend struct load_caller_impl;
template<class T0, bool isPod> friend struct save_caller_impl;
private:
	
	template<class T0>
	struct destructor_caller
	{		
		static inline void call(T0* data, std::size_t count)	{ destructor_caller_impl<T0, std::is_pod<T0>::value>::call(data, count); }
	};

	template<class T0>
	struct constructor_caller
	{
		static inline void call(T0* data, std::size_t count)	{ constructor_caller_impl<T0, std::is_pod<T0>::value>::call(data, count); }
	};

	template<class T0>
	struct load_caller
	{
		static inline void call(cm_array<T0>& a, stream::istream& is)
		{
			std::size_t count; is >> count;
			if(count > 0)
			{
				a.init(count);
				load_caller_impl<T0, std::is_pod<T0>::value>::call(a, is);
			}			
		}
	};

	template<class T0>
	struct save_caller
	{
		static inline void call(const cm_array<T0>& a, stream::ostream& os)
		{
			os << a.size();
			save_caller_impl<T0, std::is_pod<T0>::value>::call(a, os);
		}
	};

	template<class T0>
	struct assign_caller
	{
		static inline decltype(auto) call(cm_array<T0>& d, const cm_array<T0>& a)
		{
			d.init(a.size());
			return assign_caller_impl<T0, std::is_pod<T0>::value>::call(d, a);
		}
	};

public:	
	cm_array(){}
	cm_array(const cm_array& a) { operator=(a); }
	~cm_array(){destroy();}

	void init(std::size_t sz)
	{
		destroy();
		if (sz > 0)
		{
			CM_KERNEL_ENFORCE(!data && count == 0);
			data = (T*) cm_malloc(sz * sizeof(T));
			constructor_caller<T>::call(data, sz);
			count = sz;
		}			   		
	}

	void destroy()
	{
		if(data && count > 0)
		{
			destructor_caller<T>::call(data, count);
			cm_free(data);
			count = 0;
			data = 0;
		}		
	}

	T& operator[](std::size_t index)
	{
		CM_KERNEL_ENFORCE(index >= 0 && index < count);
		return data[index];
	}

	const T& operator[](std::size_t index) const
	{
		CM_KERNEL_ENFORCE(index >= 0 && index < count);
		return data[index];
	}

	cm_array& operator=(const cm_array& arr)
	{	  
		return assign_caller<T>::call(*this, arr);
	}

	std::size_t size() const { return count; }
	void zero() { mem::memset(data, 0, count * sizeof(T)); }

	void save(stream::ostream& os) const
	{
		save_caller<T>::call(*this, os);
	}

	void load(stream::istream& is)
	{
		destroy();
		load_caller<T>::call(*this, is);
	}

	T* begin() { return &data[0]; }
	T* end() { return &data[count]; }

	const T* begin() const { return &data[0]; }
	const T* end() const { return &data[count]; }

protected:
	std::size_t count = 0;
	T* data = nullptr;
};





// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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