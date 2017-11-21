#pragma once

#include <memory>
#include <unordered_map>

template<class T>
struct environment
{
	environment() {}
	~environment() {}

	// return a reference to the innermost environment where 'var' appears
	environment* find(const std::string& var)
	{
		if (env.find(var) != env.end())
		{
			return this; // the symbol exists in this environment
		}

		// attempt to find the symbol in some "outer" env
		return outer ? outer->find(var) : nullptr;
	}

	// return a reference to the cell associated with the given symbol 'var'
	T& operator[] (const std::string & var)	{ return env[var]; }

private:
	std::unordered_map<std::string, T> env; // inner symbol->cell mapping
	std::shared_ptr<environment> outer; // next adjacent outer env, or 0 if there are no further environments
};