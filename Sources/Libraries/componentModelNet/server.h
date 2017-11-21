#pragma once

#include "defaultLogs/library.include.h"
#include "networkApplication/library.include.h"

#include "common/fsm.h"

#include "support.h"

namespace ComponentModel {
namespace server {

	struct StateBase : public network::BaseStateT<network::COMPONENT_MODEL_EVENT, WorldMaster> {};
	struct Start : public network::StartState<StateBase> {};

	struct WorldDataExchange : public StateBase
	{
		virtual void enter(udtnet::Connection* connection);
		virtual void exit(udtnet::Connection* connection);
		virtual void internalMessage(udtnet::IOStream& is, udtnet::Connection* connection);
		virtual void externalMessage(udtnet::IStream& is, udtnet::Connection* connection);
	};

	struct Work : public StateBase
	{
		virtual void enter(udtnet::Connection* connection);
		virtual void exit(udtnet::Connection* connection);
		virtual void internalMessage(udtnet::IOStream& is, udtnet::Connection* connection);
		virtual void externalMessage(udtnet::IStream& is, udtnet::Connection* connection);
	};

	//
	//
	//
	typedef Loki::MakeTypelist
		<
			FSM_TRANSITION(Start, WorldDataExchange, network::COMPONENT_MODEL_EVENT, network::COMPONENT_MODEL_EVENT_START),
			FSM_TRANSITION(WorldDataExchange, Work, network::COMPONENT_MODEL_EVENT, network::COMPONENT_MODEL_EVENT_WORK),
			FSM_TRANSITION(Work, Start, network::COMPONENT_MODEL_EVENT, network::COMPONENT_MODEL_EVENT_STOP_WORK)
		>
		TransitionsList;
}
}