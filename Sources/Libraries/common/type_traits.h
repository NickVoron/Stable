#pragma once

#include <type_traits>

namespace stl
{
	template <class T> std::true_type has_value_type_d(typename T::value_type*);
	template <class T> std::false_type has_value_type_d(...);
	template <class T>	struct has_value_type : decltype(has_value_type_d<T>(nullptr)) {};

	template <class T> std::true_type has_mapped_type_d(typename T::mapped_type*);
	template <class T> std::false_type has_mapped_type_d(...);
	template <class T>	struct has_mapped_type : decltype(has_mapped_type_d<T>(nullptr)) {};

	template <class T> std::true_type has_key_compare_d(typename T::key_compare*);
	template <class T> std::false_type has_key_compare_d(...);
	template <class T>	struct has_key_compare : decltype(has_key_compare_d<T>(nullptr)) {};

	template <class T> std::true_type has_key_type_d(typename T::key_type*);
	template <class T> std::false_type has_key_type_d(...);
	template <class T>	struct has_key_type : decltype(has_key_type_d<T>(nullptr)) {};

	template <class T> std::true_type has_key_equal_d(typename T::key_equal*);
	template <class T> std::false_type has_key_equal_d(...);
	template <class T>	struct has_key_equal : decltype(has_key_equal_d<T>(nullptr)) {};

	template <class T> std::true_type has_hasher_d(typename T::hasher*);
	template <class T> std::false_type has_hasher_d(...);
	template <class T>	struct has_hasher : decltype(has_hasher_d<T>(nullptr)) {};


	//
	//
	//
	template <typename C, bool B = has_value_type<C>::value>
	struct is_std_vector;

	template <class C>
	struct is_std_vector<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_vector<C, true>
	{
		constexpr static bool value = std::is_base_of<std::vector<typename C::value_type, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, std::size_t Size, bool B = has_value_type<C>::value>
	struct is_std_array;

	template <class C, std::size_t Size>
	struct is_std_array<C, Size, false>
	{
		constexpr static bool value = false;
	};

	template <class C, std::size_t Size>
	struct is_std_array<C, Size, true>
	{
		constexpr static bool value = std::is_base_of<std::array<typename C::value_type, Size>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = has_value_type<C>::value>
	struct is_std_list;

	template <class C>
	struct is_std_list<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_list<C, true>
	{
		constexpr static bool value = std::is_base_of<std::list<typename C::value_type, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = has_value_type<C>::value>
	struct is_std_deque;

	template <class C>
	struct is_std_deque<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_deque<C, true>
	{
		constexpr static bool value = std::is_base_of<std::deque<typename C::value_type, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = std::conjunction_v<has_value_type<C>, has_key_compare<C>>>
	struct is_std_set;

	template <class C>
	struct is_std_set<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_set<C, true>
	{
		constexpr static bool value = std::is_base_of<std::set<typename C::value_type, typename C::key_compare, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = std::conjunction_v<has_key_type<C>, has_key_compare<C>, has_mapped_type<C> >>
	struct is_std_map;

	template <class C>
	struct is_std_map<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_map<C, true>
	{
		constexpr static bool value = std::is_base_of<std::map<typename C::key_type, typename C::mapped_type, typename C::key_compare, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = std::conjunction_v<has_value_type<C>, has_hasher<C>, has_key_equal<C>>>
	struct is_std_unordered_set;

	template <class C>
	struct is_std_unordered_set<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_unordered_set<C, true>
	{
		constexpr static bool value = std::is_base_of<std::unordered_set<typename C::value_type, typename C::hasher, typename C::key_equal, typename C::allocator_type>, C>::value;
	};

	//
	//
	//
	template <typename C, bool B = std::conjunction_v<has_key_type<C>, has_mapped_type<C>, has_hasher<C>, has_key_equal<C>>>
	struct is_std_unordered_map;

	template <class C>
	struct is_std_unordered_map<C, false>
	{
		constexpr static bool value = false;
	};

	template <class C>
	struct is_std_unordered_map<C, true>
	{
		constexpr static bool value = std::is_base_of<std::unordered_map<typename C::key_type, typename C::mapped_type, typename C::hasher, typename C::key_equal, typename C::allocator_type>, C>::value;
	};


	//
	//
	//
	template <typename P>
	struct is_std_pair
	{
		constexpr static bool value = false;
	};

	template <typename F, typename S>
	struct is_std_pair<std::pair<F, S>>
	{
		constexpr static bool value = true;
	};

	template <typename P>
	struct is_std_tuple
	{
		constexpr static bool value = false;
	};

	template <typename... V>
	struct is_std_tuple<std::tuple<V...>>
	{
		constexpr static bool value = true;
	};

	template<class T> struct IsConstant { static const bool value = (std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<T>::value); };
	template<> struct IsConstant<std::string> { static const bool value = true; };
	template<> struct IsConstant<str::string64> { static const bool value = true; };
	template<> struct IsConstant<const char*> { static const bool value = true; };
	template<> struct IsConstant<const wchar_t*> { static const bool value = true; };

	template<class T> struct IsContainer
	{
		static const bool value = std::disjunction_v<
			is_std_vector<T>
			, is_std_list<T>
			, is_std_deque<T>
			, is_std_set<T>
			, is_std_map<T>
			, is_std_unordered_map<T>
			, is_std_unordered_set<T>
		>;
	};

	template<class T> struct IsStructure
	{
		static const bool value = (
			!IsContainer<T>::value
			&& !IsConstant<T>::value
			&& !is_std_pair<T>::value
			&& !is_std_tuple<T>::value);
	};
}