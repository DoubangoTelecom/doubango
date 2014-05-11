/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_types.h
 * @brief ????.
 *
 */
#ifndef TNET_TYPES_H
#define TNET_TYPES_H

#include "tinynet_config.h"

#if TNET_UNDER_WINDOWS
#	include	<winsock2.h>
#	include	<ws2tcpip.h>
#	if !TNET_UNDER_WINDOWS_RT
#		include <iphlpapi.h>
#	endif
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/select.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#	include <fcntl.h>
#	include <sys/ioctl.h>
#	include <unistd.h>
#	include <net/if.h>
#	if HAVE_IFADDRS_H
#		include <ifaddrs.h>
#	endif
#	if HAVE_POLL_H
#		include <poll.h>
#	endif /* HAVE_POLL_H */
#endif

#if defined(TNET_HAVE_SCTP)
#include <netinet/sctp.h>
#endif

#include "tsk_errno.h"
#include "tsk_list.h"

TNET_BEGIN_DECLS

#define TNET_FINGERPRINT_MAX	256

typedef int32_t tnet_fd_t;
typedef uint16_t tnet_port_t;
typedef int32_t tnet_family_t;
typedef char tnet_host_t[NI_MAXHOST];
typedef char tnet_ip_t[INET6_ADDRSTRLEN];
typedef unsigned char tnet_fingerprint_t[TNET_FINGERPRINT_MAX + 1];

typedef tsk_list_t tnet_interfaces_L_t; /**< List of @ref tnet_interface_t elements*/
typedef tsk_list_t tnet_addresses_L_t; /**< List of @ref tnet_address_t elements*/

typedef enum tnet_dtls_setup_e
{
	tnet_dtls_setup_none,
	tnet_dtls_setup_actpass,
	tnet_dtls_setup_active,
	tnet_dtls_setup_passive,

	TNET_DTLS_SETUP_MAX
}
tnet_dtls_setup_t;

static const char* TNET_DTLS_SETUP_NAMES[TNET_DTLS_SETUP_MAX] =
{
	"UNKNOWN", "actpass", "active", "passive"
};

typedef enum tnet_dtls_hash_type_e
{
	tnet_dtls_hash_type_none,
	tnet_dtls_hash_type_md5,
	tnet_dtls_hash_type_sha1,
	tnet_dtls_hash_type_sha256,
	tnet_dtls_hash_type_sha512,

	TNET_DTLS_HASH_TYPE_MAX
}
tnet_dtls_hash_type_t;

// Hash names are case-insensitive but use lower case values because of https://bugzilla.mozilla.org/show_bug.cgi?id=828027
static const char* TNET_DTLS_HASH_NAMES[TNET_DTLS_HASH_TYPE_MAX] =
{
	"UNKNOWN", "md5", "sha-1", "sha-256", "sha-512"
};

#if TNET_UNDER_WINDOWS
#	define TNET_INVALID_SOCKET				INVALID_SOCKET
#	define TNET_ERROR_WOULDBLOCK			WSAEWOULDBLOCK
#	define TNET_ERROR_INPROGRESS			WSAEINPROGRESS
#	define TNET_ERROR_CONNRESET				WSAECONNRESET
#	define TNET_ERROR_INTR					WSAEINTR
#	define TNET_ERROR_ISCONN				WSAEISCONN
#	define TNET_ERROR_EAGAIN				TNET_ERROR_WOULDBLOCK /* WinSock FIX */
#	if TNET_UNDER_WINDOWS_RT /* gai_strerrorA() links against FormatMessageA which is not allowed on the store */
		static TNET_INLINE const char* tnet_gai_strerror(int ecode)
		{
			static char aBuff[1024] = {0};
			
			WCHAR *wBuff = gai_strerrorW(ecode);
			int len;
			if((len = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wBuff, wcslen(wBuff), aBuff, sizeof(aBuff) - 1, NULL, NULL)) > 0)
			{
				aBuff[len] = '\0';
			}
			else
			{
				aBuff[0] = '\0';
			}
			return aBuff;
		}
#	else
#		define tnet_gai_strerror			gai_strerrorA
#	endif
#else
#	define TNET_INVALID_SOCKET				-1
#	define TNET_ERROR_WOULDBLOCK			EWOULDBLOCK
#	define TNET_ERROR_INPROGRESS			EINPROGRESS
#	define TNET_ERROR_CONNRESET				ECONNRESET
#	define TNET_ERROR_INTR					EINTR
#	define TNET_ERROR_ISCONN				EISCONN
#	define TNET_ERROR_EAGAIN				EAGAIN
#	define tnet_gai_strerror				gai_strerror
#endif
#define TNET_INVALID_FD				TNET_INVALID_SOCKET

#ifdef _WIN32_WCE
typedef TCHAR tnet_error_t[512];
#else
typedef char tnet_error_t[512];
#endif

TNET_END_DECLS

#endif /* TNET_TYPES_H */


