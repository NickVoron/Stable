#include "client.h"
#include "server.h"

namespace ComponentModel {
namespace client {

	//
	//
	//
	void WorldDataExchange::enter(udtnet::Connection* connection)
	{
		//LOG_MSG("CLIENT::WorldDataExchange::enter");

	}

	void WorldDataExchange::exit(udtnet::Connection* connection)
	{

	}

	void WorldDataExchange::internalMessage(udtnet::IOStream& is, udtnet::Connection* connection)
	{

	}

	void WorldDataExchange::externalMessage(udtnet::IStream& is, udtnet::Connection* connection)
	{
		LOG_MSG("CLIENT::WorldDataExchange::externalMessage: " << connection);
 		parentFSM->additionalData.load(is);
 
   		RecvEvent event;
   		event.header.eventcls = network::EVENT_CLASS::CHANGE_STATE_NO_WAIT;
   		event.header.event = network::COMPONENT_MODEL_EVENT_WORK;
   		parentFSM->internal_post(event);
	}

	//
	//
	//
	void Work::enter(udtnet::Connection* connection)
	{
		//LOG_MSG("CLIENT::Work::enter()");	
	}

	void Work::exit(udtnet::Connection* connection)
	{

	}

	void Work::internalMessage(udtnet::IOStream& is, udtnet::Connection* connection)
	{
		//LOG_MSG("CLIENT::Work::internalMessage()");
	}

	void Work::externalMessage(udtnet::IStream& is, udtnet::Connection* connection)
	{	  
		//LOG_MSG("CLIENT::Work::externalMessage()");
		parentFSM->additionalData.syncronize(is);
	}


}}