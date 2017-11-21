#include "os_win_err.h"

#include "defaultLogs/library.include.h"

#ifdef WIN32
#include <WS2tcpip.h>
#endif

namespace net
{
	bool error(int errCode, const char* t)
	{
#ifdef WIN32
		if(errCode == 0 || errCode == WSAEWOULDBLOCK) return true;

		switch(errCode)
		{
		case WSAEINTR:				printf("Error at %s: WSAEINTR: A blocking operation was interrupted by a call to WSACancelBlockingCall\n", t);	return true;
		case WSAEBADF:				printf("Error at %s: WSAEBADF: The file handle supplied is not valid\n", t);	return false;
		case WSAEACCES:				printf("Error at %s: WSAEACCES: An attempt was made to access a socket in a way forbidden by its access permissions\n", t);	return false;
		case WSAEFAULT:				printf("Error at %s: WSAEFAULT: The system detected an invalid pointer address in attempting to use a pointer argument in a call\n", t);	return false;
		case WSAEINVAL:				printf("Error at %s: WSAEINVAL: An invalid argument was supplied\n", t);	return false;
		case WSAEWOULDBLOCK:		printf("Error at %s: WSAEWOULDBLOCK: A non-blocking socket operation could not be completed immediately\n", t);	return true;
		case WSAEINPROGRESS:		printf("Error at %s: WSAEINPROGRESS: A blocking operation is currently executing\n", t);	return true;
		case WSAEALREADY:			printf("Error at %s: WSAEALREADY: An operation was attempted on a non-blocking socket that already had an operation in progress\n", t);	return true;
		case WSAENOTSOCK:			printf("Error at %s: WSAENOTSOCK: An operation was attempted on something that is not a socket\n", t);	return false;
		case WSAEDESTADDRREQ:		printf("Error at %s: WSAEDESTADDRREQ: A required address was omitted from an operation on a socket\n", t);	return false;
		case WSAEMSGSIZE:			printf("Error at %s: WSAEMSGSIZE: A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself\n", t);	return false;
		case WSAEPROTOTYPE:			printf("Error at %s: WSAEPROTOTYPE: A protocol was specified in the socket function call that does not support the semantics of the socket type requested\n", t);	return false;
		case WSAENOPROTOOPT:		printf("Error at %s: WSAENOPROTOOPT: An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call\n", t);	return false;
		case WSAEPROTONOSUPPORT:	printf("Error at %s: WSAEPROTONOSUPPORT: The requested protocol has not been configured into the system, or no implementation for it exists\n", t);	return false;
		case WSAESOCKTNOSUPPORT:	printf("Error at %s: WSAESOCKTNOSUPPORT: The support for the specified socket type does not exist in this address family\n", t);	return false;
		case WSAEOPNOTSUPP:			printf("Error at %s: WSAEOPNOTSUPP: The attempted operation is not supported for the type of object referenced\n", t);	return false;
		case WSAEPFNOSUPPORT:		printf("Error at %s: WSAEPFNOSUPPORT: The protocol family has not been configured into the system or no implementation for it exists\n", t);	return false;
		case WSAEAFNOSUPPORT:		printf("Error at %s: WSAEAFNOSUPPORT: An address incompatible with the requested protocol was used\n", t);	return false;
		case WSAEADDRINUSE:			printf("Error at %s: WSAEADDRINUSE: Only one usage of each socket address (protocol/network address/port) is normally permitted\n", t);	return false;
		case WSAEADDRNOTAVAIL:		printf("Error at %s: WSAEADDRNOTAVAIL: The requested address is not valid in its context\n", t);	return false;
		case WSAENETDOWN:			printf("Error at %s: WSAENETDOWN: A socket operation encountered a dead network\n", t);	return false;
		case WSAENETUNREACH:		printf("Error at %s: WSAENETUNREACH: A socket operation was attempted to an unreachable network\n", t);	return false;
		case WSAENETRESET:			printf("Error at %s: WSAENETRESET: The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress\n", t);	return false;
		case WSAECONNABORTED:		printf("Error at %s: WSAECONNABORTED: An established connection was aborted by the software in your host machine\n", t);	return false;
		case WSAECONNRESET:			printf("Error at %s: WSAECONNRESET: An existing connection was forcibly closed by the remote host\n", t);	return false;
		case WSAENOBUFS:			printf("Error at %s: WSAENOBUFS: An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full\n", t);	return false;
		case WSAEISCONN:			printf("Error at %s: WSAEISCONN: A connect request was made on an already connected socket\n", t);	return true;
		case WSAENOTCONN:			printf("Error at %s: WSAENOTCONN: A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied\n", t);	return false;
		case WSAESHUTDOWN:			printf("Error at %s: WSAESHUTDOWN: A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call\n", t);	return false;
		case WSAETOOMANYREFS:		printf("Error at %s: WSAETOOMANYREFS: Too many references to some kernel object\n", t);	return false;
		case WSAETIMEDOUT:			printf("Error at %s: WSAETIMEDOUT: No connection could be made because the target machine actively refused it\n", t);	return false;
		case WSAELOOP:				printf("Error at %s: WSAELOOP: Cannot translate name\n", t);	return false;
		case WSAENAMETOOLONG:		printf("Error at %s: WSAENAMETOOLONG: Name component or name was too long\n", t);	return false;
		case WSAEHOSTDOWN:			printf("Error at %s: WSAEHOSTDOWN: A socket operation failed because the destination host was down\n", t);	return false;
		case WSAEHOSTUNREACH:		printf("Error at %s: WSAEHOSTUNREACH: A socket operation was attempted to an unreachable host\n", t);	return false;
		case WSAENOTEMPTY:			printf("Error at %s: WSAENOTEMPTY: Cannot remove a directory that is not empty\n", t);	return false;
		case WSAEPROCLIM:			printf("Error at %s: WSAEPROCLIM: A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously\n", t);	return false;
		case WSAEUSERS:				printf("Error at %s: WSAEUSERS: Ran out of quota\n", t);	return false;
		case WSAEDQUOT:				printf("Error at %s: WSAEDQUOT: Ran out of disk quota\n", t);	return false;
		case WSAESTALE:				printf("Error at %s: WSAESTALE: File handle reference is no longer available\n", t);	return false;
		case WSAEREMOTE:			printf("Error at %s: WSAEREMOTE: Item is not available locally\n", t);	return false;
		case WSASYSNOTREADY:		printf("Error at %s: WSASYSNOTREADY: WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable\n", t);	return false;
		case WSAVERNOTSUPPORTED:	printf("Error at %s: WSAVERNOTSUPPORTED: The Windows Sockets version requested is not supported\n", t);	return false;
		case WSANOTINITIALISED:		printf("Error at %s: WSANOTINITIALISED: Either the application has not called WSAStartup, or WSAStartup failed\n", t);	return false;
		case WSAEDISCON:			printf("Error at %s: WSAEDISCON: Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence\n", t);	return false;
		case WSAENOMORE:			printf("Error at %s: WSAENOMORE: No more results can be returned by WSALookupServiceNext\n", t);	return false;
		case WSAECANCELLED:			printf("Error at %s: WSAECANCELLED: A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled\n", t);	return false;
		case WSAEINVALIDPROCTABLE:	printf("Error at %s: WSAEINVALIDPROCTABLE: The procedure call table is invalid\n", t);	return false;
		case WSAEINVALIDPROVIDER:	printf("Error at %s: WSAEINVALIDPROVIDER: The requested service provider is invalid\n", t);	return false;
		case WSAEPROVIDERFAILEDINIT:printf("Error at %s: WSAEPROVIDERFAILEDINIT: The requested service provider could not be loaded or initialized\n", t);	return false;
		case WSASYSCALLFAILURE:		printf("Error at %s: WSASYSCALLFAILURE: A system call that should never fail has failed\n", t);	return false;
		case WSASERVICE_NOT_FOUND:	printf("Error at %s: WSASERVICE_NOT_FOUND: No such service is known. The service cannot be found in the specified name space.\n", t);	return false;
		case WSATYPE_NOT_FOUND:		printf("Error at %s: WSATYPE_NOT_FOUND: The specified class was not found\n", t);	return false;
		default: 					printf("Error at %s: Unknown error\n", t);	return false;
		}
#endif
        return false;
	}

	bool last_error()
	{
#ifdef WIN32
		return error(WSAGetLastError());
#else
		return true;
#endif
	}

	bool error(int errCode)
	{
		return error(errCode, "");
	}

	bool error_socket(socket_type s, const char* t)
	{
#ifdef WIN32
		return (s == SOCKET_ERROR) ? last_error() : true;
#else
		return (s != SOCKET_ERROR);
#endif
	}

	bool error_socket(socket_type s)
	{
		return error_socket(s, "");
	}

	//
	//
	//
	bool error_getaddrinfo(int errCode)
	{
		if (errCode != 0)
		{
			const char* prefix = "getaddrinfo failed: ";
			switch (errCode)
			{
#ifdef WIN32
			case EAI_AGAIN:    LOG_ERROR(prefix << "WSATRY_AGAIN"); return false;
			case EAI_BADFLAGS:    LOG_ERROR(prefix << "WSAEINVAL"); return false;
			case EAI_FAIL:    LOG_ERROR(prefix << "WSANO_RECOVERY"); return false;
			case EAI_FAMILY:    LOG_ERROR(prefix << "WSAEAFNOSUPPORT"); return false;
			case EAI_MEMORY:    LOG_ERROR(prefix << "WSA_NOT_ENOUGH_MEMORY"); return false;
			case EAI_NOSECURENAME:    LOG_ERROR(prefix << "WSA_SECURE_HOST_NOT_FOUND"); return false;
			case EAI_NONAME:    LOG_ERROR(prefix << "WSAHOST_NOT_FOUND"); return false;
			case EAI_SERVICE:    LOG_ERROR(prefix << "WSATYPE_NOT_FOUND"); return false;
			case EAI_SOCKTYPE:    LOG_ERROR(prefix << "WSAESOCKTNOSUPPORT"); return false;
			case EAI_IPSECPOLICY:    LOG_ERROR(prefix << "WSA_IPSEC_NAME_POLICY_ERROR"); return false;
#elif defined(__ANDROID__)
			case EAI_ADDRFAMILY:LOG_ERROR(prefix << "EAI_ADDRFAMILY address family for hostname not supported"); return false;
			case EAI_AGAIN:LOG_ERROR(prefix << "EAI_AGAIN	 	temporary failure in name resolution"); return false;
			case EAI_BADFLAGS:LOG_ERROR(prefix << "EAI_BADFLAGS	invalid value for ai_flags"); return false;
			case EAI_FAIL:LOG_ERROR(prefix << "EAI_FAIL	 	non-recoverable failure in name resolution"); return false;
			case EAI_FAMILY:LOG_ERROR(prefix << "EAI_FAMILY	 	ai_family not supported"); return false;
			case EAI_MEMORY:LOG_ERROR(prefix << "EAI_MEMORY	 	memory allocation failure"); return false;
			case EAI_NODATA:LOG_ERROR(prefix << "EAI_NODATA	 	no address associated with hostname"); return false;
			case EAI_NONAME:LOG_ERROR(prefix << "EAI_NONAME	 	hostname nor servname provided, or not known"); return false;
			case EAI_SERVICE:LOG_ERROR(prefix << "EAI_SERVICE	servname not supported for ai_socktype"); return false;
			case EAI_SOCKTYPE:LOG_ERROR(prefix << "EAI_SOCKTYPE	ai_socktype not supported"); return false;
			case EAI_SYSTEM:LOG_ERROR(prefix << "EAI_SYSTEM		system error returned in errno"); return false;
			case EAI_BADHINTS:LOG_ERROR(prefix << "EAI_BADHINTS	invalid value for hints"); return false;
			case EAI_PROTOCOL:LOG_ERROR(prefix << "EAI_PROTOCOL	resolved protocol is unknown"); return false;
			case EAI_OVERFLOW:LOG_ERROR(prefix << "EAI_OVERFLOW	argument buffer overflow"); return false;
			case EAI_MAX:LOG_ERROR(prefix << "EAI_MAX		"); return false;
#endif
			default:LOG_ERROR(prefix << " unknown error");			break;
			}
		}

		return true;
	}

}
