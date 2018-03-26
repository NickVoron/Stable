// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#include "getifaddrs.h"

#if defined(USE_ANDROID) || defined(USE_LINUX)
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#elif defined(USE_APPLE)

#else
#include <IPTypes.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2ipdef.h>
#endif

#if defined(USE_ANDROID) || defined(USE_LINUX)
namespace {

	struct netlinkrequest {
		nlmsghdr header;
		ifaddrmsg msg;
	};

	const int kMaxReadSize = 4096;

}  

int set_ifname(struct ifaddrs* ifaddr, int interface) {
  char buf[IFNAMSIZ] = {0};
  char* name = if_indextoname(interface, buf);
  if (name == nullptr) {
    return -1;
  }
  ifaddr->ifa_name = new char[strlen(name) + 1];
  strncpy(ifaddr->ifa_name, name, strlen(name) + 1);
  return 0;
}

int set_flags(struct ifaddrs* ifaddr) {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    return -1;
  }
  ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, ifaddr->ifa_name, IFNAMSIZ - 1);
  int rc = ioctl(fd, SIOCGIFFLAGS, &ifr);
  close(fd);
  if (rc == -1) {
    return -1;
  }
  ifaddr->ifa_flags = ifr.ifr_flags;
  return 0;
}

int set_addresses(struct ifaddrs* ifaddr, ifaddrmsg* msg, void* data,
                  size_t len) {
  if (msg->ifa_family == AF_INET) {
    sockaddr_in* sa = new sockaddr_in;
    sa->sin_family = AF_INET;
    memcpy(&sa->sin_addr, data, len);
    ifaddr->ifa_addr = reinterpret_cast<sockaddr*>(sa);
  } else if (msg->ifa_family == AF_INET6) {
    sockaddr_in6* sa = new sockaddr_in6;
    sa->sin6_family = AF_INET6;
    sa->sin6_scope_id = msg->ifa_index;
    memcpy(&sa->sin6_addr, data, len);
    ifaddr->ifa_addr = reinterpret_cast<sockaddr*>(sa);
  } else {
    return -1;
  }
  return 0;
}

int make_prefixes(struct ifaddrs* ifaddr, int family, int prefixlen) {
  char* prefix = nullptr;
  if (family == AF_INET) {
    sockaddr_in* mask = new sockaddr_in;
    mask->sin_family = AF_INET;
    memset(&mask->sin_addr, 0, sizeof(in_addr));
    ifaddr->ifa_netmask = reinterpret_cast<sockaddr*>(mask);
    if (prefixlen > 32) {
      prefixlen = 32;
    }
    prefix = reinterpret_cast<char*>(&mask->sin_addr);
  } else if (family == AF_INET6) {
    sockaddr_in6* mask = new sockaddr_in6;
    mask->sin6_family = AF_INET6;
    memset(&mask->sin6_addr, 0, sizeof(in6_addr));
    ifaddr->ifa_netmask = reinterpret_cast<sockaddr*>(mask);
    if (prefixlen > 128) {
      prefixlen = 128;
    }
    prefix = reinterpret_cast<char*>(&mask->sin6_addr);
  } else {
    return -1;
  }
  for (int i = 0; i < (prefixlen / 8); i++) {
    *prefix++ = 0xFF;
  }
  char remainder = 0xff;
  remainder <<= (8 - prefixlen % 8);
  *prefix = remainder;
  return 0;
}

int populate_ifaddrs(struct ifaddrs* ifaddr, ifaddrmsg* msg, void* bytes,
                     size_t len) {
  if (set_ifname(ifaddr, msg->ifa_index) != 0) {
    return -1;
  }
  if (set_flags(ifaddr) != 0) {
    return -1;
  }
  if (set_addresses(ifaddr, msg, bytes, len) != 0) {
    return -1;
  }
  if (make_prefixes(ifaddr, msg->ifa_family, msg->ifa_prefixlen) != 0) {
    return -1;
  }
  return 0;
}

int getifaddrs(struct ifaddrs** result) {
  int fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  if (fd < 0) {
    return -1;
  }

  netlinkrequest ifaddr_request;
  memset(&ifaddr_request, 0, sizeof(ifaddr_request));
  ifaddr_request.header.nlmsg_flags = NLM_F_ROOT | NLM_F_REQUEST;
  ifaddr_request.header.nlmsg_type = RTM_GETADDR;
  ifaddr_request.header.nlmsg_len = NLMSG_LENGTH(sizeof(ifaddrmsg));

  ssize_t count = send(fd, &ifaddr_request, ifaddr_request.header.nlmsg_len, 0);
  if (static_cast<size_t>(count) != ifaddr_request.header.nlmsg_len) {
    close(fd);
    return -1;
  }
  struct ifaddrs* start = nullptr;
  struct ifaddrs* current = nullptr;
  char buf[kMaxReadSize];
  ssize_t amount_read = recv(fd, &buf, kMaxReadSize, 0);
  while (amount_read > 0) {
    nlmsghdr* header = reinterpret_cast<nlmsghdr*>(&buf[0]);
    size_t header_size = static_cast<size_t>(amount_read);
    for ( ; NLMSG_OK(header, header_size);
          header = NLMSG_NEXT(header, header_size)) {
      switch (header->nlmsg_type) {
        case NLMSG_DONE:
          
          *result = start;
          close(fd);
          return 0;
        case NLMSG_ERROR:
          close(fd);
          freeifaddrs(start);
          return -1;
        case RTM_NEWADDR: {
          ifaddrmsg* address_msg =
              reinterpret_cast<ifaddrmsg*>(NLMSG_DATA(header));
          rtattr* rta = IFA_RTA(address_msg);
          ssize_t payload_len = IFA_PAYLOAD(header);
          while (RTA_OK(rta, payload_len)) {
            if (rta->rta_type == IFA_ADDRESS) {
              int family = address_msg->ifa_family;
              if (family == AF_INET || family == AF_INET6) {
                ifaddrs* newest = new ifaddrs;
                memset(newest, 0, sizeof(ifaddrs));
                if (current) {
                  current->ifa_next = newest;
                } else {
                  start = newest;
                }
                if (populate_ifaddrs(newest, address_msg, RTA_DATA(rta),
                                     RTA_PAYLOAD(rta)) != 0) {
                  freeifaddrs(start);
                  *result = nullptr;
                  return -1;
                }
                current = newest;
              }
            }
            rta = RTA_NEXT(rta, payload_len);
          }
          break;
        }
      }
    }
    amount_read = recv(fd, &buf, kMaxReadSize, 0);
  }
  close(fd);
  freeifaddrs(start);
  return -1;
}

void freeifaddrs(struct ifaddrs* addrs) {
  struct ifaddrs* last = nullptr;
  struct ifaddrs* cursor = addrs;
  while (cursor) {
    delete[] cursor->ifa_name;
    delete cursor->ifa_addr;
    delete cursor->ifa_netmask;
    last = cursor;
    cursor = cursor->ifa_next;
    delete last;
  }
}

#elif defined(USE_APPLE)
#else

#pragma comment(lib, "IPHLPAPI.lib")

#define INET

#define SCTPDBG(...)
#define SCTP_DEBUG_USR

int getifaddrs(struct ifaddrs** interfaces)
{
	int ret;
#if defined(INET) || defined(INET6)
	DWORD Err, AdapterAddrsSize;
	int count;
	PIP_ADAPTER_ADDRESSES pAdapterAddrs, pAdapt;
	struct ifaddrs *ifa;
#endif
#if defined(INET)
	struct sockaddr_in *addr;
#endif
#if defined(INET6)
	struct sockaddr_in6 *addr6;
#endif
#if defined(INET) || defined(INET6)
	count = 0;
#endif
	ret = 0;
#if defined(INET)
	AdapterAddrsSize = 0;
	pAdapterAddrs = NULL;
	if ((Err = GetAdaptersAddresses(AF_INET, 0, NULL, NULL, &AdapterAddrsSize)) != 0) {
		if ((Err != ERROR_BUFFER_OVERFLOW) && (Err != ERROR_INSUFFICIENT_BUFFER)) 
		{
			SCTPDBG(SCTP_DEBUG_USR, "GetAdaptersV4Addresses() sizing failed with error code %d and AdapterAddrsSize = %d\n", Err, AdapterAddrsSize);
			ret = -1;
			goto cleanup;
		}
	}
	
	if ((pAdapterAddrs = (PIP_ADAPTER_ADDRESSES) GlobalAlloc(GPTR, AdapterAddrsSize)) == NULL) {
		SCTPDBG(SCTP_DEBUG_USR, "Memory allocation error!\n");
		ret = -1;
		goto cleanup;
	}
	
	if ((Err = GetAdaptersAddresses(AF_INET, 0, NULL, pAdapterAddrs, &AdapterAddrsSize)) != ERROR_SUCCESS) {
		SCTPDBG(SCTP_DEBUG_USR, "GetAdaptersV4Addresses() failed with error code %d\n", Err);
		ret = -1;
		goto cleanup;
	}
	
	for (pAdapt = pAdapterAddrs, count; pAdapt; pAdapt = pAdapt->Next, count++) {
		addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
		ifa = (struct ifaddrs *)malloc(sizeof(struct ifaddrs));
		if ((addr == NULL) || (ifa == NULL)) {
			SCTPDBG(SCTP_DEBUG_USR, "Can't allocate memory\n");
			ret = -1;
			goto cleanup;
		}
		ifa->ifa_name = _strdup(pAdapt->AdapterName);
		ifa->ifa_flags = pAdapt->Flags;
		ifa->ifa_addr = (struct sockaddr *)addr;
		auto addrsz = sizeof(addr);
		auto addrsz1 = sizeof(pAdapt->FirstUnicastAddress->Address.lpSockaddr);
		*ifa->ifa_addr = *pAdapt->FirstUnicastAddress->Address.lpSockaddr;
		interfaces[count] = ifa;
		if (count > 0)
		{
			interfaces[count - 1]->ifa_next = ifa;
		}
	}
	GlobalFree(pAdapterAddrs);
#endif
#if defined(INET6)
	AdapterAddrsSize = 0;
	pAdapterAddrs = NULL;
	if ((Err = GetAdaptersAddresses(AF_INET6, 0, NULL, NULL, &AdapterAddrsSize)) != 0) {
		if ((Err != ERROR_BUFFER_OVERFLOW) && (Err != ERROR_INSUFFICIENT_BUFFER)) {
			SCTPDBG(SCTP_DEBUG_USR, "GetAdaptersV6Addresses() sizing failed with error code %d AdapterAddrsSize = %d\n", Err, AdapterAddrsSize);
			ret = -1;
			goto cleanup;
		}
	}
	
	if ((pAdapterAddrs = (PIP_ADAPTER_ADDRESSES) GlobalAlloc(GPTR, AdapterAddrsSize)) == NULL) {
		SCTPDBG(SCTP_DEBUG_USR, "Memory allocation error!\n");
		ret = -1;
		goto cleanup;
	}
	
	if ((Err = GetAdaptersAddresses(AF_INET6, 0, NULL, pAdapterAddrs, &AdapterAddrsSize)) != ERROR_SUCCESS) {
		SCTPDBG(SCTP_DEBUG_USR, "GetAdaptersV6Addresses() failed with error code %d\n", Err);
		ret = -1;
		goto cleanup;
	}
	
	for (pAdapt = pAdapterAddrs, count; pAdapt; pAdapt = pAdapt->Next, count++) {
		addr6 = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6));
		ifa = (struct ifaddrs *)malloc(sizeof(struct ifaddrs));
		if ((addr6 == NULL) || (ifa == NULL)) {
			SCTPDBG(SCTP_DEBUG_USR, "Can't allocate memory\n");
			ret = -1;
			goto cleanup;
		}
		ifa->ifa_name = _strdup(pAdapt->AdapterName);
		ifa->ifa_flags = pAdapt->Flags;
		ifa->ifa_addr = (struct sockaddr *)addr6;
		memcpy(addr6, &pAdapt->FirstUnicastAddress->Address.lpSockaddr, sizeof(struct sockaddr_in6));
		interfaces[count] = ifa;
		if (count > 0)
		{
			interfaces[count - 1]->ifa_next = ifa;
		}
	}
#endif
	interfaces[count - 1]->ifa_next = nullptr;
#if defined(INET) || defined(INET6)
	cleanup:
		   if (pAdapterAddrs != NULL) {

		   }
#endif
	return (ret);
}

void freeifaddrs(struct ifaddrs* addrs)
{
	
}


#endif  




// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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