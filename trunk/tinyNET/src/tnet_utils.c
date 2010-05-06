/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @brief Network utility functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tnet_utils.h"

#include "tsk_debug.h"
#include "tsk_string.h"
#include "tsk_memory.h"

#include "tnet_socket.h"
#include "tnet_endianness.h"
#include "dns/tnet_dns_resolvconf.h"

#include <string.h>

/**@defgroup tnet_utils_group Network utility functions.
*/


/**@ingroup tnet_utils_group
* Creates new @ref tnet_interface_t object.
*/
tnet_interface_t* tnet_interface_create(const char* description, const void* mac_address, tsk_size_t mac_address_length)
{
	return tsk_object_new(tnet_interface_def_t, description, mac_address, mac_address_length);
}

/**@ingroup tnet_utils_group
* Creates new @ref tnet_address_t object.
*/
tnet_address_t* tnet_address_create(const char* ip)
{
	return tsk_object_new(tnet_address_def_t, ip);
}

/**@ingroup tnet_utils_group
 *
 * Gets last network error description.
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
	strerror_r(err, *error, sizeof(*error));
	//sprintf(*error, "Network error (%d).", err);
#endif
}

/**@ingroup tnet_utils_group
* Gets last error number. Will call @a WSAGetLastError() on Windows and
* errno on unix-like systems.
* @retval Error number.
*/
int tnet_geterrno()
{
#if TNET_UNDER_WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif
}


/**@ingroup tnet_utils_group
 * Gets the list of all network interfaces/adapters. 
 *
 * @retval	Network interfaces.
**/
tnet_interfaces_L_t* tnet_get_interfaces()
{
	tnet_interfaces_L_t * ifaces = tsk_list_create();

#if TNET_UNDER_WINDOWS /*=== WINDOWS XP/VISTA/7/CE===*/

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
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
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
			tnet_interface_t *iface;

			if(pAdapter->Type == MIB_IF_TYPE_LOOPBACK){
				continue;
			}

			iface = tnet_interface_create(pAdapter->Description, pAdapter->Address, pAdapter->AddressLength);
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
				tnet_interface_t *iface = tnet_interface_create(ifr->ifr_name, ifr->ifr_hwaddr.sa_data, 6);
				tsk_list_push_back_data(ifaces, (void**)&(iface));
			}
		next:
			tnet_sockfd_close(&fd);
		}
#else
		{
			//struct sockaddr_dl* sdl = (struct sockaddr_dl*)ifa->ifa_addr;
			tnet_interface_t *iface = tnet_interface_create(ifa->ifa_name, ifa->ifa_addr, 6);
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
					tnet_interface_t *iface = tnet_interface_create(ifr->ifr_name, ifr->ifr_hwaddr.sa_data, 6);
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

/**@ingroup tnet_utils_group
* Get all IP addresses of the local machine.
* @param family The @a family of the addresses to return.
* @param unicast Indicates whether to return @a unicast addresses or not (1=yes and 0=no).
* @param anycast Indicates whether to return @a anycast addresses or not (1=yes and 0=no).
* @param multicast Indicates whether to return @a multicast addresses or not (1=yes and 0=no).
* @param dnsserver Indicates whether to include dns servers or not (1=yes and 0=no).
* @param if_index the index of the interface for which to to retrieve IP addresses.
* -1 mean all interfaces.
* @retval List of all addresses.
*/
tnet_addresses_L_t* tnet_get_addresses(tnet_family_t family, unsigned unicast, unsigned anycast, unsigned multicast, unsigned dnsserver, long if_index)
{
	tnet_addresses_L_t *addresses = tsk_list_create();

#if TSK_UNDER_WINDOWS
	
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

	/* Declare and initialize variables */
	tnet_ip_t ip;
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
    if(GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW){
        FREE(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    }
	else goto bail;

    if(pAddresses == NULL){
        TSK_DEBUG_ERROR("Memory allocation failed for IP_ADAPTER_ADDRESSES struct.");
        goto bail;
    }

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if(dwRetVal == NO_ERROR) 
	{
        pCurrAddresses = pAddresses;
		while (pCurrAddresses)
		{
			if((if_index != -1) && (pCurrAddresses->IfIndex != if_index && pCurrAddresses->Ipv6IfIndex != if_index)){
				goto next;
			}

			/* == UNICAST addresses == */
			pUnicast = pCurrAddresses->FirstUnicastAddress;
            while(unicast && pUnicast)
			{
				//memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pUnicast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = tnet_address_create(ip);
					address->family = pUnicast->Address.lpSockaddr->sa_family;
					address->unicast = 1;
					tsk_list_push_ascending_data(addresses, &address);
				}

                pUnicast = pUnicast->Next;
            }

			/* == ANYCAST addresses == */
			pAnycast = pCurrAddresses->FirstAnycastAddress;
            while(anycast && pAnycast)
			{
				//memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pAnycast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = tnet_address_create(ip);
					address->family = pAnycast->Address.lpSockaddr->sa_family;
					address->anycast = 1;
					tsk_list_push_ascending_data(addresses, &address);
				}

                pAnycast = pAnycast->Next;
            }

			/* == MULTYCAST addresses == */
			pMulticast = pCurrAddresses->FirstMulticastAddress;
            while(multicast && pMulticast)
			{
				//memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pMulticast->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = tnet_address_create(ip);
					address->family = pMulticast->Address.lpSockaddr->sa_family;
					address->multicast = 1;
					tsk_list_push_ascending_data(addresses, &address);
				}

                pMulticast = pMulticast->Next;
            }

			/* == DNS servers == */
			pDnServer = pCurrAddresses->FirstDnsServerAddress;
            while(dnsserver && pDnServer)
			{
				//memset(ip, '\0', sizeof(ip));
				tnet_get_sockip(pDnServer->Address.lpSockaddr, &ip);
				{
					tnet_address_t *address = tnet_address_create(ip);
					address->family = pDnServer->Address.lpSockaddr->sa_family;
					address->dnsserver = 1;
					tsk_list_push_ascending_data(addresses, &address);
				}

                pDnServer = pDnServer->Next;
            }
next:
			pCurrAddresses = pCurrAddresses->Next;
		}
	}

	if(pAddresses){
		FREE(pAddresses);
	}

#undef MALLOC
#undef FREE

bail:


#else	/* !TSK_UNDER_WINDOWS (MAC OS X, UNIX, ANDROID ...) */

	/* == DNS servers == */
	if(dnsserver){
		tnet_addresses_L_t * dns_servers;
		if((dns_servers = tnet_dns_resolvconf_parse("/etc/resolv.conf"))){
			tsk_list_pushback_list(addresses, dns_servers);
			TSK_OBJECT_SAFE_FREE(dns_servers);
		}
	}


#endif


	return addresses;
}

/**@ingroup tnet_utils_group
* Retrieves the @a source IP address that has the best route to the specified IPv4 or IPv6 @a destination.
* @param destination The destination address.
* @param port The destination port.
* @param type The socket type.
* @param source The best @a source.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_getbestsource(const char* destination, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *source)
{
	int ret = -1;
	struct sockaddr_storage destAddr;
	tnet_addresses_L_t* addresses = 0;
	const tsk_list_item_t* item;

	long dwBestIfIndex;

	if(!destination || !source){
		goto bail;
	}

	if((ret = tnet_sockaddr_init(destination, port, type, &destAddr))){
		goto bail;
	}

#if TNET_UNDER_WINDOWS
	if(GetBestInterfaceEx((struct sockaddr*)&destAddr, &dwBestIfIndex) != NO_ERROR){
		ret = WSAGetLastError();
		TNET_PRINT_LAST_ERROR("GetBestInterfaceEx failed.");
		goto bail;
	}
#endif

	if(!(addresses = tnet_get_addresses(TNET_SOCKET_TYPE_IS_IPV6(type) ? AF_INET6 : AF_INET, 1, 0, 0, 0, dwBestIfIndex))){
		ret = -2;
		TSK_DEBUG_ERROR("Failed to retrieve addresses.");
		goto bail;
	}

	tsk_list_foreach(item, addresses)
	{
		const tnet_address_t* address = item->data;
		if(address && address->ip){
			memset(*source, '\0', sizeof(*source));
			memcpy(*source, address->ip, tsk_strlen(address->ip) > sizeof(*source) ? sizeof(*source) : tsk_strlen(address->ip));
			ret = 0;
			goto bail; // First is good for us.
		}
	}

bail:
	TSK_OBJECT_SAFE_FREE(addresses);
	return ret;
}


/**@ingroup tnet_utils_group
 *
 * Converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked list of struct addrinfo structures. 
 *			You MUST call @ref tnet_freeaddrinfo() function to free the result.
 *
 * @param [in]	node	A pointer to a NULL-terminated ANSI string that contains a host (node) name or a numeric host address string. For the Internet protocol, the numeric host address string is a dotted-decimal IPv4 address or an IPv6 hex address.. 
 * @param [in]	service	A pointer to a NULL-terminated ANSI string that contains either a service name or port number represented as a string. 
 * @param [in]	hints	A pointer to an addrinfo structure that provides hints about the type of socket the caller supports. 
 * @param [out]	res		A pointer to a linked list of one or more addrinfo structures that contains response information about the host. 
 *
 * @retval	Success returns zero. Failure returns a nonzero error code.
**/
int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,  struct addrinfo **res)
{
	int ret = -1;
	if(hints && (ret = getaddrinfo(node, service, hints, res))){
		TSK_DEBUG_ERROR("getaddrinfo(family=%d, node=%s and service=%s) failed: [%s]", hints->ai_family, node, service, tnet_gai_strerror(ret));
	}
	return ret;
}

/**@ingroup tnet_utils_group
 *
 * This function frees address information previously allocated using @ref tnet_getaddrinfo.
 *
 * @param [in] ai	The address information to free. 
**/
void tnet_freeaddrinfo(struct addrinfo *ai)
{
	freeaddrinfo(ai);
}

/**@ingroup tnet_utils_group
* Converts a descriptor to @b sockaddr_storage structure.
* @param fd The descriptor to convert.
* @param result @b sockaddr_storage structre representing the desciptor.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_get_sockaddr(tnet_fd_t fd, struct sockaddr_storage *result)
{
	if(fd >0){
		socklen_t namelen = sizeof(*result);
		return getsockname(fd, (struct sockaddr*)result, &namelen);
	}
	return -1;
}

/**@ingroup tnet_utils_group
* Retrieves the socket type of a File Descriptor.
* @param fd The File descriptor for which to retrive the type.
* @retval @ref tnet_socket_type_t.
*/
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

/**@ingroup tnet_utils_group
* Gets the IP family of the @a host (e.g. "google.com" or "192.168.16.104" or "::1").
* If the @a host is FQDN associated with both IPv4 and IPv6 then the result is unpredictable.
* @param host The IP address or hostname for which to get the IP family.
* @param port The port associated to the @a host. Will be used as the default service.
* @retval @a AF_* if succeed and @a AF_UNSPEC otherwise.
*/
tnet_family_t tnet_get_family(const char* host, tnet_port_t port)
{
	tnet_family_t ret = AF_UNSPEC;
	if(host){
		int status;
		tsk_istr_t srv;
		struct addrinfo *result = tsk_null;
		struct addrinfo hints;

		/* set the port: used as the default service */
		if(port){
			tsk_itoa(port, &srv);
		}
		else{
			memset(srv, '\0', sizeof(srv));
		}
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		if((status = tnet_getaddrinfo(host, srv, &hints, &result))){
			TNET_PRINT_LAST_ERROR("getaddrinfo failed:");
			goto done;
		}

		/* Get the First result. */
		if(result){
			ret = result->ai_family;
			goto done;
		}
done:
		freeaddrinfo(result);
	}

	return ret;
}

/**@ingroup tnet_utils_group
* Gets the IP address and the Port of a @b sockaddr object.
* @param addr [in] A pointer to @b sockaddr structure for which to retrieve the IP address and port.
* @param ip [out] The IP address.
* @param port [out] The port.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_get_sockip_n_port(struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)
{
	int status = -1;

	if(addr->sa_family == AF_INET)
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)addr;
		if(port){
			*port = tnet_ntohs(sin->sin_port);
			status = 0;
		}
		if(ip){
			if((status = tnet_getnameinfo((struct sockaddr*)sin, sizeof(*sin), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST))){
				return status;
			}
		}
	}
	else if(addr->sa_family == AF_INET6)
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;
#if TNET_UNDER_WINDOWS
		int index;
#endif
		if(port){
			*port = tnet_ntohs(sin6->sin6_port);
			status = 0;
		}
		if(ip){
			if((status = tnet_getnameinfo((struct sockaddr*)sin6, sizeof(*sin6), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST))){
				return status;
			}

#if TNET_UNDER_WINDOWS
			if((index = tsk_strindexOf(*ip, tsk_strlen(*ip), "%")) > 0){
				*(*ip + index) = '\0';
			}
#endif
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Unsupported address family.");
		return -1;
	}

	return status;
}

/**@ingroup tnet_utils_group
* Gets the IP address and port of the remote peer.
* <b>The socket MUST be connect()ed.</b>
* @param localFD Local socket.
* @param ip [out] The IP address of the remote peer.
* @param port [out] The remote (peer) port.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_get_peerip_n_port(tnet_fd_t localFD, tnet_ip_t *ip, tnet_port_t *port)
{
	if(port){
		*port = 0;
	}

	if(localFD > 0){
		int status;
		socklen_t len;
		struct sockaddr_storage ss;

		len = sizeof(ss);
		if((status = getpeername(localFD, (struct sockaddr *)&ss, &len))){
			TSK_DEBUG_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
			return -1;
		}

		return tnet_get_sockip_n_port(((struct sockaddr *)&ss), ip, port);
	}

	TSK_DEBUG_ERROR("Could not use an invalid socket description.");
	return -1;
}

/**@ingroup tnet_utils_group
* Gets the IP address and the Port of a local socket (File descriptor).
* @param fd The decriptor for which to retrive the IP address and port.
* @param ip [out] The IP address of the local socket.
* @param port [out] The port of the local socket.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_get_ip_n_port(tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)
{
	if(port){
		*port = 0;
	}

	if(fd > 0){
		int status;
		struct sockaddr_storage ss;
		if((status = tnet_get_sockaddr(fd, &ss))){
			TSK_DEBUG_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
			return -1;
		}

		return tnet_get_sockip_n_port(((struct sockaddr *)&ss), ip, port);
	}

	TSK_DEBUG_ERROR("Could not use an invalid socket description.");
	return -1;
}

/**@ingroup tnet_utils_group
* Provides protocol-independent name resolution from an address to an ANSI host name and from a port number to the ANSI service name.
* @param sa A pointer to a socket address structure that contains the address and port number of the socket. For IPv4, the sa parameter points to a sockaddr_in structure. For IPv6, the sa parameter points to a @b sockaddr_in6 structure.
* @param salen The length, in bytes, of the structure pointed to by the sa parameter.
* @param node A pointer to an ANSI string used to hold the host name. On success, a pointer to the host name is returned as a Fully Qualified Domain Name (FQDN) by default. If the host parameter is NULL, this indicates the caller does not want to receive a host name string.
* @param nodelen The length, in bytes, of the buffer pointed to by the host parameter. The caller must provide a buffer large enough to hold the host name, including the terminating NULL character.
* @param service A pointer to an ANSI string to hold the service name. On success, a pointer is returned to an ANSI string that represents the service name associated with the port number. If the serv parameter is NULL, this indicates the caller does not want to receive a service name string.
* @param servicelen The length, in bytes, of the buffer pointed to by the serv parameter. The caller must provide a buffer large enough to hold the service name, including the terminating NULL character.
* @param flags A value used to customize processing of the @b getnameinfo function. See the Remarks section.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_getnameinfo(const struct sockaddr *sa, socklen_t salen, char* node, socklen_t nodelen, char* service, socklen_t servicelen, int flags)
{
	return getnameinfo(sa, salen, node, nodelen, service, servicelen, flags);
}

/**@ingroup tnet_utils_group
* Retrieves the standard host name for the local computer.
* @param result A pointer to a buffer that receives the local host name.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_gethostname(tnet_host_t* result)
{
	return gethostname(*result, sizeof(*result));
}

/**@ingroup tnet_utils_group
* Waits until the socket becomes writable/readable or @a timeout milliseconds passed.
* This function could be used just after you have @a connect()ed a non-blocking socket.
* @param fd The socket for which to check writability/readability.
* @param timeout The number of milliseconds to wait. The function will immediately return if the socket
* is already connected and writable/readable. Set the @a timeout value to -1 to wait indefinitely.
* @param writable Indicates whether to wait for writability or readability.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_sockfd_waitUntil(tnet_fd_t fd, long timeout, tsk_bool_t writable)
{
	int ret = -1;
	fd_set fds;
	struct timeval timetowait;

	if(fd<=0){
		goto bail;
	}

	if(timeout >=0){
		timetowait.tv_sec = (timeout/1000);
		timetowait.tv_usec = (timeout%1000) * 1000;
	}
	
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	ret = select(fd + 1, writable?0:&fds, writable?&fds:0, 0, (timeout >=0) ? &timetowait : 0);

	if(ret == 0){ /* timedout */
		ret = -2;
	}
	else if(ret == 1/* the total number of socket handles that are ready */){
		ret = 0; // Ok
	}
	//else: error

bail:
	return ret;
}

/**@ingroup tnet_utils_group
* NOT IMPLEMENTED.
*/
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
/**@ingroup tnet_utils_group
* NOT IMPLEMENTED.
*/
int tnet_sockfd_leavegroup6(tnet_fd_t fd, const char* multiaddr, unsigned iface_index)
{
	//if(multiaddr)
	{
	}
	return -1;
}

/**@ingroup tnet_utils_group
* Converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked list of struct addrinfo structures.
*/
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
	hints.ai_family =  TNET_SOCKET_TYPE_IS_IPV46(type) ? AF_UNSPEC : (TNET_SOCKET_TYPE_IS_IPV6(type) ? AF_INET6 : AF_INET);
	hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(type) ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(type) ? IPPROTO_TCP : IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	/* Performs getaddrinfo */
	if((status = tnet_getaddrinfo(host, p, &hints, &result))){
		TNET_PRINT_LAST_ERROR("getaddrinfo have failed.");
		goto bail;
	}
	
	/* Find our address. */
	for(ptr = result; ptr; ptr = ptr->ai_next)
	{
		/* Only IPv4 and IPv6 are supported */
		if(ptr->ai_family != AF_INET6 && ptr->ai_family != AF_INET){
			continue;
		}
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
		
		/* We prefer IPv4 but IPv6 can also work */
		if(ptr->ai_family == AF_INET){
			break;
		}
	}

bail:
	tnet_freeaddrinfo(result);

	return status;
}

/**@ingroup tnet_utils_group
* Converts human-readable text strings representing hostnames or IP addresses into a @b sockaddr_storage structure.
* @param host The hostname/IP address to convert.
* @param port The local port associated to the host.
* @param type The type of the socket to create.
* @param addr [out] @b sockaddr_storage structure representing the @a host:port address.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_sockaddr_init(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)
{
	int status;
	struct sockaddr_storage ai_addr;

	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, 0, 0, 0))){
		return status;
	}
	
	memcpy(addr, &ai_addr, sizeof(ai_addr));

	return status;
}

/**@ingroup tnet_utils_group
* Converts human-readable text strings representing hostnames or IP addresses as socket (File descriptor).
* @param host The hostname/IP address to convert.
* @param port The local port associated to the host.
* @param type The type of the socket to create.
* @param fd [out] The socket  representing the @a host:port address.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_sockfd_init(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd)
{
	int status = -1;
	struct sockaddr_storage ai_addr;
	int ai_family, ai_socktype, ai_protocol;
	*fd = TNET_INVALID_SOCKET;
	
	if((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, &ai_family, &ai_socktype, &ai_protocol))){
		goto bail;
	}
	
	if((*fd = socket(ai_family, ai_socktype, ai_protocol)) == TNET_INVALID_SOCKET){
		TNET_PRINT_LAST_ERROR("Failed to create new socket.");
		goto bail;
	}

#if TNET_USE_POLL /* For win32 WSA* function the socket is auto. set to nonblocking mode. */
	if((status = tnet_sockfd_set_nonblocking(*fd))){
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

/**@ingroup tnet_utils_group
* Changes the blocking mode of the socket.
* @param fd The socket for which to change to mode.
* @param nonBlocking The new mode (0 =blocking and 1=non-blocking).
* @retval Zero if succeed and non-zero error code otherwise.
*/
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

tnet_tls_socket_handle_t* tnet_sockfd_set_tlsfiles(tnet_fd_t fd, int isClient, const char* tlsfile_ca, const char* tlsfile_pvk, const char* tlsfile_pbk)
{
	tnet_tls_socket_handle_t* tlshandle = 0;
	if(fd == TNET_INVALID_FD){
		return 0;
	}

	if(isClient){
		tlshandle = tnet_tls_socket_client_create(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk);
	}
	else{
		tlshandle = tnet_tls_socket_server_create(fd, tlsfile_ca, tlsfile_pvk, tlsfile_pbk);
	}

	if(tnet_tls_socket_isok(tlshandle)){
		return tlshandle;
	}
	else{
		TSK_OBJECT_SAFE_FREE(tlshandle);
		return 0;
	}
}

/**@ingroup tnet_utils_group
* Sends data to a specific destination.
* @param fd The source socket.
* @param to The destination socket.
* @param buf A pointer to the buffer to send over the network.
* @param size The size of the buffer.
* @retval If no error occurs, sendto returns the total number of bytes sent, which can be less than the number indicated by @b size.
* Otherwise, non-zero (negative) error code is returned.
*/
int tnet_sockfd_sendto(tnet_fd_t fd, const struct sockaddr *to, const void* buf, tsk_size_t size)
{
	tsk_size_t sent = 0;
	int ret = -1;

	if(fd == TNET_INVALID_FD){
		TSK_DEBUG_ERROR("Using invalid FD to send data.");
		goto bail;
	}
	if(!buf || !size){
		TSK_DEBUG_ERROR("Using invalid BUFFER.");
		ret = -2;
		goto bail;
	}

	while(sent < size)
	{
#if TNET_HAVE_SA_LEN
		ret = sendto(fd, (buf+sent), (size-sent), 0, to, to->sa_len);
#else
		//return sendto(fd, (buf+sent), (size-sent), 0, to, sizeof(*to));
		ret = sendto(fd, (((const uint8_t*)buf)+sent), (size-sent), 0, to, 
			to->sa_family == AF_INET6 ? sizeof(struct sockaddr_in6): (to->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(*to))); //FIXME: why sizeof(*to) don't work for IPv6 on XP?
#endif
		if(ret <= 0){
			goto bail;
		}
		else{
			sent += ret;
		}
	}

bail:
	return (size == sent) ? sent : ret;
}

/**@ingroup tnet_utils_group
* Receives a datagram and stores the source address.
* @param fd A descriptor identifying a bound socket.
* @param buf A buffer for the incoming data.
* @param size The length, in bytes, of the buffer pointed to by the buf parameter.
* @param flags A set of options that modify the behavior of the function call beyond the options specified for the associated socket.
* All flags which can be passed to @b recvfrom.
* @param from An optional pointer to a buffer in a @b sockaddr structure that will hold the source address upon return.
* If no error occurs, recvfrom returns the number of bytes received. If the connection has been gracefully closed, the return value is zero. 
* Otherwise, non-zero (negative) error code is returned.
*/
int tnet_sockfd_recvfrom(tnet_fd_t fd, void* buf, tsk_size_t size, int flags, struct sockaddr *from)
{
	socklen_t fromlen;

	if(fd == TNET_INVALID_FD){
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

/**@ingroup tnet_utils_group
* Sends data on a connected socket.
* @param fd A descriptor identifying a connected socket.
* @param buf A pointer to a buffer containing the data to be transmitted.
* @param size The length, in bytes, of the data in buffer pointed to by the buf parameter.
* @param flags A set of flags that specify the way in which the call is made.
* All flags which can be passed to @b recv.
* @retval If no error occurs, send returns the total number of bytes sent, which can be less than the number requested to be sent in the @b size parameter.
*/
tsk_size_t tnet_sockfd_send(tnet_fd_t fd, const void* buf, tsk_size_t size, int flags)
{
	int ret = -1;
	tsk_size_t sent = 0;

	if(fd == TNET_INVALID_FD){
		TSK_DEBUG_ERROR("Using invalid FD to send data.");
		goto bail;
	}

	while(sent < size){
		if((ret = send(fd, (((const uint8_t*)buf)+sent), (size-sent), flags)) <= 0){
			if(tnet_geterrno() == TNET_ERROR_WOULDBLOCK){
				// FIXME: HORRIBLE HACK
				if((ret = tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT))){
					break;
				}
				else continue;
			}
			else{
				TNET_PRINT_LAST_ERROR("send failed.");
				// Under Windows XP if WSAGetLastError()==WSAEINTR then try to disable both the ICS and the Firewall
				// More info about How to disable the ISC: http://support.microsoft.com/?scid=kb%3Ben-us%3B230112&x=6&y=11
				goto bail;
			}
		}
		else{
			sent += ret;
		}
	}

bail:
	//return (size == sent) ? sent : ret;
	return sent;
}

/**@ingroup tnet_utils_group
* Receives data from a connected socket or a bound connectionless socket.
* @param fd The descriptor that identifies a connected socket.
* @param buf A pointer to the buffer to receive the incoming data.
* @param size The length, in bytes, of the buffer pointed to by the buf parameter.
* @param flags A set of flags that influences the behavior of this function.
* All flags which can be passed to @b recv.
* @retval If no error occurs, recv returns the number of bytes received and the buffer pointed to by the buf parameter will contain this data received. If the connection has been gracefully closed, the return value is zero.
* Otherwise, non-zero (negative) error code is returned.
*/
int tnet_sockfd_recv(tnet_fd_t fd, void* buf, tsk_size_t size, int flags)
{
	int ret = -1;

	if(fd == TNET_INVALID_FD){
		TSK_DEBUG_ERROR("Using invalid FD to recv data.");
		goto bail;
	}

	if((ret = recv(fd, buf, size, flags)) <= 0){
		TNET_PRINT_LAST_ERROR("recv failed.");
		goto bail;
	}

bail:
	return ret;
}

/**@ingroup tnet_utils_group
* Establishes a connection to a specified socket.
* @param fd A descriptor identifying an unconnected socket.
* @param to A pointer to the @b sockaddr_storage structure to which the connection should be established.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_sockfd_connectto(tnet_fd_t fd, const struct sockaddr_storage *to)
{
	int status = -1;

#if TNET_UNDER_WINDOWS

	if((status = WSAConnect(fd, (LPSOCKADDR)to, sizeof(*to), NULL, NULL, NULL, NULL)) == SOCKET_ERROR){
		status = WSAGetLastError();
		if(status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_ISCONN || status == TNET_ERROR_INTR || status == TNET_ERROR_INPROGRESS){
			TSK_DEBUG_WARN("TNET_ERROR_WOULDBLOCK/TNET_ERROR_ISCONN/TNET_ERROR_INTR/TNET_ERROR_INPROGRESS  ==> use tnet_sockfd_waitUntilWritable.");
			status = 0;
		}
		else{
			TNET_PRINT_LAST_ERROR("WSAConnect have failed.");
		}
	}

#else /* !TNET_UNDER_WINDOWS */

#if TNET_HAVE_SS_LEN
		if((status = connect(fd, (struct sockaddr*)to, to->ss_len)))
#	else
		if((status = connect(fd, (struct sockaddr*)to, sizeof(*to))))
#	endif
		{
			status = tnet_geterrno();
			if(status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_ISCONN || status == TNET_ERROR_INPROGRESS || status == TNET_ERROR_EAGAIN){
				TSK_DEBUG_WARN("TNET_ERROR_WOULDBLOCK/TNET_ERROR_ISCONN/TNET_ERROR_INPROGRESS/TNET_ERROR_EAGAIN  ==> use tnet_sockfd_waitUntilWritable.");
				status = 0;
			}
			else{
				TNET_PRINT_LAST_ERROR("connect have failed.");
			}
		}

#endif /* TNET_UNDER_WINDOWS */

	return status;
}

/**@ingroup tnet_utils_group
*/
int tnet_sockfd_listen(tnet_fd_t fd, int backlog)
{
	if(fd>0){
		return listen(fd, backlog);
	}
	else return -1;
}

/**@ingroup tnet_utils_group
*/
tnet_fd_t tnet_sockfd_accept(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen)
{
	tnet_fd_t ret = TNET_INVALID_FD;

	if(fd>0){
		ret = accept(fd, addr, addrlen);
	}
	
	return ret;
}

/**@ingroup tnet_utils_group
* Closes an existing socket.
* @param fd A descriptor identifying the socket to close.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_sockfd_close(tnet_fd_t *fd)
{
	int ret;
#if TNET_UNDER_WINDOWS
	ret = closesocket(*fd);
#else
	ret = close(*fd);
#endif

	*fd = TNET_INVALID_FD;
	return ret;
}









































//=================================================================================================
//	INTERFACE object definition
//
static tsk_object_t* tnet_interface_ctor(tsk_object_t * self, va_list * app)
{
	tnet_interface_t *iface = self;
	if(iface){
		const char* description = va_arg(*app, const char*);
		const void* mac_address = va_arg(*app, const void*);
		tsk_size_t mac_address_length = va_arg(*app, tsk_size_t);

		iface->description = tsk_strdup(description);
		if((iface->mac_address = tsk_calloc(mac_address_length, sizeof(uint8_t)))){
			memcpy(iface->mac_address, mac_address, mac_address_length);
		}
		iface->mac_address_length = mac_address_length;
	}
	return self;
}

static tsk_object_t* tnet_interface_dtor(tsk_object_t * self)
{ 
	tnet_interface_t *iface = self;
	if(iface){
		TSK_FREE(iface->description);
		TSK_FREE(iface->mac_address);
	}

	return self;
}

static int tnet_interface_cmp(const tsk_object_t *if1, const tsk_object_t *if2)
{
	const tnet_interface_t *iface1 = if1;
	const tnet_interface_t *iface2 = if2;
	
	if(iface1 && iface2){
		return tsk_stricmp(iface1->description, iface1->description);
	}
	else if(!iface1 && !iface2) return 0;
	else return -1;
}

static const tsk_object_def_t tnet_interface_def_s = 
{
	sizeof(tnet_interface_t),
	tnet_interface_ctor, 
	tnet_interface_dtor,
	tnet_interface_cmp, 
};
const tsk_object_def_t *tnet_interface_def_t = &tnet_interface_def_s;




//=================================================================================================
//	ADDRESS object definition
//
static tsk_object_t* tnet_address_ctor(tsk_object_t * self, va_list * app)
{
	tnet_address_t *address = self;
	if(address){
		address->ip = tsk_strdup(va_arg(*app, const char*));
	}
	return self;
}

static tsk_object_t* tnet_address_dtor(tsk_object_t * self)
{ 
	tnet_address_t *address = self;
	if(address){
		TSK_FREE(address->ip);
	}

	return self;
}

static int tnet_address_cmp(const tsk_object_t *_a1, const tsk_object_t *_a2)
{
	const tnet_address_t *a1 = _a1;
	const tnet_address_t *a2 = _a2;
	
	if(a1 && a2){
		// to have AF_UNSPEC, AF_UNIX, AF_INET, ... first
		return (a1->family - a2->family);
	}
	else if(!a1 && !a2) return 0;
	else return -1;
}

static const tsk_object_def_t tnet_address_def_s = 
{
	sizeof(tnet_address_t),
	tnet_address_ctor, 
	tnet_address_dtor,
	tnet_address_cmp, 
};
const tsk_object_def_t *tnet_address_def_t = &tnet_address_def_s;

