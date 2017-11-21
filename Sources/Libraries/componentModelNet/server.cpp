#include "client.h"
#include "server.h"

namespace ComponentModel {
namespace server {

	//
	//
	//
	void WorldDataExchange::enter(udtnet::Connection* connection)
	{
		LOG_MSG("SERVER::WorldDataExchange::enter");

		SendEvent::SEND_FUNC(network::COMPONENT_MODEL_EVENT_DATA_EXCHANGE, parentFSM->additionalData.clientData, [this](udtnet::IOStream& ios)
		{
			parentFSM->additionalData.load("test/categories.desc");
		}, connection);
	}

	void WorldDataExchange::exit(udtnet::Connection* connection)
	{

	}

	void WorldDataExchange::internalMessage(udtnet::IOStream& is, udtnet::Connection* connection)
	{

	}

	void WorldDataExchange::externalMessage(udtnet::IStream& is, udtnet::Connection* connection)
	{

	}

	//
	//
	//
	void Work::enter(udtnet::Connection* connection)
	{
		//LOG_MSG("SERVER::Work::enter()");

	}

	void Work::exit(udtnet::Connection* connection)
	{

	}

	void Work::internalMessage(udtnet::IOStream& is, udtnet::Connection* connection)
	{
		//LOG_MSG("SERVER::Work::internalMessage()");
		SendEvent::SEND_FUNC(network::COMPONENT_MODEL_EVENT_WORK, is, [this, is](udtnet::IOStream& is)
		{
			parentFSM->additionalData.syncronize((stream::ostream&)is);
		}, connection);
	}

	void Work::externalMessage(udtnet::IStream& is, udtnet::Connection* connection)
	{
		//LOG_MSG("SERVER::Work::externalMessage(stream::istream& is)");
	}


}}
