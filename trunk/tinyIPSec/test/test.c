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
#include "stdafx.h"
#include "tipsec.h"

#include "tsk_debug.h"

//#define ADDR_REMOTE		"192.168.0.9" //"2a01:e35:8b32:7050:6122:2706:2124:32ca"//"192.168.0.15"
//#define ADDR_LOCAL		"192.168.0.12" //"2a01:e35:8b32:7050:6122:2706:2124:32cb"//"192.168.0.14"
#define ADDR_REMOTE		"2001:5c0:1502:1800::225"
#define ADDR_LOCAL		"2001:5c0:1502:1800:5cc8:4a4e:3ef7:3314"
#define USE_IPV6		1

#define IK		"1234567890123456"
#define CK		"1234567890121234"

#define PORT_UC 		1010
#define PORT_US			2020

#define PORT_PC			3030
#define PORT_PS			4040
#define SPI_PC			2222
#define SPI_PS			3333

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	int ret;
	tipsec_lifetime_t lifetime = 600000; /* Always set it to the maximum value. (Not possible to update the value after REGISTER 200OK. ) */

	tipsec_context_t * ctx = TIPSEC_CONTEXT_CREATE(ipproto_icmp, USE_IPV6, mode_trans, ealg_aes, algo_hmac_md5_96, proto_both);

	if((ret = tipsec_set_local(ctx, ADDR_LOCAL, ADDR_REMOTE, PORT_UC, PORT_US))){
		goto bail;
	}

	/* Now send your first request with security-client header using
	*/
	TSK_DEBUG_INFO("Security-Client: ipsec-3gpp; alg=%s; ealg=%s; spi-c=%u; spi-s=%u; port-c=%u; port-s=%u\n\n",
			TIPSEC_ALG_TO_STR(ctx->alg), TIPSEC_EALG_TO_STR(ctx->ealg), ctx->spi_uc, ctx->spi_us, ctx->port_uc, ctx->port_us
		);
	getchar();

	/* get and process the 401/407
	*/

	if((ret = tipsec_set_remote(ctx, SPI_PC, SPI_PS, PORT_PC, PORT_PS, lifetime))){
		goto bail;
	}

	tipsec_set_keys(ctx, IK, CK);

	/* Start */
	if((ret = tipsec_start(ctx))){
		goto bail;
	}

	TSK_DEBUG_INFO("ALL IS OOOOOOOK");
	getchar();

	if((ret = tipsec_stop(ctx))){
		goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(ctx);

	//ipsec_context_t* context = 0;
	//uint32_t spi_local_s = 0, spi_local_c = 0, spi_remote_s = 0, spi_remote_c = 0;

	////http://www.arib.or.jp/IMT-2000/V740Dec09/2_T63/ARIB-STD-T63/Rel7/33/A33203-790.pdf
	///* create context */
	//IPSEC_CONTEXT_CREATE(context);

	//// FIXME: check context validity

	///* set algorithms, proto and mode */
	//context->auth = iaa_hmac_md5_96;
	//context->encrypt = iea_null;
	//context->proto = ipp_esp;
	//context->mode = ipm_transport;
	//
	///* set ik and ck */
	//ipsec_set_ck(context, CK);
	//ipsec_set_ik(context, IK);

	///* set local and remote IPs */
	//context->localIP = ipsec_strdup(LOCAL_IP);
	//context->remoteIP = ipsec_strdup(REMOTE_IP);
	//context->ipv6 = 0;

	///* set local ports */
	//context->SAs[ipd_incoming_req].port = PORT_LOCAL_S; /* US <- PC */
	//context->SAs[ipd_incoming_rep].port = PORT_LOCAL_C; /* UC <- PS */

	///* get local spis */
	//spi_local_s = ipsec_get_spi(context, ipd_incoming_req);
	//spi_local_c = ipsec_get_spi(context, ipd_incoming_rep);

	///******* 
	//	now send your first request with security-client header using
	//	...
	//*/
	//printf("Security-Client: ipsec-3gpp; alg=%s; ealg=%s; spi-c=%u; spi-s=%u; port-c=%u; port-s=%u\n\n",
	//		IPSEC_AUTH_ALG_STR(context->auth), IPSEC_ENC_ALG_STR(context->encrypt), spi_local_c, spi_local_s, PORT_LOCAL_C, PORT_LOCAL_S
	//	);
	//getchar();

	///* 
	//	get and process the 401/407 
	//	...
	//*/
	//printf("Security-Server: ipsec-3gpp; q=0.1; alg=%s; ealg=%s; spi-c=%u; spi-s=%u; port-c=%u; port-s=%u\n\n",
	//	IPSEC_AUTH_ALG_STR(context->auth), IPSEC_ENC_ALG_STR(context->encrypt), SPI_REMOTE_C, SPI_REMOTE_S, PORT_REMOTE_C, PORT_REMOTE_S
	//	);

	///* set values for remote entity */
	//context->SAs[ipd_outgoing_rep].port = PORT_REMOTE_C; /* US -> PC */
	//context->SAs[ipd_outgoing_rep].spi = SPI_REMOTE_C; /* US -> PC */
	//context->SAs[ipd_outgoing_req].port = PORT_REMOTE_S; /* UC -> PS */
	//context->SAs[ipd_outgoing_req].spi = SPI_REMOTE_S; /* UC -> PS */


	///* bound SAs and begin sending/receiving sip messages */
	//ipsec_sa_bound(context, ipd_incoming_rep);
	//ipsec_sa_bound(context, ipd_incoming_req);
	//ipsec_sa_bound(context, ipd_outgoing_rep);
	//ipsec_sa_bound(context, ipd_outgoing_req);

	//getchar();

	///* free context */
	//IPSEC_CONTEXT_SAFE_FREE(context);

	return 0;
}

