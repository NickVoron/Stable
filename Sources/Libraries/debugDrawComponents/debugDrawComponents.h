#pragma once

#include "commonComponents/library.include.h"
#include "image/library.include.h"

namespace DebugDrawComponents
{

class BaseDebugDrawer : public UpdatableComponent<BaseDebugDrawer>
{
public:
	CM_DEFINE_BASE_CLSID(BaseDebugDrawer)

	virtual void update() = 0;
	void linker();

protected:
	const Position* position;
};

//
class PositionDrawer : public BaseDebugDrawer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(PositionDrawer);
	virtual void update() override;
};

class SphereDrawer : public BaseDebugDrawer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SphereDrawer);

	struct Resource : public ResourceBase
	{
		void properties(SphereDrawer& component);
	};

	SphereDrawer();
	virtual void update() override;

	float radius = 1.0f;
	image::Color<image::RGBA32F> color;
	std::string text;
};

typedef ComponentModel::Module<BaseDebugDrawer, PositionDrawer, SphereDrawer> Module;
}

namespace properties_inheritance
{
	void inherite(DebugDrawComponents::SphereDrawer& parent, DebugDrawComponents::SphereDrawer& child);
};



