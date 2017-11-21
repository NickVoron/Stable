#pragma once

#include "componentModel/library.include.h"

class BaseTimer : public UpdatableComponent<BaseTimer>
{
public:
	CM_DEFINE_BASE_CLSID(BaseTimer)

	struct Resource : public ResourceBase 
	{
		void properties(BaseTimer& component);
	};

	void update(float dt);

protected:
	virtual void tick() = 0;

	float interval = 1.0f;
	float accumulatedTime = 0.0f;
};

template<class TickHandlerComponent, class HandlerType, HandlerType handler>
class ConsumerTimer : public AutoLink<TickHandlerComponent>, public BaseTimer
{
public:
	virtual void tick() override
	{
		//component([](TickHandlerComponent& handler) { (*handler.handler)(); });
		(ConsumerTimer::template component<TickHandlerComponent>().*handler)();
	}

	void linker()
	{
		link_tuple(this->components_tuple);
	}
};