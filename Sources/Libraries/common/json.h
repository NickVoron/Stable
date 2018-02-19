// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "json.hpp"

#include <fstream>

namespace json
{
	template<class T>
	void serialize_to(nlohmann::json& j, const T& value, const char* name)
	{
		j[name] = value;
	}

	template<class T>
	bool serialize_from(const nlohmann::json& j, T& value, const char* name)
	{
		const auto& ref = j.at(name);

		if (!ref.empty())
		{
			value = j.at(name).get<T>();
			return true;
		}

		return false;
	}

	template<class T>
	void serialize_to(const std::string& fileName, const T& value, const char* name)
	{
		std::ofstream os(fileName);
		if (os.good())
		{
			nlohmann::json json;
			serialize_to(json, value, name);
			os << json;
		}
	}

	template<class T>
	bool serialize_from(const std::string& fileName, T& value, const char* name)
	{
		std::ifstream is(fileName);
		if (is.good())
		{
			nlohmann::json j;
			is >> j;
			json::serialize_from(j, value, name);
			return true;
		}

		return false;
	}

	template<class T>
	void serialize(nlohmann::json& j, const T& value, const char* name)
	{
		serialize_to(j, value, name);
	}

	template<class T>
	bool serialize(const nlohmann::json& j, T& value, const char* name)
	{
		return serialize_from(j, value, name);
	}
	
}




// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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