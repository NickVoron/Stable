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
			//parameter<0>().push_back(typeid(*this).name());
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
			//parameter<0>().push_back(typeid(*this).name());
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
			//parameter<0>().push_back(typeid(*this).name());
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

}//
#endif