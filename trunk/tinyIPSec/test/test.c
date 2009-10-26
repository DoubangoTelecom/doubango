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
#include "stdafx.h"
#include "ipsec_api.h"

#define REMOTE_IP		"192.168.0.9" //"2a01:e35:8b32:7050:6122:2706:2124:32ca"//"192.168.0.15"
#define LOCAL_IP		"192.168.0.14" //"2a01:e35:8b32:7050:6122:2706:2124:32cb"//"192.168.0.14"

#define IK		"1234567890123456"
#define CK		"1234567890121234"

#define PORT_LOCAL_C	1010
#define PORT_LOCAL_S	2020

#define PORT_REMOTE_C	3030
#define PORT_REMOTE_S	4040
#define SPI_REMOTE_C	2222
#define SPI_REMOTE_S	3333

int _tmain(int argc, _TCHAR* argv[])
{
	ipsec_context_t* context = 0;
	uint32_t spi_local_s = 0, spi_local_c = 0, spi_remote_s = 0, spi_remote_c = 0;

	/* create context */
	IPSEC_CONTEXT_CREATE(context);

	// FIXME: check context validity

	/* set algorithms, proto and mode */
	context->auth = iaa_hmac_md5_96;
	context->encrypt = iea_3des;
	context->proto = ipp_both;
	context->mode = ipm_transport;
	
	/* set ik and ck */
	ipsec_set_ck(context, CK);
	ipsec_set_ik(context, IK);

	/* set local and remote IPs */
	context->localIP = ipsec_strdup(LOCAL_IP);
	context->remoteIP = ipsec_strdup(REMOTE_IP);
	context->ipv6 = 0;

	/* set local ports */
	context->SAs[ipd_incoming_req].port = PORT_LOCAL_S; /* US <- PC */
	context->SAs[ipd_incoming_rep].port = PORT_LOCAL_C; /* UC <- PS */

	/* get local spis */
	spi_local_s = ipsec_get_spi(context, ipd_incoming_req);
	spi_local_c = ipsec_get_spi(context, ipd_incoming_rep);

	/******* 
		now send your first request with security-client header using
		...
	*/
	printf("Security-Client: ipsec-3gpp; alg=%s; ealg=%s; spi-c=%u; spi-s=%u; port-c=%u; port-s=%u\n\n",
			IPSEC_AUTH_ALG_STR(context->auth), IPSEC_ENC_ALG_STR(context->encrypt), spi_local_c, spi_local_s, PORT_LOCAL_C, PORT_LOCAL_S
		);
	getchar();

	/* 
		get and process the 401/407 
		...
	*/
	printf("Security-Server: ipsec-3gpp; q=0.1; alg=%s; ealg=%s; spi-c=%u; spi-s=%u; port-c=%u; port-s=%u\n\n",
		IPSEC_AUTH_ALG_STR(context->auth), IPSEC_ENC_ALG_STR(context->encrypt), SPI_REMOTE_C, SPI_REMOTE_S, PORT_REMOTE_C, PORT_REMOTE_S
		);

	/* set values for remote entity */
	context->SAs[ipd_outgoing_rep].port = PORT_REMOTE_C; /* US -> PC */
	context->SAs[ipd_outgoing_rep].spi = SPI_REMOTE_C; /* US -> PC */
	context->SAs[ipd_outgoing_req].port = PORT_REMOTE_S; /* UC -> PS */
	context->SAs[ipd_outgoing_req].spi = SPI_REMOTE_S; /* UC -> PS */


	/* bound SAs and begin sending/receiving sip messages */
	ipsec_sa_bound(context, ipd_incoming_rep);
	ipsec_sa_bound(context, ipd_incoming_req);
	ipsec_sa_bound(context, ipd_outgoing_rep);
	ipsec_sa_bound(context, ipd_outgoing_req);

	getchar();

	/* free context */
	IPSEC_CONTEXT_SAFE_FREE(context);

	return 0;
}

