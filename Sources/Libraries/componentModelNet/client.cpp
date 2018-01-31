// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "client.h"
#include "server.h"

namespace ComponentModel {
namespace client {

	
	
	
	void WorldDataExchange::enter(udtnet::Connection* connection)
	{
		

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

	
	
	
	void Work::enter(udtnet::Connection* connection)
	{
		
	}

	void Work::exit(udtnet::Connection* connection)
	{

	}

	void Work::internalMessage(udtnet::IOStream& is, udtnet::Connection* connection)
	{
		
	}

	void Work::externalMessage(udtnet::IStream& is, udtnet::Connection* connection)
	{	  
		
		parentFSM->additionalData.syncronize(is);
	}


}}



// Copyright (C) 2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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