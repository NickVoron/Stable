// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "defaultLogs/library.include.h"
#include "multimethods2/library.include.h"

#include <string>
#include <memory>
#include <map>

#include <boost/any.hpp>

struct mirror
{
	template<class AccessorType, AccessorType accessor_>
	struct MemberAccessor
	{
		MemberAccessor(const char* name) : name(name) {}
		const char* name;
		static AccessorType accessor() { return accessor_; }
	};

	template<class Struct>
	struct AccessorHolder
	{
		AccessorHolder(const std::string& name, std::size_t index) : name(name), index(index) {}
		virtual ~AccessorHolder() {}

		
		virtual void get(const Struct& source, void* data, std::size_t dataSize) const = 0;
		virtual void set(Struct& dest, const void* data, std::size_t dataSize) = 0;
		virtual boost::any get(Struct& source) const = 0;

		virtual std::unique_ptr<AccessorHolder> clone() const = 0;

		const std::string name;
		const std::size_t index;
	};

	template<class Struct, class AccessorType>
	struct AccessorHolderT : public AccessorHolder<Struct>
	{
		AccessorHolderT(const std::string& name, std::size_t index, AccessorType accessor) : AccessorHolder<Struct>(name, index), accessor(accessor) {}

		virtual void get(const Struct& source, void* data, std::size_t dataSize) const override
		{
			typedef std::remove_const<std::remove_reference<decltype(source.*accessor)>::type>::type ValueType;
			ENFORCE(dataSize == sizeof(ValueType));
			*(ValueType*)data = source.*accessor;
		}

		virtual boost::any get(Struct& source) const override
		{
			return boost::any(&(source.*accessor));
		}

		virtual void set(Struct& dest, const void* data, std::size_t dataSize) override
		{
			typedef std::remove_reference<decltype(dest.*accessor)>::type  ValueType;
			ENFORCE(dataSize == sizeof(ValueType));
			dest.*accessor = *(ValueType*)data;
		}

		virtual std::unique_ptr<AccessorHolder<Struct>> clone() const override
		{
			return std::unique_ptr<AccessorHolder<Struct>>(new AccessorHolderT(this->name, this->index, accessor));
		}

		AccessorType accessor;
	};

	struct runtime
	{
		struct ValueInterface
		{
			virtual ~ValueInterface() {}
			virtual void get(void* data, std::size_t dataSize) const = 0;
			virtual void set(const void* data, std::size_t dataSize) = 0;
		};

		template<class Struct>
		struct ValueInterfaceT : public ValueInterface
		{
			ValueInterfaceT(Struct& target, AccessorHolder<Struct>* accessor) :target(target), accessor(accessor) {}

			virtual void get(void* data, std::size_t dataSize) const
			{
				accessor->get(target, data, dataSize);
			}

			virtual void set(const void* data, std::size_t dataSize)
			{
				accessor->set(target, data, dataSize);
			}

			Struct& target;
			AccessorHolder<Struct>* accessor;
		};


		class Type : public multimethods2::BaseClient
		{
		public:
			virtual ~Type() {}

			template<class Value>
			Type& set(const std::string& name, const Value& output)
			{
				if (auto value = valueInterface(name))
				{
					value->set(&output, sizeof(output));
				}

				return *this;
			}

			template<class Value>
			Type& get(const std::string& name, Value& output)
			{
				if (auto value = valueInterface(name))
				{
					value->get(&output, sizeof(output));
				}

				return *this;
			}

			template<class Value>
			Type& get(Value& output) const
			{
				get_data(&output, sizeof(output));
				return (Type&)*this;
			}

			template<class Value>
			Type& set(const Value& output)
			{
				set_data(&output, sizeof(output));
				return *this;
			}

			virtual std::pair<bool, std::size_t> index(const std::string& name) = 0;

			virtual std::unique_ptr<Type> clone() const = 0;

		private:
			virtual void get_data(void* data, std::size_t dataSize) const = 0;
			virtual void set_data(const void* data, std::size_t dataSize) = 0;
			virtual std::unique_ptr<ValueInterface> valueInterface(const std::string& name) = 0;
		};

		static auto& types()
		{
			static std::map<std::string, Type*> holder;
			return holder;
		}

		static std::unique_ptr<Type> type(const std::string& name)
		{
			auto it = types().find(name);
			return (it != types().end()) ? it->second->clone() : std::unique_ptr<Type>(nullptr);
		}
	};

	template<class Struct>
	class Type : public multimethods2::ClientT<Type<Struct>, runtime::Type>
	{
	public:
		Type() {}
		Type(const Struct& value) { operator=(value); }
		Type(const Type& value) { operator=(value); }

		Type& operator=(const Type& source)
		{
			typeName = source.typeName;
			value = source.value;

			accessors.clear();

			for (auto& val : source.accessors)
			{
				accessors.try_emplace(val.first, val.second->clone());
			}

			return *this;
		}

		operator Struct() const { return value; };
		Type& operator=(const Struct& v) { value = v; return *this; }
		Struct& val() { return value; }

		template<class Value> 
		Type& get(const std::size_t& index, Value& output)
		{
			if (auto getter = accessor(index))
				getter->get(value, &output, sizeof(output));

			return *this;
		}

		template<class Value> 
		Type& set(std::size_t index, const Value& output)
		{
			if (auto setter = accessor(index))
				setter->set(value, &output, sizeof(output));

			return *this;
		}

		void member(std::size_t index, Struct& output, boost::any& result)
		{
			if (auto a = accessor(index))
				result = a->get(output);
		}

		template<class Accessor> 
		Type& add(const std::string& name, Accessor accessor)
		{
			
			accessors.try_emplace(name, new AccessorHolderT<Struct, Accessor>(name, accessors.size(), accessor));
			return *this;
		}

		void name(const std::string& n) { typeName = n; }
		const std::string& name() const { return typeName; }
		std::size_t size() const { return accessors.size(); }

		virtual std::unique_ptr<runtime::Type> clone() const override
		{
			return std::unique_ptr<Type>(new Type(*this));
		}

		virtual std::pair<bool, std::size_t> index(const std::string& name) override
		{
			auto it = accessors.find(name);
			std::pair<bool, std::size_t> result;
			result.first = (it != accessors.end());
			if (result.first)
			{
				result.second = it->second->index;
			}

			return  result;
		}

	private:
		virtual std::unique_ptr<runtime::ValueInterface> valueInterface(const std::string& name)
		{
			auto acr = accessor(name);
			return std::unique_ptr<runtime::ValueInterface>(acr ? (new runtime::ValueInterfaceT<Struct>(value, acr)) : nullptr);
		}

		AccessorHolder<Struct>* accessor(const std::string& name)
		{
			auto it = accessors.find(name);
			return (it != accessors.end()) ? it->second.get() : nullptr;
		}

		AccessorHolder<Struct>* accessor(std::size_t index)
		{
			for (auto& it : accessors)
			{
				if (it.second->index == index)
				{
					return it.second.get();
				}
			}
			
			return nullptr;
		}

		virtual void get_data(void* data, std::size_t dataSize) const
		{
			ENFORCE(dataSize = sizeof(value));
			*(Struct*)data = value;
		}

		virtual void set_data(const void* data, std::size_t dataSize)
		{
			ENFORCE(dataSize = sizeof(value));
			value = *(Struct*)data;
		}

		std::string typeName;
		Struct value;
		std::map<std::string, std::unique_ptr<AccessorHolder<Struct>>> accessors;
	};

	template<class Struct>
	struct TypeBuilder
	{
		TypeBuilder(const std::string& name)
		{
			ethalon.name(name);
			runtime::types().try_emplace(name, &ethalon);
		}

		template<class Accessor>
		decltype(auto) property(const std::string& name, Accessor accessor)
		{
			ethalon.add(name, accessor);
		}

		template<class Accessor>
		decltype(auto) operator()(const std::string& name, Accessor accessor)
		{
			return property(name, accessor);
		}

		Type<Struct> ethalon;
	};

	template<class Struct>
	struct Declaration
	{
		template<class Accessor>
		auto property(const std::string& name, Accessor accessor)
		{
			builder<Struct>("").property(name, accessor);
			return Declaration();
		}

		template<class Accessor>
		auto operator()(const std::string& name, Accessor accessor)
		{
			return property(name, accessor);
		}
	};

	template<class Struct>
	static TypeBuilder<Struct>& builder(const std::string& name)
	{
		static TypeBuilder<Struct> type(name);
		return type;
	}

	template<class Struct>
	static auto declare(const std::string& name)
	{
		builder<Struct>(name);
		return Declaration<Struct>();
	}

	template<class Struct>
	static Type<Struct> type()
	{
		return builder<Struct>("").ethalon;
	}

	template<class Struct>
	static Type<Struct> type(const Struct& value)
	{
		return type<Struct>() = value;
	}
};

#define MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	auto mirror_declarator_##TYPE_NAME = mirror::declare<TYPE_NAME>(STPP_STRINGIZE(TYPE_NAME))

#define MIRROR_STRUCT_DECLARE_1(TYPE_NAME, M0) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0);

#define MIRROR_STRUCT_DECLARE_2(TYPE_NAME, M0, M1) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1);

#define MIRROR_STRUCT_DECLARE_3(TYPE_NAME, M0, M1, M2) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2);

#define MIRROR_STRUCT_DECLARE_4(TYPE_NAME, M0, M1, M2, M3) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3);

#define MIRROR_STRUCT_DECLARE_5(TYPE_NAME, M0, M1, M2, M3, M4) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3) \
	(STPP_STRINGIZE(M4), &TYPE_NAME::M4);

#define MIRROR_STRUCT_DECLARE_6(TYPE_NAME, M0, M1, M2, M3, M4, M5) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3) \
	(STPP_STRINGIZE(M4), &TYPE_NAME::M4) \
	(STPP_STRINGIZE(M5), &TYPE_NAME::M5);

#define MIRROR_STRUCT_DECLARE_7(TYPE_NAME, M0, M1, M2, M3, M4, M5, M6) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3) \
	(STPP_STRINGIZE(M4), &TYPE_NAME::M4) \
	(STPP_STRINGIZE(M5), &TYPE_NAME::M5) \
	(STPP_STRINGIZE(M6), &TYPE_NAME::M6);

#define MIRROR_STRUCT_DECLARE_8(TYPE_NAME, M0, M1, M2, M3, M4, M5, M6, M7) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3) \
	(STPP_STRINGIZE(M4), &TYPE_NAME::M4) \
	(STPP_STRINGIZE(M5), &TYPE_NAME::M5) \
	(STPP_STRINGIZE(M6), &TYPE_NAME::M6) \
	(STPP_STRINGIZE(M7), &TYPE_NAME::M7);

#define MIRROR_STRUCT_DECLARE_9(TYPE_NAME, M0, M1, M2, M3, M4, M5, M6, M7, M8) \
	MIRROR_TYPE_DECLARATOR(TYPE_NAME) \
	(STPP_STRINGIZE(M0), &TYPE_NAME::M0) \
	(STPP_STRINGIZE(M1), &TYPE_NAME::M1) \
	(STPP_STRINGIZE(M2), &TYPE_NAME::M2) \
	(STPP_STRINGIZE(M3), &TYPE_NAME::M3) \
	(STPP_STRINGIZE(M4), &TYPE_NAME::M4) \
	(STPP_STRINGIZE(M5), &TYPE_NAME::M5) \
	(STPP_STRINGIZE(M6), &TYPE_NAME::M6) \
	(STPP_STRINGIZE(M7), &TYPE_NAME::M7) \
	(STPP_STRINGIZE(M8), &TYPE_NAME::M8);

#define MIRROR_STRUCT_DECLARE(...) MACRO_ID(GET_MACRO(__VA_ARGS__, \
	MIRROR_STRUCT_DECLARE_9, \
	MIRROR_STRUCT_DECLARE_8, \
	MIRROR_STRUCT_DECLARE_7, \
	MIRROR_STRUCT_DECLARE_6, \
	MIRROR_STRUCT_DECLARE_5, \
	MIRROR_STRUCT_DECLARE_4, \
	MIRROR_STRUCT_DECLARE_3, \
	MIRROR_STRUCT_DECLARE_2, \
	MIRROR_STRUCT_DECLARE_1 \
	)(__VA_ARGS__))



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