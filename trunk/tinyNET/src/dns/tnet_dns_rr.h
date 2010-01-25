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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file tnet_dns_rr.h
 * @brief DNS Resource Record (RFCS 1034 and 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef TNET_DNS_RR_H
#define TNET_DNS_RR_H

#include "tinyNET_config.h"

#include "tsk_list.h"

TNET_BEGIN_DECLS

#define TNET_DNS_RR_CREATE()						tsk_object_new(tnet_dns_rr_def_t)

/**
*	RFC 1035 - 3.2.2. TYPE values
*/
typedef enum tnet_dns_qtype_e
{
	qtype_a = 0x0001,		/**< A               1 a host address */
	qtype_ns = 0x0002,		/**< NS              2 an authoritative name server */
	qtype_md = 0x0003,		/**< MD              3 a mail destination (Obsolete - use MX) */
	qtype_mf = 0x0004,		/**< MF              4 a mail forwarder (Obsolete - use MX) */
	qtype_cname = 0x0005,	/**< CNAME           5 the canonical name for an alias */
	qtype_soa = 0x0006,		/**< SOA             6 marks the start of a zone of authority */
	qtype_mb = 0x0007,		/**< MB              7 a mailbox domain name (EXPERIMENTAL) */
	qtype_mg = 0x0008,		/**< MG              8 a mail group member (EXPERIMENTAL) */
	qtype_mr = 0x0009,		/**< MR              9 a mail rename domain name (EXPERIMENTAL) */
	qtype_null = 0x000a,	/**< NULL            10 a null RR (EXPERIMENTAL) */
	qtype_wks = 0x000b,		/**< WKS             11 a well known service description */
	qtype_ptr = 0x000c,		/**< PTR             12 a domain name pointer */
	qtype_hinfo = 0x000d,	/**< HINFO           13 host information */
	qtype_minfo = 0x000e,	/**< MINFO           14 mailbox or mail list information */
	qtype_mx = 0x000f,		/**< MX              15 mail exchange */
	qtype_txt = 0x0010,		/**< TXT             16 text strings */

	qtype_aaa = 0x001c,		/**< AAAA			 28 IPv6 host address */

	qtype_any = 0x00ff		/**< *               255 A request for all records (3.2.3. QTYPE values)*/
}
tnet_dns_qtype_t;

/**
*	RFC 1035 - 3.2.4. CLASS values
*/
typedef enum tnet_dns_qclass_e
{
	qclass_in = 0x0001,		/**< IN              1 the Internet */
	qclass_ics = 0x0002,	/**< CS              2 the CSNET class (Obsolete - used only for examples in some obsolete RFCs) */
	qclass_ch = 0x0003,		/**< CH              3 the CHAOS class */
	qclass_hs = 0x0004,		/**< HS              4 Hesiod [Dyer 87] */

	qclass_any = 0x00ff		/**< *               255 any class (3.2.5. QCLASS values) */
}
tnet_dns_qclass_t;

/**
*	RFC 1034 (3.6. Resource Records) and 1035 (3.2.1. Format) 
*/
typedef struct tnet_dns_rr_s
{
	TSK_DECLARE_OBJECT;

	/*	RFC 1035 - 3.2.1. Format
	                                  1  1  1  1  1  1
		0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                                               |
		/                                               /
		/                      NAME                     /
		|                                               |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                      TYPE                     |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                     CLASS                     |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                      TTL                      |
		|                                               |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
		|                   RDLENGTH                    |
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
		/                     RDATA                     /
		/                                               /
		+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/

	unsigned initialized:1;

	/** An owner name, i.e., the name of the node to which this resource record pertains. */
	char* name;
	
	/** Two octets containing one of the RR TYPE codes. */
	tnet_dns_qtype_t qtype;

	/** Two octets containing one of the RR CLASS codes. */
	tnet_dns_qclass_t qclass;

	/** A 32 bit signed integer that specifies the time interval that the resource record may be cached before the source
		of the information should again be consulted.  
		Zero values are interpreted to mean that the RR can only be used for the transaction in progress, and should not be cached.  
		For example, SOA records are always distributed with a zero TTL to prohibit caching.  Zero values can also be used for extremely volatile data. */
	int32_t ttl;

	/** An unsigned 16 bit integer that specifies the length in octets of the RDATA field. */
	uint16_t rdlength;
	
	/** A variable length string of octets that describes the resource.  
		The format of this information varies according to the TYPE and CLASS of the resource record.*/
	void *rpdata;
}
tnet_dns_rr_t;
typedef tsk_list_t  tnet_dns_rrs_L_t;

int tnet_dns_rr_init(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass);
int tnet_dns_rr_deinit(tnet_dns_rr_t *rr);

TINYNET_GEXTERN const void *tnet_dns_rr_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_RR_H */