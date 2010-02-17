/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tnet_utils.c
 * @brief Network utilities functions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tnet_utils.h"

#include "tsk_debug.h"
#include "tsk_string.h"
#include "tsk_memory.h"

#include "tnet_socket.h"

#include <string.h>

/**
 * @fn	void tnet_getlasterror(tnet_error_t *error)
 *
 * @brief	Gets last network error description.
 *
 * @author	Mamadou
 * @date	12/12/2009
 *
 * @param [out]	error	The short description of the last network error. 
**/
void tnet_getlasterror(tnet_error_t *error)
{
	int err  = tnet_geterrno();
	memset(*error, 0, sizeof(*error));

#if TNET_UNDER_WINDOWS
	{
#ifdef _WIN32_WCE
		FormatMessage
#else
		FormatMessageA
#endif
		(
		  FORMAT_MESSAGE_FROM_SYSTEM, 
		  0,
		  err,
		  0,
		  *error, 
		  sizeof(*error)-1,
		  0);
	}
#else
	//FIXME: use strerror(errno);
	sprintf(*error, "Network error (%d).", err);
#endif
}

int tnet_geterrno()
{
#if TNET_UNDER_WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif
}


/**
 * @fn	tnet_interfaces_L_t* tnet_get_interfaces()
 *
 * @brief	Gets list of all network interfaces/adapters. 
 *
 * @author	Mamadou
 * @date	1/25/2010
 *
 * @return	List of interfaces. 
 *			It is up to the caller to free the returned list using @ref TSK_OBJECT_SAFE_FREE. 
 *
**/
tnet_interfaces_L_t* tnet_get_interfaces()
{
	tnet_interfaces_L_t * ifaces = TSK_LIST_CREATE();

#if TSK_UNDER_WINDOWS /*=== WINDOWS XP/VISTA/7/CE===*/

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
	{
		TSK_DEBUG_ERROR("Error allocating memory needed to call GetAdaptersinfo.");
		goto bail;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if(pAdapterInfo == NULL)
		{
			TSK_DEBUG_ERROR("Error allocating memory needed to call GetAdaptersinfo.");
			goto bail;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while(pAdapter) 
		{
			tnet_interface_t *iface = TNET_INTERFACE_CREATE(pAdapter->Description, pAdapter->Address, pAdapter->AddressLength);
			iface->index = pAdapter->Index;
			tsk_list_push_back_data(ifaces, &(iface));
			
			pAdapter = pAdapter->Next;
		}
	}

	if(pAdapterInfo)
	{
		FREE(pAdapterInfo);
	}


#undef MALLOC
#undef FREE


#elif HAVE_IFADDRS /*=== Using getifaddrs ===*/

	// see http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html
	struct ifaddrs *ifaddr = 0, *ifa = 0;

	/* Get interfaces */
	if(getifaddrs(&ifaddr) == -1)
	{
		TSK_DEBUG_ERROR("getifaddrs failed and errno= [%d]", tnet_geterrno());
		goto bail;
	}
	
	for(ifa = ifaddr; ifa; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr->sa_family != AF_LINK){
			continue;
		}
		
#if defined(__linux__)
		{
			struct ifreq ifr;
			tnet_fd_t fd = TNET_INVALID_FD;
			
			if((fd = socket(AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP)) < 0){
				TSK_DEBUG_ERROR("Failed to create new DGRAM socket and errno= [%d]", tnet_geterrno());
				goto next;
			}
			
			ifr.ifr_addr.sa_family = ifa->ifa_addr->sa_family;
			strcpy(ifr.ifr_name, ifa.ifa_name);
			if(tnet_ioctl(fd, SIOCGIFHWADDR, &ifr)<0){
				TSK_DEBUG_ERROR("tnet_ioctl(SIOCGIFHWADDR)", tnet_geterrno());
				goto next;
			}
			else{
				//sockaddr_dl* sdl = (struct sockaddr_dl *)ifa->ifa_addr;
				tnet_interface_t *iface = TNET_INTERFACE_CREATE(ifr->ifr_name, ifr->ifr_hwaddr.sa_data, 6);
				tsk_list_push_back_data(ifaces, (void**)&(iface));
			}
		next:
			tnet_sockfd_close(&fd);
		}
#else
		{
			//struct sockaddr_dl* sdl = (struct sockaddr_dl*)ifa->ifa_addr;
			tnet_interface_t *iface = TNET_INTERFACE_CREATE(ifa->ifa_name, ifa->ifa_addr, 6);
			iface->index = if_nametoindex(ifa->ifa_name);
			tsk_list_push_back_data(ifaces, (void**)&(iface));
		}
#endif
		
	}/* for */
	
	freeifaddrs(ifaddr);

#else /*=== ANDROID,... --> Using SIOCGIFCONF and SIOCGIFHWADDR ===*/

	tnet_fd_t fd = TNET_INVALID_FD;
	char buffer[1024];
	struct ifconf ifc;

	struct sockaddr_in *sin;
	struct ifreq *ifr;

	if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		TSK_DEBUG_ERROR("Failed to create new DGRAM socket and errno= [%d]", tnet_geterrno());
		goto done;
	}

	ifc.ifc_len = sizeof(buffer);
	ifc.ifc_buf = buffer;

	if(ioctl(fd, SIOCGIFCONF, &ifc) < 0)
	{
		TSK_DEBUG_ERROR("ioctl(SIOCGIFCONF) failed and errno= [%d]", tnet_geterrno());
		goto done;
	}

	for(ifr = ifc.ifc_req; ifr && !tsk_strempty(ifr->ifr_name); ifr++)
	{
		sin = (struct sockaddr_in *)&(ifr->ifr_addr);
		// TODO: IPAddress if needed
		if(/*ioctl(fd, SIOCGIFFLAGS, &ifr) == 0*/1)
		{
			if (!(ifr->ifr_flags & IFF_LOOPBACK))
			{
				if(/*ioctl(fd, SIOCGIFHWADDR, &ifr) == 0*/1)
				{
					tnet_interface_t *iface = TNET_INTERFACE_CREATE(ifr->ifr_name, ifr->ifr_hwaddr.sa_data, 6);
					tsk_list_push_back_data(ifaces, (void**)&(iface));
					//iface->index = if_nametoindex(ifr->ifr_name);
				}
			}
		 }
		else
		{
			TSK_DEBUG_ERROR("ioctl(SIOCGIFFLAGS) failed and errno= [%d]", tnet_geterrno());
		}
	}

done:
	tnet_sockfd_close(&fd);


#endif

bail:
	return ifaces;
}

tnet_addresses_L_t* tnet_get_addresses(tnet_family_t family, unsigned unicast, unsigned anycast, unsigned multicast, unsigned dnsserver)
{
	tnet_addresses_L_t *addresses = TSK_LIST_CREATE();
	tnet_ip_t ip;

#if TSK_UNDER_WINDOWS

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

	/* Declare and initialize variables */
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;


	outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
    pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);

    // Make an initial call to GetAdaptersAddresses to get the 
    // size needed into the outBufLen variable
    if(GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
        FREE(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    }

    if(pAddresses == NULL)
	{
        TSK_DEBUG_ERROR("Memory allocation failed for IP_ADAPTER_ADDRESSES struct.");
        goto bail;
    }

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if(dwRetVal == NO_ERROR) 
	{
        pCurrAddresses = pAddresses;
		while (pCurrAddresses)
		{
			/* UNICAST addresses
			*/
			pUnicast = pCurrAddresses->FirstUnicastAddress;
            while(unicast && pUnicast)
			{
				memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pUnicast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = TNET_ADDRESS_CREATE(ip);
					address->family = pUnicast->Address.lpSockaddr->sa_family;
					address->unicast = 1;
					tsk_list_push_back_data(addresses, &address);
				}

                pUnicast = pUnicast->Next;
            }

			/* ANYCAST addresses
			*/
			pAnycast = pCurrAddresses->FirstAnycastAddress;
            while(anycast && pAnycast)
			{
				memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pAnycast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = TNET_ADDRESS_CREATE(ip);
					address->family = pAnycast->Address.lpSockaddr->sa_family;
					address->anycast = 1;
					tsk_list_push_back_data(addresses, &address);
				}

                pAnycast = pAnycast->Next;
            }

			/* MULTYCAST addresses
			*/
			pMulticast = pCurrAddresses->FirstMulticastAddress;
            while(multicast && pMulticast)
			{
				memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pMulticast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = TNET_ADDRESS_CREATE(ip);
					address->family = pMulticast->Address.lpSockaddr->sa_family;
					address->multicast = 1;
					tsk_list_push_back_data(addresses, &address);
				}

                pMulticast = pMulticast->Next;
            }

			/* DNS servers
			*/
			pDnServer = pCurrAddresses->FirstDnsServerAddress;
            while(dnsserver && pDnServer)
			{
				memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pDnServer->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = TNET_ADDRESS_CREATE(ip);
					address->family = pDnServer->Address.lpSockaddr->sa_family;
					address->dnsserver = 1;
					tsk_list_push_back_data(addresses, &address);
				}

                pDnServer = pDnServer->Next;
            }

			pCurrAddresses = pCurrAddresses->Next;
		}
	}

	if(pAddresses)
	{
		FREE(pAddresses);
	}

#undef MALLOC
#undef FREE




#else	/* !TSK_UNDER_WINDOWS */




#endif


bail:
	return addresses;
}

/**
 * @fn	int tnet_getaddrinfo(const char *node, const char *service,
 * 		const struct addrinfo *hints, struct addrinfo **res)
 *
 * @brief	converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked list of struct addrinfo structures. 
 *			You MUST call @ref tnet_freeaddrinfo() function to free the result.
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @param [in]	node	A pointer to a NULL-terminated ANSI string that contains a host (node) name or a numeric host address string. For the Internet protocol, the numeric host address string is a dotted-decimal IPv4 address or an IPv6 hex address.. 
 * @param [in]	service	A pointer to a NULL-terminated ANSI string that contains either a service name or port number represented as a string. 
 * @param [in]	hints	A pointer to an addrinfo structure that provides hints about the type of socket the caller supports. 
 * @param [out]	res		A pointer to a linked list of one or more addrinfo structures that contains response information about the host. 
 *
 * @return	Success returns zero. Failure returns a nonzero error code.
**/
int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res)
{
	int ret;
	if((ret = getaddrinfo(node, service, hints, res))){
		TSK_DEBUG_ERROR("getaddrinfo failed: [%s]", tnet_gai_strerror(ret));
	}
	return ret;
}

/**
 * @fn	void tnet_freeaddrinfo(struct addrinfo *ai)
 *
 * @brief	This function frees address information previously allocated using @ref tnet_getaddrinfo.
 *
 * @author	Mamadou
 * @date	12/11/2009
 *
 * @param [in]	The address information to free. 
**/
void tnet_freeaddrinfo(struct addrinfo *ai)
{
	freeaddrinfo(ai);
}

int tnet_get_sockaddr(tnet_fd_t fd, struct sockaddr_storage *result)
{
	if(fd >0)
	{
		socklen_t namelen = sizeof(*result);
		return getsockname(fd, (struct sockaddr*)result, &namelen);
	}
	return -1;
}

tnet_socket_type_t tnet_get_socket_type(tnet_fd_t fd)
{
	tnet_socket_type_t type = tnet_socket_type_invalid;
	
	/*if(fd >0)
	{
		struct sockaddr_storage ss;
		if(!tnet_get_sockaddr(fd, &ss))
		{
			if(((struct sockaddr *)&ss)->sa_family == AF_INET)
			{
				TNET_SOCKET_TYPE_AS_IPV4(type);
			}
			else if(((struct sockaddr *)&ss)->sa_family == AF_INET6)
			{
				TNET_SOCKET_TYPE_AS_IPV6(type);
			}
		}
	}*/

	return type;
}

int tnet_get_sockip_n_port(struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)
{
	int status = -1;

	if(addr->sa_family == AF_INET)
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)addr;
		if(port)
		{
			*port = ntohs(sin->sin_port);
			status = 0;
		}
		if(ip)
		{
			if((status = tnet_getnameinfo((struct sockaddr*)sin, sizeof(*sin), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST)))
			{
				return status;
			}
		}
	}
	else if(addr->sa_family == AF_INET6)
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;
		if(port)
		{
			*port = ntohs(sin6->sin6_port);
			status = 0;
		}
		if(ip)
		{
			if((status = tnet_getnameinfo((struct sockaddr*)sin6, sizeof(*sin6), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST)))
			{
				return status;
			}
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Unsupported address family.");
		return -1;
	}

	return status;
}

int tnet_get_ip_n_port(tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	*port = 0;

	if(fd > 0)
	{
		int status;
		struct sockaddr_storage ss;
		if((status = tnet_get_sockaddr(fd, &ss)))
		{
			TSK_DEBUG_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
			return -1;
		}

		return tnet_get_sockip_n_port(((struct sockaddr *)&ss), ip, port);
	}

	TSK_DEBUG_ERROR("Could not use an invalid socket description.");
	return -1;
}

int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags)
{
	return getnameinfo(sa, salen, node, nodelen, service, servicelen, flags);
}

int tnet_gethostname(tnet_host_t* result)
{
	return gethostname(*result, sizeof(*result));
}

int tnet_sockfd_joingroup6(tnet_fd_t fd, const char* multiaddr, unsigned iface_index)
{
	int ret = -1;
	//struct ipv6_mreq mreq6;
	//struct sockaddr_storage ss;

	//if((ret = tnet_sockaddr_init(multiaddr, 0, tnet_socket_type_udp_ipv6, &ss)))
	//{
	//	return ret;
	//}

	//memcpy(&mreq6.ipv6mr_multiaddr, &((struct sockaddr_in6 *) &ss)->sin6_addr, sizeof(struct in6_addr));
	//mreq6.ipv6mr_interface = iface_index;

	//if((ret = setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (const char*)&mreq6, sizeof(mreq6))))
	//{
	//	TNET_PRINT_LAST_ERROR("Failed to join IPv6 multicast group.");
	//	return ret;
	//}

	return ret;
}

int tnet_sockfd_leavegroup6(tnet_fd_t fd, const char* multiaddr, unsigned iface_index)
{
	//if(multiaddr)
	{
	}
	return -1;
}

int tnet_sockaddrinfo_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol)
{
	int status = 0;
	struct addrinfo *result = 0;
	struct addrinfo *ptr = 0;
	struct addrinfo hints;
	tsk_istr_t p;

	tsk_itoa(port, &p);

	/* hints address info structure */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = TNET_SOCKET_TYPE_IS_IPV6(type) ? AF_INET6 : AF_INET;
	hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(type) ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(type) ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	/* Performs getaddrinfo */
	if((status = tnet_getaddrinfo(host, p, &hints, &result)))
	{
		TNET_PRINT_LAST_ERROR("getaddrinfo have failed.");
		goto bail;
	}
	
	/* Find our address. */
	for(ptr = result; ptr; ptr = ptr->ai_next)
	{
		if(ptr->ai_family == hints.ai_family && ptr->ai_socktype == hints.ai_socktype && ptr->ai_protocol == hints.ai_protocol)
		{
			/* duplicate addrinfo ==> Bad idea
			*ai = tsk_calloc(1, sizeof (struct addrinfo));
			memcpy (*ai, ptr, sizeof (struct addrinfo));
			(*ai)->ai_addr = tsk_calloc(1, ptr->ai_addrlen);
			memcpy((*ai)->ai_addr, ptr->ai_addr, ptr->ai_addrlen);
			(*ai)->ai_addrlen = ptr->ai_addrlen;
			(*ai)->ai_next = 0;
			(*ai)->ai_canonname = 0;*/

			if(ai_addr)memcpy(ai_addr, ptr->ai_addr, ptr->ai_addrlen);
			if(ai_family) *ai_family = ptr->ai_family;
			if(ai_socktype) *ai_socktype = ptr->ai_socktype;
			if(ai_protocol) *ai_protocol = ptr->ai_protocol;
			
			break;
		}
	}

bail:
	tnet_freeaddrinfo(result);

	return status;
}

int tnet_sockaddr_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)
{
	int status;
	struct sockaddr_storage ai_addr;

	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, 0, 0, 0)))
	{
		return status;
	}
	
	memcpy(addr, &ai_addr, sizeof(ai_addr));

	return status;
}

int tnet_sockfd_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd)
{
	int status = -1;
	struct sockaddr_storage ai_addr;
	int ai_family, ai_socktype, ai_protocol;
	*fd = TNET_INVALID_SOCKET;
	
	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, &ai_family, &ai_socktype, &ai_protocol)))
	{
		goto bail;
	}
	
	if((*fd = socket(ai_family, ai_socktype, ai_protocol)) == TNET_INVALID_SOCKET)
	{
		TNET_PRINT_LAST_ERROR("Failed to create new socket.");
		goto bail;
	}

#if TNET_USE_POLL /* For win32 WSA* function the socket is auto. set to nonblocking mode. */
	if((status = tnet_sockfd_set_nonblocking(*fd)))
	{
		goto bail;
	}
#endif
	
#if TNET_HAVE_SS_LEN
	if((status = bind(*fd, (const struct sockaddr*)&ai_addr, ai_addr.ss_len)))
#else
	if((status = bind(*fd, (const struct sockaddr*)&ai_addr, sizeof(ai_addr))))
#endif
	{
		TNET_PRINT_LAST_ERROR("bind have failed.");
		tnet_sockfd_close(fd);
		
		goto bail;
	}
	
bail:
	return (*fd == TNET_INVALID_SOCKET) ? status : 0;
}


int tnet_sockfd_set_mode(tnet_fd_t fd, int nonBlocking)
{
	if(fd != TNET_INVALID_FD)
	{
#if TNET_UNDER_WINDOWS
	ULONG mode = nonBlocking;
	if(ioctlsocket(fd, FIONBIO, &mode))
	//if(WSAIoctl(fd, FIONBIO, &nonblocking, sizeof(nonblocking), NULL, 0, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		TNET_PRINT_LAST_ERROR("ioctlsocket(FIONBIO) have failed.");
		return -1;
	}
#else
	int flags;
    if((flags = fcntl(fd, F_GETFL, 0)) < 0) 
	{ 
		TNET_PRINT_LAST_ERROR("fcntl(F_GETFL) have failed.");
		return -1;
	} 
	if(fcntl(fd, F_SETFL, flags | (nonBlocking ? O_NONBLOCK : ~O_NONBLOCK)) < 0)
	{ 
		TNET_PRINT_LAST_ERROR("fcntl(O_NONBLOCK/O_NONBLOCK) have failed.");
		return -1;
	} 
#endif

	// int on = 1;
	// ioctl(fd, FIONBIO, (char *)&on);

	}
	return 0;
}

int tnet_sockfd_sendto(tnet_fd_t fd, const struct sockaddr *to, const void* buf, size_t size)
{
	if(fd == TNET_INVALID_FD)
	{
		TSK_DEBUG_ERROR("Using invalid FD to send data.");
		return -1;
	}
	if(!buf || !size)
	{
		TSK_DEBUG_ERROR("Using invalid BUFFER.");
		return -2;
	}

#if TNET_HAVE_SA_LEN
	return sendto(fd, buf, size, 0, to, to->sa_len);
#else
	//return sendto(fd, buf, size, 0, to, sizeof(*to));
	return sendto(fd, buf, size, 0, to, 
		to->sa_family == AF_INET6 ? sizeof(struct sockaddr_in6): (to->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(*to))); //FIXME: why sizeof(*to) don't work for IPv6 on XP?
#endif
}

int tnet_sockfd_recvfrom(tnet_fd_t fd, void* buf, size_t size, int flags, struct sockaddr *from)
{
	socklen_t fromlen;

	if(fd == TNET_INVALID_FD)
	{
		TSK_DEBUG_ERROR("Using invalid FD to recv data.");
		return -1;
	}

#if TNET_HAVE_SA_LEN
	fromlen = from->sa_len;
#else
	fromlen = sizeof(*from);
#endif

	return recvfrom(fd, buf, size, flags, from, &fromlen);
}

int tnet_sockfd_send(tnet_fd_t fd, void* buf, size_t size, int flags)
{
	if(fd == TNET_INVALID_FD)
	{
		TSK_DEBUG_ERROR("Using invalid FD to send data.");
		return -1;
	}

	return send(fd, buf, size, flags);
}

int tnet_sockfd_recv(tnet_fd_t fd, void* buf, size_t size, int flags)
{
	if(fd == TNET_INVALID_FD)
	{
		TSK_DEBUG_ERROR("Using invalid FD to recv data.");
		return -1;
	}

	return recv(fd, buf, size, flags);
}

int tnet_sockfd_close(tnet_fd_t *fd)
{
	int ret;
#if TNET_UNDER_WINDOWS
	ret = closesocket(*fd);
#else
	ret = close(*fd);
#endif

	*fd = TNET_INVALID_SOCKET;
	return ret;
}









































//=================================================================================================
//	INTERFACE object definition
//
static void* tnet_interface_create(void * self, va_list * app)
{
	tnet_interface_t *iface = self;
	if(iface)
	{
		const char* description = va_arg(*app, const char*);
		const uint8_t* mac_address = va_arg(*app, const uint8_t*);
		size_t mac_address_length = va_arg(*app, size_t);

		iface->description = tsk_strdup(description);
		if((iface->mac_address = tsk_calloc(mac_address_length, sizeof(uint8_t)))){
			memcpy(iface->mac_address, mac_address, mac_address_length);
		}
		iface->mac_address_length = mac_address_length;
	}
	return self;
}

static void* tnet_interface_destroy(void * self)
{ 
	tnet_interface_t *iface = self;
	if(iface)
	{
		TSK_FREE(iface->description);
		TSK_FREE(iface->mac_address);
	}

	return self;
}

static int tnet_interface_cmp(const void *if1, const void *if2)
{
	const tnet_interface_t *iface1 = if1;
	const tnet_interface_t *iface2 = if2;
	
	if(iface1 && iface2)
	{
		return tsk_stricmp(iface1->description, iface1->description);
	}
	else if(!iface1 && !iface2) return 0;
	else return -1;
}

static const tsk_object_def_t tnet_interface_def_s = 
{
	sizeof(tnet_interface_t),
	tnet_interface_create, 
	tnet_interface_destroy,
	tnet_interface_cmp, 
};
const void *tnet_interface_def_t = &tnet_interface_def_s;




//=================================================================================================
//	ADDRESS object definition
//
static void* tnet_address_create(void * self, va_list * app)
{
	tnet_address_t *address = self;
	if(address)
	{
		address->ip = tsk_strdup(va_arg(*app, const char*));
	}
	return self;
}

static void* tnet_address_destroy(void * self)
{ 
	tnet_address_t *address = self;
	if(address)
	{
		TSK_FREE(address->ip);
	}

	return self;
}

static const tsk_object_def_t tnet_address_def_s = 
{
	sizeof(tnet_address_t),
	tnet_address_create, 
	tnet_address_destroy,
	0, 
};
const void *tnet_address_def_t = &tnet_address_def_s;

