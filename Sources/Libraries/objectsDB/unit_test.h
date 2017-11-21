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

//	LOG_MSG(res.str());
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