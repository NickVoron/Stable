#pragma once

#include <utility>

namespace stl
{
	template <typename F, typename Tuple, size_t... I>
	decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
	{
		return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
	}

	template <typename F, typename Tuple>
	decltype(auto) apply(F&& f, Tuple&& t)
	{
		using indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>;
		return apply_impl(std::forward<F>(f), std::forward<Tuple>(t), indices{});
	}

	template<typename F, std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
	for_each(std::tuple<Tp...>& t, F&& func)
	{ }

	template<typename F, std::size_t I = 0, typename... Tp>
	inline typename std::enable_if < I < sizeof...(Tp), void>::type
	for_each(std::tuple<Tp...>& t, F&& func)
	{
		func(std::get<I>(t));
		for_each<F, I + 1, Tp...>(t, std::forward<F>(func));
	}

	template<typename F, std::size_t I = 0, typename... Tp>
	inline typename std::enable_if<I == sizeof...(Tp), void>::type
	for_each(const std::tuple<Tp...>& t, F&& func)
	{ }

	template<typename F, std::size_t I = 0, typename... Tp>
	inline typename std::enable_if < I < sizeof...(Tp), void>::type
	for_each(const std::tuple<Tp...>& t, F&& func)
	{
		func(std::get<I>(t));
		for_each<F, I + 1, Tp...>(t, std::forward<F>(func));
	}

	template <typename T, typename... Ts>
	struct index_of;

	template <typename T, typename... Ts>
	struct index_of<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

	template <typename T, typename U, typename... Ts>
	struct index_of<T, U, Ts...> : std::integral_constant<std::size_t, 1 + index_of<T, Ts...>::value> {};

	//
	template <typename T, typename... Ts>
	struct contains;

	template <typename T>
	struct contains<T> : std::false_type {};

	template <typename T, typename... Ts>
	struct contains<T, T, Ts...> : std::true_type {};

	template <typename T, typename U, typename... Ts>
	struct contains<T, U, Ts...> : contains<T, Ts...> {};
	

	//
	//
	//
	template<class... SourceTypes, class... TargetTypes>
	std::tuple<TargetTypes...> select_tuple_items(std::tuple<SourceTypes...>& source, std::tuple<TargetTypes...>* val = nullptr)
	{
		return std::tuple<TargetTypes...>(std::get<TargetTypes>(source)...);
	}

}
