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
#include "ipsec.h"
#include <stdio.h>
#include <time.h>

#include <Fwpmu.h>

#if HAVE_WIN32_IPSEC

#define CLEANUP_ON_ERROR(result)				if(result != ERROR_SUCCESS) goto CLEANUP;
#define TINYIPSEC_FILTER_NAME					TEXT("tinyIPSEC")
#define TINYIPSEC_PROVIDER_KEY					NULL
#define WIN32_SA_MAX_LIFETIME					172799
#define TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST		20
#define GET_CONTEXT_OPAQUE(context)				((ipsec_win32_context_opaque_t*)(context)->opaque)
#define GET_SA_OPAQUE(sa)						((ipsec_win32_sa_opaque_t*)(sa).opaque)
#if defined(DEBUG) || defined(_DEBUG)
#	define DEBUG_PRINT printf
#else
#	define DEBUG_PRINT
#endif

#define GET_WIN32_AUTH_ID(auth) (auth == iaa_hmac_md5_96) ? IPSEC_AUTH_TRANSFORM_ID_HMAC_MD5_96 : IPSEC_AUTH_TRANSFORM_ID_HMAC_SHA_1_96
#define GET_WIN32_ENC_ID(enc)	 (enc == iea_3des) ? IPSEC_CIPHER_TRANSFORM_ID_CBC_3DES : ( (enc == iea_aes) ? IPSEC_CIPHER_TRANSFORM_ID_AES_128 : IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL )
#define GET_WIN32_MODE(mode)	 (mode == ipm_tunnel) ? IPSEC_TRAFFIC_TYPE_TUNNEL : IPSEC_TRAFFIC_TYPE_TRANSPORT
#define GET_WIN32_TRANS(trans)	 (trans == ipt_tcp) ? IPPROTO_TCP : IPPROTO_UDP
#define GET_WIN32_IPVER(ipv6)	 (ipv6) ? FWP_IP_VERSION_V6 : FWP_IP_VERSION_V4
#define GET_WIN32_PROTO(proto)	 (proto == ipp_ah) ? IPSEC_TRANSFORM_AH : ( (proto == ipp_esp) ? IPSEC_TRANSFORM_ESP_AUTH : IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER );

/* ipsec opaques for win32 platform */
typedef struct ipsec_win32_context_opaque_s
{
	FWP_BYTE_BLOB ik;
	FWP_BYTE_BLOB ck;

	HANDLE engine;
}
ipsec_win32_context_opaque_t;

/* sa endpoint definition*/
typedef struct ipsec_sa_endpoints_s
{
union 
    {
    UINT32 localV4Address;
    UINT8 localV6Address[ 16 ];
    };
union 
    {
    UINT32 remoteV4Address;
    UINT8 remoteV6Address[ 16 ];
    };
}ipsec_sa_endpoints_t;

/* as WFP do not provide null encryption I define my own*/
static const IPSEC_CIPHER_TRANSFORM_ID0 IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL=
{
   (IPSEC_CIPHER_TYPE)NULL,
   (IPSEC_CIPHER_TYPE)NULL
};

typedef struct ipsec_win32_sa_opaque_s
{
	IPSEC_SA_SPI spi;
	UINT64 inFilterId;
	UINT64 outFilterId;
	UINT64 saId;

	ipsec_sa_endpoints_t endpoint;
}
ipsec_win32_sa_opaque_t;

void DeleteSaContextAndFilters(__in HANDLE engine, __in UINT64 inFilterId, __in UINT64 outFilterId, __in UINT64 saId);
DWORD getInboundSPI(ipsec_context_t* context, ipsec_direction_t direction);
ipsec_sa_endpoints_t get_endpoint(const ipsec_context_t* context);

/* free win32 opaque */
void ipsec_win32_sa_opaque_free(void** _opaque)
{
	if(*_opaque)
	{
		ipsec_win32_sa_opaque_t** opaque = ((ipsec_win32_sa_opaque_t**)_opaque);
		free(*opaque);
		(*opaque) = 0;
	}
}

/* initialize ipsec context */
/* returns 0 if succeed and nonzero otherwise */
ipsec_error_t ipsec_context_init(ipsec_context_t* context)
{
	ipsec_error_t code = ERROR_SUCCESS;
	int i = 0;

	memset(context, 0, sizeof(ipsec_context_t));
	srand((unsigned int)time(0));

	/* opaque */
	context->opaque =  malloc(sizeof(ipsec_win32_context_opaque_t));
	memset(context->opaque, 0, sizeof(ipsec_win32_context_opaque_t));

	/* SAs */
	for(i=0; i<SA_COUNT; i++)
	{
		ipsec_sa_init(&(context->SAs[i]));
		context->SAs[i].opaque = malloc(sizeof(ipsec_win32_sa_opaque_t));
		memset(context->SAs[i].opaque, 0, sizeof(ipsec_win32_sa_opaque_t));
		context->SAs[i].opaque_func_free = ipsec_win32_sa_opaque_free;
	}

	/* open engine */
	if(code = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &GET_CONTEXT_OPAQUE(context)->engine))
	{
		return code;
	}
	
	return code;
}

/* get spi */
uint32_t ipsec_get_spi(const ipsec_context_t* context, ipsec_direction_t direction)
{
	switch(direction)
	{
	case ipd_incoming_req: /* US <- PC */
	case ipd_incoming_rep: /* UC <- PS */
		{
			/* already set do not change */
			if(!context->SAs[direction].spi) 
				getInboundSPI(context, direction);

			return context->SAs[direction].spi;
		}
	case ipd_outgoing_req:	/* UC -> PS */
	case ipd_outgoing_rep:	/* US -> PC */
	default:
		{
			return context->SAs[direction].spi;
		}
	}
}

/* set integrity key */
void ipsec_set_ik(ipsec_context_t* context, const char ik[IPSEC_KEY_LEN])
{
	IPSEC_SAFE_FREE(GET_CONTEXT_OPAQUE(context)->ik.data);

	GET_CONTEXT_OPAQUE(context)->ik.data = malloc(IPSEC_IK_LEN); 
	memset(GET_CONTEXT_OPAQUE(context)->ik.data, 0, IPSEC_IK_LEN);

	memcpy(GET_CONTEXT_OPAQUE(context)->ik.data, ik, IPSEC_KEY_LEN);
	GET_CONTEXT_OPAQUE(context)->ik.size = IPSEC_KEY_LEN;
}

/* set cipher key */
void ipsec_set_ck(ipsec_context_t* context, const char ck[IPSEC_KEY_LEN])
{
	IPSEC_SAFE_FREE(GET_CONTEXT_OPAQUE(context)->ck.data);

	GET_CONTEXT_OPAQUE(context)->ck.data = malloc(IPSEC_CK_LEN);
	memset(GET_CONTEXT_OPAQUE(context)->ck.data, 0, IPSEC_IK_LEN);

	memcpy(GET_CONTEXT_OPAQUE(context)->ck.data, ck, IPSEC_KEY_LEN);
	GET_CONTEXT_OPAQUE(context)->ck.size = IPSEC_KEY_LEN;
}

/* free ipsec context */
void ipsec_context_free(ipsec_context_t** context)
{
	int i = 0;

	if(*context)
	{
		/* flush all SAs */
		ipsec_flush(*context);

		/* SAs */
		for(i=0; i<SA_COUNT; i++)
		{
			ipsec_sa_free(&((*context)->SAs[i]));
		}

		/* free ik and ck */
		IPSEC_SAFE_FREE(GET_CONTEXT_OPAQUE(*context)->ck.data);
		IPSEC_SAFE_FREE(GET_CONTEXT_OPAQUE(*context)->ik.data);

		/* close engine */
		FwpmEngineClose0(GET_CONTEXT_OPAQUE(*context)->engine);
		
		/* free local/remote ip */
		IPSEC_SAFE_FREE((*context)->localIP);
		IPSEC_SAFE_FREE((*context)->remoteIP);

		free(*context);
		(*context) = 0;
	}
}

/* bound SA */
/* returns zero if succeed and nozero otherwise */
int ipsec_sa_bound(ipsec_context_t* context, ipsec_direction_t direction)
{
	int i=0, j=0;
	DWORD result = NO_ERROR;
	IPSEC_SA0 sa;
	IPSEC_SA_BUNDLE0 bundle;
	IPSEC_SA_AUTH_INFORMATION0 authInfo;
	HANDLE engine = GET_CONTEXT_OPAQUE(context)->engine;
	
	memset(&sa, 0, sizeof(sa));
	sa.spi = context->SAs[direction].spi;
	sa.saTransformType = GET_WIN32_PROTO(context->proto);

	//
	//	Keys padding
	//
	if(context->auth == iaa_hmac_sha1_96)
	{
		if(GET_CONTEXT_OPAQUE(context)->ik.size < IPSEC_IK_LEN)
		{
			for(i=GET_CONTEXT_OPAQUE(context)->ik.size; i<IPSEC_IK_LEN; i++)
			{
				((UINT8*)GET_CONTEXT_OPAQUE(context)->ik.data)[i] = 0x00;
			}
			GET_CONTEXT_OPAQUE(context)->ik.size = IPSEC_IK_LEN;
		}
	}
	if(context->encrypt == iea_3des)
	{
		if(GET_CONTEXT_OPAQUE(context)->ck.size < IPSEC_CK_LEN)
		{
			for(i=GET_CONTEXT_OPAQUE(context)->ck.size; i<IPSEC_CK_LEN; i++)
			{
				((UINT8*)GET_CONTEXT_OPAQUE(context)->ck.data)[i] = ((UINT8*)GET_CONTEXT_OPAQUE(context)->ck.data)[j++];
			}
			GET_CONTEXT_OPAQUE(context)->ck.size = IPSEC_CK_LEN;
		}
	}

	//
	//	In all case create Authentication info
	//
	memset(&authInfo, 0, sizeof(authInfo));
	authInfo.authTransform.authTransformId = GET_WIN32_AUTH_ID(context->auth);
	authInfo.authKey = GET_CONTEXT_OPAQUE(context)->ik;

	if( sa.saTransformType == IPSEC_TRANSFORM_AH )
	{
		sa.ahInformation = &authInfo;
	}
	else if( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH )
	{
		sa.espAuthInformation = &authInfo;
	}
	else if( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER )
	{
		IPSEC_SA_CIPHER_INFORMATION0 cipherInfo;
		IPSEC_SA_AUTH_AND_CIPHER_INFORMATION0 cipherAuthInfo;

		memset(&cipherInfo, 0, sizeof(cipherInfo));
		cipherInfo.cipherTransform.cipherTransformId = GET_WIN32_ENC_ID(context->encrypt);
		cipherInfo.cipherKey = GET_CONTEXT_OPAQUE(context)->ck;
		
		memset(&cipherAuthInfo, 0, sizeof(cipherAuthInfo));
		cipherAuthInfo.saAuthInformation = authInfo;
		cipherAuthInfo.saCipherInformation = cipherInfo;

		sa.espAuthAndCipherInformation = &cipherAuthInfo;
	}
	
	memset(&bundle, 0, sizeof(bundle));
	bundle.numSAs = 1;
	bundle.saList = &sa;
	bundle.ipVersion = (context->ipv6) ? FWP_IP_VERSION_V6 : FWP_IP_VERSION_V4;
	bundle.lifetime.lifetimeSeconds = (context->SAs[direction].lifetime > WIN32_SA_MAX_LIFETIME) ? WIN32_SA_MAX_LIFETIME : context->SAs[direction].lifetime;

	/* inbound ? */
	if(direction == ipd_incoming_rep || direction == ipd_incoming_req){
		result = IPsecSaContextAddInbound0(engine, GET_SA_OPAQUE(context->SAs[direction])->saId, &bundle);
	}
	else{
		result = IPsecSaContextAddOutbound0(engine, GET_SA_OPAQUE(context->SAs[direction == ipd_outgoing_rep ? ipd_incoming_req : ipd_incoming_rep])->saId, &bundle);
		DEBUG_PRINT("saId=%d\n", GET_SA_OPAQUE(context->SAs[direction])->saId);
	}
	
	CLEANUP_ON_ERROR(result);

CLEANUP:
	return result;
}

/* get inbound spi */
DWORD getInboundSPI(ipsec_context_t* context, ipsec_direction_t direction)
{
#define CONDITIONS_COUNT 4
	DWORD result = NO_ERROR;
	UINT64 tmpInFilterId = 0, tmpOutFilterId = 0, tmpSaId = 0;
	FWPM_FILTER0 filter;
	IPSEC_TRAFFIC0 outTraffic;
	IPSEC_GETSPI0 getSpi;
	ipsec_sa_endpoints_t endpoint = get_endpoint(context);
	HANDLE engine = GET_CONTEXT_OPAQUE(context)->engine;
	FWPM_FILTER_CONDITION0 conds[CONDITIONS_COUNT];
	conds[0].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
	conds[0].matchType = FWP_MATCH_EQUAL;
	
	if(context->ipv6)
	{
		conds[0].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
		conds[0].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)endpoint.localV6Address;
	}
	else
	{
		conds[0].conditionValue.type = FWP_UINT32;
		conds[0].conditionValue.uint32 = endpoint.localV4Address;
	}
	conds[1].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
	conds[1].matchType = FWP_MATCH_EQUAL;
	if(context->ipv6)
	{
		conds[1].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
		conds[1].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)endpoint.remoteV6Address;
	}
	else
	{
		conds[1].conditionValue.type = FWP_UINT32;
		conds[1].conditionValue.uint32 = endpoint.remoteV4Address;
	}

	conds[2].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
	conds[2].matchType = FWP_MATCH_EQUAL;
	conds[2].conditionValue.type = FWP_UINT16;
	conds[2].conditionValue.uint16 = context->SAs[direction].port;

	conds[3].fieldKey = FWPM_CONDITION_IP_PROTOCOL;
	conds[3].matchType = FWP_MATCH_EQUAL;
	conds[3].conditionValue.type = FWP_UINT8;
	conds[3].conditionValue.uint16 = GET_WIN32_TRANS(context->trans);

	// Fill in the common fields shared by both filters.
	memset(&filter, 0, sizeof(filter));
	// For MUI compatibility, object names should be indirect strings. See
	// SHLoadIndirectString for details.
	filter.displayData.name = (PWSTR)TINYIPSEC_FILTER_NAME;
	// Link all objects to our provider. When multiple providers are installed
	// on a computer, this makes it easy to determine who added what.
	filter.providerKey = (GUID*)TINYIPSEC_PROVIDER_KEY;
	filter.numFilterConditions = CONDITIONS_COUNT;
	filter.filterCondition = conds;
	filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
	//filter.weight.type = FWP_EMPTY;

	// Add the inbound filter.
	filter.layerKey = (context->ipv6) ? FWPM_LAYER_INBOUND_TRANSPORT_V6 : FWPM_LAYER_INBOUND_TRANSPORT_V4;
	if(context->mode == ipm_tunnel)
	{
		filter.action.calloutKey = (context->ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V4;
	}
	else
	{
		filter.action.calloutKey = (context->ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V4;
	}
	result = FwpmFilterAdd0(engine, &filter, NULL, &tmpInFilterId);
	CLEANUP_ON_ERROR(result);

	// Add the outbound filter.
	filter.layerKey = (context->ipv6) ? FWPM_LAYER_OUTBOUND_TRANSPORT_V6 : FWPM_LAYER_OUTBOUND_TRANSPORT_V4;
	if(context->mode == ipm_tunnel)
	{
		filter.action.calloutKey = (context->ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V4;
	}
	else
	{
		filter.action.calloutKey = (context->ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V4;
	}
	result = FwpmFilterAdd0(engine, &filter, NULL, &tmpOutFilterId);
	CLEANUP_ON_ERROR(result);

	// Create the SA context using the outbound traffic descriptor.
	memset(&outTraffic, 0, sizeof(outTraffic));
	outTraffic.ipVersion = GET_WIN32_IPVER(context->ipv6);
	if(context->ipv6)
	{
		memcpy(outTraffic.localV6Address, endpoint.localV6Address, 16);
		memcpy(outTraffic.remoteV6Address, endpoint.remoteV6Address, 16);
	}
	else
	{
		outTraffic.localV4Address = endpoint.localV4Address;
		outTraffic.remoteV4Address = endpoint.remoteV4Address;
	}
	outTraffic.trafficType = GET_WIN32_MODE(context->mode);
	outTraffic.ipsecFilterId = tmpOutFilterId;
	result = IPsecSaContextCreate0(engine, &outTraffic, NULL, &tmpSaId);
	CLEANUP_ON_ERROR(result);
	
	// Get the inbound SPI using the inbound traffic descriptor.
	memset(&getSpi, 0, sizeof(getSpi));
	getSpi.inboundIpsecTraffic.ipVersion = GET_WIN32_IPVER(context->ipv6);
	if(context->ipv6)
	{
		memcpy(getSpi.inboundIpsecTraffic.localV6Address, endpoint.localV6Address, 16);
		memcpy(getSpi.inboundIpsecTraffic.remoteV6Address, endpoint.remoteV6Address, 16);
	}
	else
	{
		getSpi.inboundIpsecTraffic.localV4Address = endpoint.localV4Address;
		getSpi.inboundIpsecTraffic.remoteV4Address = endpoint.remoteV4Address;
	}
	getSpi.inboundIpsecTraffic.trafficType = GET_WIN32_MODE(context->mode);
	getSpi.inboundIpsecTraffic.ipsecFilterId = tmpInFilterId;
	getSpi.ipVersion = GET_WIN32_IPVER(context->ipv6);
	result = IPsecSaContextGetSpi0(engine, tmpSaId, &getSpi, &(context->SAs[direction].spi));
	CLEANUP_ON_ERROR(result);

	//// Return the various LUIDs to the caller, so he can clean up.
	GET_SA_OPAQUE(context->SAs[direction])->inFilterId = tmpInFilterId;
	GET_SA_OPAQUE(context->SAs[direction])->outFilterId = tmpOutFilterId;
	GET_SA_OPAQUE(context->SAs[direction])->saId = tmpSaId;
	DEBUG_PRINT("tmpSaId=%d\n", tmpSaId);
CLEANUP:
	if (result != NO_ERROR)
	{
		DeleteSaContextAndFilters(engine, tmpInFilterId, tmpOutFilterId, tmpSaId);
	}
	return result;
}

/* flush all associated ipsec SAs */
void ipsec_flush(const ipsec_context_t* context)
{
	HANDLE engine = NULL;
	UINT32 i;

	if(context && (engine = GET_CONTEXT_OPAQUE(context)->engine))
	{
		HANDLE enumHandle = NULL;
		IPSEC_SA_DETAILS0** entries = NULL;
		UINT32 numEntriesReturned = 0;

		DWORD result = IPsecSaCreateEnumHandle0(engine, NULL, &enumHandle);
		CLEANUP_ON_ERROR(result);
		result = IPsecSaEnum0(engine, enumHandle, TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST, &entries, &numEntriesReturned);
		CLEANUP_ON_ERROR(result);

		for(i = 0; i<numEntriesReturned; i++)
		{
			IPSEC_SA_DETAILS0* entry = (entries)[i];
			if( !wcscmp(entry->transportFilter->displayData.name, TINYIPSEC_FILTER_NAME))
			{
				result = FwpmFilterDeleteById0(engine, entry->transportFilter->filterId);
				CLEANUP_ON_ERROR(result);
			}
		}

		result = IPsecSaDestroyEnumHandle0(engine, enumHandle);
		CLEANUP_ON_ERROR(result);

	CLEANUP:
		FwpmFreeMemory0((void**)entries);
	}
}

/* Helper function to delete an SA context and the associated transport filters. */
void DeleteSaContextAndFilters(__in HANDLE engine, __in UINT64 inFilterId, __in UINT64 outFilterId, __in UINT64 saId)
{
   DWORD result;

   // Allow the LUIDs to be zero, so we can use this function to cleanup
   // partial results.
   if (saId != 0)
   {
      result = IPsecSaContextDeleteById0(engine, saId);
      if (result != ERROR_SUCCESS)
      {
         // There's not much we can do if delete fails, so continue trying to
         // clean up the remaining objects.
         DEBUG_PRINT("IPsecSaContextDeleteById0 = 0x%08X\n", result);
      }
   }
   if (outFilterId != 0)
   {
      result = FwpmFilterDeleteById0(engine, outFilterId);
      if (result != ERROR_SUCCESS)
      {
         DEBUG_PRINT("FwpmFilterDeleteById0 = 0x%08X\n", result);
      }
   }
   if (inFilterId != 0)
   {
      result = FwpmFilterDeleteById0(engine, inFilterId);
      if (result != ERROR_SUCCESS)
      {
         DEBUG_PRINT("FwpmFilterDeleteById0 = 0x%08X\n", result);
      }
   }
}

ipsec_sa_endpoints_t get_endpoint(const ipsec_context_t* context)
{
#define IPV4_ADDR_LEN	4
#define IPV6_ADDR_LEN	16

	ipsec_sa_endpoints_t endpoint;
	void *localBuff, *remoteBuff;

	if(!context->ipv6)
	{
		localBuff = malloc(IPV4_ADDR_LEN); memset(localBuff, NULL, IPV4_ADDR_LEN);
		remoteBuff = malloc(IPV4_ADDR_LEN); memset(remoteBuff, NULL, IPV4_ADDR_LEN);

		if ( inet_pton(AF_INET, context->localIP, localBuff) == 1 )
			endpoint.localV4Address = ntohl  ( *((ULONG*)localBuff) );

		if ( inet_pton(AF_INET, context->remoteIP, remoteBuff) == 1 )
			endpoint.remoteV4Address = ntohl  ( *((ULONG*)remoteBuff) );

		free(localBuff);
		free(remoteBuff);
	}
	else
	{
		localBuff = malloc(IPV6_ADDR_LEN); memset(localBuff, NULL, IPV6_ADDR_LEN);
		remoteBuff = malloc(IPV6_ADDR_LEN); memset(remoteBuff, NULL, IPV6_ADDR_LEN);

		if ( inet_pton(AF_INET6, context->localIP, localBuff) == 1 )
			memcpy(endpoint.localV6Address, localBuff, IPV6_ADDR_LEN);

		if ( inet_pton(AF_INET6, context->remoteIP, remoteBuff) == 1 )
			memcpy(endpoint.remoteV6Address, remoteBuff, IPV6_ADDR_LEN);

		free(localBuff);
		free(remoteBuff);
	}

	return endpoint;
}

#undef GET_CONTEXT_OPAQUE
#endif /* HAVE_WIN32_IPSEC */