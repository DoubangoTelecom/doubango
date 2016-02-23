/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file tnet_dns_rr.h
 * @brief DNS Resource Record (RFCS 1034 and 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TNET_DNS_RR_H
#define TNET_DNS_RR_H

#include "tinynet_config.h"

#include "tsk_list.h"
#include "tsk_buffer.h"

/** @def TNET_DNS_RR
* Converts any DNS RR to a pointer to @ref tnet_dns_rr_t.
* @retval A pointer to @ref tnet_dns_rr_t object.
*/
TNET_BEGIN_DECLS

#define TNET_DNS_RR(self)							((tnet_dns_rr_t*)(self))

/**	RFC 1035 - 3.2.2. TYPE values
*	@sa http://en.wikipedia.org/wiki/List_of_DNS_record_types
*/
typedef enum tnet_dns_qtype_e {
    qtype_a = 1,		/**< A               1 a host address */
    qtype_ns = 2,		/**< NS              2 an authoritative name server */
    qtype_md = 3,		/**< MD              3 a mail destination (Obsolete - use MX) */
    qtype_mf = 4,		/**< MF              4 a mail forwarder (Obsolete - use MX) */
    qtype_cname = 5,	/**< CNAME           5 the canonical name for an alias */
    qtype_soa = 6,		/**< SOA             6 marks the start of a zone of authority */
    qtype_mb = 7,		/**< MB              7 a mailbox domain name (EXPERIMENTAL) */
    qtype_mg = 8,		/**< MG              8 a mail group member (EXPERIMENTAL) */
    qtype_mr = 9,		/**< MR              9 a mail rename domain name (EXPERIMENTAL) */
    qtype_null = 10,	/**< NULL            10 a null RR (EXPERIMENTAL) */
    qtype_wks = 11,		/**< WKS             11 a well known service description */
    qtype_ptr = 12,		/**< PTR             12 a domain name pointer */
    qtype_hinfo = 13,	/**< HINFO           13 host information */
    qtype_minfo = 14,	/**< MINFO           14 mailbox or mail list information */
    qtype_mx = 15,		/**< MX              15 mail exchange */
    qtype_txt = 16,		/**< TXT             16 text strings */

    qtype_aaaa = 28,	/**< AAAA			 28 IPv6 host address */

    qtype_srv = 33,		/**< SRV			 33 Service locator */

    qtype_naptr = 35,	/**< NAPTR			 35 Naming Authority Pointer */

    qtype_opt = 41,		/**< OPT			 41 Option */

    qtype_ipseckey = 45,/**< IPSECKEY		 45 IPSEC Key */

    qtype_spf = 99,		/**< SPF			 99 SPF record */

    qtype_any = 255		/**< *               255 A request for all records (3.2.3. QTYPE values)*/
}
tnet_dns_qtype_t;

/**	RFC 1035 - 3.2.4. CLASS values
*/
typedef enum tnet_dns_qclass_e {
    qclass_in = 1,		/**< IN              1 the Internet */
    qclass_ics = 2,	/**< CS              2 the CSNET class (Obsolete - used only for examples in some obsolete RFCs) */
    qclass_ch = 3,		/**< CH              3 the CHAOS class */
    qclass_hs = 4,		/**< HS              4 Hesiod [Dyer 87] */

    qclass_any = 255		/**< *               255 any class (3.2.5. QCLASS values) */
}
tnet_dns_qclass_t;

/**	RFC 1034 (3.6. Resource Records) and 1035 (3.2.1. Format)
*/
typedef struct tnet_dns_rr_s {
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

    tsk_bool_t initialized;

    /** An owner name, i.e., the name of the node to which this resource record pertains. */
    char* name;

    /** Two octets containing one of the RR TYPE codes. */
    tnet_dns_qtype_t qtype;

    /** Two octets containing one of the RR CLASS codes. */
    tnet_dns_qclass_t qclass;

    /** A 32 bit signed integer that specifies the time interval (seconds) that the resource record may be cached before the source
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

#define TNET_DECLARE_DNS_RR tnet_dns_rr_t dns_rr

typedef tsk_list_t  tnet_dns_rrs_L_t; /**< List of @ref tnet_dns_rr_t elements. */

int tnet_dns_rr_init(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass);
int tnet_dns_rr_deinit(tnet_dns_rr_t *rr);

int tnet_dns_rr_charstring_deserialize(const void* data, char** name, tsk_size_t *offset);

int tnet_dns_rr_qname_deserialize(const void* data, char** name, tsk_size_t *offset);
int tnet_dns_rr_qname_serialize(const char* qname, tsk_buffer_t* output);

tnet_dns_rr_t* tnet_dns_rr_deserialize(const void* data, tsk_size_t size, tsk_size_t* offset);
int tnet_dns_rr_serialize(const tnet_dns_rr_t* rr, tsk_buffer_t *output);

TINYNET_GEXTERN const tsk_object_def_t *tnet_dns_rr_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_RR_H */
