// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "032.spawnerPrototypeInternalLinks.h"
#include "commonComponents/library.include.h"
#include "utils.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	struct UpdateParamsHolder
	{
		void update(std::vector<std::string>& values)
		{
		}
	};

	template<class ComponentType, class... Links>
	struct ComponentInternalLinkBase : public UpdateParamsHolder, public UpdatableComponentAutoLink<ComponentType, Links...>
	{

	};

	class ComponentInternalLinkReference : public ComponentInternalLinkBase<ComponentInternalLinkReference>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentInternalLinkReference);
		void finalize() override
		{
			
		}
	};

	class ComponentInternalLink : public ComponentInternalLinkBase<ComponentInternalLink>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentInternalLink);

		struct Resource : public ResourceBase
		{
			void properties(ComponentInternalLink& component)
			{
				property("reference", component.reference);
			}
		};

		void finalize() override
		{
			
			reference([](auto& ref)
			{
				parameter<0>().push_back(typeid(ref).name());
			});
		}

		ComponentLink<ComponentInternalLinkReference> reference;
	};

	class ComponentInternalLinkValidator : public ComponentInternalLinkBase<ComponentInternalLinkValidator, ComponentInternalLink>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentInternalLinkValidator);

		void finalize() override
		{
			ENFORCE_POINTER(component<ComponentInternalLink>().reference.component<ComponentInternalLinkReference>());
		}
	};

	class ComponentInternalLinkSpawner : public ComponentInternalLinkBase<ComponentInternalLinkSpawner, AccumulatingSpawner<ComponentInternalLink, LinearMover>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentInternalLinkSpawner);

		struct Resource : public ResourceBase
		{
			void properties(ComponentInternalLinkSpawner& component)
			{
				property("vel", component.vel);
			}
		};

		void finalize() override
		{
			
			spawn([this](ComponentInternalLink& intlink, LinearMover& linearMover)
			{
				linearMover.velocity = vel;
			});
		}

		Vector3 vel;
	};


	SpawnerPrototypeInternalLinks::SpawnerPrototypeInternalLinks()
	{
		ComponentModel::components<ComponentInternalLink, ComponentInternalLinkReference, ComponentInternalLinkSpawner, ComponentInternalLinkValidator>();

		try
		{
			std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/032.spawnerPrototypeInternalLinks.desc");
			ComponentModel::descriptionTest(path.c_str());
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

}
#endif



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