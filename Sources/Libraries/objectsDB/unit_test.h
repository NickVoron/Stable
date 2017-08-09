// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "library.include.h"

#include "unitTest/library.include.h"

struct IntObject : public ObjectsDB::ObjectBase<IntObject>
{
	unsigned int value;
};

template<unsigned int count>
struct IntsContainer : public ObjectsDB::Container<IntObject>
{
	IntsContainer()
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			objects[i].value = i;
			push_back(objects[i]);
		}
	}

	IntObject objects[count];
};

struct WorldObjectBase : public Base::IntrusiveList<WorldObjectBase>::Node
{

};


struct WorldObject : public Base::IntrusiveIntersection<Base::IntrusiveList<WorldObject>::Node, ObjectsDB::ObjectBase<WorldObject>>
{
};

struct World : public Base::IntrusiveList<WorldObject>
{

};

template<class Result>
void debug(const std::string& header, const Result& container)
{
	str::stringize res(header, ": (", str::comma());
	container.foreach([&res](auto& obj) { res(obj.value); });
	res(str::nodelim(), ")");


}

template<class Result>
std::string stringize(const Result& container)
{
	str::stringize res;
	container.foreach([&res](auto& obj) { res(obj.value); });
	return res.str();
}

TEST_SUITE(ObjectsDBUnitTest)

TEST_CASE(find_first) 
{ 
	IntsContainer<10> container;
	auto result = container.find_first([](auto& obj)
	{
		return obj.value < 5;
	});
	debug("find_first", result);
	ENFORCE_EQUAL(stringize(result), "0");
}

TEST_CASE(find_all)
{
	IntsContainer<10> container;
	auto result = container.find_all([](auto& obj)
	{
		return obj.value < 5;
	});
	debug("find_all", result);
	ENFORCE_EQUAL(stringize(result), "01234");
}

TEST_CASE(find_first_n)
{
	IntsContainer<10> container;
	auto result = container.find_first_n(3, [](auto& obj)
	{
		return obj.value < 5;
	});
	debug("find_first_n", result);
	ENFORCE_EQUAL(stringize(result), "012");
}

TEST_CASE(find_rand_n)
{
	IntsContainer<10> container;
	auto result = container.find_rand_n(2, [](auto& obj)
	{
		return obj.value < 5;
	});
	debug("find_rand_n", result);
	ENFORCE_EQUAL(stringize(result), "01");
}

TEST_CASE(complex_list)
{
	World world;
	WorldObject object;

	world.push_back(object);
	object.remove();
}

TEST_SUITE_END();



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