// Copyright (C) 2016-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once








#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <utility>
#include "common/apply.h"

namespace scheme
{
	enum cell_type { SYMBOL, NUMBER, LIST, PROCEDURE, LAMBDA };
	
	struct cell;
	struct cells : public std::vector<cell> {};
	struct environment;
	struct caller_base;


	struct cell
	{
		typedef cell(*proc_type)(const cells& params);

		cell(cell_type type = SYMBOL) : type(type) {}
		cell(cell_type type, const std::string& val) : type(type), val(val) {}
		cell(cell_type type, const cells& list) : type(type), list(list) {}
		cell(proc_type proc) : type(PROCEDURE), proc(proc) {}
		cell(caller_base* c) : type(PROCEDURE), call(c) {}

		cell_type type;
		std::string val;

		cells list;
		proc_type proc = nullptr;
		std::shared_ptr<caller_base> call;
		std::shared_ptr<environment> env;
	};

	template<class T> T convert(const cell& input);
	template<> inline std::string convert<std::string>(const cell& input) { return input.val; }
	template<> inline const char* convert<const char*>(const cell& input) { return input.val.c_str(); }
	template<> inline int convert<int>(const cell& input) { return std::atoi(input.val.c_str());	}

	template<class T> struct CellType;
	template<> struct CellType<int>			{ static const cell_type type = NUMBER; };
	template<> struct CellType<bool>		{ static const cell_type type = NUMBER; };
	template<> struct CellType<std::string> { static const cell_type type = SYMBOL; };
	template<> struct CellType<const char*> { static const cell_type type = SYMBOL; };
	template<> struct CellType<void>		{ static const cell_type type = SYMBOL; };


	template<typename... T, size_t... I>
	decltype(auto) init_tuple_impl(const cells& list, std::index_sequence<I...>)
	{
		return std::make_tuple(convert<T>(list[I])...);
	}

	template<typename... P>
	decltype(auto) init_tuple(const cells& list)
	{
		return init_tuple_impl<std::decay_t<P>...>(list, std::make_index_sequence<sizeof...(P)>{});
	}

	struct caller_base
	{
		virtual ~caller_base() {}
		virtual	cell call(cells& list) = 0;
	};

	template<class ReturnType, class... P>
	struct caller : public caller_base
	{
		typedef ReturnType(*Procedure)(P...);

		caller(Procedure proc) : procedure(proc) {}

		virtual	cell call(cells& list)
		{
			stl::apply(procedure, init_tuple<P...>(list));
			cell_type restype = CellType<decltype(apply(procedure, init_tuple<P...>(list)))>::type;
			return cell(restype);
		}

		Procedure procedure;
	};

	
	struct environment
	{
		environment() {}
		environment(const cells& params, const cells& args, const std::shared_ptr<environment>& outer);
		~environment() {}

		
		environment* find(const std::string& var);
		cell& operator[] (const std::string& var);

		void defun(const std::string& name, cell::proc_type procedure);

		template<class ReturnType, class... P>
		void defun(const std::string& name, ReturnType(*proc)(P...))
		{
			env[name] = cell( new caller<ReturnType, P...>(proc) );
		}

	private:
		std::unordered_map<std::string, cell> env; 
		std::shared_ptr<environment> outer; 
	};

	typedef std::shared_ptr<environment> environment_ptr;

	environment_ptr global_env();
	void repl(const std::string& prompt);
	void repl(const std::string& prompt, const environment_ptr& env);
	bool test(const std::string& code, const environment_ptr& env, const std::string& expectedResult);
	void exec(const std::string& code, const environment_ptr& env);
	void exec_file(const std::string& fileName, const environment_ptr& env);

	template<class ReturnType, class... P>
	void defun(const std::string& name, ReturnType(*proc)(P...))
	{
		global_env()->defun(name, proc);
	}
}



// Copyright (C) 2016-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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