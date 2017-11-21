#pragma once

#include "os_win.h"

#ifndef WIN32
#define SOCKET_ERROR -1
#endif

namespace net
{
	bool error(int errCode);
	bool error(int errCode, const char* t);
	bool last_error();

	bool error_socket(socket_type s);
	bool error_socket(socket_type s, const char* t);

	bool error_getaddrinfo(int errCode);
}