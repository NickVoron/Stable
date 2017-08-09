// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "descParser/library.include.h"
#include "componentModel/library.include.h"

namespace ComponentModel
{
	struct ComponentModelConfigurator : public ObjectParser::ComponentModelConfigurator
	{	
		struct ComponentState
		{
			template<class ValueType>
			struct ExpressionProperty
			{
				ExpressionProperty(const std::string& name_, const ValueType& expression_) : name(name_), expression(expression_) {}
				
				std::string debug() const { return str::stringize("name: ", name, " source: ", expression, " value: ", stringize(expression)).str(); }

				std::string name;
				ValueType expression;
			};

			ComponentDesc* current = nullptr;

			template<class ValueType>
			struct Properties : public std::vector<ExpressionProperty<ValueType>>
			{
				ValueType value(const std::string& name) const
				{
					auto it = std::find_if(this->begin(), this->end(), [name](auto&& property) { return name == property.name; });
					return (it != this->end()) ? it->expression : ValueType();
				}

				void debug() const
				{
					for (auto&& property : *this)
					{
						LOG_MSG(property.debug());
					}
				}
			};


			Properties<std::string> runtimeProperties;
			Properties<const Expressions::Expression*> properties;
		};


		struct ComponentHandle;

		template<class T>
		struct less_handle
		{
			bool operator()(const T& ch0, const T& ch1) const
			{	
				ENFORCE(ch0 && ch1);
				return std::tie(ch0->type, ch0) < std::tie(ch1->type, ch1);
			}
		};
		
		struct InstanceHandle : public ObjectParser::InstanceHandle
		{
			InstanceHandle(const std::string& name) : ObjectParser::InstanceHandle(name) {}
			InstanceHandle(std::size_t indexInEntityList, InstanceHandle* parent_, const std::string& name_, ClassDesc* classDesc_) :
				ObjectParser::InstanceHandle(name_),
				indexInEntityList(indexInEntityList),
				parent(parent_),
				classDesc(classDesc_)
			{
				parent->instances.push_back(this);
			}

			void debug(int& depth) const;

			auto initData() const
			{				
				std::vector<decltype(std::make_tuple(decltype(std::make_tuple(std::string(), components.begin()->first->index()))(), components.begin()->second.properties))> result;
				for (auto& componentData : components)
				{
					auto ni = std::make_tuple(componentData.first->name, componentData.first->index());
					result.push_back(std::make_tuple(ni, componentData.second.properties));
				}

				return result;
			}

			ClassDesc* classDesc = nullptr;
			InstanceHandle* parent = nullptr;
			std::size_t indexInEntityList = 0;
			std::map<ComponentHandle*, ComponentState, less_handle<ComponentHandle*> > components;
			std::vector<InstanceHandle*> instances;
		};

		struct ComponentHandle : public ObjectParser::ComponentHandle
		{
			ComponentHandle(InstanceHandle* parent, ComponentDesc* component) : parent(parent), component(component) {}
			InstanceHandle* parent = nullptr;
			ComponentDesc* component = nullptr;

			int index() const 
			{
				ENFORCE(parent && component);
				return parent->classDesc->components.index(component->name);
			}
		};

		struct ClassState 
		{
			ClassDesc* current = nullptr;
			InstanceHandle* instance = nullptr;
			ComponentState& componentState();
			std::stack<ComponentState> components;
		};

		ComponentModelConfigurator() : root("root") {}
		virtual void beginCreateInstance(const std::string& name, const std::string& type) override;
		virtual ObjectParser::InstanceHandle* endCreateInstance() override;

		virtual ObjectParser::ComponentHandle* preCreateComponent(const std::string& type, const std::string& name) override;
		virtual void beginCreateComponent(ObjectParser::ComponentHandle* handle, const std::string& type, const std::string& name) override;
		virtual void bindRuntimeProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const std::string& value) override;
		virtual void bindLink(ObjectParser::ComponentHandle* handle, const std::string& name, const ObjectParser::ComponentHandle* value) override;
		virtual void bindComponentProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const Expressions::Expression* value) override;
		virtual void endCreateComponent(ObjectParser::ComponentHandle* handle) override;

		ClassDescList classes;
		InstanceHandle root;
		InstanceHandle* currentInstance = &root;

		void debug() const;
		EntitiesList& configure(ObjectParser::Unroller& unroller, EntitiesList& entities);
		static EntitiesList& configure(ObjectParser::Unroller& unroller, const InstanceHandle& rootInstance, EntitiesList& entities);

	private:

		ClassState& classState();
		ClassDesc* currentClass();
		ComponentDesc* currentComponent();
		
		std::stack<ClassState> state;
		std::size_t instanceCounter = 0;
		std::size_t level = 0;
	};

	EntitiesList& createInstance(const ObjectParser::InstanceDefinitionExpression& instance, ObjectParser::Unroller& unroller, ComponentModelConfigurator& configurator, EntitiesList& entities, bool debug);
	void descriptionTest(const char* filename);

	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities);

	template<class StrType>
	EntitiesList& descriptionLoad(const StrType& filename, EntitiesList& entities)
	{
		return descriptionLoad(str::c_str(filename), entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		entities.execute();
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
		}		
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class Iteration>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, Iteration&& iteration)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);
		
		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			entities.execute();
			iteration(i, entities);
		}
		epilogue(entities);
	}

	template<class StrType, class Prologue, class Epilogue, class IterationBegin, class IterationEnd>
	void descriptionTest(const StrType& filename, std::size_t iterationsCount, Prologue&& prologue, Epilogue&& epilogue, IterationBegin&& iterationBegin, IterationEnd&& iterationEnd)
	{
		ENFORCE(str::c_str(filename));

		EntitiesList entities;
		descriptionLoad(str::c_str(filename), entities);
		entities.activate(true);

		prologue(entities);
		for (std::size_t i = 0; i < iterationsCount; ++i)
		{
			iterationBegin(i, entities);
			entities.execute();
			iterationEnd(i, entities);
		}
		epilogue(entities);
	}
}





// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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