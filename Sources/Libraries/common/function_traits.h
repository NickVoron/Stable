// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <type_traits>
#include <tuple>
#include <functional>

namespace stl
{
	
	
	
	template <typename T>	struct function_traits		: public function_traits<decltype(&T::operator())>	{};

	namespace xx_impl
	{
		template <typename C, typename R, typename... A>
		struct memfn_type
		{
			typedef typename std::conditional<
				std::is_const<C>::value,
				typename std::conditional<
				std::is_volatile<C>::value,
				R(C::*)(A...) const volatile,
				R(C::*)(A...) const
				>::type,
				typename std::conditional<
				std::is_volatile<C>::value,
				R(C::*)(A...) volatile,
				R(C::*)(A...)
				>::type
			>::type type;
		};
	}

	template <typename ReturnType, typename... Args>
	struct function_traits<ReturnType(Args...)>
	{
		typedef ReturnType			result_type;
		typedef std::tuple<Args...> params_tuple;

		typedef ReturnType	function_type(Args...);

		
		template <typename OwnerType>
		using member_function_type = typename xx_impl::memfn_type<typename std::remove_pointer<typename std::remove_reference<OwnerType>::type>::type, ReturnType, Args...>::type;

		
		enum { arity = sizeof...(Args) };

		template <size_t i>
		struct arg
		{
			typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
		};
	};

	template <typename ReturnType, typename... Args>  	struct function_traits<ReturnType(*)(Args...)>		: public function_traits<ReturnType(Args...)>	{};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...)> : public function_traits<ReturnType(Args...)>
	{
		typedef ClassType& owner_type;
	};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const> : public function_traits<ReturnType(Args...)>
	{
		typedef const ClassType& owner_type;
	};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) volatile> : public function_traits<ReturnType(Args...)>
	{
		typedef volatile ClassType& owner_type;
	};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const volatile> : public function_traits<ReturnType(Args...)>
	{
		typedef const volatile ClassType& owner_type;
	};

	template <typename FunctionType>  	struct function_traits<std::function<FunctionType>>	: public function_traits<FunctionType>	{};

#if defined(_GLIBCXX_FUNCTIONAL)
#define MEM_FN_SYMBOL std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
#define MEM_FN_SYMBOL std::__mem_fn
#endif

#ifdef MEM_FN_SYMBOL

	template <typename R, typename C>					struct function_traits<MEM_FN_SYMBOL<R C::*>>						: public function_traits<R(C*)>	{};
	template <typename R, typename C, typename... A>	struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...)>>				: public function_traits<R(C*, A...)>	{};
	template <typename R, typename C, typename... A>	struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) const>>			: public function_traits<R(const C*, A...)>	{};
	template <typename R, typename C, typename... A>	struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) volatile>>		: public function_traits<R(volatile C*, A...)>	{};
	template <typename R, typename C, typename... A>	struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) const volatile>>	: public function_traits<R(const volatile C*, A...)>	{};

#undef MEM_FN_SYMBOL
#endif

	template <typename T>	struct function_traits<T&> : public function_traits<T> {};
	template <typename T>	struct function_traits<const T&> : public function_traits<T> {};
	template <typename T>	struct function_traits<volatile T&> : public function_traits<T> {};
	template <typename T>	struct function_traits<const volatile T&> : public function_traits<T> {};
	template <typename T>	struct function_traits<T&&> : public function_traits<T> {};
	template <typename T>	struct function_traits<const T&&> : public function_traits<T> {};
	template <typename T>	struct function_traits<volatile T&&> : public function_traits<T> {};
	template <typename T>	struct function_traits<const volatile T&&> : public function_traits<T> {};
}




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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