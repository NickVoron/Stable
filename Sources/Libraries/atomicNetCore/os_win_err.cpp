// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "os_win_err.h"

#ifdef WIN32
#include <WS2tcpip.h>
#endif

namespace net
{
	bool error(int errCode, const char* t)
	{
#ifdef WIN32
		if(errCode == 0 || errCode == WSAEWOULDBLOCK) return true;

		switch (errCode)
		{
		case WSAEINTR:				LOG_ERROR("Error at" << t << ": WSAEINTR: A blocking operation was interrupted by a call to WSACancelBlockingCall");	return true;
		case WSAEBADF:				LOG_ERROR("Error at" << t << ": WSAEBADF: The file handle supplied is not valid");	return false;
		case WSAEACCES:				LOG_ERROR("Error at" << t << ": WSAEACCES: An attempt was made to access a socket in a way forbidden by its access permissions");	return false;
		case WSAEFAULT:				LOG_ERROR("Error at" << t << ": WSAEFAULT: The system detected an invalid pointer address in attempting to use a pointer argument in a call");	return false;
		case WSAEINVAL:				LOG_ERROR("Error at" << t << ": WSAEINVAL: An invalid argument was supplied");	return false;
		case WSAEWOULDBLOCK:		LOG_ERROR("Error at" << t << ": WSAEWOULDBLOCK: A non-blocking socket operation could not be completed immediately");	return true;
		case WSAEINPROGRESS:		LOG_ERROR("Error at" << t << ": WSAEINPROGRESS: A blocking operation is currently executing");	return true;
		case WSAEALREADY:			LOG_ERROR("Error at" << t << ": WSAEALREADY: An operation was attempted on a non-blocking socket that already had an operation in progress");	return true;
		case WSAENOTSOCK:			LOG_ERROR("Error at" << t << ": WSAENOTSOCK: An operation was attempted on something that is not a socket");	return false;
		case WSAEDESTADDRREQ:		LOG_ERROR("Error at" << t << ": WSAEDESTADDRREQ: A required address was omitted from an operation on a socket");	return false;
		case WSAEMSGSIZE:			LOG_ERROR("Error at" << t << ": WSAEMSGSIZE: A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself");	return false;
		case WSAEPROTOTYPE:			LOG_ERROR("Error at" << t << ": WSAEPROTOTYPE: A protocol was specified in the socket function call that does not support the semantics of the socket type requested");	return false;
		case WSAENOPROTOOPT:		LOG_ERROR("Error at" << t << ": WSAENOPROTOOPT: An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call");	return false;
		case WSAEPROTONOSUPPORT:	LOG_ERROR("Error at" << t << ": WSAEPROTONOSUPPORT: The requested protocol has not been configured into the system, or no implementation for it exists");	return false;
		case WSAESOCKTNOSUPPORT:	LOG_ERROR("Error at" << t << ": WSAESOCKTNOSUPPORT: The support for the specified socket type does not exist in this address family");	return false;
		case WSAEOPNOTSUPP:			LOG_ERROR("Error at" << t << ": WSAEOPNOTSUPP: The attempted operation is not supported for the type of object referenced");	return false;
		case WSAEPFNOSUPPORT:		LOG_ERROR("Error at" << t << ": WSAEPFNOSUPPORT: The protocol family has not been configured into the system or no implementation for it exists");	return false;
		case WSAEAFNOSUPPORT:		LOG_ERROR("Error at" << t << ": WSAEAFNOSUPPORT: An address incompatible with the requested protocol was used");	return false;
		case WSAEADDRINUSE:			LOG_ERROR("Error at" << t << ": WSAEADDRINUSE: Only one usage of each socket address (protocol/network address/port) is normally permitted");	return false;
		case WSAEADDRNOTAVAIL:		LOG_ERROR("Error at" << t << ": WSAEADDRNOTAVAIL: The requested address is not valid in its context");	return false;
		case WSAENETDOWN:			LOG_ERROR("Error at" << t << ": WSAENETDOWN: A socket operation encountered a dead network");	return false;
		case WSAENETUNREACH:		LOG_ERROR("Error at" << t << ": WSAENETUNREACH: A socket operation was attempted to an unreachable network");	return false;
		case WSAENETRESET:			LOG_ERROR("Error at" << t << ": WSAENETRESET: The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress");	return false;
		case WSAECONNABORTED:		LOG_ERROR("Error at" << t << ": WSAECONNABORTED: An established connection was aborted by the software in your host machine");	return false;
		case WSAECONNRESET:			LOG_ERROR("Error at" << t << ": WSAECONNRESET: An existing connection was forcibly closed by the remote host");	return false;
		case WSAENOBUFS:			LOG_ERROR("Error at" << t << ": WSAENOBUFS: An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full");	return false;
		case WSAEISCONN:			LOG_ERROR("Error at" << t << ": WSAEISCONN: A connect request was made on an already connected socket");	return true;
		case WSAENOTCONN:			LOG_ERROR("Error at" << t << ": WSAENOTCONN: A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied");	return false;
		case WSAESHUTDOWN:			LOG_ERROR("Error at" << t << ": WSAESHUTDOWN: A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call");	return false;
		case WSAETOOMANYREFS:		LOG_ERROR("Error at" << t << ": WSAETOOMANYREFS: Too many references to some kernel object");	return false;
		case WSAETIMEDOUT:			LOG_ERROR("Error at" << t << ": WSAETIMEDOUT: No connection could be made because the target machine actively refused it");	return false;
		case WSAELOOP:				LOG_ERROR("Error at" << t << ": WSAELOOP: Cannot translate name");	return false;
		case WSAENAMETOOLONG:		LOG_ERROR("Error at" << t << ": WSAENAMETOOLONG: Name component or name was too long");	return false;
		case WSAEHOSTDOWN:			LOG_ERROR("Error at" << t << ": WSAEHOSTDOWN: A socket operation failed because the destination host was down");	return false;
		case WSAEHOSTUNREACH:		LOG_ERROR("Error at" << t << ": WSAEHOSTUNREACH: A socket operation was attempted to an unreachable host");	return false;
		case WSAENOTEMPTY:			LOG_ERROR("Error at" << t << ": WSAENOTEMPTY: Cannot remove a directory that is not empty");	return false;
		case WSAEPROCLIM:			LOG_ERROR("Error at" << t << ": WSAEPROCLIM: A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously");	return false;
		case WSAEUSERS:				LOG_ERROR("Error at" << t << ": WSAEUSERS: Ran out of quota");	return false;
		case WSAEDQUOT:				LOG_ERROR("Error at" << t << ": WSAEDQUOT: Ran out of disk quota");	return false;
		case WSAESTALE:				LOG_ERROR("Error at" << t << ": WSAESTALE: File handle reference is no longer available");	return false;
		case WSAEREMOTE:			LOG_ERROR("Error at" << t << ": WSAEREMOTE: Item is not available locally");	return false;
		case WSASYSNOTREADY:		LOG_ERROR("Error at" << t << ": WSASYSNOTREADY: WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable");	return false;
		case WSAVERNOTSUPPORTED:	LOG_ERROR("Error at" << t << ": WSAVERNOTSUPPORTED: The Windows Sockets version requested is not supported");	return false;
		case WSANOTINITIALISED:		LOG_ERROR("Error at" << t << ": WSANOTINITIALISED: Either the application has not called WSAStartup, or WSAStartup failed");	return false;
		case WSAEDISCON:			LOG_ERROR("Error at" << t << ": WSAEDISCON: Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence");	return false;
		case WSAENOMORE:			LOG_ERROR("Error at" << t << ": WSAENOMORE: No more results can be returned by WSALookupServiceNext");	return false;
		case WSAECANCELLED:			LOG_ERROR("Error at" << t << ": WSAECANCELLED: A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled");	return false;
		case WSAEINVALIDPROCTABLE:	LOG_ERROR("Error at" << t << ": WSAEINVALIDPROCTABLE: The procedure call table is invalid");	return false;
		case WSAEINVALIDPROVIDER:	LOG_ERROR("Error at" << t << ": WSAEINVALIDPROVIDER: The requested service provider is invalid");	return false;
		case WSAEPROVIDERFAILEDINIT:LOG_ERROR("Error at" << t << ": WSAEPROVIDERFAILEDINIT: The requested service provider could not be loaded or initialized");	return false;
		case WSASYSCALLFAILURE:		LOG_ERROR("Error at" << t << ": WSASYSCALLFAILURE: A system call that should never fail has failed");	return false;
		case WSASERVICE_NOT_FOUND:	LOG_ERROR("Error at" << t << ": WSASERVICE_NOT_FOUND: No such service is known. The service cannot be found in the specified name space.");	return false;
		case WSATYPE_NOT_FOUND:		LOG_ERROR("Error at" << t << ": WSATYPE_NOT_FOUND: The specified class was not found");	return false;
		default: 					LOG_ERROR("Error at" << t << ": Unknown error");	return false;
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




// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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