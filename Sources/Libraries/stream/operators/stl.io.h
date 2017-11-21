#pragma once

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <memory>
#include <type_traits>
#include <boost/type_traits.hpp>

#include "../stream/stream.h"
#include <optional>


namespace stream
{

template <class T>
istream& operator >>(istream& is, std::deque<T>& deq)
{
	std::size_t size; is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		T val; is >> val;
		deq.push_back( val );
	}

	return is;
}

template <class T>
ostream& operator<<(ostream& os, const std::deque<T>& deq)
{
	std::size_t size = deq.size();
	os << size;

	for (auto& obj : deq)
		os << obj;

	return os;
}

template<bool trivial> struct VectorLoader;

template<> struct VectorLoader<true>
{
	template<typename T> static inline istream& load(istream& s, std::vector<T>& t)
	{
		std::size_t sz;
		s >> sz;

		if (sz > 0)
		{
			t.resize(sz);
			s.read(&t[0], sz * sizeof(T));
		}

		return s;
	}
};

template<> struct VectorLoader<false>
{
	template<typename T> static inline istream& load(istream& s, std::vector<T>& t)
	{
		std::size_t sz;
		s >> sz;

		if (sz > 0)
		{
			t.resize(sz);
			for (std::size_t b = 0; b < sz; ++b)
				s >> t[b];
		}

		return s;
	}
};

//
//
//
template<bool trivial> struct VectorSaver;

template<> struct VectorSaver<true>
{
	template<typename T> static inline ostream& save(ostream& s, const std::vector<T>& t)
	{
		std::size_t sz = t.size();
		s << sz;

		if (sz > 0)
		{
			s.write(&t[0], sz * sizeof(T));
		}

		return s;
	}
};

template<> struct VectorSaver<false>
{
	template<typename T> static inline ostream& save(ostream& s, const std::vector<T>& t)
	{
		std::size_t sz = t.size();
		s << sz;

		if (sz > 0)
		{
			for (std::size_t b = 0; b < sz; ++b)
				s << t[b];
		}

		return s;
	}
};


#ifdef WIN32
#define IS_TRIVIALLY_COPYABLE(T) std::is_trivially_copyable<T>::value
#elif defined(__ANDROID__)
#define IS_TRIVIALLY_COPYABLE(T) std::has_trivial_copy_constructor<T>::value
#elif defined(__APPLE__)
#define IS_TRIVIALLY_COPYABLE(T) boost::has_trivial_copy_constructor<T>::value
#else
#define IS_TRIVIALLY_COPYABLE(T) false
#endif


// std::vector
template <class T> istream& operator>>(istream& is, std::vector<T>& vec)		{ return VectorLoader<IS_TRIVIALLY_COPYABLE(T) && std::is_trivially_destructible<T>::value>::load(is, vec); }
template <class T> ostream& operator<<(ostream& os, const std::vector<T>& vec)	{ return VectorSaver<IS_TRIVIALLY_COPYABLE(T) && std::is_trivially_destructible<T>::value>::save(os, vec);	}

template <class T>
istream& load_vector(istream& is, std::vector<T>& vec)
{
	std::size_t size; is >> size;
	vec.resize(size);
	for (std::size_t i = 0; i < size; ++i)
		is >> vec[i];
	
	return is;
}

template <class T>
ostream& save_vector(ostream& os, const std::vector<T>& vec)
{
	std::size_t size = vec.size();
	os << size;
	for (std::size_t i = 0; i < size; ++i)
		os << vec[i];

	return os;
}

template <class T>
istream& load_vector_func(istream& is, std::vector<T>& vec)
{
	std::size_t size; is >> size;
	vec.resize(size);
	for (std::size_t i = 0; i < size; ++i)
		vec[i].load(is);

	return is;
}

template <class T>
ostream& save_vector_func(ostream& os, const std::vector<T>& vec)
{
	std::size_t size = vec.size();
	os << size;
	for (std::size_t i = 0; i < size; ++i)
		vec[i].save(os);

	return os;
}


// std::list
template <class T>
istream& operator >>(istream& is, std::list<T>& list)
{
	std::size_t size; is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		list.push_back( T() );
		is >> list.back();
	}
	return is;
}

template <class T>
ostream& operator<<(ostream& os, const std::list<T>& list)
{
	std::size_t size = list.size();
	os << size;

	for (auto& obj : list)
		os << obj;

	return os;
}

// std::list
template <class T>
istream& operator >> (istream& is, std::list<T*>& list)
{
	std::size_t size; is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		list.push_back(new T());
		is >> *list.back();
	}
	return is;
}

template <class T>
ostream& operator<<(ostream& os, const std::list<T*>& list)
{
	std::size_t size = list.size(); 
	os << size;

	for (auto& obj : list)
		os << *obj;

	return os;
}

// std::vector
template <class T>
istream& operator >> (istream& is, std::vector<std::unique_ptr<T>>& vector)
{
	std::size_t size; is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		vector.emplace_back(new T());
		is >> *vector.back();
	}

	return is;
}

template <class T>
ostream& operator<<(ostream& os, const std::vector<std::unique_ptr<T>>& vector)
{
	std::size_t size = vector.size();
	os << size;

	for (auto& obj : vector)
		os << *obj;

	return os;
}



// std::pair
template <class T1, class T2> istream& operator>>(istream& is, std::pair<T1, T2>& p)		{	return is >> p.first >> p.second;	}
template <class T1, class T2> ostream& operator<<(ostream& os, const std::pair<T1, T2>& p)	{	return os << p.first << p.second;	}

// std::map
template <class T1, class T2>
istream& operator>>(istream& is, std::map<T1, T2>& map)
{
	map.clear();
	int size; is >> size;
	for (int i = 0; i < size; ++i)
	{
		T1 key;
		is >> key;
		is >> map[key];
	}
	return is;
}

template <class T1, class T2>
ostream& operator<<(ostream& os, const std::map<T1, T2>& map)
{
	os << int(map.size());
	for(auto& v : map)
	{
		os << v;
	}

	return os;
}

// std::set
template <class T, class C>
istream& operator>>(istream& is, std::set<T, C>& set)
{
	set.clear();
	int size; is >> size;
	for (int i = 0; i < size; ++i)
	{
		T val; is >> val;
		set.insert(val).second;
	}
	return is;
}

template <class T, class C>
ostream& operator<<(ostream& os, const std::set<T, C>& set)
{
	int size = int(set.size()); 
	os << size;
	auto i = set.begin();
	auto ie = set.end();
	for (; i != ie; ++i) os << *i;

	return os;
}

// std::string
inline ostream& operator <<(ostream& stream, const std::string& s)
{
	int size = int(s.size()); 
	stream << size;
	if(size > 0)stream.write(&s[0], size);
	return stream;
}


inline istream& operator >>(istream& stream, std::string& s)
{
	int size;
	stream >> size;

	if(size > 0) 
	{
		s.resize(size);
		stream.read(&s[0], size);
	}
	else
	{
		s.clear();
	}
		
	return stream;
}

// std::string
inline ostream& operator <<(ostream& stream, const std::vector<std::string>& s)
{
	int count = (int)s.size();
	stream << count;
	for (int i = 0; i < count; ++i)
	{
		stream << s[i];
	}

	return stream;
}


inline istream& operator >>(istream& stream, std::vector<std::string>& s)
{
	int count; 
	stream >> count;
	s.resize(count);
	for (int i = 0; i < count; ++i)
	{
		stream >> s[i];
	}

	return stream;
}

// std::wstring
inline ostream& operator <<(ostream& stream, const std::wstring& s)
{
	int size = int(s.size()); 
	stream << size;
	if(size > 0)stream.write(&s[0], size*sizeof(wchar_t));
	return stream;
}


inline istream& operator >>(istream& stream, std::wstring& s)
{
	int size;
	stream >> size;
	s.resize(size);
	if(size > 0)stream.read(&s[0], size*sizeof(wchar_t));
	return stream;
}

template <size_t n, typename... T> typename std::enable_if<(n >= sizeof...(T))>::type
out_tuple(ostream&, const std::tuple<T...>&){}

template <size_t n, typename... T> typename std::enable_if<(n < sizeof...(T))>::type
out_tuple(ostream& os, const std::tuple<T...>& tup)
{
	out_tuple<n + 1>(os << std::get<n>(tup), tup);
}

template <size_t n, typename... T> typename std::enable_if<(n >= sizeof...(T))>::type
in_tuple(istream&, std::tuple<T...>&) {}

template <size_t n, typename... T> typename std::enable_if<(n < sizeof...(T))>::type
in_tuple(istream& is, std::tuple<T...>& tup)
{
	in_tuple<n + 1>(is >> std::get<n>(tup), tup);
}

template <typename... T>
ostream& operator<<(ostream& os, const std::tuple<T...>& tup)
{
	out_tuple<0>(os, tup);
	return os;
}

template <typename... T>
istream& operator>>(istream& is, std::tuple<T...>& tup)
{
	in_tuple<0>(is, tup);
	return is;
}


template <typename T>
ostream& operator<<(ostream& os, const std::optional<T>& value)
{
	bool has_value = value.has_value();
	os << has_value;
	if(has_value)
	{
		os << value.value();	
	}	

	return os;
}

template <typename T>
istream& operator>>(istream& is, std::optional<T>& value)
{
	bool has_value;
	is >> has_value;
	if(has_value)
	{
		T val;
		is >> val;
		value = val;
	}

	return is;
}


}
