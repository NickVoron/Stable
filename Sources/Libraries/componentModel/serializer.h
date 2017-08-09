// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


struct SerializerInterface;

struct Serializer
{
	struct ImplBase
	{
		virtual void serialize(SerializerInterface& component, int shift, serializer& os) = 0;
	};

	template<class T>
	struct Impl : public ImplBase
	{
		virtual void serialize(SerializerInterface& component, int shift, serializer& os)
		{
			os << (*((T*) ((const char*) &component + shift)));
		}
	};

	struct info
	{
		struct Place { char place[sizeof(Impl<int>)]; };

		template<class T>
		info(SerializerInterface& component, T* field)
		{
			static_assert(sizeof(Impl<T>) == sizeof(Impl<int>), "");
			impl = (ImplBase*) &place;
			impl = new (impl) Impl<T>();
			shift = (int)pointers_distance(field, &component);
		}

		info(const info& inf)
		{
			impl = (ImplBase*)&place;
			place = inf.place;
			shift = inf.shift;
		}

		ImplBase* impl;
		Place place;
		int shift;

		void serialize(SerializerInterface& component, serializer& os) { impl->serialize(component, shift, os); }
	};

	void serialize(SerializerInterface& component, serializer& os)
	{
		for (auto& entry : entries)
		{
			entry.serialize(component, os);
		}
	}

	template<class... T0>
	void add(SerializerInterface& component, T0*... field)
	{
		info data[]{ info(component, field)... };
		entries.insert(std::end(entries), std::begin(data), std::end(data));
	}

	std::vector<info> entries;
};



// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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