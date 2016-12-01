/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
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
 */

#include "tnet_utils.h"

#include "tsk_thread.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include "tnet_socket.h"
#include "tnet_endianness.h"
#include "dns/tnet_dns_resolvconf.h"

#include <string.h>

#if TSK_UNDER_WINDOWS_RT
#include <vector>
extern std::vector<char> rt_tsk_str_to_native(Platform::String^ str);
extern Platform::String^  rt_tsk_str_to_managed(char const* str);

#endif /* TSK_UNDER_WINDOWS_RT */

#if HAVE_NET_ROUTE_H
#	if TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR // Header missing in device SDK but exist in simulator
#		include "net/_route.h" // from Doubango 3rd parties folder beacuse the one from iOS SDK is incomplete
#	else
#		include <net/route.h>
#	endif
#endif /* HAVE_NET_ROUTE_H */

#if HAVE_NET_IF_TYPES_H
# 	include <net/if_types.h>
#endif /* HAVE_NET_IF_TYPES_H */

#if HAVE_NET_IF_DL_H
# 	include <net/if_dl.h>
#endif /* HAVE_NET_IF_DL_H */

#if HAVE_SYS_RESOURCE_H
#	include <sys/resource.h>
#endif /* HAVE_SYS_RESOURCE_H */

#if HAVE_NETPACKET_PACKET_H
# 	include <netpacket/packet.h>
#endif /* HAVE_NETPACKET_PACKET_H */

#if HAVE_UNISTD_H
#	include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if HAVE_DIRENT_H
#	include <dirent.h>
#endif /* HAVE_DIRENT_H */

#if HAVE_FCNTL_H
#	include <fcntl.h>
#endif /* HAVE_FCNTL_H */

#if HAVE_ARPA_INET_H
#	include <arpa/inet.h>
#endif /* HAVE_ARPA_INET_H */

#ifndef AF_LINK
#	define AF_LINK AF_PACKET
#endif /* AF_LINK */

/**@defgroup tnet_utils_group Network utility functions.
 */


/**@ingroup tnet_utils_group
 * Creates new @ref tnet_interface_t object.
 */
tnet_interface_t* tnet_interface_create(const char* description, const void* mac_address, tsk_size_t mac_address_length)
{
    return (tnet_interface_t*)tsk_object_new(tnet_interface_def_t, description, mac_address, mac_address_length);
}

/**@ingroup tnet_utils_group
 * Creates new @ref tnet_address_t object.
 */
tnet_address_t* tnet_address_create(const char* ip)
{
    return (tnet_address_t*)tsk_object_new(tnet_address_def_t, ip);
}

/**@ingroup tnet_utils_group
 *
 * Gets last network error description.
 *
 * @param [out]	error	The short description of the last network error.
 **/
void tnet_getlasterror(tnet_error_t *error)
{
    int err = tnet_geterrno();
    memset(*error, 0, sizeof(*error));

#if TNET_UNDER_WINDOWS_RT
    // FormatMessageA Not allowed on Market
    static WCHAR wBuff[1024] = { 0 };
    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM,
        tsk_null,
        err,
        0,
        wBuff,
        sizeof(wBuff)-1,
        tsk_null);
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wBuff, wcslen(wBuff), *error, sizeof(*error) - 1, NULL, NULL);
#elif TNET_UNDER_WINDOWS
    {
#ifdef _WIN32_WCE
        FormatMessage
#else
        FormatMessageA
#endif
        (
            FORMAT_MESSAGE_FROM_SYSTEM,
            tsk_null,
            err,
            0,
            *error,
            sizeof(*error) - 1,
            tsk_null);
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

#if TNET_UNDER_WINDOWS/*=== WINDOWS XP/VISTA/7/CE===*/
#if TNET_UNDER_WINDOWS_RT
    TSK_DEBUG_ERROR("Not implemented on your OS");
    goto bail;
#else /* !TNET_UNDER_WINDOWS_RT */
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        TSK_DEBUG_ERROR("Error allocating memory needed to call GetAdaptersinfo.");
        goto bail;
    }
    // Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            TSK_DEBUG_ERROR("Error allocating memory needed to call GetAdaptersinfo.");
            goto bail;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            tnet_interface_t *iface;

            if (pAdapter->Type == MIB_IF_TYPE_LOOPBACK) {
                continue;
            }

            iface = tnet_interface_create(pAdapter->Description, pAdapter->Address, pAdapter->AddressLength);
            iface->index = pAdapter->Index;
            tsk_list_push_back_data(ifaces, &(iface));

            pAdapter = pAdapter->Next;
        }
    }

    if (pAdapterInfo) {
        FREE(pAdapterInfo);
    }


#undef MALLOC
#undef FREE
#endif /* !TNET_UNDER_WINDOWS_RT */

#elif HAVE_IFADDRS_H && HAVE_GETIFADDRS /*=== Using getifaddrs ===*/

    // see http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html
    struct ifaddrs *ifaddr = tsk_null, *ifa = tsk_null;

    /* Get interfaces */
    if(getifaddrs(&ifaddr) == -1) {
        TSK_DEBUG_ERROR("getifaddrs failed and errno= [%d]", tnet_geterrno());
        goto bail;
    }

    for(ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if((ifa->ifa_flags & IFF_LOOPBACK) || !(ifa->ifa_flags & IFF_UP)) {
            continue;
        }

        if(ifa->ifa_addr->sa_family != AF_LINK) {
            continue;
        }

#if defined(__linux__) && 0 /* FIXME */
        {
            struct ifreq ifr;
            tnet_fd_t fd = TNET_INVALID_FD;

            if((fd = socket(AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                TSK_DEBUG_ERROR("Failed to create new DGRAM socket and errno= [%d]", tnet_geterrno());
                goto next;
            }

            ifr.ifr_addr.sa_family = ifa->ifa_addr->sa_family;
            strcpy(ifr.ifr_name, ifa.ifa_name);
            if(tnet_ioctl(fd, SIOCGIFHWADDR, &ifr)<0) {
                TSK_DEBUG_ERROR("tnet_ioctl(SIOCGIFHWADDR)", tnet_geterrno());
                goto next;
            }
            else {
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
    struct ifreq *ifr = NULL;

    if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        TSK_DEBUG_ERROR("Failed to create new DGRAM socket and errno= [%d]", tnet_geterrno());
        goto done;
    }

    ifc.ifc_len = sizeof(buffer);
    ifc.ifc_buf = buffer;

    if(ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        TSK_DEBUG_ERROR("ioctl(SIOCGIFCONF) failed and errno= [%d]", tnet_geterrno());
        goto done;
    }
    if(!ifr || !ifc.ifc_req) {
        TSK_DEBUG_ERROR("ifr or ifc.ifc_req is null");
        goto done;
    }

    if(!ifr->ifr_name) {
        TSK_DEBUG_ERROR("ifr->ifr_name is null");
        goto done;
    }

    for(ifr = ifc.ifc_req; ifr && !tsk_strempty(ifr->ifr_name); ifr++) {
        sin = (struct sockaddr_in *)&(ifr->ifr_addr);
        // TODO: IPAddress if needed
        if(/*ioctl(fd, SIOCGIFFLAGS, &ifr) == 0*/1) {
            if (!(ifr->ifr_flags & IFF_LOOPBACK) && (ifr->ifr_flags & IFF_UP)) {
                if(/*ioctl(fd, SIOCGIFHWADDR, &ifr) == 0*/1) {
                    tnet_interface_t *iface = tnet_interface_create(ifr->ifr_name, ifr->ifr_hwaddr.sa_data, 6);
                    tsk_list_push_back_data(ifaces, (void**)&(iface));
                    //iface->index = if_nametoindex(ifr->ifr_name);
                }
            }
        }
        else {
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
tnet_addresses_L_t* tnet_get_addresses(tnet_family_t family, tsk_bool_t unicast, tsk_bool_t anycast, tsk_bool_t multicast, tsk_bool_t dnsserver, long if_index)
{
    tnet_addresses_L_t *addresses = tsk_list_create();

#if TNET_UNDER_WINDOWS
#if TNET_UNDER_WINDOWS_RT
    TSK_DEBUG_ERROR("Not implemented on your OS");
#else /* !TSK_UNDER_WINDOWS_RT */

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
    pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);

    // Make an initial call to GetAdaptersAddresses to get the
    // size needed into the outBufLen variable
    if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
    }
    else {
        goto bail;
    }

    if (pAddresses == NULL) {
        TSK_DEBUG_ERROR("Memory allocation failed for IP_ADAPTER_ADDRESSES struct.");
        goto bail;
    }

    dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

    if (dwRetVal == NO_ERROR) {
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            if ((if_index != -1) && (pCurrAddresses->IfIndex != if_index && pCurrAddresses->Ipv6IfIndex != if_index)) {
                goto next;
            }
            if (pCurrAddresses->OperStatus != IfOperStatusUp) {
                goto next;
            }

            /* == UNICAST addresses == */
            pUnicast = pCurrAddresses->FirstUnicastAddress;
            while (unicast && pUnicast) {
                //memset(ip, '\0', sizeof(ip));
                tnet_get_sockip(pUnicast->Address.lpSockaddr, &ip);
                TSK_DEBUG_INFO("Found local IP address = AdapterName=%s Ip=%s", pCurrAddresses->AdapterName, ip);
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
            while (anycast && pAnycast) {
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
            while (multicast && pMulticast) {
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
            while (dnsserver && pDnServer) {
                //memset(ip, '\0', sizeof(ip));
                if (!tnet_get_sockip(pDnServer->Address.lpSockaddr, &ip)) {
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

    if (pAddresses) {
        FREE(pAddresses);
    }

#undef MALLOC
#undef FREE

bail :

#endif /* !TSK_UNDER_WINDOWS_RT */

#else	/* !TSK_UNDER_WINDOWS (MAC OS X, UNIX, ANDROID ...) */

    tnet_ip_t ip;
#if HAVE_IFADDRS_H && HAVE_GETIFADDRS /*=== Using getifaddrs ===*/

    // see http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html
    struct ifaddrs *ifaddr = tsk_null, *ifa = tsk_null;
    struct sockaddr *addr;

    /* Get interfaces */
    if(getifaddrs(&ifaddr) == -1) {
        TSK_DEBUG_ERROR("getifaddrs failed and errno= [%d]", tnet_geterrno());
        goto bail;
    }

    /* == Unicast addresses == */
    for(ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if(!ifa->ifa_addr) {
            continue;
        }
        // Skip loopback
        if ((ifa->ifa_flags & IFF_LOOPBACK) || !(ifa->ifa_flags & IFF_UP)) {
            continue;
        }

        // Skip unwanted interface
        if (if_index != -1 && if_nametoindex(ifa->ifa_name) != if_index) {
            continue;
        }

        // Only deal with Unicast address
        if (unicast) {
            if (family == AF_INET && ifa->ifa_addr->sa_family != AF_INET) {
                continue;
            }
            if (family == AF_INET6 && ifa->ifa_addr->sa_family != AF_INET6) {
                continue;
            }
            if (ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6) {
                continue;
            }

            // Get the IP string
            addr = (struct sockaddr *) ifa->ifa_addr;
            tnet_get_sockip(addr, &ip);

            // Push a new address
            tnet_address_t *address = tnet_address_create(ip);
            address->family = ifa->ifa_addr->sa_family;
            address->unicast = 1;
            tsk_list_push_ascending_data(addresses, (void **) &address);
        }
    }

    if (ifaddr) {
        free(ifaddr);
    }

#else /* ANDROID or any system without getifaddrs */

    tnet_address_t *address;
    tnet_fd_t fd = TNET_INVALID_FD;
    struct ifconf ifc;
    struct ifreq *ifr = 0;
    memset(&ifc, 0, sizeof(ifc));

    if((fd = socket(family, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        TSK_DEBUG_ERROR("Failed to create new DGRAM socket and errno= [%d]", tnet_geterrno());
        goto done;
    }

    if(ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        TSK_DEBUG_ERROR("ioctl(SIOCGIFCONF) failed and errno= [%d]", tnet_geterrno());
        goto done;
    }

    if (!(ifr = (struct ifreq*) malloc(ifc.ifc_len))) {
        TSK_DEBUG_ERROR("Could not malloc ifreq with size =%d", ifc.ifc_len);
        goto done;
    }

    ifc.ifc_ifcu.ifcu_req = ifr;
    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        TSK_DEBUG_ERROR("ioctl SIOCGIFCONF failed");
        goto done;
    }

    int i;
    for(i = 0; i < ifc.ifc_len / sizeof(struct ifreq); ++i) {
        if (unicast) {

        }
        // Skip unwanted interface
        if (if_index != -1 && ifr->ifr_ifindex != if_index) {
            continue;
        }

        // Get the IP string
        if(tnet_get_sockip(&ifr[i].ifr_addr, &ip) == 0) {
            // Push a new address
            if((address = tnet_address_create(ip))) {
                address->family = family;
                address->unicast = unicast;
                tsk_list_push_ascending_data(addresses, (void **) &address);
            }
        }
    }

done:
    TSK_FREE(ifr);
    tnet_sockfd_close(&fd);

#endif /* HAVE_IFADDRS_H && HAVE_GETIFADDRS */

bail:

    /* == DNS servers == */
    if(dnsserver) {
        TSK_DEBUG_INFO("Calling 'tnet_dns_resolvconf_parse()' to load DNS servers");
        tnet_addresses_L_t * dns_servers;
        if((dns_servers = tnet_dns_resolvconf_parse("/etc/resolv.conf"))) {
            tsk_list_pushback_list(addresses, dns_servers);
            TSK_OBJECT_SAFE_FREE(dns_servers);
        }
    }

#endif

    return addresses;
}


/**@ingroup tnet_utils_group
*/
int tnet_get_mac_address(tnet_mac_address* address)
{
    static const tsk_size_t __tnet_mac_address_len = sizeof(tnet_mac_address) / sizeof(uint8_t/*tnet_mac_address[0]*/);
    int ret = -1;
    if (!address) {
        TSK_DEBUG_ERROR("Invalid parameter");
    }
#if TNET_UNDER_WINDOWS
#	if TNET_UNDER_WINDOWS_RT
    TSK_DEBUG_ERROR("Not implemented on your OS");
#	else /* !TSK_UNDER_WINDOWS_RT */
    {
        IP_ADAPTER_INFO *info = NULL, *pos;
        DWORD size = 0;
        ULONG _ret;

        if ((_ret = GetAdaptersInfo(info, &size)) == ERROR_SUCCESS || _ret == ERROR_BUFFER_OVERFLOW) {
            info = (IP_ADAPTER_INFO *)tsk_calloc(size + 1, 1);
            if (info) {
                if ((_ret = GetAdaptersInfo(info, &size)) == ERROR_SUCCESS) {
                    UINT i;
                    for (pos = info; pos != NULL && ret != 0; pos = pos->Next) {
                        if (pos->Type == MIB_IF_TYPE_LOOPBACK && pos->Next) { // skip loopback if we still have items to check
                            continue;
                        }
                        for (i = 0; i < pos->AddressLength && i < __tnet_mac_address_len; ++i) {
                            (*address)[i] = pos->Address[i];
                        }
                        ret = 0;
                    }
                }
            }
            TSK_FREE(info);
        }
    }
#	endif /* TSK_UNDER_WINDOWS_RT */
#elif HAVE_IFADDRS_H && HAVE_GETIFADDRS && HAVE_STRUCT_SOCKADDR_DL
    struct ifaddrs *ifaddrs, *ifaddr;
    struct sockaddr_dl* sdl;
    if (getifaddrs(&ifaddrs) == 0) {
        for (ifaddr = ifaddrs; ifaddr; ifaddr = ifaddr->ifa_next) {
            // Skip loopback
            if ((ifaddr->ifa_addr->sa_family != AF_LINK)) {
                continue;
            }
            sdl = (struct sockaddr_dl*)ifaddr->ifa_addr;
            if (sdl->sdl_alen == __tnet_mac_address_len) {
                memcpy(&(*address)[0], LLADDR(sdl), sdl->sdl_alen);
                ret = 0;
                break;
            }
        }
        freeifaddrs(ifaddrs);
    }
    else {
        TSK_DEBUG_ERROR("getifaddrs failed");
    }
#elif defined(SIOCGIFHWADDR)
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];

    tnet_fd_t fd = tnet_soccket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (fd == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Failed to create socket");
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(fd, SIOCGIFCONF, &ifc) != 0) {
        TSK_DEBUG_ERROR("ioctl(%d, SIOCGIFCONF) failed", fd);
        tnet_sockfd_close(&fd);
        return -1;
    }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(fd, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // skip loopback
                if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
                    tsk_size_t min_size = TSK_MIN(__tnet_mac_address_len, 6);
                    memcpy(&(*address)[0], &ifr.ifr_hwaddr.sa_data[0], min_size);
                    ret = 0;
                    break;
                }
            }
        }
        else {
            TSK_DEBUG_WARN("ioctl(%d, SIOCGIFFLAGS) failed", fd);
        }
    }
#endif

    return ret;
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

#if TNET_UNDER_WINDOWS
    long dwBestIfIndex = -1;
#endif
#if TNET_UNDER_WINDOWS_RT
    Windows::Networking::Connectivity::ConnectionProfile^ profile;
#endif

    if (!destination || !source) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    memset(*source, '\0', sizeof(*source));

    // special cases for Windows Phone device and emulator
#if TNET_UNDER_WINDOWS_PHONE
    if (tsk_strequals(destination, "127.0.0.1")) {
        memcpy(*source, "127.0.0.1", 9);
        ret = 0;
        goto bail;
    }
    if(tsk_strequals(destination, "::1")) {
        memcpy(*source, "::1", 3);
        ret = 0;
        goto bail;
    }
#endif

    if((ret = tnet_sockaddr_init(destination, port, type, &destAddr))) {
        goto bail;
    }

#if TNET_UNDER_WINDOWS_RT /* Windows Phone 8, Surface or any RT */
    profile = Windows::Networking::Connectivity::NetworkInformation::GetInternetConnectionProfile();

    if (profile != nullptr && profile->NetworkAdapter != nullptr) {
        TSK_DEBUG_INFO("Network profile IanaInterfaceType = %d", profile->NetworkAdapter->IanaInterfaceType);
        Windows::Foundation::Collections::IVectorView<Windows::Networking::HostName^>^ HostNames = Windows::Networking::Connectivity::NetworkInformation::GetHostNames();

        if(HostNames->Size > 0) {
            Windows::Foundation::Collections::IIterator<Windows::Networking::HostName^>^ HostName = HostNames->First();
            do {
                std::vector<char> CanonicalName = rt_tsk_str_to_native(HostName->Current->CanonicalName);
                TSK_DEBUG_INFO("Checking IP address = %s", CanonicalName.data());
                if((TNET_SOCKET_TYPE_IS_IPV4(type) && HostName->Current->IPInformation->PrefixLength->Value > 32) || (TNET_SOCKET_TYPE_IS_IPV6(type) && HostName->Current->IPInformation->PrefixLength->Value > 128)) {
                    TSK_DEBUG_INFO("Type mismatch - Skiping IP address=%s, IanaInterfaceType=%d, PrefixLength=%d", CanonicalName.data(), HostName->Current->IPInformation->NetworkAdapter->IanaInterfaceType, HostName->Current->IPInformation->PrefixLength->Value);
                    continue;
                }


                if(HostName->Current->IPInformation != nullptr) {
                    // http://msdn.microsoft.com/en-us/library/windows/apps/windows.networking.connectivity.networkadapter.networkadapterid.aspx
                    // HostName->Current->IPInformation->NetworkAdapter->NetworkAdapterId not implemented on WP8
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
                    tnet_socket_t* ss = tnet_socket_create(CanonicalName.data(), TNET_SOCKET_PORT_ANY, type);
                    if(ss) {
                        ret = connect(ss->fd, (const sockaddr*)&destAddr, tnet_get_sockaddr_size((const sockaddr*)&destAddr));
                        if(ret && tnet_geterrno() == TNET_ERROR_EAGAIN) {
                            ret = tnet_sockfd_waitUntilWritable(ss->fd, 500);
                        }
                        TSK_OBJECT_SAFE_FREE(ss);
                    }
# else
                    if(HostName->Current->IPInformation->NetworkAdapter->IanaInterfaceType == profile->NetworkAdapter->IanaInterfaceType) {
                        ret = 0;
                    }
#endif /*  */

                    if(ret == 0) {
                        TSK_DEBUG_INFO("Using best IP address = %s :)", CanonicalName.data());
                        memcpy(*source, CanonicalName.data(), TSK_MIN(tsk_strlen(CanonicalName.data()), sizeof(*source)));
                        ret = 0;
                        goto bail;
                    }
                    TSK_DEBUG_INFO("Connection check - Skiping IP address = %s", CanonicalName.data());
                }
            }
            while(HostName->MoveNext());
        }
    }
    else {
        TSK_DEBUG_ERROR("No network connection available");
    }

#elif TNET_UNDER_WINDOWS /* Windows XP/Vista/7 and Windows Mobile */
    if(GetBestInterfaceEx((struct sockaddr*)&destAddr, &dwBestIfIndex) != NO_ERROR) {
        ret = tnet_geterrno();
        TNET_PRINT_LAST_ERROR("GetBestInterfaceEx() failed.");
        goto bail;
    }
    else {
        tnet_addresses_L_t* addresses = tsk_null;
        const tsk_list_item_t* item;

		if (!(addresses = tnet_get_addresses(destAddr.ss_family, tsk_true, tsk_false, tsk_false, tsk_false, dwBestIfIndex))) {
            ret = -2;
            TSK_DEBUG_ERROR("Failed to retrieve addresses.");
            goto bail;
        }

        tsk_list_foreach(item, addresses) {
            const tnet_address_t* address = item->data;
            if (address && address->ip) {
                memcpy(*source, address->ip, tsk_strlen(address->ip) > sizeof(*source) ? sizeof(*source) : tsk_strlen(address->ip));
                ret = 0;
                break; // First is good for us.
            }
        }
        TSK_OBJECT_SAFE_FREE(addresses);
    }
#elif HAVE_NET_ROUTE_H && HAVE_IFADDRS_H && HAVE_GETIFADDRS /* Mac OS X, iPhone, iPod Touch, iPad and Linux family except Android */
    /* Thanks to Laurent Etiemble */

    int sdl_index = -1;

#if HAVE_STRUCT_RT_METRICS && HAVE_STRUCT_SOCKADDR_DL
    static int seq = 1234;
    char buf[1024];
    char *cp;
    int s, i, l, rlen;
    int pid = getpid();
    u_long rtm_inits;
    struct rt_metrics rt_metrics;
    struct sockaddr_dl *ifp = NULL;
    struct rt_msghdr *rtm = (struct	rt_msghdr *)buf;
    struct sockaddr_dl so_ifp;
#endif /* HAVE_STRUCT_RT_METRICS && HAVE_STRUCT_SOCKADDR_DL */

    struct sockaddr_storage so_dst = destAddr;
    struct sockaddr *sa = NULL;
    struct ifaddrs *ifaddr = 0, *ifa = tsk_null;
    tnet_ip_t ip;

#if HAVE_STRUCT_RT_METRICS && HAVE_STRUCT_SOCKADDR_DL
    bzero(rtm, 1024);
    cp = (char *)(rtm + 1);

    so_ifp.sdl_index = 0;
    so_ifp.sdl_family = AF_LINK;
    so_ifp.sdl_len = sizeof(struct sockaddr_dl);

    rtm->rtm_type = RTM_GET;
    rtm->rtm_flags = RTF_STATIC | RTF_UP | RTF_GATEWAY;
    rtm->rtm_version = RTM_VERSION;
    rtm->rtm_seq = ++seq;
    rtm->rtm_addrs = RTA_DST | RTA_IFP;
    rtm->rtm_rmx = rt_metrics;
    rtm->rtm_inits = rtm_inits;
    rtm->rtm_index = 0;

    /** Roundup value to a 4 bytes boundary. */
#define ROUNDUP(a) \
((a) > 0 ? (1 + (((a) - 1) | (sizeof(uint32_t) - 1))) : sizeof(uint32_t))

    l = ROUNDUP(so_dst.ss_len);
    memcpy(&so_dst, cp, l);
    cp += l;

    l = ROUNDUP(so_ifp.sdl_len);
    memcpy(&so_ifp, cp, l);
    cp += l;

    l = cp - buf;
    rtm->rtm_msglen = l;

    s = socket(PF_ROUTE, SOCK_RAW, 0);
    if (s < 0) {
        // TODO
    }

    if ((rlen = write(s, rtm, l)) < 0) {
        TSK_DEBUG_INFO("writing to routing socket");
        // TODO
    }
    do {
        l = read(s, rtm, 1024);
    }
    while (l > 0 && (rtm->rtm_seq != seq || rtm->rtm_pid != pid));

    /** Advance an address to the closest 4 bytes boundary. */
#define ADVANCE(x, n) (x += ROUNDUP((n)->sa_len))

    if (rtm->rtm_errno == 0 && rtm->rtm_addrs) {
        cp = (char *)(rtm + 1);
        for (i = 1; i; i <<= 1) {
            if (i & rtm->rtm_addrs) {
                sa = (struct sockaddr *)cp;
                switch (i) {
                case RTA_IFP:
                    ifp = (struct sockaddr_dl *) sa;
                    break;
                }
                ADVANCE(cp, sa);
            }
        }
    }
    if(ifp) {
        sdl_index = ifp->sdl_index;
    }
#endif /* HAVE_STRUCT_RT_METRICS && HAVE_STRUCT_SOCKADDR_DL */

    /* Get interfaces */
    if(getifaddrs(&ifaddr) == -1) {
        TNET_PRINT_LAST_ERROR("getifaddrs() failed.");
        goto bail;
    }

    for(ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if ((ifa->ifa_flags & IFF_LOOPBACK) || !(ifa->ifa_flags & IFF_UP)) {
            continue;
        }

        if (sdl_index != -1 && if_nametoindex(ifa->ifa_name) != sdl_index) {
            continue;
        }

        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != destAddr.ss_family) {
            continue;
        }
        
        if (destAddr.ss_family == AF_INET) {
            if (tnet_is_linklocal(ifa->ifa_addr) ^ tnet_is_linklocal(((struct sockaddr *)&destAddr))) {
                TSK_DEBUG_INFO("Ignoring IPv4 linklocal address");
                continue;
            }
        }

        if (destAddr.ss_family == AF_INET6) {
            if (IN6_IS_ADDR_LINKLOCAL(&((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr) ^
                    IN6_IS_ADDR_LINKLOCAL(&((struct sockaddr_in6 *) &destAddr)->sin6_addr)) {
                TSK_DEBUG_INFO("Ignoring IPv6 linklocal address");
                continue;
            }
            if (IN6_IS_ADDR_SITELOCAL(&((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr) ^
                    IN6_IS_ADDR_SITELOCAL(&((struct sockaddr_in6 *) &destAddr)->sin6_addr)) {
                TSK_DEBUG_INFO("Ignoring IPv6 sitelocal address");
                continue;
            }
        }

        tnet_get_sockip((struct sockaddr *) ifa->ifa_addr, &ip);

        memcpy(*source, ip, tsk_strlen(ip) > sizeof(*source) ? sizeof(*source) : tsk_strlen(ip));
        ret = 0;
        goto bail; // First is good for us.
    }


#else /* All other systems (Google Android, Unix-Like systems, uLinux, ....) */
    TSK_DEBUG_WARN("getbestroute() not supported on this OS");
    memcpy(*source,
           TNET_SOCKET_TYPE_IS_IPV6(type) ? "::" : "0.0.0.0",
           TNET_SOCKET_TYPE_IS_IPV6(type) ? 2 : 7
          );
#endif

bail:
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
int tnet_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    int ret = -1;
    if (hints && (ret = getaddrinfo(node, service, hints, res))) {
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
    if (ai) {
        freeaddrinfo(ai);
    }
}

/**@ingroup tnet_utils_group
 * Converts a descriptor to @b sockaddr_storage structure.
 * @param fd The descriptor to convert.
 * @param result @b sockaddr_storage structre representing the desciptor.
 * @retval Zero if succeed and non-zero error code otherwise.
 */
int tnet_getsockname(tnet_fd_t fd, struct sockaddr_storage *result)
{
    if (fd > 0 && result) {
        socklen_t namelen = sizeof(*result);
        return getsockname(fd, (struct sockaddr*)result, &namelen);
    }
    return -1;
}

int tnet_getpeername(tnet_fd_t fd, struct sockaddr_storage *result)
{
    if (fd > 0 && result) {
        socklen_t namelen = sizeof(*result);
        return getpeername(fd, (struct sockaddr*)result, &namelen);
    }
    return -1;
}

tnet_socket_type_t tnet_get_type(const char* host, tnet_port_t port)
{
	tnet_socket_type_t ret = TNET_SOCKET_TYPE_UDP;
    if (host) {
        int status;
        tsk_istr_t srv;
        struct addrinfo *result = tsk_null;
        struct addrinfo hints;
		const struct addrinfo *ptr = tsk_null;

        /* set the port: used as the default service */
        tsk_itoa(port ? port : 5060, &srv); // service must not be empty -> Android IPv6 issue

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        if ((status = tnet_getaddrinfo(host, srv, &hints, &result))) {
            TNET_PRINT_LAST_ERROR("getaddrinfo(%s:%d) failed", host, port);
            goto done;
        }
     
		for (ptr = result; ptr; ptr = ptr->ai_next) {
			if (ptr->ai_family == AF_INET) {
				TNET_SOCKET_TYPE_SET_IPV4(ret);
			}
			else if (ptr->ai_family == AF_INET6) {
				TNET_SOCKET_TYPE_SET_IPV6(ret);
			}
		}
done:
        tnet_freeaddrinfo(result);
    }

    return ret;
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
    if (host) {
        int status;
        tsk_istr_t srv;
        struct addrinfo *result = tsk_null;
        struct addrinfo hints;

        /* set the port: used as the default service */
        if (port) {
            tsk_itoa(port, &srv);
        }
        else {
            TSK_DEBUG_WARN("Empty port may lead to getaddrinfo issue on Android");
            memset(srv, '\0', sizeof(srv));
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;

        if ((status = tnet_getaddrinfo(host, srv, &hints, &result))) {
            TNET_PRINT_LAST_ERROR("getaddrinfo(%s:%d) failed", host, port);
            goto done;
        }

        /* Get the First result. */
        if (result) {
            ret = result->ai_family;
            goto done;
        }
done:
        tnet_freeaddrinfo(result);
    }

    return ret;
}

tsk_bool_t tnet_is_ipv6(const char* host, tnet_port_t port)
{
    // getaddrinfo with empty port fails on Android, set default port to 5060
    tnet_socket_type_t type = tnet_get_type(host, port ? port : 5060);
	return TNET_SOCKET_TYPE_IS_IPV6(type);
}

/**@ingroup tnet_utils_group
 * Gets the IP address and the Port of a @b sockaddr object.
 * @param addr [in] A pointer to @b sockaddr structure for which to retrieve the IP address and port.
 * @param ip [out] The IP address.
 * @param port [out] The port.
 * @retval Zero if succeed and non-zero error code otherwise.
 */
int tnet_get_sockip_n_port(const struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)
{
    int status = -1;

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
        if (port) {
            *port = tnet_ntohs(sin->sin_port);
            status = 0;
        }
        if (ip) {
            if ((status = tnet_getnameinfo((const struct sockaddr*)sin, sizeof(*sin), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST))) {
                return status;
            }
        }
    }
    else if (addr->sa_family == AF_INET6) {
        const struct sockaddr_in6 *sin6 = (const struct sockaddr_in6 *)addr;
#if TNET_UNDER_WINDOWS
        int index;
#endif
        if (port) {
            *port = tnet_ntohs(sin6->sin6_port);
            status = 0;
        }
        if (ip) {
            if ((status = tnet_getnameinfo((const struct sockaddr*)sin6, sizeof(*sin6), *ip, sizeof(*ip), 0, 0, NI_NUMERICHOST))) {
                return status;
            }

#if TNET_UNDER_WINDOWS
            if ((index = tsk_strindexOf(*ip, tsk_strlen(*ip), "%")) > 0) {
                *(*ip + index) = '\0';
            }
#endif
        }
    }
    else {
        TSK_DEBUG_ERROR("Unsupported address family.");
        return status;
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
    if (port) {
        *port = 0;
    }

    if (localFD > 0) {
        int status;
        socklen_t len;
        struct sockaddr_storage ss;

        len = sizeof(ss);
        if ((status = getpeername(localFD, (struct sockaddr *)&ss, &len))) {
            TSK_DEBUG_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
            return -1;
        }

        return tnet_get_sockip_n_port(((struct sockaddr *)&ss), ip, port);
    }

    TSK_DEBUG_ERROR("Could not use an invalid socket description.");
    return -1;
}

/**@ingroup tnet_utils_group
 * Gets the IP address and the Port of a socket (File descriptor).
 * @param fd The decriptor for which to retrive the IP address and port.
 * @param getlocal Whether to get local or remote ip and port
 * @param ip [out] The IP address of the local socket.
 * @param port [out] The port of the local socket.

 * @retval Zero if succeed and non-zero error code otherwise.
 */
int tnet_get_ip_n_port(tnet_fd_t fd, tsk_bool_t getlocal, tnet_ip_t *ip, tnet_port_t *port)
{
    if (port) {
        *port = 0;
    }

    if (fd > 0) {
        int status;
        struct sockaddr_storage ss;
        status = getlocal ? tnet_getsockname(fd, &ss) : tnet_getpeername(fd, &ss);
        if (status) {
            TNET_PRINT_LAST_ERROR("TNET_GET_SOCKADDR has failed with status code: %d", status);
            return -1;
        }

        return tnet_get_sockip_n_port(((struct sockaddr *)&ss), ip, port);
    }

    TSK_DEBUG_ERROR("Could not use an invalid socket description.");
    return -1;
}

/**@ingroup tnet_utils_group
 */
tsk_bool_t tnet_is_loopback(const struct sockaddr *sa)
{
    if (!sa || (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)) {
        TSK_DEBUG_ERROR("Invalid paramete");
        return tsk_false;
    }
    if (sa->sa_family == AF_INET) {
        const uint8_t* u8 = (const uint8_t*)&((const struct sockaddr_in*)sa)->sin_addr;
        return u8[0] == 127;
    }
    else {
        const uint32_t* u32 = (const uint32_t*)&((const struct sockaddr_in6*)sa)->sin6_addr;
        return (u32[0] == 0) && (u32[4] == 0) && (u32[8] == 0) && (u32[12] == tnet_ntohl(1));
    }
}

/**@ingroup tnet_utils_group
 */
tsk_bool_t tnet_is_linklocal(const struct sockaddr *sa)
{
    if (!sa || (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)) {
        TSK_DEBUG_ERROR("Invalid paramete");
        return tsk_false;
    }
    if (sa->sa_family == AF_INET) {
        const uint8_t* u8 = (const uint8_t*)&((const struct sockaddr_in*)sa)->sin_addr;
        return u8[0] == 169 && u8[1] == 254;
    }
    else {
        const uint8_t* u8 = (const uint8_t*)&((const struct sockaddr_in6*)sa)->sin6_addr;
        return ((u8[0] == 0xfe) && ((u8[1] & 0xc0) == 0x80));
    }
}

/**@ingroup tnet_utils_group
 * Gets the maximum number of file descriptors (FDs) this process is allowed to open.
 */
int tnet_get_fd_max_allowed(tsk_size_t* fd_size)
{
#if HAVE_GETRLIMIT
    struct rlimit rl;
    int ret;
    if (!fd_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    ret = getrlimit(RLIMIT_NOFILE, &rl);
    if (ret) {
        TSK_DEBUG_ERROR("getrlimit(RLIMIT_NOFILE) failed with error code = %d", tnet_geterrno());
        return ret;
    }
    *fd_size = rl.rlim_cur;
    return 0;
#elif HAVE_GETDTABLESIZE
    return getdtablesize();
#else
    return -1;
#endif
}

/**@ingroup tnet_utils_group
 * Sets the maximum number of file descriptors (FDs) this process is allowed to open.
 */
int tnet_set_fd_max_allowed(tsk_size_t fd_size)
{
#if HAVE_SETRLIMIT && HAVE_GETRLIMIT
    struct rlimit rl;
    int ret;
    ret = getrlimit(RLIMIT_NOFILE, &rl);
    if (!ret) {
        rl.rlim_cur = fd_size;
        ret = setrlimit(RLIMIT_NOFILE, &rl);
    }
    return ret;
#else
    return -1;
#endif
}

/**@ingroup tnet_utils_group
 * Gets the number of FDs opened by this process.
 */
int tnet_get_fd_opened_count(tsk_size_t* count)
{
#if HAVE_OPENDIR && HAVE_CLOSEDIR && HAVE_GETPID && HAVE_STRUCT_DIRENT
    int fd_count;
    char buf[1024];
    struct dirent *dp;
    DIR *dir;

    if (!count) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *count = 0;
    snprintf(buf, 1024, "/proc/%i/fd/", getpid());
    dir = opendir(buf);
    while ((dp = readdir(dir))) {
        (*count)++;
    }
    closedir(dir);
    return 0;
#else
    return -1;
#endif
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
 * see http://man7.org/linux/man-pages/man3/inet_pton.3.html
 * @retval 1 if succeed.
 */
int tnet_inet_pton(int af, const char* src, void* dst)
{
    if (!src || !dst) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
#if HAVE_INET_PTON || TNET_UNDER_APPLE
    return inet_pton(af, src, dst);
#elif TNET_UNDER_WINDOWS && !(TNET_UNDER_WINDOWS_RT || TNET_UNDER_WINDOWS_CE)
#	if (_WIN32_WINNT <= 0x0501)
    {
        struct sockaddr_storage addr = { 0 };
        int addr_len = (af == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
        if (WSAStringToAddressA((LPSTR)src, af, NULL, (struct sockaddr *)&addr, &addr_len) == 0) {
            if (af == AF_INET6) {
                memcpy(dst, &((struct sockaddr_in6 *)&addr)->sin6_addr, 16);
            }
            else {
                memcpy(dst, &((struct sockaddr_in *)&addr)->sin_addr, 4);
            }
            return 1;
        }
        TNET_PRINT_LAST_ERROR("WSAStringToAddressA failed");
        return -2;
    }
#	else
    return InetPtonA(af, src, dst);
#	endif // TNET_UNDER_WINDOWS
#else
    {
        struct sockaddr_storage addr = { 0 };
        int ret;
        if ((ret = tnet_sockaddr_init(src, 0, (af == AF_INET6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4), &addr))) {
            return -2;
        }
        if (af == AF_INET6) {
            *((struct in6_addr *)dst) = ((struct sockaddr_in6 *)&addr)->sin6_addr;
        }
        else {
            *((struct in_addr *)dst) = ((struct sockaddr_in *)&addr)->sin_addr;
        }
        return 1;
    }
#endif
}

/**@ingroup tnet_utils_group
 * see http://pubs.opengroup.org/onlinepubs/009695399/functions/inet_ntop.html
 */
const char *tnet_inet_ntop(int af, const void *src, char *dst, int size)
{
    if (!src || !dst || size <= 0) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    memset(dst, 0, size);
#if HAVE_INET_NTOP || TNET_UNDER_APPLE
    return inet_ntop(af, src, dst, size);
#elif TNET_UNDER_WINDOWS && !(TNET_UNDER_WINDOWS_RT || TNET_UNDER_WINDOWS_CE)
#	if (_WIN32_WINNT <= 0x0501)
    {
        struct sockaddr_storage addr = { 0 };
        int addr_len = sizeof(addr);
        if (af == AF_INET6) {
            if (size < INET6_ADDRSTRLEN) {
                TSK_DEBUG_ERROR("Destination size too short(%d)", size);
                return tsk_null;
            }
            ((struct sockaddr_in6 *)&addr)->sin6_family = AF_INET6;
            memcpy(&((struct sockaddr_in6 *)&addr)->sin6_addr, src, 16);
        }
        else {
            if (size < INET_ADDRSTRLEN) {
                TSK_DEBUG_ERROR("Destination size too short(%d)", size);
                return tsk_null;
            }
            ((struct sockaddr_in *)&addr)->sin_family = AF_INET;
            memcpy(&((struct sockaddr_in *)&addr)->sin_addr, src, 4);
        }
        if (WSAAddressToStringA((struct sockaddr*)&addr, addr_len, NULL, dst, &size) == 0) {
            return dst;
        }
        TNET_PRINT_LAST_ERROR("WSAAddressToStringA failed");
        return tsk_null;
    }
#	else
    return InetNtopA(af, (PVOID)src, dst, size);
#	endif // TNET_UNDER_WINDOWS
#else
    {
        struct sockaddr_storage addr = { 0 };
        tnet_ip_t ip;
        if (af == AF_INET6) {
            if (size < INET6_ADDRSTRLEN) {
                TSK_DEBUG_ERROR("Destination size too short(%d)", size);
                return tsk_null;
            }
            addr.ss_family = AF_INET6;
            ((struct sockaddr_in6 *)&addr)->sin6_addr = *((struct in6_addr *)src);

            if (tnet_get_sockip((const struct sockaddr *)&addr, &ip)) {
                return tsk_null;
            }
            memcpy(dst, ip, INET6_ADDRSTRLEN);
            return dst;
        }
        else {
            if (size < INET_ADDRSTRLEN) {
                TSK_DEBUG_ERROR("Destination size too short(%d)", size);
                return tsk_null;
            }
            addr.ss_family = AF_INET;
            ((struct sockaddr_in *)&addr)->sin_addr = *((struct in_addr *)src);
            if (tnet_get_sockip((const struct sockaddr *)&addr, &ip)) {
                return tsk_null;
            }
            memcpy(dst, ip, INET_ADDRSTRLEN);
            return dst;
        }
    }
#endif
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

    if (fd <= 0) {
        goto bail;
    }

    if (timeout >= 0) {
        timetowait.tv_sec = (timeout / 1000);
        timetowait.tv_usec = (timeout % 1000) * 1000;
    }

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    ret = select(fd + 1, writable ? 0 : &fds, writable ? &fds : 0, 0, (timeout >= 0) ? &timetowait : 0);

    if (ret == 0) { /* timedout */
        ret = -2;
    }
    else if (ret == 1/* the total number of socket handles that are ready */) {
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
 * Performs DNS A/AAAA to convert the FQDN to IP address.
 */
int tnet_resolve(const char *fqdn, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t* out_ip, tnet_port_t* out_port)
{
    struct sockaddr_storage addr;
    int ret = tnet_sockaddr_init(fqdn, port, type, &addr);
    if (ret == 0) {
        return tnet_get_sockip_n_port((const struct sockaddr *)&addr, out_ip, out_port);
    }
    return ret;
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
    hints.ai_family = TNET_SOCKET_TYPE_IS_IPV46(type) ? AF_UNSPEC : (TNET_SOCKET_TYPE_IS_IPV6(type) ? AF_INET6 : AF_INET);
    hints.ai_socktype = TNET_SOCKET_TYPE_IS_STREAM(type) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_protocol = TNET_SOCKET_TYPE_IS_STREAM(type) ? IPPROTO_TCP : IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;

    /* Performs getaddrinfo */
    if ((status = tnet_getaddrinfo(host, p, &hints, &result))) {
        TNET_PRINT_LAST_ERROR("getaddrinfo have failed.");
        goto bail;
    }

    /* Find our address. */
    for (ptr = result; ptr; ptr = ptr->ai_next) {
        /* Only IPv4 and IPv6 are supported */
        if (ptr->ai_family != AF_INET6 && ptr->ai_family != AF_INET) {
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

        if (ai_addr) {
            memcpy(ai_addr, ptr->ai_addr, ptr->ai_addrlen);
        }
        if (ai_family) {
            *ai_family = ptr->ai_family;
        }
        if (ai_socktype) {
            *ai_socktype = ptr->ai_socktype;
        }
        if (ai_protocol) {
            *ai_protocol = ptr->ai_protocol;
        }

        /* We prefer IPv4 but IPv6 can also work */
        if (ptr->ai_family == AF_INET) {
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

    if ((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, 0, 0, 0))) {
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

    if ((status = tnet_sockaddrinfo_init(host, port, type, &ai_addr, &ai_family, &ai_socktype, &ai_protocol))) {
        goto bail;
    }

    if ((*fd = (tnet_fd_t)socket(ai_family, ai_socktype, ai_protocol)) == TNET_INVALID_SOCKET) {
        TNET_PRINT_LAST_ERROR("Failed to create new socket.");
        goto bail;
    }

#if TNET_USE_POLL || USE_POLL /* For win32 WSA* function the socket is auto. set to nonblocking mode. */
    if ((status = tnet_sockfd_set_nonblocking(*fd))) {
        goto bail;
    }
#endif

#if TNET_HAVE_SS_LEN
    if((status = bind(*fd, (const struct sockaddr*)&ai_addr, ai_addr.ss_len)))
#else
    if ((status = bind(*fd, (const struct sockaddr*)&ai_addr, sizeof(ai_addr))))
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
    if (fd != TNET_INVALID_FD) {
#if TNET_UNDER_WINDOWS
        ULONG mode = nonBlocking;
        if (ioctlsocket(fd, FIONBIO, &mode))
            //if(WSAIoctl(fd, FIONBIO, &nonblocking, sizeof(nonblocking), NULL, 0, NULL, NULL, NULL) == SOCKET_ERROR)
        {
            TNET_PRINT_LAST_ERROR("ioctlsocket(FIONBIO) have failed.");
            return -1;
        }
#else
        int flags;
        if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
            TNET_PRINT_LAST_ERROR("fcntl(F_GETFL) have failed.");
            return -1;
        }
        if(fcntl(fd, F_SETFL, flags | (nonBlocking ? O_NONBLOCK : ~O_NONBLOCK)) < 0) {
            TNET_PRINT_LAST_ERROR("fcntl(O_NONBLOCK/O_NONBLOCK) have failed.");
            return -1;
        }
#endif

        // int on = 1;
        // ioctl(fd, FIONBIO, (char *)&on);

    }
    return 0;
}

/**@ingroup tnet_utils_group
 */
int tnet_sockfd_reuseaddr(tnet_fd_t fd, int reuseAddr)
{
    if (fd != TNET_INVALID_FD) {
        int ret;
#if defined(SOLARIS)
        static const char yes = '1';
        static const char no = '0';
#else
        static const int yes = 1;
        static const int no = 0;
#endif
        if ((ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)(reuseAddr ? &yes : &no), sizeof(int)))) {
            TNET_PRINT_LAST_ERROR("setsockopt(SO_REUSEADDR, fd=%d) have failed", fd);
            return ret;
        }
#if defined(SO_REUSEPORT)
        if ((ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char*)(reuseAddr ? &yes : &no), sizeof(int)))) {
            TNET_PRINT_LAST_ERROR("setsockopt(SO_REUSEPORT, fd=%d) have failed", fd);
            return ret;
        }
#endif
        return 0;
    }
    return -1;
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

    if (fd == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Using invalid FD to send data.");
        goto bail;
    }
    if (!buf || !size) {
        TSK_DEBUG_ERROR("Using invalid BUFFER.");
        ret = -2;
        goto bail;
    }

    while (sent < size) {
        int try_guard = 10;
#if TNET_UNDER_WINDOWS
        WSABUF wsaBuffer;
        DWORD numberOfBytesSent = 0;
        wsaBuffer.buf = ((CHAR*)buf) + sent;
        wsaBuffer.len = (ULONG)(size - sent);
try_again:
        ret = WSASendTo(fd, &wsaBuffer, 1, &numberOfBytesSent, 0, to, tnet_get_sockaddr_size(to), 0, 0); // returns zero if succeed
        if (ret == 0) {
            ret = numberOfBytesSent;
        }
#else
try_again:
        ret = sendto(fd, (((const uint8_t*)buf) + sent), (size - sent), 0, to, tnet_get_sockaddr_size(to)); // returns number of sent bytes if succeed
#endif
        if (ret <= 0) {
            if (tnet_geterrno() == TNET_ERROR_WOULDBLOCK) {
                TSK_DEBUG_INFO("SendUdp(fd=%d) - WouldBlock. Retrying...", fd);
                if (try_guard--) {
                    tsk_thread_sleep(10);
                    goto try_again;
                }
            }
            else {
                TNET_PRINT_LAST_ERROR("sendto(fd=%d) failed", fd);
            }
            goto bail;
        }
        else {
            sent += ret;
        }
    }

bail:
    return (int)((size == sent) ? sent : ret);
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

    if (fd == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Using invalid FD to recv data.");
        return -1;
    }

    fromlen = tnet_get_sockaddr_size(from);
    return recvfrom(fd, (char*)buf, (int)size, flags, from, &fromlen);
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

    if (fd == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Using invalid FD to send data.");
        goto bail;
    }

    while (sent < size) {
        if ((ret = send(fd, (((const char*)buf) + sent), (int)(size - sent), flags)) <= 0) {
            if (tnet_geterrno() == TNET_ERROR_WOULDBLOCK) {
                if ((ret = tnet_sockfd_waitUntilWritable(fd, TNET_CONNECT_TIMEOUT))) {
                    break;
                }
                else {
                    continue;
                }
            }
            else {
                TNET_PRINT_LAST_ERROR("send failed");
                // Under Windows XP if WSAGetLastError()==WSAEINTR then try to disable both the ICS and the Firewall
                // More info about How to disable the ISC: http://support.microsoft.com/?scid=kb%3Ben-us%3B230112&x=6&y=11
                goto bail;
            }
        }
        else {
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

    if (fd == TNET_INVALID_FD) {
        TSK_DEBUG_ERROR("Using invalid FD to recv data.");
        goto bail;
    }

    if ((ret = (int)recv(fd, (char*)buf, (int)size, flags)) <= 0) {
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

    if ((status = WSAConnect(fd, (LPSOCKADDR)to, sizeof(*to), NULL, NULL, NULL, NULL)) == SOCKET_ERROR) {
        status = WSAGetLastError();
        if (status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_ISCONN || status == TNET_ERROR_INTR || status == TNET_ERROR_INPROGRESS) {
            TSK_DEBUG_WARN("TNET_ERROR_WOULDBLOCK/TNET_ERROR_ISCONN/TNET_ERROR_INTR/TNET_ERROR_INPROGRESS  -> you should use tnet_sockfd_waitUntilWritable() before trying to send data");
            status = 0;
        }
        else {
            TNET_PRINT_LAST_ERROR("WSAConnect have failed");
        }
    }

#else /* !TNET_UNDER_WINDOWS */

#if TNET_HAVE_SS_LEN
    if ((status = connect(fd, (struct sockaddr*)to, to->ss_len)))
#	else
    if((status = connect(fd, (struct sockaddr*)to, sizeof(*to))))
#	endif
    {
        status = tnet_geterrno();
        if(status == TNET_ERROR_WOULDBLOCK || status == TNET_ERROR_ISCONN || status == TNET_ERROR_INPROGRESS || status == TNET_ERROR_EAGAIN) {
            TSK_DEBUG_INFO("TNET_ERROR_WOULDBLOCK/TNET_ERROR_ISCONN/TNET_ERROR_INPROGRESS/TNET_ERROR_EAGAIN  ==> use tnet_sockfd_waitUntilWritable.");
            status = 0;
        }
        else {
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
    if (fd > 0) {
        return listen(fd, backlog);
    }
    else {
        return -1;
    }
}

/**@ingroup tnet_utils_group
 */
tnet_fd_t tnet_sockfd_accept(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen)
{
    tnet_fd_t ret = TNET_INVALID_FD;

    if (fd > 0) {
        ret = (tnet_fd_t)accept(fd, addr, addrlen);
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
    if (*fd != TNET_INVALID_FD) {
        int ret;
#if TNET_UNDER_WINDOWS
        ret = closesocket(*fd);
#else
        ret = close(*fd);
#endif
        TSK_DEBUG_INFO("CloseSocket(%d)", *fd);
        *fd = TNET_INVALID_FD;
        return ret;
    }
    return 0;
}

/**@ingroup tnet_utils_group
 * Disables both receiving and sending functions. Will raise POLLHUP event.
 * IMPORTANT: The socket still need to be closed.
 * @param fd A descriptor identifying the socket to shutdown.
 * @retval Zero if succeed and non-zero error code otherwise.
 */
int tnet_sockfd_shutdown(tnet_fd_t fd)
{
    return shutdown(fd, 2/*SD_BOTH*/);
}

/**@ingroup tnet_utils_group
 */
tnet_proxy_type_t tnet_proxy_type_from_string(const char* type)
{
    if (tsk_striequals(type, "http")) {
        return tnet_proxy_type_http;
    }
    else if (tsk_striequals(type, "https")) {
        return tnet_proxy_type_https;
    }
    else if (tsk_striequals(type, "socks4")) {
        return tnet_proxy_type_socks4;
    }
    else if (tsk_striequals(type, "socks4a")) {
        return tnet_proxy_type_socks4a;
    }
    else if (tsk_striequals(type, "socks5")) {
        return tnet_proxy_type_socks5;
    }
    else {
        return tnet_proxy_type_none;
    }
}

/**@ingroup tnet_utils_group
 */
const char* tnet_proxy_type_to_string(tnet_proxy_type_t type)
{
    switch (type) {
    case tnet_proxy_type_http:
        return "http";
    case tnet_proxy_type_https:
        return "https";
    case tnet_proxy_type_socks4:
        return "socks4";
    case tnet_proxy_type_socks4a:
        return "socks4a";
    case tnet_proxy_type_socks5:
        return "socks5";
    default:
        return "none";
    }
}








































//=================================================================================================
//	INTERFACE object definition
//
static tsk_object_t* tnet_interface_ctor(tsk_object_t * self, va_list * app)
{
    tnet_interface_t *iface = (tnet_interface_t *)self;
    if (iface) {
        const char* description = va_arg(*app, const char*);
        const void* mac_address = va_arg(*app, const void*);
        tsk_size_t mac_address_length = va_arg(*app, tsk_size_t);

        iface->description = tsk_strdup(description);
        if ((iface->mac_address = (uint8_t*)tsk_calloc(mac_address_length, sizeof(uint8_t)))) {
            memcpy(iface->mac_address, mac_address, mac_address_length);
        }
        iface->mac_address_length = mac_address_length;
    }
    return self;
}

static tsk_object_t* tnet_interface_dtor(tsk_object_t * self)
{
    tnet_interface_t *iface = (tnet_interface_t *)self;
    if (iface) {
        TSK_FREE(iface->description);
        TSK_FREE(iface->mac_address);
    }

    return self;
}

static int tnet_interface_cmp(const tsk_object_t *if1, const tsk_object_t *if2)
{
    const tnet_interface_t *iface1 = (const tnet_interface_t *)if1;
    const tnet_interface_t *iface2 = (const tnet_interface_t *)if2;

    if (iface1 && iface2) {
        return tsk_stricmp(iface1->description, iface1->description);
    }
    else if (!iface1 && !iface2) {
        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tnet_interface_def_s = {
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
    tnet_address_t *address = (tnet_address_t *)self;
    if (address) {
        address->ip = tsk_strdup(va_arg(*app, const char*));
    }
    return self;
}

static tsk_object_t* tnet_address_dtor(tsk_object_t * self)
{
    tnet_address_t *address = (tnet_address_t *)self;
    if (address) {
        TSK_FREE(address->ip);
    }

    return self;
}

static int tnet_address_cmp(const tsk_object_t *_a1, const tsk_object_t *_a2)
{
    const tnet_address_t *a1 = (const tnet_address_t *)_a1;
    const tnet_address_t *a2 = (const tnet_address_t *)_a2;

    if (a1 && a2) {
        // to have AF_UNSPEC, AF_UNIX, AF_INET, ... first
        return (a1->family - a2->family);
    }
    else if (!a1 && !a2) {
        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tnet_address_def_s = {
    sizeof(tnet_address_t),
    tnet_address_ctor,
    tnet_address_dtor,
    tnet_address_cmp,
};
const tsk_object_def_t *tnet_address_def_t = &tnet_address_def_s;

