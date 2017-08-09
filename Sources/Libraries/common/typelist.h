// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	
	template <class TList> std::size_t length_v;
	template <>	std::size_t length_v<null_type> = 0;
	template <class T, class U>	std::size_t length_v<typelist<T, U>> = 1 + length_v<U>;
	
}



// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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