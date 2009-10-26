/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#ifndef _TINYIPSEC_IPSEC_H_
#define _TINYIPSEC_IPSEC_H_

#include "tinyipsec_config.h"
#include "ipsec_utils.h"

typedef unsigned long ipsec_error_t;
#define SA_COUNT				4
#define SA_DEFAULT_LIFETIME		3600 /* 1 hour */

#define IPSEC_KEY_LEN			16
#define IPSEC_IK_LEN			20
#define IPSEC_CK_LEN			24

#define IPSEC_AUTH_ALG_STR(alg) (alg == iaa_hmac_md5_96) ? "hmac-md5-96" : "hmac-sha-1-96"
#define IPSEC_ENC_ALG_STR(enc) (enc == iea_3des) ? "des-ede3-cbc" : (enc== iea_aes ? "aes-cbc" : "null")
#define IPSEC_MODE_STR(mode) (mode == ipm_transport) ? "trans" : "tun"
#define IPSEC_PROTO_STR(proto) (proto == ipp_ah) ? "ah" : "esp"

/* function pointer ... */
typedef void (*ipsec_sa_opaque_func_free)(void** opaque);

/* context */
#define IPSEC_CONTEXT_CREATE(ctx)		IPSEC_XXX_CREATE(ctx, context)
#define IPSEC_CONTEXT_SAFE_FREE(ctx)	IPSEC_XXX_SAFE_FREE(ctx, context)

/* SA */
#define IPSEC_SA_CREATE(_sa)		IPSEC_XXX_CREATE(_sa, sa)
#define IPSEC_SA_SAFE_FREE(_sa)		IPSEC_XXX_SAFE_FREE(_sa, sa)

/* directions */
typedef enum ipsec_direction_s
{
	ipd_incoming_req = 0x00,	/* US <- PC */
	ipd_incoming_rep,			/* UC <- PS */
	ipd_outgoing_req,			/* UC -> PS */
	ipd_outgoing_rep			/* US -> PC */
}
ipsec_direction_t;

/* encryption algorithm */
typedef enum ipsec_encrypt_algo_s
{
	iea_null = 0x00,
	iea_des,
	iea_3des,
	iea_aes
}
ipsec_encrypt_algo_t;

/* authentication algorithm */
typedef enum ipsec_auth_algo_s
{
	iaa_hmac_md5_96 = 0x00,
	iaa_hmac_sha1_96
}
ipsec_auth_algo_t;

/* ipsec protocol */
typedef enum ipsec_protocol_s
{
	ipp_esp = (0x01 << 1),
	ipp_ah = (0x01 << 2),
	ipp_both = (ipp_esp | ipp_ah)
}
ipsec_protocol_t;

/* ipsec transport */
typedef enum ipsec_transport_s
{
	ipt_udp = 0x00,
	ipt_tcp
}
ipsec_transport_t;

/* ipsec mode of operation */
typedef enum ipsec_mode_s
{
	ipm_transport = 0x00,
	ipm_tunnel
}
ipsec_mode_t;

/* ipsec sa */
typedef struct ipsec_sa_s
{
	uint32_t lifetime;				/* sa lifetime */
	uint32_t spi;					/* a unique parameter index */
	//ipsec_direction_t direction;	/* direction */

	//char* ip;
	uint16_t port;

	ipsec_sa_opaque_func_free opaque_func_free;
	void* opaque; /* platform specific */
}
ipsec_sa_t;

/* ipsec context */
typedef struct ipsec_context_s
{
	ipsec_encrypt_algo_t encrypt;	/* encryption algorithm */
	ipsec_auth_algo_t auth;			/* authentication algorithm */
	ipsec_mode_t mode;				/* mode of operation */
	ipsec_transport_t trans;		/* network transport */
	ipsec_protocol_t proto;			/* protocol type */
	
	int ipv6;
	char* localIP;
	char* remoteIP;

	ipsec_sa_t SAs[SA_COUNT];

	void* opaque; /* platform specific */
}
ipsec_context_t;

TINYIPSEC_API ipsec_error_t ipsec_context_init(ipsec_context_t* context);
TINYIPSEC_API uint32_t ipsec_get_spi(const ipsec_context_t* context, ipsec_direction_t direction);
TINYIPSEC_API void ipsec_set_ik(ipsec_context_t* context, const char ik[IPSEC_KEY_LEN]);
TINYIPSEC_API void ipsec_set_ck(ipsec_context_t* context, const char ck[IPSEC_KEY_LEN]);
TINYIPSEC_API int ipsec_sa_bound(ipsec_context_t* context, ipsec_direction_t direction);
TINYIPSEC_API void ipsec_flush(const ipsec_context_t* context);
TINYIPSEC_API void ipsec_context_free(ipsec_context_t** context);

TINYIPSEC_API void ipsec_sa_init(ipsec_sa_t* sa);
TINYIPSEC_API void ipsec_sa_free(ipsec_sa_t* sa);

#endif /* _TINYIPSEC_IPSEC_H_ */