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
/**@file tnet_dns.c
 * @brief DNS utility functions (RFCS [1034 1035] [3401 3402 3403 3404] [3761]).
 *
 */
#include "tnet_dns.h"

#include "tnet_dns_regexp.h"
#include "tnet_dns_message.h"
#include "tnet_dns_opt.h"
#include "tnet_dns_srv.h"
#include "tnet_dns_naptr.h"

#include "tnet_types.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_string.h"

#include <string.h> /* tsk_strlen, memser, .... */
#include <ctype.h> /* isdigist */

/* DNS cache functions */
int tnet_dns_cache_maintenance(tnet_dns_ctx_t *ctx);
int tnet_dns_cache_entry_add(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t* response);
const tnet_dns_cache_entry_t* tnet_dns_cache_entry_get(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype);

/**@defgroup tnet_dns_group DNS utility functions (RFCS [1034 1035] [3401 3402 3403 3404]).
*
* <h1>11 DNS</h1>
* <p>
* The DNS Stack (RFC 1034 and RFC 1035) contains all network functions to send queries (both IPv4 and IPv6) and parse responses. <br>
* The core framework implements RFC 3401, 3402, 3403 and 3404, also known as Dynamic Delegation Discovery System (DDDS).
* </p>
* <p>
* The DNS servers are automatically loaded by the stack when you create a context (@ref tnet_dns_ctx_create()). <br>
* On Windows systems (XP, VISTA, 7 and CE) the servers are retrieved using WIN32 APIs. On Unix-like systems (both desktop and embedded) such as Debian, Ubuntu, Nokia’s N900… the list of DNS servers comes from “/etc/resolv.conf” file. <br>
* On Google Android operating system, this file is missing and the DNS settings are stored in the shared memory. You can access this shared memory by using @a property_get() and @a property_set() function which are part of Bionic. <br>
* In all cases, you can retrieve the DNS servers yourself (e.g. using java/C# Frameworks) and add them to the context using @ref tnet_dns_add_server().
* </p>
* <p>
* DNS resolution is always performed in a synchronous manner and is thread-safe. For all DNS requests the default timeout value is 5 seconds (@ref TNET_DNS_TIMEOUT_DEFAULT).
* The stack also implements the ENUM protocol (RFC 3761).
* </p>
*
* <h2>11.1	Resource Records</h2>
* The table below lists all DNS Resource Records (RR) for which we provide a parser.
*
* <table>
* <tr><td>Code</td>	<td>RFC</td>	<td>Description</td>	<td>Well-defined type</td></tr>
* <tr><td>A</td>	<td>RFC 1035</td>	<td>IPv4 address</td> 	<td>tnet_dns_a_t</td></tr>
* <tr><td>AAAA</td>	<td>RFC 3596</td>	<td>IPv6 address</td>	<td>tnet_dns_aaaa_t</td></tr>
* <tr><td>CNAME</td>	<td>RFC 1035</td>	<td>Canonical name</td>	<td>tnet_dns_cname_t</td></tr>
* <tr><td>MX</td>	<td>RFC 2035</td>	<td>Mail exchange</td>	<td>tnet_dns_mx_t</td></tr>
* <tr><td>NAPTR</td>	<td>RFC 3403</td>	<td>Naming Authority Pointer</td>	<td>tnet_dns_naptr_t</td></tr>
* <tr><td>NS</td>	<td>RFC 1035</td>	<td>Name Server</td>	<td>tnet_dns_ns_t</td></tr>
* <tr><td>OPT</td>	<td>RFC 2671</td>	<td>Option</td>	<td>tnet_dns_opt_t</td></tr>
* <tr><td>PTR</td>	<td>RFC 1035</td>	<td>Pointer record</td>	<td>tnet_dns_ptr_t</td></tr>
* <tr><td>SOA</td>	<td>RFC1035</td>	<td>Start Of Authority record</td>	<td>tnet_dns_soa_t</td></tr>
* <tr><td>SRV</td>	<td>RFC 2782</td>	<td>Service locator</td>	<td>tnet_dns_srv_t</td></tr>
* </table>
*
* Here is an example of how to use the DNS stack to perform DNS NAPTR resolution and print the result to the console.
*
* @code
tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
tnet_dns_response_t *response = tsk_null;
const tsk_list_item_t* item;
const tnet_dns_rr_t* rr;

if((response = tnet_dns_resolve(ctx, "sip2sip.info", qclass_in, qtype_naptr)))
{
if(TNET_DNS_RESPONSE_IS_SUCCESS(response)){
TSK_DEBUG_INFO("We got a success response from the DNS server.");
// loop through the answers
tsk_list_foreach(item, response->Answers){
rr = item->data;
if(rr->qtype == qtype_naptr){
const tnet_dns_naptr_t *naptr = (const tnet_dns_naptr_t*)rr;

TSK_DEBUG_INFO("order=%u pref=%u flags=%s services=%s regexp=%s replacement=%s",
naptr->order,
naptr->preference,
naptr->flags,
naptr->services,
naptr->regexp,
naptr->replacement);
}
}
}
else{
TSK_DEBUG_ERROR("We got an error response from the DNS server. Error code: %u", response->Header.RCODE);
}
}

TSK_OBJECT_SAFE_FREE(response);
TSK_OBJECT_SAFE_FREE(ctx);

* @endcode
* The @a ctx could be used several times and is a well-defined object.<br>
* The console will output:
* <i>INFO: We got a success response from the DNS server.</i><br>
* <i>INFO: order=10 pref=0 flags=S services=SIP+d2u regexp=(null) replacement=_sip._udp.sip2sip.info</i><br>
*
* <h2>11.2	ENUM</h2>
* ENUM (E.164 Number Mapping) protocol has been defined in RFC 3761.
* ENUM protocol is used to transform telephone numbers of the PSTN network (e.g. +33600000) into internet resource addresses (e.g. sip:diopmamadou@example.com) by using DNS lookup (NAPTR). The internet resource address could be an email, ICQ, IAX2, H.323 …
* In our case (3GPP IMS) it is typically used to convert TEL URIs (e.g. tel:+33600000) into SIP URIs (sip:+33600000;user=phone). The telephone number to convert should be a valid E.164 number.
*
* <h3>11.2.1	Usage</h3>
* The code below shows how to gets the SIP address (with the higher order) associated to an E.164 telephone number.
*
* @code
tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
tnet_dns_response_t* response = tsk_null;
char* uri = tsk_null;

if((uri = tnet_dns_enum_2(ctx, "E2U+SIP", "+1-800-555-5555","e164.org"))){
TSK_DEBUG_INFO("URI=%s", uri);
TSK_FREE(uri);
}
else{
TSK_DEBUG_ERROR("ENUM(%s) failed", "+1-800-555-5555");
}

TSK_OBJECT_SAFE_FREE(response);
TSK_OBJECT_SAFE_FREE(ctx);
* @endcode
*
* Console Output:<br>
* <i>INFO: URI=sip:16416418000-555-5555@sip.tollfreegateway.com</i><br>
*
* E2U+SIP is the name of SIP ENUM service assigned by the IANA. Any assigned service () could be used even if the associated addresse type isn’t a well-knonw internet address.
* To get all internet addresses (email, IAX2, ICQ, H.323 …) associated to the telephone, use @ref tnet_dns_enum() instead of @ref tnet_dns_enum_2().
*/

/**@ingroup tnet_dns_group
* Creates new DNS context.
*/
tnet_dns_ctx_t* tnet_dns_ctx_create()
{
    return tsk_object_new(tnet_dns_ctx_def_t);
}

/**@ingroup tnet_dns_group
* Creates new DNS cache entry.
*/
tnet_dns_cache_entry_t* tnet_dns_cache_entry_create(const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t* answer)
{
    return tsk_object_new(tnet_dns_cache_entry_def_t, qname, qclass, qtype, answer);
}


/**@ingroup tnet_dns_group
* Cleanup the internal DNS cache.
* @param ctx The DNS context containing the cache to cleanup.
* The context contains the user's preference and should be created using @ref tnet_dns_ctx_create().
* @retval Zero if succeeed and non-zero error code otherwise.
*/
int tnet_dns_cache_clear(tnet_dns_ctx_t* ctx)
{
    if (ctx) {
        tsk_safeobj_lock(ctx);
        tsk_list_clear_items(ctx->cache);
        tsk_safeobj_unlock(ctx);

        return 0;
    }
    return -1;
}

/**@ingroup tnet_dns_group
* Sends DNS request over the network. The request will be sent each 500 milliseconds until @ref TNET_DNS_TIMEOUT_DEFAULT milliseconds is reached.
* @param ctx The DNS context to use. The context contains the user's preference and should be created using @ref tnet_dns_ctx_create().
* @param qname The domain name (e.g. google.com).
* @param qclass The CLASS of the query.
* @param qtype The type of the query.
* @retval The DNS response. The @a answers in the @a response are already filtered.
* MUST be destroyed using @a TSK_OBJECT_SAFE_FREE() macro.
* @sa @ref tnet_dns_query_srv, @ref tnet_dns_query_naptr_srv, @ref tnet_dns_enum.
*
* @code
* tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
* tnet_dns_response_t *response = tnet_dns_resolve(ctx, "sip2sip.info", qclass_in, qtype_srv);
* TSK_OBJECT_SAFE_FREE(response);
* TSK_OBJECT_SAFE_FREE(ctx);
* @endcode
*/
tnet_dns_response_t *tnet_dns_resolve(tnet_dns_ctx_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
#if HAVE_DNS_H
    struct sockaddr_storage result;
    struct sockaddr *from;
    uint32_t fromlen;
    char buf[TNET_DNS_DGRAM_SIZE_DEFAULT];
    int32_t ret;

    tnet_dns_response_t *response = tsk_null;

    tnet_socket_t *localsocket4 = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv4);
    tnet_socket_t *localsocket6 = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv6);

    tsk_safeobj_lock(ctx);

    // First, try with IPv4
    if(TNET_SOCKET_IS_VALID(localsocket4)) {
        if((ret = tnet_getsockname(localsocket4->fd, &result))) {
            TNET_PRINT_LAST_ERROR("tnet_getsockname() failed.");
            goto ipv6;
        }
        from = (struct sockaddr *) &result;
        fromlen = tnet_get_sockaddr_size(from);

        if ((ret = dns_search(ctx->resolv_handle, qname, qclass, qtype, buf, TNET_DNS_DGRAM_SIZE_DEFAULT, from, &fromlen)) > 0) {
            response = tnet_dns_message_deserialize((uint8_t *) buf, ret);
            goto done;
        }
        else {
            TNET_PRINT_LAST_ERROR("dns_search_v4()");
        }
    }
ipv6:
    // Then, try with IPv6
    if(TNET_SOCKET_IS_VALID(localsocket6)) {
        if((ret = tnet_getsockname(localsocket6->fd, &result))) {
            TNET_PRINT_LAST_ERROR("tnet_getsockname() failed.");
            goto done;
        }
        from = (struct sockaddr *) &result;
        fromlen = tnet_get_sockaddr_size(from);

        if((ret = dns_search(ctx->resolv_handle, qname, qclass, qtype, buf, TNET_DNS_DGRAM_SIZE_DEFAULT, from, &fromlen)) > 0) {
            response = tnet_dns_message_deserialize((uint8_t *) buf, ret);
            goto done;
        }
        else {
            TNET_PRINT_LAST_ERROR("dns_search_v6()");
        }
    }

done:
    tsk_safeobj_unlock(ctx);

    TSK_OBJECT_SAFE_FREE(localsocket4);
    TSK_OBJECT_SAFE_FREE(localsocket6);

    return response;
#else
    tsk_buffer_t *output = tsk_null;
    tnet_dns_query_t* query = tnet_dns_query_create(qname, qclass, qtype);
    tnet_dns_response_t *response = tsk_null;
    tsk_bool_t from_cache = tsk_false;

    /* Check validity */
    if (!ctx || !query) {
        goto bail;
    }

    /* Is there any DNS Server? */
    if (TSK_LIST_IS_EMPTY(ctx->servers)) {
        TSK_DEBUG_ERROR("Failed to load DNS Servers. You can add new DNS servers by using \"tnet_dns_add_server\".");
        goto bail;
    }

    /* Cache maintenance */
    if (!TSK_LIST_IS_EMPTY(ctx->cache)) {
        /* Only do maintenance if the cache is not empty */
        tnet_dns_cache_maintenance(ctx);
    }

    /* Retrieve data from cache. */
    if (ctx->caching) {
        const tnet_dns_cache_entry_t *entry = tnet_dns_cache_entry_get(ctx, qname, qclass, qtype);
        if (entry) {
            response = tsk_object_ref(((tnet_dns_cache_entry_t*)entry)->response);
            from_cache = tsk_true;
            goto bail;
        }
    }

    /* Set user preference */
    query->Header.RD = ctx->recursion;

    /* EDNS0 */
    if (ctx->edns0) {
        tnet_dns_opt_t *rr_opt = tnet_dns_opt_create(TNET_DNS_DGRAM_SIZE_DEFAULT);
        if (!query->Additionals) {
            query->Additionals = tsk_list_create();
        }
        tsk_list_push_back_data(query->Additionals, (void**)&rr_opt);
        query->Header.ARCOUNT++;
    }

    /* Serialize and send to the server. */
    if (!(output = tnet_dns_message_serialize(query))) {
        TSK_DEBUG_ERROR("Failed to serialize the DNS message.");
        goto bail;
    }

    /* ============================ */
    //	Send and Recaive data
    /* ============================ */
    {
        int ret = -1;
        struct timeval tv;
        fd_set set;
        tnet_fd_t maxFD;
        uint64_t timeout = 0;
        tsk_list_item_t *item;
        const tnet_address_t *address;
        struct sockaddr_storage server;
        tnet_socket_t *localsocket4 = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv4);
        tnet_socket_t *localsocket6 = tnet_socket_create(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv6);
        tsk_bool_t useIPv6 = TNET_SOCKET_IS_VALID(localsocket6);

        tsk_safeobj_lock(ctx);

        /* Check socket validity */
        if (!TNET_SOCKET_IS_VALID(localsocket4)) {
            goto done;
        }

        /* Always wait 500ms before retransmission */
        tv.tv_sec = 0;
        tv.tv_usec = (500 * 1000);

        do {
            //
            //	Send data (loop through all intefaces)
            //
            tsk_list_foreach(item, ctx->servers) {
                address = item->data;
                if (!address->ip ||
                        (address->family != AF_INET && address->family != AF_INET6) ||
                        (address->family == AF_INET6 && !TNET_SOCKET_IS_VALID(localsocket6))) {
                    continue;
                }

                if (tnet_sockaddr_init(address->ip, ctx->server_port, (address->family == AF_INET ? tnet_socket_type_udp_ipv4 : tnet_socket_type_udp_ipv6), &server)) {
                    TSK_DEBUG_ERROR("Failed to initialize the DNS server address: \"%s\"", address->ip);
                    continue;
                }

                TSK_DEBUG_INFO("Sending DNS query to \"%s\"", address->ip);

                if (address->family == AF_INET6) {
                    if ((ret = tnet_sockfd_sendto(localsocket6->fd, (const struct sockaddr*)&server, output->data, output->size))) {
                        // succeed?
                        break;
                    }
                }
                else {
                    if ((ret = tnet_sockfd_sendto(localsocket4->fd, (const struct sockaddr*)&server, output->data, output->size))) {
                        // succeed?
                        break;
                    }
                }
            }

            //
            //	Received data
            //
            /* First time? ==> set timeout value */
            if (!timeout) {
                timeout = tsk_time_epoch() + ctx->timeout;
            }

            /* Set FDs */
            FD_ZERO(&set);
            FD_SET(localsocket4->fd, &set);
            if (useIPv6) {
                FD_SET(localsocket6->fd, &set);
                maxFD = TSK_MAX(localsocket4->fd, localsocket6->fd);
            }
            else {
                maxFD = localsocket4->fd;
            }

            /* wait for response */
            if ((ret = select(maxFD + 1, &set, NULL, NULL, &tv)) < 0) { /* Error */
                TNET_PRINT_LAST_ERROR("Select failed.");
                goto done;
            }
            else if (ret == 0) { /* timeout ==> do nothing */

            }
            else { /* there is data to read */
                unsigned int len = 0;
                void* data = 0;
                tnet_fd_t active_fd;

                /* Find active file descriptor */
                if (FD_ISSET(localsocket4->fd, &set)) {
                    active_fd = localsocket4->fd;
                }
                else if (FD_ISSET(localsocket6->fd, &set)) {
                    active_fd = localsocket4->fd;
                }
                else {
                    TSK_DEBUG_ERROR("FD_ISSET ==> Invalid file descriptor.");
                    continue;
                }

                /* Check how how many bytes are pending */
                if ((ret = tnet_ioctlt(active_fd, FIONREAD, &len)) < 0) {
                    TSK_DEBUG_ERROR("tnet_ioctlt failed with error code:%d", tnet_geterrno());
                    goto done;
                }

                /* Receive pending data */
                data = tsk_calloc(len, sizeof(uint8_t));
                if ((ret = tnet_sockfd_recv(active_fd, data, len, 0)) < 0) {
                    TSK_FREE(data);

                    TSK_DEBUG_ERROR("tnet_sockfd_recv failed with error code:%d", tnet_geterrno());
                    goto done;
                }

                /* Parse the incoming response. */
                response = tnet_dns_message_deserialize(data, (tsk_size_t)ret);
                TSK_FREE(data);

                if (response) {
                    /* response successfuly parsed */
                    if (query->Header.ID != response->Header.ID || !TNET_DNS_MESSAGE_IS_RESPONSE(response)) {
                        /* Not same transaction id ==> continue*/
                        TSK_OBJECT_SAFE_FREE(response);
                    }
                    else {
                        goto done;
                    }
                }
            }
        }
        while (timeout > tsk_time_epoch());

done:
        tsk_safeobj_unlock(ctx);

        TSK_OBJECT_SAFE_FREE(localsocket4);
        TSK_OBJECT_SAFE_FREE(localsocket6);
        goto bail;
    }


bail:
    TSK_OBJECT_SAFE_FREE(query);
    TSK_OBJECT_SAFE_FREE(output);

    /* Add the result to the cache. */
    if (response) {
        if (!from_cache && ctx->caching) {
            tnet_dns_cache_entry_add(ctx, qname, qclass, qtype, response);
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to contact the DNS server.");
    }

    return response;
#endif
}

/**@ingroup tnet_dns_group
* Gets list of URIs associated to this telephone number by using ENUM protocol (RFC 3761).
* @param ctx The DNS context.
* The context contains the user's preference and should be created using @ref tnet_dns_ctx_create().
* @param e164num A valid E.164 number (e.g. +1-800-555-5555).
* @param domain The domain name (e.g e164.arpa, e164.org, ...). If Null, default value is "e164.arpa" (IANA).
* @retval The DNS response with NAPTR RRs. The @a answers in the @a response are already filtered.
* MUST be destroyed using @a TSK_OBJECT_SAFE_FREE macro.
* @sa @ref tnet_dns_resolve, @ref tnet_dns_enum_2.
*/
tnet_dns_response_t* tnet_dns_enum(tnet_dns_ctx_t* ctx, const char* e164num, const char* domain)
{
    char e164domain[255];
    tnet_dns_response_t* ret = tsk_null;
    int e164size;
    int i, j; // must be signed

    e164size = (int)tsk_strlen(e164num);

    if (!ctx || !e164num || !e164size) {
        goto bail;
    }

    if (e164size /* max=15 digits + ".e164.arpa" + '+' */ >= (sizeof(e164domain) - 1)) {
        TSK_DEBUG_ERROR("%s is an invalid E.164 number.", e164num);
        goto bail;
    }

    memset(e164domain, '\0', sizeof(e164domain));

    /*	RFC 3761 - 2.4.  Valid Databases
    	1. Remove all characters with the exception of the digits.  For
    	example, the First Well Known Rule produced the Key
    	"+442079460148".  This step would simply remove the leading "+",
    	producing "442079460148".

    	2. Put dots (".") between each digit.  Example:
    	4.4.2.0.7.9.4.6.0.1.4.8

    	3. Reverse the order of the digits.  Example:
    	8.4.1.0.6.4.9.7.0.2.4.4

    	4. Append the string ".e164.arpa" to the end.  Example:
    	8.4.1.0.6.4.9.7.0.2.4.4.e164.arpa

    	This domain-name is used to request NAPTR records which may contain
    	the end result or, if the flags field is blank, produces new keys in
    	the form of domain-names from the DNS.
    	*/
    for (i = e164size - 1, j = 0; i >= 0; i--) {
        if (!isdigit(e164num[i])) {
            continue;
        }
        e164domain[j++] = e164num[i];
        e164domain[j++] = '.';
    }

    // append domain name
    if (domain) {
        memcpy(&e164domain[j], domain, ((tsk_strlen(domain) + j) >= sizeof(e164domain) - 1) ? (sizeof(e164domain) - j - 1) : tsk_strlen(domain));
    }
    else {
        memcpy(&e164domain[j], "e164.arpa", 9);
    }

    /* == Performs DNS (NAPTR) lookup  == */
    ret = tnet_dns_resolve(ctx, e164domain, qclass_in, qtype_naptr);

bail:

    return ret;
}

/**@ingroup tnet_dns_group
* Gets the internate address associated to this telephone number by using ENUM protocol (RFC 3761).
* Only terminale rules containing uris(flags="u") will be considered and the regex string will be executed on the original string for
* substitution. <br>
* <b> Parsing complex regexp will probably fail (99.99% chance). Please use @ref tnet_dns_enum if you want to use your own regexp parser. </b>
* @param ctx The DNS context.
* The context contains the user's preference and should be created using @ref tnet_dns_ctx_create().
* @param service The ENUM service (e.g. E2U+SIP).
* @param e164num A valid E.164 number (e.g. +1-800-555-5555).
* @param domain The domain name (e.g e164.arpa, e164.org, ...). If Null, default value is "e164.arpa" (IANA).
* @retval The Internet address (SIP, email, ICQ, fax, ...) associated to this service.
* MUST be freed using @a TSK_FREE macro.
* @sa @ref tnet_dns_resolve, @ref tnet_dns_enum.
*/
char* tnet_dns_enum_2(tnet_dns_ctx_t* ctx, const char* service, const char* e164num, const char* domain)
{
    tnet_dns_response_t* response;
    const tsk_list_item_t* item;
    char* ret = tsk_null;
    const tnet_dns_rr_t* rr;

    if ((response = tnet_dns_enum(ctx, e164num, domain))) {
        if (TNET_DNS_RESPONSE_IS_SUCCESS(response)) {
            tsk_list_foreach(item, response->Answers) {
                rr = item->data;
                if (rr->qtype == qtype_naptr) {
                    const tnet_dns_naptr_t *naptr = (const tnet_dns_naptr_t*)rr;
                    /*	RFC 3403 - 6.2 E164 Example
                    	Both the ENUM [18] and URI  Resolution [4] Applications use the 'u'
                    	flag.  This flag states that the Rule is terminal and that the output
                    	is a URI which contains the information needed to contact that
                    	telephone service.
                    	*/
                    if (tsk_striequals(naptr->flags, "u") && tsk_striequals(naptr->services, service)) {
                        /* RFC 3403 - 4.1 Packet Format
                        	The fields (replacement and regexp) are also mutually exclusive.  If a record is
                        	returned that has values for both fields then it is considered to
                        	be in error and SHOULD be either ignored or an error returned.
                        	*/
                        if (naptr->regexp && naptr->replacement) {
                            continue;
                        }

                        if ((ret = tnet_dns_regex_parse(e164num, naptr->regexp))) {
                            break;
                        }
                    }
                }
            }
        }
        else {
            TSK_DEBUG_ERROR("We got an error response from the DNS server. Error code: %u", response->Header.RCODE);
        }

        TSK_OBJECT_SAFE_FREE(response);
    }

    return ret;
}

/**@ingroup tnet_dns_group
* Performs DNS SRV resolution.
* @param ctx The DNS context.
* The context contains the user's preference and should be created using @ref tnet_dns_ctx_create.
* @param service The name of the service (e.g. SIP+D2U).
* @param hostname The result containing an IP address or FQDN.
* @param port The port associated to the result.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tnet_dns_resolve.
*
* @code
* tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
* char* hostname = 0;
* tnet_port_t port = 0;
*
* if(!tnet_dns_query_srv(ctx, "_sip._udp.sip2sip.info", &hostname, &port)){
* 	TSK_DEBUG_INFO("DNS SRV succeed ==> hostname=%s and port=%u", hostname, port);
* }
*
* TSK_FREE(hostname);
* TSK_OBJECT_SAFE_FREE(ctx);
* @endcode
*/
int tnet_dns_query_srv(tnet_dns_ctx_t *ctx, const char* service, char** hostname, tnet_port_t* port)
{
    tnet_dns_response_t *response;

    if (!ctx) {
        return -1;
    }

    // tnet_dns_resolve is thread-safe
    if ((response = tnet_dns_resolve(ctx, service, qclass_in, qtype_srv))) {
        const tsk_list_item_t *item;
        const tnet_dns_rr_t* rr;
        tsk_list_foreach(item, response->Answers) { /* Already Filtered ==> Peek the first One */
            rr = item->data;
            if (rr->qtype == qtype_srv) {
                const tnet_dns_srv_t *srv = (const tnet_dns_srv_t*)rr;

                tsk_strupdate(hostname, srv->target);
                *port = srv->port;
                break;
            }
        }
    }

    TSK_OBJECT_SAFE_FREE(response);

    return (hostname && !tsk_strnullORempty(*hostname)) ? 0 : -2;
}

/**@ingroup tnet_dns_group
* Performs DNS NAPTR followed by DNS SRV resolution.
* @param ctx The DNS context.
* The context contains the user's preference and should be created using @ref tnet_dns_ctx_create().
* @param domain The Name of the domain (e.g. google.com).
* @param service The name of the service (e.g. SIP+D2U).
* @param hostname The result containing an IP address or FQDN. Should be Null.
* @param port The port associated to the result.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tnet_dns_resolve.
*
* @code
* tnet_dns_ctx_t *ctx = tnet_dns_ctx_create();
* char* hostname = tsk_null;
* tnet_port_t port = 0;
*
* if(!tnet_dns_query_naptr_srv(ctx, "sip2sip.info", "SIP+D2U", &hostname, &port)){
* 	TSK_DEBUG_INFO("DNS NAPTR+SRV succeed ==> hostname=%s and port=%u", hostname, port);
* }
*
* TSK_FREE(hostname);
* TSK_OBJECT_SAFE_FREE(ctx);
* @endcode
*/
int tnet_dns_query_naptr_srv(tnet_dns_ctx_t *ctx, const char* domain, const char* service, char** hostname, tnet_port_t* port)
{
    tnet_dns_response_t *response;

    if (!ctx || !hostname) {
        TSK_DEBUG_ERROR("Invalid parameters.");
        return -1;
    }

    /* reset (do not free the user supplied value). trying to free dummy value will cause access violation error ==> zero. */
    *hostname = tsk_null;

    // tnet_dns_resolve is thread-safe
    if ((response = tnet_dns_resolve(ctx, domain, qclass_in, qtype_naptr))) {
        const tsk_list_item_t *item;
        const tnet_dns_rr_t* rr;

        char* replacement = tsk_null; /* e.g. _sip._udp.example.com */
        char* flags = tsk_null;/* e.g. S, A, AAAA, A6, U, P ... */

        tsk_list_foreach(item, response->Answers) { /* Already Filtered ==> Peek the first One */
            rr = item->data;
            if (rr->qtype == qtype_naptr) {
                tnet_dns_naptr_t *naptr = (tnet_dns_naptr_t*)rr;

                if (tsk_striequals(service, naptr->services)) {
                    tsk_strupdate(&replacement, naptr->replacement);
                    tsk_strupdate(&flags, naptr->flags);

                    break;
                }
            }
        }

        if (flags && replacement) {
            if (tsk_striequals(flags, "S")) {
                tnet_dns_query_srv(ctx, replacement, hostname, port);
            }
            else if (tsk_striequals(flags, "A") || tsk_striequals(flags, "AAAA") || tsk_striequals(flags, "A6")) {
                TSK_DEBUG_WARN("Defaulting port value.");
                tsk_strupdate(hostname, replacement);
                *port = 5060;
            }
            else {
                TSK_DEBUG_ERROR("DNS NAPTR query returned invalid flags");
            }
        }
        else {
            TSK_DEBUG_INFO("DNS NAPTR (%s) query returned zero result", domain);
        }

        TSK_FREE(flags);
        TSK_FREE(replacement);
    }

    TSK_OBJECT_SAFE_FREE(response);

    return (hostname && *hostname && !tsk_strempty(*hostname)) ? 0 : -2;
}

// remove timedout entries
int tnet_dns_cache_maintenance(tnet_dns_ctx_t *ctx)
{

    if (ctx) {
        tsk_list_item_t *item;
        tsk_safeobj_lock(ctx);
again:

        tsk_list_foreach(item, ctx->cache) {
            // FIXME: ttl should be from RR::ttl
            tnet_dns_cache_entry_t *entry = (tnet_dns_cache_entry_t*)item->data;
            if ((entry->epoch + ctx->cache_ttl) < tsk_time_epoch()) {
                tsk_list_remove_item_by_data(ctx->cache, entry);
                goto again; /* Do not delete data while looping */
            }
        }

        tsk_safeobj_unlock(ctx);

        return 0;
    }
    return -1;
}

// add an entry to the cache
int tnet_dns_cache_entry_add(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t* response)
{
    int ret = -1;

    if (ctx) {
        tnet_dns_cache_entry_t *entry;

        tsk_safeobj_lock(ctx);

        entry = 0;

        /* Retrieve from cache */
        entry = (tnet_dns_cache_entry_t*)tnet_dns_cache_entry_get(ctx, qname, qclass, qtype);

        if (entry) {
            /* UPDATE */
            TSK_OBJECT_SAFE_FREE(entry->response);
            entry->response = tsk_object_ref(response);
            entry->epoch = tsk_time_epoch();
            ret = 0;
            goto done;
        }
        else {
            /* CREATE */
            entry = tnet_dns_cache_entry_create(qname, qclass, qtype, response);
            if (entry) {
                tsk_list_push_back_data(ctx->cache, (void**)&entry);
                ret = 0;
                goto done;
            }
            else {
                ret = -2;
                goto done;
            }
        }
done:
        tsk_safeobj_unlock(ctx);
    }
    return ret;
}

// get an entry from the cache
const tnet_dns_cache_entry_t* tnet_dns_cache_entry_get(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
    tnet_dns_cache_entry_t *ret = tsk_null;
    if (ctx) {
        tsk_list_item_t *item;

        tsk_safeobj_lock(ctx);

        tsk_list_foreach(item, ctx->cache) {
            tnet_dns_cache_entry_t *entry = (tnet_dns_cache_entry_t*)item->data;
            if (entry->qtype == qtype && entry->qclass == qclass && tsk_strequals(entry->qname, qname)) {
                ret = entry;
                break;
            }
        }

        tsk_safeobj_unlock(ctx);
    }

    return ret;
}


/**@ingroup tnet_dns_group
* Adds new DNS server to the list of the list of servers to query.
* @param ctx DNS context containing the user parameters. The new DNS server will be added to this context.
* @param host The IP address (or FQDN) of the dns server to add to the server.
* @retval zero if succeed and non-zero error code otherwise.
*/
int tnet_dns_add_server(tnet_dns_ctx_t *ctx, const char* host)
{
    tnet_address_t *address;

    if (!ctx || !host) {
        return -1;
    }

    if (!ctx->servers) {
        ctx->servers = tsk_list_create();
    }

    if ((address = tnet_address_create(host))) {
        address->family = tnet_get_family(host, TNET_DNS_SERVER_PORT_DEFAULT);
        address->dnsserver = 1;
        tsk_list_push_ascending_data(ctx->servers, (void**)&address);

        return 0;
    }

    return -2;
}

//=================================================================================================
//	[[DNS CACHE ENTRY]] object definition
//
static tsk_object_t* tnet_dns_cache_entry_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dns_cache_entry_t *entry = self;
    if (entry) {
        entry->qname = tsk_strdup(va_arg(*app, const char*));
        entry->qclass = va_arg(*app, tnet_dns_qclass_t);
        entry->qtype = va_arg(*app, tnet_dns_qtype_t);
        entry->response = tsk_object_ref(va_arg(*app, tnet_dns_response_t*));

        entry->epoch = tsk_time_epoch();
    }
    return self;
}

static tsk_object_t* tnet_dns_cache_entry_dtor(tsk_object_t * self)
{
    tnet_dns_cache_entry_t *entry = self;
    if (entry) {
        TSK_OBJECT_SAFE_FREE(entry->response);
    }
    return self;
}

static const tsk_object_def_t tnet_dns_cache_entry_def_s = {
    sizeof(tnet_dns_cache_entry_t),
    tnet_dns_cache_entry_ctor,
    tnet_dns_cache_entry_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dns_cache_entry_def_t = &tnet_dns_cache_entry_def_s;


//=================================================================================================
//	[[DNS CONTEXT]] object definition
//
static tsk_object_t* tnet_dns_ctx_ctor(tsk_object_t * self, va_list * app)
{
    tnet_dns_ctx_t *ctx = self;
    if (ctx) {
        ctx->timeout = TNET_DNS_TIMEOUT_DEFAULT;
        ctx->recursion = tsk_true;
        ctx->edns0 = tsk_true;
        ctx->caching = tsk_false;

        ctx->cache_ttl = TNET_DNS_CACHE_TTL;

        ctx->server_port = TNET_DNS_SERVER_PORT_DEFAULT;

        /* Gets all dns servers. */
        ctx->servers = tnet_get_addresses_all_dnsservers();
        /* Creates empty cache. */
        ctx->cache = tsk_list_create();

#if HAVE_DNS_H
        ctx->resolv_handle = dns_open(NULL);
#endif

        tsk_safeobj_init(ctx);
    }
    return self;
}

static tsk_object_t* tnet_dns_ctx_dtor(tsk_object_t * self)
{
    tnet_dns_ctx_t *ctx = self;
    if (ctx) {
        tsk_safeobj_deinit(ctx);

        TSK_OBJECT_SAFE_FREE(ctx->servers);
        TSK_OBJECT_SAFE_FREE(ctx->cache);

#if HAVE_DNS_H
        dns_free(ctx->resolv_handle);
#endif
    }
    return self;
}

static const tsk_object_def_t tnet_dns_ctx_def_s = {
    sizeof(tnet_dns_ctx_t),
    tnet_dns_ctx_ctor,
    tnet_dns_ctx_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_dns_ctx_def_t = &tnet_dns_ctx_def_s;
