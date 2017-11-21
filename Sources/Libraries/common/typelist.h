#pragma once

namespace typelists
{
	struct null_type {};

	template <class T, class U>
	struct typelist
	{
		typedef T head;
		typedef U tail;
	};

	namespace Private
	{
		template<class... TList>
		struct MakeTypelistHelper;

		template<class Head, class... Tail>
		struct MakeTypelistHelper<Head, Tail...>
		{
			typedef typelist<Head, typename MakeTypelistHelper<Tail...>::list> list;
		};

		template<>
		struct MakeTypelistHelper<>
		{
			typedef null_type list;
		};			 

		template <class TList, class T> struct Append;

		template <> struct Append<null_type, null_type>
		{
			typedef null_type Result;
		};

		template <class T> struct Append<null_type, T>
		{
			typedef typename MakeTypelistHelper<T>::list Result;
		};

		template <class Head, class Tail>
		struct Append<null_type, typelist<Head, Tail> >
		{
			typedef typelist<Head, Tail> Result;
		};

		template <class Head, class Tail, class T>
		struct Append<typelist<Head, Tail>, T>
		{
			typedef typelist<Head, typename Append<Tail, T>::Result> Result;
		};
	}

	template<class... TList> using list = typename Private::MakeTypelistHelper<TList...>::list;
	template<class T0, class T1> using append = typename Private::Append<T0, T1>::Result;

	//
	template <class TList> std::size_t length_v;
	template <>	std::size_t length_v<null_type> = 0;
	template <class T, class U>	std::size_t length_v<typelist<T, U>> = 1 + length_v<U>;
	
}