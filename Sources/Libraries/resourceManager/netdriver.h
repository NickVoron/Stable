#pragma once

#ifdef NET_RES_DRIVER
#include "postal/library.include.h"
#include "strings/library.include.h"
#include "postalNetworkInterfaces/library.include.h"

enum RESOURCES_EVENT
{
	RESOURCES_EVENT_START,
	RESOURCES_EVENT_DATA_EXCHANGE,
	RESOURCES_EVENT_WORK,
	RESOURCES_EVENT_STOP_WORK,
};

namespace postal
{
	template<> inline RESOURCES_EVENT initial<RESOURCES_EVENT>() { return RESOURCES_EVENT_START; }
}

namespace Resources
{
	class Resource;
	struct ResourceNetQuery
	{
		Resource* target;
		int platformID;
		int resourceTypeID;
		str::string256 fileName;
	};

	typedef std::vector<ResourceNetQuery> ResourceNetQueries;


	using MessageIOStream = stream::smemstream<1024>;
	using MessageIStream = stream::simemstream<1024>;

	namespace client { struct TransitionsList; }
	namespace server { struct TransitionsList; }


	//typedef postal::ApplicationTypes<postal::NetworkAddress, MessageIOStream, MessageIStream, client::TransitionsList, server::TransitionsList, postal::ConnectionSimulator, postal::ConnectionsSimulator, RESOURCES_EVENT> ApplicationBase;
	//typedef postal::ApplicationTypes<postal::NetworkAddress, MessageIOStream, MessageIStream, client::TransitionsList, server::TransitionsList, postal::UDTConnection, postal::UDTConnections, RESOURCES_EVENT> ApplicationBase;
	//typedef postal::ApplicationTypes<postal::NetworkAddress, MessageIOStream, MessageIStream, client::TransitionsList, server::TransitionsList, postal::UDPConnection, postal::UDPConnections, RESOURCES_EVENT> ApplicationBase;
	typedef postal::ApplicationTypes<postal::NetworkAddress, MessageIOStream, MessageIStream, client::TransitionsList, server::TransitionsList, postal::UTPConnection, postal::UTPConnections, RESOURCES_EVENT> ApplicationBase;
	typedef ApplicationBase::ClientConnection ClientConnection;
	typedef ApplicationBase::ServerConnection ServerConnection;
	typedef ApplicationBase::ClientConnections ClientConnections;
	typedef ApplicationBase::ServerConnections ServerConnections;

	namespace client
	{
		struct Work;
		struct WorldDataExchange;

		struct TransitionsList : public Loki::MakeTypelist
			<
			FSM_TRANSITION(ApplicationBase::ClientStart, WorldDataExchange, RESOURCES_EVENT_START),
			FSM_TRANSITION(WorldDataExchange, Work, RESOURCES_EVENT_WORK)
			> {};
	}

	namespace server
	{
		struct Work;
		struct WorldDataExchange;

		struct TransitionsList : public Loki::MakeTypelist
			<
			FSM_TRANSITION(ApplicationBase::ServerStart, WorldDataExchange, RESOURCES_EVENT_START),
			FSM_TRANSITION(WorldDataExchange, Work, RESOURCES_EVENT_WORK),
			FSM_TRANSITION(Work, ApplicationBase::ServerStart, RESOURCES_EVENT_STOP_WORK)
			> {};
	}

	namespace client
	{
		struct WorldDataExchange : public ApplicationBase::ClientStateBase
		{
			virtual void enter(ClientConnection& connection) override;
			virtual void exit(ClientConnection& connection) override;
			virtual void internalMessage(MessageIOStream& is, ClientConnection& connection) override;
			virtual void externalMessage(MessageIStream& is, ClientConnection& connection) override;
		};

		struct Work : public ApplicationBase::ClientStateBase
		{
			virtual void enter(ClientConnection& connection) override;
			virtual void exit(ClientConnection& connection) override;
			virtual void internalMessage(MessageIOStream& is, ClientConnection& connection) override;
			virtual void externalMessage(MessageIStream& is, ClientConnection& connection) override;
		};
	}

	//
	//
	//
	namespace server
	{
		struct WorldDataExchange : public ApplicationBase::ServerStateBase
		{
			virtual void enter(ServerConnection& connection) override;
			virtual void exit(ServerConnection& connection) override;
			virtual void internalMessage(MessageIOStream& is, ServerConnection& connection) override;
			virtual void externalMessage(MessageIStream& is, ServerConnection& connection) override;
		};

		struct Work : public ApplicationBase::ServerStateBase
		{
			virtual void enter(ServerConnection& connection) override;
			virtual void exit(ServerConnection& connection) override;
			virtual void internalMessage(MessageIOStream& is, ServerConnection& connection) override;
			virtual void externalMessage(MessageIStream& is, ServerConnection& connection) override;

			typedef std::map<ServerConnection*, std::vector<Resources::ResourceNetQuery>> ClientQueries;
			void executeQuery(const Resources::ResourceNetQuery& query, ServerConnection& connection);

			ClientQueries clientQueries;
		};
	}

	struct HostApplication : public postal::Application<ApplicationBase, server::TransitionsList, client::TransitionsList>
	{
		HostApplication();

		void net_host(uint16_t port);
		void net_server(uint16_t port);
		void net_client(const Address& address);

		template<class ResourceT>
		void query(ResourceT* resource, int platformID, const char* fileName)
		{
			queries.send({ resource, platformID, resource->typeId(), fileName });
		}

		mt::waitable_queue<ResourceNetQuery> queries;
	};
}

#endif