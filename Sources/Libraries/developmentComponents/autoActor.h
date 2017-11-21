#pragma once

#include "baseActor.h"

#include "componentModel/library.include.h"
#include "common/waitable_queue.h"


template<template <class> class ActorInterface, class Component, class... Links>
class AutoActorOnce : public UpdatableComponentAutoLink<Component, Links...>
{
public:
	void update()
	{
		if (!executed)
		{
			ActorBase<ActorInterface, Links...>::action(this);
			executed = true;
		}
	}

private:
	bool executed = false;
};

template<template <class> class ActorInterface, class MessageType, class Component, class... Links>
class OnMessageActor : public UpdatableComponentAutoLink<Component, Links...>
{
public:
	void update(mt::waitable_queue<MessageType>& queue)
	{
		MessageType value;
		if(queue.try_receive(value))
		{
			((Component*)this)->receive(value);
			ActorBase<ActorInterface, Links...>::action(this, value);
		}		
	}
};