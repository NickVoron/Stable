#pragma once

#include "unitTest/library.include.h"

struct Object
{
	Vector3 v0;
	Vector3 v1;
	Vector3 v2;
};

TEST_SUITE(ReflectionTest)

template<class Struct>
void assign_test(const Struct& ref)
{
	auto refl = mirror::type(ref);
	Struct value = refl;
	value = (Struct)refl;
	refl = ref;

	TEST_CHECK(value == ref);	
}

void test_vector3()
{
	const Vector3 ref(1, 2, 3);
	auto refl0 = mirror::type(Vector3(0, 0, 0));
	auto refl1 = mirror::type(Vector3(0, 0, 0));

	refl0
		.set("x", ref.x)
		.set("y", ref.y)
		.set("z", ref.z);

	refl1
		.set(0, ref.x)
		.set(1, ref.y)
		.set(2, ref.z);

	Vector3 extracted0 = refl0;
	Vector3 extracted1 = refl1;

	TEST_CHECK(extracted0 == ref);
	TEST_CHECK(extracted1 == ref);

	Vector3 extracted2 = Vector3(0, 0, 0);
	Vector3 extracted3 = Vector3(0, 0, 0);

	refl0
		.get("x", extracted2.x)
		.get("y", extracted2.y)
		.get("z", extracted2.z);

	refl1
		.get(0, extracted3.x)
		.get(1, extracted3.y)
		.get(2, extracted3.z);

	TEST_CHECK(extracted2 == ref);
	TEST_CHECK(extracted3 == ref);
}

TEST_CASE(Mirror_reflection)
{
	mirror::declare<Vector3>("Vector3")
		.property("x", &Vector3::x)
		.property("y", &Vector3::y)
		.property("z", &Vector3::z);

	mirror::declare<Object>("Object")
		.property("v0", &Object::v0)
		.property("v1", &Object::v1)
		.property("v2", &Object::v2);

	const Vector3 ref0(1, 2, 3);
	const Vector3 ref1(4, 5, 6);
	const Vector3 ref2(7, 8, 9);

	assign_test(ref0);
	assign_test(ref1);
	assign_test(ref2);
	test_vector3();
}

TEST_SUITE_END()