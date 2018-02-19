// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

﻿#include "prototypesHolder.h"

EntitiesLoadStream* PrototypeHolder::find(void* id) const
{
	auto it = data.find(id);
	return it != data.end() ? it->second.get() : 0;
}

void PrototypeHolder::clear()
{
	data.clear();
}

EntitiesLoadStream& PrototypeHolder::create(void* id, const std::string& className)
{
	auto& os = data[id];
	CM_KERNEL_ENFORCE(os == 0);
	os = std::make_unique<EntitiesLoadStream>();
	os->prototypeClassName = className;
	return *os;
}

void PrototypeHolder::save(stream::ostream& os) const
{
	os << data.size();

	for (auto& it : data)
	{
		os << it.first << it.second->prototypeClassName << *it.second;
	}
}

void PrototypeHolder::load(stream::istream& is)
{
	data.clear();

	std::size_t size = 0;
	is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		void* id;
		std::string className;
		is >> id >> className;
		auto& els = create(id, className);
		is >> els;
	}
}

std::string PrototypeHolder::debugstr() const
{
	str::stringize result("count: ", data.size(), "\n");

	for (auto& it : data)
	{
		result(str::delimeter("\n\t"), it.first, " : ", it.second->prototypeClassName);
	}

	return result.str();
}


std::ostream& operator<<(std::ostream& os, const EntitiesStream& es) { return os; }

stream::ostream& operator<<(stream::ostream& os, const EntitiesLoadStream& es)
{
	return os << es.ios;
}

stream::istream& operator>>(stream::istream& is, EntitiesLoadStream& es)
{
	return is >> es.ios;
}

stream::ostream& operator<<(stream::ostream& os, const ComponentExternalLink& link)
{
	return os << link.link.address;
}

stream::istream& operator>>(stream::istream& is, ComponentExternalLink& link)
{
	return is >> link.link.address;
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