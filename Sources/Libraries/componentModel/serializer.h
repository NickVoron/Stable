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