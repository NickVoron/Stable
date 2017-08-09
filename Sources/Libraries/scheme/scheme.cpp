// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "scheme.h"

#include "stuff/stringUtils.h"

#include <fstream>
#include <streambuf>
#include <locale>

namespace scheme
{
	const cell false_sym(SYMBOL, "#f");
	const cell true_sym(SYMBOL, "#t"); 
	const cell nil(SYMBOL, "nil");

	
	cell proc_add(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i) n += atol(i->val.c_str());
		return cell(NUMBER, str::stringize(n));
	}

	cell proc_sub(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i) n -= atol(i->val.c_str());
		return cell(NUMBER, str::stringize(n));
	}

	cell proc_mul(const cells & c)
	{
		long n(1);
		for (auto i = c.begin(); i != c.end(); ++i) n *= atol(i->val.c_str());
		return cell(NUMBER, str::stringize(n));
	}

	cell proc_div(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i) n /= atol(i->val.c_str());
		return cell(NUMBER, str::stringize(n));
	}

	cell proc_equal(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i)
			if (n != atol(i->val.c_str()))
				return false_sym;

		return true_sym;
	}

	cell proc_greater(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i)
			if (n <= atol(i->val.c_str()))
				return false_sym;
		return true_sym;
	}

	cell proc_greater_equal(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i)
			if (n < atol(i->val.c_str()))
				return false_sym;

		return true_sym;
	}

	cell proc_less(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i)
			if (n >= atol(i->val.c_str()))
				return false_sym;
		return true_sym;
	}

	cell proc_less_equal(const cells & c)
	{
		long n(atol(c[0].val.c_str()));
		for (auto i = c.begin() + 1; i != c.end(); ++i)
			if (n > atol(i->val.c_str()))
				return false_sym;
		return true_sym;
	}

	cell proc_length(const cells & c) { return cell(NUMBER, str::stringize(c[0].list.size())); }
	cell proc_nullp(const cells & c) { return c[0].list.empty() ? true_sym : false_sym; }
	cell proc_car(const cells & c) { return c[0].list[0]; }

	cell proc_cdr(const cells & c)
	{
		if (c[0].list.size() < 2)
		{
			return nil;
		}
		else
		{
			cell result(c[0]);
			result.list.erase(result.list.begin());
			return result;
		}
	}

	cell proc_append(const cells & c)
	{
		cell result(LIST);
		result.list = c[0].list;
		result.list.insert(result.list.end(), c[1].list.begin(), c[1].list.end());
		return result;
	}

	cell proc_cons(const cells & c)
	{
		cell result(LIST);
		result.list.push_back(c[0]);
		result.list.insert(result.list.end(), c[1].list.begin(), c[1].list.end());
		return result;
	}

	cell proc_list(const cells& c)
	{
		return cell(LIST, c);
	}

	
	void add_globals(environment& env)
	{
		env["nil"] = nil;
		env["#f"] = false_sym;
		env["#t"] = true_sym;

		env.defun("append", &proc_append);
		env.defun("car", &proc_car);
		env.defun("cdr", &proc_cdr);
		env.defun("cons", &proc_cons);
		env.defun("length", &proc_length);
		env.defun("list", &proc_list);
		env.defun("null?", &proc_nullp);
		env.defun("+", &proc_add);
		env.defun("-", &proc_sub);
		env.defun("*", &proc_mul);
		env.defun("/", &proc_div);
		env.defun("=", &proc_equal);
		env.defun(">", &proc_greater);
		env.defun("<", &proc_less);
		env.defun("<=", &proc_less_equal);
		env.defun(">=", &proc_greater_equal);
	}

	
	environment::environment(const cells& params, const cells& args, const std::shared_ptr<environment>& outer)
		: outer(outer)
	{
		auto a = args.begin();
		for (auto& p : params)
			env[p.val] = *(a++);
	}

	
	environment* environment::find(const std::string& var)
	{
		if (env.find(var) != env.end())
		{
			return this; 
		}			

		
		return outer ? outer->find(var) : nullptr;
	}

	
	cell& environment::operator[] (const std::string & var) 
	{ 
		return env[var]; 
	}

	void environment::defun(const std::string& name, cell::proc_type procedure)
	{
		env[name] = procedure;
	}


	
	cell eval(cell x, const std::shared_ptr<environment>& env)
	{
		if (x.type == SYMBOL)
		{
			if (auto container = env->find(x.val))
			{
				return (*container)[x.val];
			}
			else
			{
				return nil;
			}
		}

		if (x.type == NUMBER)	return x;
		if (x.list.empty())		return nil;

		if (x.list[0].type == SYMBOL)
		{
			const auto& value = x.list[0].val;

			if (value == "quote")	return x.list[1];     
			if (value == "if")		return eval(eval(x.list[1], env).val == "#f" ? (x.list.size() < 4 ? nil : x.list[3]) : x.list[2], env);         
			
			if (value == "define")	return (*env)[x.list[1].val] = eval(x.list[2], env);     

			if (value == "lambda")
			{    
				x.type = LAMBDA;
				
				
				x.env = env;
				return x;
			}
			if (value == "begin")
			{     
				for (size_t i = 1; i < x.list.size() - 1; ++i)
					eval(x.list[i], env);
				return eval(x.list[x.list.size() - 1], env);
			}
		}
		
		cell proc(eval(x.list[0], env));
		cells exps;
		for (auto exp = x.list.begin() + 1; exp != x.list.end(); ++exp)
			exps.push_back(eval(*exp, env));

		if (proc.type == LAMBDA)
		{
			
			
			
			
			
			
			
			return eval(proc.list[2], std::make_shared<environment>(proc.list[1].list, exps, proc.env));
		}
		else if (proc.type == PROCEDURE)
		{
			return proc.proc ? proc.proc(exps) : proc.call->call(exps);
		}

		std::cout << "not a function\n";
		return nil;
	}

	
	std::list<std::string> tokenize(const std::string & str)
	{
		std::list<std::string> tokens;
		const char * s = str.c_str();
		while (*s)
		{
			while (isspace(*s))
				++s;

			if (*s == '(' || *s == ')')
			{
				tokens.push_back(*s++ == '(' ? "(" : ")");
			}
			else
			{
				const char* t = s;
				while ((*t) && !isspace(*t) && (*t != '(') && (*t != ')'))
					++t;
				tokens.push_back(std::string(s, t));
				s = t;
			}
		}
		return tokens;
	}

	
	cell atom(const std::string& token)
	{
		auto type = (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) ? NUMBER : SYMBOL;
		return cell(type, token);
	}

	
	cell read_from(std::list<std::string> & tokens)
	{
		if (!tokens.empty())
		{
			const std::string token(tokens.front());
			tokens.pop_front();
			if (token == "(")
			{
				cell c(LIST);
				while (tokens.front() != ")")
				{
					c.list.push_back(read_from(tokens));
				}

				tokens.pop_front();
				return c;
			}
			else
			{
				return atom(token);
			}
		}

		return nil;
	}

	
	cell read(const std::string& s)
	{
		auto tokens = tokenize(s);
		return read_from(tokens);
	}

	
	std::string to_string(const cell& exp)
	{
		switch (exp.type)
		{
		case LIST:
		{
			std::string s("(");
			for (auto& e : exp.list)
				s += to_string(e) + ' ';

			if (s[s.size() - 1] == ' ')
				s.erase(s.size() - 1);
			return s + ')';
		}
		case LAMBDA:
		{
			return "<lambda>";
		}
		case PROCEDURE:
		{
			return "<procedure>";
		}
		}

		return exp.val;
	}

	
	void repl(const std::string & prompt, const std::shared_ptr<environment>& env)
	{
		while (true)
		{
			std::cout << prompt;
			std::string line; std::getline(std::cin, line);
			std::cout << to_string(eval(read(line), env)) << '\n';
		}
	}

	void repl(const std::string& prompt)
	{
		repl(prompt, global_env());
	}

	std::shared_ptr<environment> global_env()
	{
		static environment_ptr env = nullptr;

		if (!env)
		{
			env = std::make_shared<environment>();
			add_globals(*env);
		}	
		
		return env;
	}


	
	template <typename T1, typename T2>
	bool test_equal(const T1& value, const T2& expected_value)
	{
		bool res = (value != expected_value);

		if (res)
		{
			std::cout << " expected " << expected_value	<< ", got " << value << std::endl;
		}
		else
		{
			std::cout << value << ": " << "success" << std::endl;
		}

		return res;
	}

	bool test(const std::string& code, const std::shared_ptr<environment>& env, const std::string& expectedResult)
	{
		return test_equal(to_string(eval(read(code), env)), expectedResult);
	}

	void exec(const std::string& code, const std::shared_ptr<environment>& env)
	{
		eval(read(code), env);
	}

	void exec_file(const std::string& fileName, const std::shared_ptr<environment>& env)
	{
		std::ifstream t(fileName.c_str());
		if (t.good())
		{
			std::string str;

			t.seekg(0, std::ios::end);
			str.reserve(t.tellg());
			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			exec(str, env);
		}		
	}
}





// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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