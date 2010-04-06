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
/**@file tnet_dns.c
 * @brief DNS utility functions (RFCS [1034 1035] [3401 3402 3403 3404] [3761]).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
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

#include <string.h> /* strlen, memser, .... */
#include <ctype.h> /* isdigist */

/* DNS cache functions */
int tnet_dns_cache_maintenance(tnet_dns_ctx_t *ctx);
int tnet_dns_cache_entry_add(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t* response);
const tnet_dns_cache_entry_t* tnet_dns_cache_entry_get(tnet_dns_ctx_t *ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype);

/**@defgroup tnet_dns_group DNS utility functions (RFCS [1034 1035] [3401 3402 3403 3404]).
*/


/**@ingroup tnet_dns_group
* Cleanup the internal DNS cache.
* @param ctx The DNS context containing the cache to cleanup. 
* The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @retval Zero if succeeed and non-zero error code otherwise.
*/
int tnet_dns_cache_clear(tnet_dns_ctx_t* ctx)
{
	if(ctx){
		tsk_safeobj_lock(ctx);
		tsk_list_clear_items(ctx->cache);
		tsk_safeobj_unlock(ctx);

		return 0;
	}
	return -1;
}

/**@ingroup tnet_dns_group
* Sends DNS request over the network. The request will be sent each 500 milliseconds until @ref TNET_DNS_TIMEOUT_DEFAULT milliseconds is reached.
* @param ctx The DNS context to use. The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @param qname The domain name (e.g. google.com).
* @param qclass The CLASS of the query.
* @param qtype The type of the query.
* @retval The DNS response. The @a answers in the @a response are already filtered.
* MUST be destroyed using @ref TSK_OBJECT_SAFE_FREE macro.
* @sa @ref tnet_dns_query_srv, @ref tnet_dns_query_naptr_srv, @ref tnet_dns_enum.
*
* @code
* tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
* tnet_dns_response_t *response = tnet_dns_resolve(ctx, "sip2sip.info", qclass_in, qtype_srv);
* TSK_OBJECT_SAFE_FREE(response);
* TSK_OBJECT_SAFE_FREE(ctx);
* @endcode
*/
tnet_dns_response_t *tnet_dns_resolve(tnet_dns_ctx_t* ctx, const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
	tsk_buffer_t *output = tsk_null;
	tnet_dns_query_t* query = TNET_DNS_QUERY_CREATE(qname, qclass, qtype);
	tnet_dns_response_t *response = tsk_null;
	tsk_bool_t from_cache = tsk_false;
	
	/* Check validity */
	if(!ctx || !query){
		goto bail;
	}

	/* Is there any DNS Server? */
	if(TSK_LIST_IS_EMPTY(ctx->servers)){
		TSK_DEBUG_ERROR("Failed to load DNS Servers. You can add new DNS servers by using \"tnet_dns_add_server\".");
		goto bail;
	}

	/* Cache maintenance */
	if(!TSK_LIST_IS_EMPTY(ctx->cache)){	
		/* Only do maintenance if the cache is not empty */
		tnet_dns_cache_maintenance(ctx);
	}

	/* Retrieve data from cache. */
	if(ctx->caching)
	{
		const tnet_dns_cache_entry_t *entry = tnet_dns_cache_entry_get(ctx, qname, qclass, qtype);
		if(entry){
			response = tsk_object_ref(((tnet_dns_cache_entry_t*)entry)->response);
			from_cache = tsk_true;
			goto bail;
		}
	}

	/* Set user preference */
	query->Header.RD = ctx->recursion;

	/* EDNS0 */
	if(ctx->edns0)
	{
		tnet_dns_opt_t *rr_opt = TNET_DNS_OPT_CREATE(TNET_DNS_DGRAM_SIZE_DEFAULT);
		if(!query->Additionals){
			query->Additionals = TSK_LIST_CREATE();
		}
		tsk_list_push_back_data(query->Additionals, (void**)&rr_opt);
		query->Header.ARCOUNT++;
	}
	
	/* Serialize and send to the server. */
	if(!(output = tnet_dns_message_serialize(query))){
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
		tnet_socket_t *localsocket4 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv4);
		tnet_socket_t *localsocket6 = TNET_SOCKET_CREATE(TNET_SOCKET_HOST_ANY, TNET_SOCKET_PORT_ANY, tnet_socket_type_udp_ipv6);
		tsk_bool_t useIPv6 = TNET_SOCKET_IS_VALID(localsocket6);
		
		tsk_safeobj_lock(ctx);

		/* Check socket validity */
		if(!TNET_SOCKET_IS_VALID(localsocket4)){
			goto done;
		}

		/* Always wait 500ms before retransmission */
		tv.tv_sec = 0;
		tv.tv_usec = (500 * 1000);
		
		do
		{
			//
			//	Send data (loop through all intefaces)
			//
			tsk_list_foreach(item, ctx->servers)
			{
				address = item->data;
				if(!address->ip || 
					(address->family != AF_INET && address->family != AF_INET6) || 
					(address->family == AF_INET6 && !TNET_SOCKET_IS_VALID(localsocket6))){
					continue;
				}
				
				if(tnet_sockaddr_init(address->ip, ctx->server_port, (address->family == AF_INET ? tnet_socket_type_udp_ipv4 : tnet_socket_type_udp_ipv6), &server)){
					TSK_DEBUG_ERROR("Failed to initialize the DNS server address: \"%s\"", address->ip);
					continue;
				}

				TSK_DEBUG_INFO("Send DNS query to \"%s\"", address->ip);

				if(address->family == AF_INET6){
					if((ret = tnet_sockfd_sendto(localsocket6->fd, (const struct sockaddr*)&server, output->data, output->size))){
						// succeed?
						break;
					}
				}
				else{
					if((ret = tnet_sockfd_sendto(localsocket4->fd, (const struct sockaddr*)&server, output->data, output->size))){
						// succeed?
						break;
					}
				}
			}			

			//
			//	Received data
			//
			/* First time? ==> set timeout value */
			if(!timeout){
				timeout = tsk_time_epoch() + ctx->timeout;
			}

			/* Set FDs */
			FD_ZERO(&set);
			FD_SET(localsocket4->fd, &set);
			if(useIPv6){
				FD_SET(localsocket6->fd, &set);
				maxFD = TSK_MAX(localsocket4->fd, localsocket6->fd);
			}
			else{
				maxFD = localsocket4->fd;
			}

			/* wait for response */
			if((ret = select(maxFD+1, &set, NULL, NULL, &tv))<0){ /* Error */
				TNET_PRINT_LAST_ERROR("Select failed.");
				goto done;
			}
			else if(ret == 0){ /* timeout ==> do nothing */
				
			}
			else{ /* there is data to read */
				size_t len = 0;
				void* data = 0;
				tnet_fd_t active_fd;
				
				/* Find active file descriptor */
				if(FD_ISSET(localsocket4->fd, &set)){
					active_fd = localsocket4->fd;
				}
				else if(FD_ISSET(localsocket6->fd, &set)){
					active_fd = localsocket4->fd;
				}
				else{
					TSK_DEBUG_ERROR("FD_ISSET ==> Invalid file descriptor.");
					continue;
				}

				/* Check how how many bytes are pending */
				if((ret = tnet_ioctlt(active_fd, FIONREAD, &len))<0){
					TSK_DEBUG_ERROR("tnet_ioctlt failed with error code:%d", tnet_geterrno());
					goto done;
				}
				
				/* Receive pending data */
				data = tsk_calloc(len, sizeof(uint8_t));
				if((ret = tnet_sockfd_recv(active_fd, data, len, 0))<0){
					TSK_FREE(data);
					
					TSK_DEBUG_ERROR("tnet_sockfd_recv failed with error code:%d", tnet_geterrno());
					goto done;
				}

				/* Parse the incoming response. */
				response = tnet_dns_message_deserialize(data, len);
				TSK_FREE(data);
				
				if(response)
				{	/* response successfuly parsed */
					if(query->Header.ID != response->Header.ID || !TNET_DNS_MESSAGE_IS_RESPONSE(response)){
						/* Not same transaction id ==> continue*/
						TSK_OBJECT_SAFE_FREE(response);
					}
					else goto done;
				}
			}
		}
		while(timeout > tsk_time_epoch());
		
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
	if(response){
		if(!from_cache && ctx->caching){
			tnet_dns_cache_entry_add(ctx, qname, qclass, qtype, response);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to contact the DNS server.");
	}

	return response;
}

/**@ingroup tnet_dns_group
* Gets list of URIs associated to this telephone number by using ENUM protocol (RFC 3761).
* @param ctx The DNS context. 
* The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @param e164num A valid E.164 number (e.g. +1-800-555-5555).
* @param domain The domain name (e.g e164.arpa, e164.org, ...). If Null, default value is "e164.arpa" (IANA).
* @retval The DNS response with NAPTR RRs. The @a answers in the @a response are already filtered.
* MUST be destroyed using @ref TSK_OBJECT_SAFE_FREE macro.
* @sa @ref tnet_dns_resolve, @ref tnet_dns_enum_2.
*/
tnet_dns_response_t* tnet_dns_enum(tnet_dns_ctx_t* ctx, const char* e164num, const char* domain)
{
	char e164domain[255];
	tnet_dns_response_t* ret = tsk_null;
	size_t e164size;
	int i, j; // must be signed
	
	e164size = strlen(e164num);
	
	if(!ctx || !e164num || !e164size){
		goto bail;
	}
	
	if(e164size /* max=15 digits + ".e164.arpa" + '+' */>=(sizeof(e164domain)-1)){
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
	for(i = e164size-1, j=0; i>=0; i--){
		if(!isdigit(e164num[i])){
			continue;
		}
		e164domain[j++] = e164num[i];
		e164domain[j++] = '.';
	}

	// append domain name
	if(domain){
		memcpy( &e164domain[j], domain, ((strlen(domain) + j) >= sizeof(e164domain)-1) ? (sizeof(e164domain)-j-1) : strlen(domain) );
	}
	else{
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
* The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @param service The ENUM service (e.g. E2U+SIP).
* @param e164num A valid E.164 number (e.g. +1-800-555-5555).
* @param domain The domain name (e.g e164.arpa, e164.org, ...). If Null, default value is "e164.arpa" (IANA).
* @retval The Internet address (SIP, email, ICQ, fax, ...) associated to this service.
* MUST be freed using @ref TSK_FREE macro.
* @sa @ref tnet_dns_resolve, @ref tnet_dns_enum.
*/
char* tnet_dns_enum_2(tnet_dns_ctx_t* ctx, const char* service, const char* e164num, const char* domain)
{
	tnet_dns_response_t* response;
	const tsk_list_item_t* item;
	char* ret = tsk_null;
	const tnet_dns_rr_t* rr;

	if((response = tnet_dns_enum(ctx, e164num, domain))){
		if(TNET_DNS_RESPONSE_IS_SUCCESS(response)){
			tsk_list_foreach(item, response->Answers){
				rr = item->data;
				if(rr->qtype == qtype_naptr){
					const tnet_dns_naptr_t *naptr = (const tnet_dns_naptr_t*)rr;
					/*	RFC 3403 - 6.2 E164 Example
						Both the ENUM [18] and URI  Resolution [4] Applications use the 'u'
						flag.  This flag states that the Rule is terminal and that the output
						is a URI which contains the information needed to contact that
						telephone service.
					   */
					if( tsk_striequals(naptr->flags, "u") && tsk_striequals(naptr->services, service)){
						/* RFC 3403 - 4.1 Packet Format
							The fields (replacement and regexp) are also mutually exclusive.  If a record is
							returned that has values for both fields then it is considered to
							be in error and SHOULD be either ignored or an error returned.
						*/
						if(naptr->regexp && naptr->replacement){
							continue;
						}

						if((ret = tnet_dns_regex_parse(e164num, naptr->regexp))){
							break;
						}
					}
				}
			}	
		}
		else{
			TSK_DEBUG_ERROR("We got an error response from the DNS server. Error code: %u", response->Header.RCODE);
		}

		TSK_OBJECT_SAFE_FREE(response);
	}

	return ret;
}

/**@ingroup tnet_dns_group
* Performs DNS SRV resolution.
* @param ctx The DNS context. 
* The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @param service The name of the service (e.g. SIP+D2U).
* @param hostname The result containing an IP address or FQDN.
* @param port The port associated to the result.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tnet_dns_resolve.
*
* @code
* tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
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

	if(!ctx){
		return -1;
	}
	
	// tnet_dns_resolve is thread-safe
	if((response = tnet_dns_resolve(ctx, service, qclass_in, qtype_srv)))
	{
		const tsk_list_item_t *item;
		const tnet_dns_rr_t* rr;
		tsk_list_foreach(item, response->Answers) /* Already Filtered ==> Peek the first One */
		{
			rr = item->data;
			if(rr->qtype == qtype_srv){
				const tnet_dns_srv_t *srv = (const tnet_dns_srv_t*)rr;
				
				tsk_strupdate(hostname, srv->target);
				*port = srv->port;
				break;
			}
		}
	}

	TSK_OBJECT_SAFE_FREE(response);

	return (hostname && !tsk_strempty(*hostname)) ? 0 : -2;
}

/**@ingroup tnet_dns_group
* Performs DNS NAPTR followed by DNS SRV resolution.
* @param ctx The DNS context.
* The context contains the user's preference and should be created using @ref TNET_DNS_CTX_CREATE.
* @param domain The Name of the domain (e.g. google.com).
* @param service The name of the service (e.g. SIP+D2U).
* @param hostname The result containing an IP address or FQDN.
* @param port The port associated to the result.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tnet_dns_resolve.
*
* @code
* tnet_dns_ctx_t *ctx = TNET_DNS_CTX_CREATE();
* char* hostname = 0;
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

	if(!ctx){
		return -1;
	}
	// tnet_dns_resolve is thread-safe
	if((response = tnet_dns_resolve(ctx, domain, qclass_in, qtype_naptr)))
	{
		const tsk_list_item_t *item;
		const tnet_dns_rr_t* rr;

		char* replacement = 0; /* e.g. _sip._udp.example.com */
		char* flags = 0;/* e.g. S, A, AAAA, A6, U, P ... */

		tsk_list_foreach(item, response->Answers) /* Already Filtered ==> Peek the first One */
		{
			rr = item->data;
			if(rr->qtype == qtype_naptr){
				tnet_dns_naptr_t *naptr = (tnet_dns_naptr_t*)rr;

				if(tsk_striequals(service, naptr->services)){
					tsk_strupdate(&replacement, naptr->replacement);
					tsk_strupdate(&flags, naptr->flags);

					break;
				}
			}
		}

		if(flags && replacement){
			if(tsk_striequals(flags, "S")){
				tnet_dns_query_srv(ctx, replacement, hostname, port);
			}
			else if(tsk_striequals(flags, "A") || tsk_striequals(flags, "AAAA") ||tsk_striequals(flags, "A6")){
				TSK_DEBUG_WARN("Defaulting port value.");
				tsk_strupdate(hostname, replacement);
				*port = 5060;
			}
			else{
				TSK_DEBUG_ERROR("DNS NAPTR query returned invalid falgs.");
			}
		}
		else{
			TSK_DEBUG_ERROR("DNS NAPTR query returned zero result.");
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

	if(ctx)
	{
		tsk_list_item_t *item;
		tsk_safeobj_lock(ctx);
again:
		
		tsk_list_foreach(item, ctx->cache)
		{
			// FIXME: ttl should be from RR::ttl
			tnet_dns_cache_entry_t *entry = (tnet_dns_cache_entry_t*)item->data;
			if((entry ->epoch + ctx->cache_ttl) < tsk_time_epoch()){
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

	if(ctx)
	{
		tnet_dns_cache_entry_t *entry;

		tsk_safeobj_lock(ctx);

		entry = 0;

		/* Retrieve from cache */
		entry = (tnet_dns_cache_entry_t*)tnet_dns_cache_entry_get(ctx, qname, qclass, qtype);

		if(entry){	
			/* UPDATE */
			TSK_OBJECT_SAFE_FREE(entry->response);
			entry->response = tsk_object_ref(response);
			entry->epoch = tsk_time_epoch();
			ret = 0;
			goto done;
		}
		else{ 
			/* CREATE */
			entry = TNET_DNS_CACHE_ENTRY_CREATE(qname, qclass, qtype, response);
			if(entry){
				tsk_list_push_back_data(ctx->cache, (void**)&entry);
				ret = 0;
				goto done;
			}
			else{
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
	if(ctx)
	{
		tsk_list_item_t *item;

		tsk_safeobj_lock(ctx);

		tsk_list_foreach(item, ctx->cache){
			tnet_dns_cache_entry_t *entry = (tnet_dns_cache_entry_t*)item->data;
			if(entry->qtype == qtype && entry->qclass == qclass && tsk_strequals(entry->qname, qname)){
				ret = entry;
				break;
			}
		}

		tsk_safeobj_unlock(ctx);
	}

	return ret;
}


/**
* Adds new DNS server to the list of the list of servers to query.
* @param ctx DNS context containing the user parameters. The new DNS server will be added to this context.
* @param host The IP address (or FQDN) of the dns server to add to the server.
* @retval zero if succeed and non-zero error code otherwise.
*/
int tnet_dns_add_server(tnet_dns_ctx_t *ctx, const char* host)
{
	tnet_address_t *address;
	
	if(!ctx || !host){
		return -1;
	}
	
	if(!ctx->servers){
		ctx->servers = TSK_LIST_CREATE();
	}
	
	if((address = TNET_ADDRESS_CREATE(host))){
		address->family = tnet_get_family(host);
		address->dnsserver = 1;
		tsk_list_push_ascending_data(ctx->servers, (void**)&address);

		return 0;
	}
	
	return -2;
}

//=================================================================================================
//	[[DNS CACHE ENTRY]] object definition
//
static tsk_object_t* tnet_dns_cache_entry_create(tsk_object_t * self, va_list * app)
{
	tnet_dns_cache_entry_t *entry = self;
	if(entry)
	{
		entry->qname = tsk_strdup(va_arg(*app, const char*));
		entry->qclass = va_arg(*app, tnet_dns_qtype_t);
		entry->qtype = va_arg(*app, tnet_dns_qtype_t);
		entry->response = tsk_object_ref(va_arg(*app, tnet_dns_response_t*));
		
		entry->epoch = tsk_time_epoch();
	}
	return self;
}

static tsk_object_t* tnet_dns_cache_entry_destroy(tsk_object_t * self) 
{ 
	tnet_dns_cache_entry_t *entry = self;
	if(entry){
		TSK_OBJECT_SAFE_FREE(entry->response);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_cache_entry_def_s =
{
	sizeof(tnet_dns_cache_entry_t),
	tnet_dns_cache_entry_create,
	tnet_dns_cache_entry_destroy,
	tsk_null,
};
const void *tnet_dns_cache_entry_def_t = &tnet_dns_cache_entry_def_s;


//=================================================================================================
//	[[DNS CONTEXT]] object definition
//
static tsk_object_t* tnet_dns_ctx_create(tsk_object_t * self, va_list * app)
{
	tnet_dns_ctx_t *ctx = self;
	if(ctx)
	{
		ctx->timeout = TNET_DNS_TIMEOUT_DEFAULT;
		ctx->recursion = tsk_true;
		ctx->edns0 = tsk_true;
		ctx->caching = tsk_false;

		ctx->cache_ttl = TNET_DNS_CACHE_TTL;

		ctx->server_port = TNET_DNS_SERVER_PORT_DEFAULT;

		/* Gets all dns servers. */
		ctx->servers = tnet_get_addresses_all_dnsservers();
		/* Creates empty cache. */
		ctx->cache = TSK_LIST_CREATE();

		tsk_safeobj_init(ctx);
	}
	return self;
}

static tsk_object_t* tnet_dns_ctx_destroy(tsk_object_t * self) 
{ 
	tnet_dns_ctx_t *ctx = self;
	if(ctx)
	{
		tsk_safeobj_deinit(ctx);

		TSK_OBJECT_SAFE_FREE(ctx->servers);
		TSK_OBJECT_SAFE_FREE(ctx->cache);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_ctx_def_s =
{
	sizeof(tnet_dns_ctx_t),
	tnet_dns_ctx_create,
	tnet_dns_ctx_destroy,
	tsk_null,
};
const void *tnet_dns_ctx_def_t = &tnet_dns_ctx_def_s;
