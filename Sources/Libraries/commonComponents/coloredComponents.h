#pragma once

#include "componentModel/library.include.h"

#include "timer.h"

class Red : public UpdatableComponent<Red>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Red);

	struct Resource : public ResourceBase 
	{
		void properties(Red& component);
	};
	
	void update(float dt);

	Vector3 color;
};

class Blue : public UpdatableComponent<Blue>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Blue);

	struct Resource : public ResourceBase 
	{
		void properties(Blue& component);
	};

	void update(float dt);

	Vector3 color;
};

class Green : public UpdatableComponent<Green>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Green);

	struct Resource : public ResourceBase 
	{
		void properties(Green& component);
	};

	void update(float dt);

	std::string parameter;
};

class RedTimer : public BaseTimer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(RedTimer);

protected:
	virtual void tick() override;
};

class BlueTimer : public BaseTimer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(BlueTimer);

protected:
	virtual void tick() override;
};