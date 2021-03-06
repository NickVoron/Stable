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

TEST_SUITE(ContainersTests)

template<class List>
std::string content(const List& data)
{
	str::stringize res;
	for (auto& element : data)
	{
		res(element.value);
	}
	return res.str();
}


TEST_CASE(IntrusiveList)
{
	struct Element : public Base::IntrusiveList<Element>::Node
	{
		Element()
		{
			static int counter = 0;
			value = counter++;
		}

		int value;
	};

	struct List : public Base::IntrusiveList<Element>
	{
	};

	{
		List list;
		Element e0;
		Element e1;
		Element e2;
		Element e3;
		Element e4;

		list.push_back(e0);
		list.push_back(e1);
		list.push_back(e2);

		TEST_CHECK(list.size() == 3);

		list.clear();
		

		TEST_CHECK(list.size() == 0);
		TEST_CHECK(list.empty());

		list.push_back(e0);
		list.push_back(e1);
		list.push_back(e2);
		list.push_back(e3);

		TEST_CHECK(list.size() == 4);
		list.clear();

		list.push_back(e0);
		TEST_CHECK(list.size() == 1);
		list.clear();

		TEST_CHECK(list.size() == 0);
		list.push_back(e0);
		TEST_CHECK(content(list) == "0");
		TEST_CHECK(list.size() == 1);
		list.push_back(e1);
		TEST_CHECK(content(list) == "01");
		TEST_CHECK(list.size() == 2);
		list.push_back(e2);
		TEST_CHECK(content(list) == "012");
		TEST_CHECK(list.size() == 3);
		list.push_back(e3);
		TEST_CHECK(content(list) == "0123");
		TEST_CHECK(list.size() == 4);
		list.push_back(e4);
		TEST_CHECK(content(list) == "01234");
		TEST_CHECK(list.size() == 5);

		list.push_back(e0);
		TEST_CHECK(list.size() == 5);
		list.push_back(e1);
		TEST_CHECK(list.size() == 5);
		list.push_back(e2);
		TEST_CHECK(list.size() == 5);
		list.push_back(e3);
		TEST_CHECK(list.size() == 5);
		list.push_back(e4);
		TEST_CHECK(list.size() == 5);
	}

	{
		{
			List list;
			{
				Element e0;
				Element e1;
				list.push_back(e0);
				list.push_back(e1);
				TEST_CHECK(list.size() == 2);
			}

			TEST_CHECK(list.size() == 0);
			TEST_CHECK(list.empty());
		}

		{
			Element e2;
			Element e3;
			Element e4;
			List list;
			{
				Element e0;
				Element e1;
				list.push_back(e0);
				list.push_back(e1);
				list.push_back(e2);
				list.push_back(e3);
				list.push_back(e4);
				TEST_CHECK(list.size() == 5);
			}

			TEST_CHECK(list.size() == 3);
		}		
	}
}

TEST_CASE(IntrusiveIntersections)
{
	struct Element0 : public Base::IntrusiveList<Element0>::Node
	{
		Element0()
		{
			static int counter = 0;
			value = counter++;
		}

		int value;
	};

	struct Element1 : public Base::IntrusiveList<Element1>::Node
	{
		Element1()
		{
			static int counter = 0;
			value = counter++;
		}

		int value;
	};

	struct Element : public Base::IntrusiveIntersection<Element0, Element1> {};

	struct List0 : public Base::IntrusiveList<Element0> {};
	struct List1 : public Base::IntrusiveList<Element1> {};

	{
		List0 list0;

		Element e0;
		Element e1;
		Element e2;
		Element e3;
		Element e4;

		list0.push_back(e0);
		list0.push_back(e1);
		list0.push_back(e2);

		List1 list1;

		list1.push_back(e0);
		list1.push_back(e1);

		TEST_CHECK(list0.size() == 3);
		TEST_CHECK(list1.size() == 2);

		list0.clear();
		list1.clear();

		TEST_CHECK(list0.size() == 0);
		TEST_CHECK(list0.empty());

		TEST_CHECK(list1.size() == 0);
		TEST_CHECK(list1.empty());

		list0.push_back(e0);
		list0.push_back(e1);
		list0.push_back(e2);
		list0.push_back(e3);

		TEST_CHECK(list0.size() == 4);
		list0.clear();

		list1.push_back(e0);
		list1.push_back(e1);
		list1.push_back(e2);
		list1.push_back(e3);

		TEST_CHECK(list1.size() == 4);
		list1.clear();

		list0.push_back(e0);
		TEST_CHECK(list0.size() == 1);
		list0.clear();

		list1.push_back(e0);
		TEST_CHECK(list1.size() == 1);
		list1.clear();

		TEST_CHECK(list0.size() == 0);
		list0.push_back(e0);
		TEST_CHECK(content(list0) == "0");
		TEST_CHECK(list0.size() == 1);
		list0.push_back(e1);
		TEST_CHECK(content(list0) == "01");
		TEST_CHECK(list0.size() == 2);
		list0.push_back(e2);
		TEST_CHECK(content(list0) == "012");
		TEST_CHECK(list0.size() == 3);
		list0.push_back(e3);
		TEST_CHECK(content(list0) == "0123");
		TEST_CHECK(list0.size() == 4);
		list0.push_back(e4);
		TEST_CHECK(content(list0) == "01234");
		TEST_CHECK(list0.size() == 5);

		list0.push_back(e0);
		TEST_CHECK(list0.size() == 5);
		list0.push_back(e1);
		TEST_CHECK(list0.size() == 5);
		list0.push_back(e2);
		TEST_CHECK(list0.size() == 5);
		list0.push_back(e3);
		TEST_CHECK(list0.size() == 5);
		list0.push_back(e4);
		TEST_CHECK(list0.size() == 5);
	}

	{
		{
			List0 list0;
			{
				Element e0;
				Element e1;
				list0.push_back(e0);
				list0.push_back(e1);
				TEST_CHECK(list0.size() == 2);
			}

			TEST_CHECK(list0.size() == 0);
			TEST_CHECK(list0.empty());
		}

		{
			Element e2;
			Element e3;
			Element e4;
			List0 list0;
			List1 list1;
			{
				Element e0;
				Element e1;
				list0.push_back(e0);
				list0.push_back(e1);
				list0.push_back(e2);
				list0.push_back(e3);
				list0.push_back(e4);

				list1.push_back(e0);
				list1.push_back(e1);
				list1.push_back(e2);
				list1.push_back(e3);
				
				TEST_CHECK(list0.size() == 5);
				TEST_CHECK(list1.size() == 4);
			}

			TEST_CHECK(list0.size() == 3);
			TEST_CHECK(list1.size() == 2);
		}
	}
}

TEST_CASE(CircularBuffer)
{
	constexpr int size = 11;
	circular_buffer<int, size> buf;
	TEST_CHECK(buf.size() == 0);
	buf.step() = 0;
	TEST_CHECK(buf.size() == 1);

	for (int i = 1; i < size; ++i)
	{
		buf.step() = i;
	}

	TEST_CHECK(buf.size() == size);

	for (int i = buf.begin_index(), j = 0; i < buf.end_index(); ++i, ++j)
	{
		auto v = buf[i];
		auto ref = j;
		TEST_CHECK(ref == v);
	}
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