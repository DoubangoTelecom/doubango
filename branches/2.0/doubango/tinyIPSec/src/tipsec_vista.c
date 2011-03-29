/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tipsec_vista.c
 * @brief Windows Vista/7 IPsec implementation using WFP.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tipsec_vista.h"

/**@defgroup tipsec_vista_group IPSec Vista/7 implementation (WFP).
* Supported algo: <b>hmac-md5-96</b> and <b>hmac-sha-1-96</b>.<br>
* Supported ealg: <b>des-ede3-cbc</b>, <b>aes-cbc</b> and <b>null</b>.<br>
* Supported mode: <b>transport</b> only.<br>
* Supported proto: <b>ah</b> and <b>esp</b>.<br>
* Supported IP proto: <b>tcp</b> and <b>udp</b>.<br>
* Supported IP version: <b>IPv4</b> and <b>IPv6</b>.
*/

#if HAVE_IPSEC_VISTA

#include "tsk_memory.h"
#include "tsk_debug.h"

#if defined(_MSC_VER)
#	pragma comment(lib, "Fwpuclnt.lib")
#endif

#include <ws2tcpip.h>
#include <Fwpmu.h>

typedef FWP_BYTE_BLOB* PFWP_BYTE_BLOB;

/**@ingroup tipsec_vista_group
* @def TINYIPSEC_FILTER_NAME
*/
#define TINYIPSEC_FILTER_NAME					TEXT("tinyIPSEC")
#define TINYIPSEC_PROVIDER_KEY					NULL

/**@ingroup tipsec_vista_group
* @def TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST
*/
/**@ingroup tipsec_vista_group
* @def TINYIPSEC_SA_MAX_LIFETIME
*/
#define TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST		20
#define TINYIPSEC_SA_MAX_LIFETIME				172799

#define TINYIPSEC_VISTA_GET_ALGO(algo) (algo ==  algo_hmac_md5_96) ? IPSEC_AUTH_TRANSFORM_ID_HMAC_MD5_96 : IPSEC_AUTH_TRANSFORM_ID_HMAC_SHA_1_96
#define TINYIPSEC_VISTA_GET_EALGO(ealg)	 (ealg == ealg_des_ede3_cbc) ? IPSEC_CIPHER_TRANSFORM_ID_CBC_3DES : ( (ealg == ealg_aes) ? IPSEC_CIPHER_TRANSFORM_ID_AES_128 : IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL )
#define TINYIPSEC_VISTA_GET_MODE(mode)	 (mode == mode_tun) ? IPSEC_TRAFFIC_TYPE_TUNNEL : IPSEC_TRAFFIC_TYPE_TRANSPORT
#define TINYIPSEC_VISTA_GET_IPPROTO(ipproto)	 (ipproto == ipproto_tcp) ? IPPROTO_TCP : IPPROTO_UDP
#define TINYIPSEC_VISTA_GET_IPVER(ipv6)	 (ipv6) ? FWP_IP_VERSION_V6 : FWP_IP_VERSION_V4
#define TINYIPSEC_VISTA_GET_PROTO(proto)	 (proto == proto_ah) ? IPSEC_TRANSFORM_AH : ( (proto == proto_esp) ? IPSEC_TRANSFORM_ESP_AUTH : IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER );

/* as WFP do not provide null encryption I define my own*/
static const IPSEC_CIPHER_TRANSFORM_ID0 IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL=
{
   (IPSEC_CIPHER_TYPE)NULL,
   (IPSEC_CIPHER_TYPE)NULL
};

/**@ingroup tipsec_vista_group
* IPSec context.
*/
typedef struct tipsec_context_vista_s
{
	TINYIPSEC_DECLARE_CONTEXT;
	
	UINT64 saId_us;
	UINT64 saId_uc;

	HANDLE engine;
}
tipsec_context_vista_t;

/**@ingroup tipsec_vista_group
*/
#define TIPSEC_CONTEXT_VISTA(ctx) ((tipsec_context_vista_t*)(ctx))

int tipsec_create_localSA(__in const tipsec_context_vista_t* context, __in tipsec_port_t local_port, __out tipsec_spi_t *spi, UINT64 *saId);
int tipsec_boundSA(__in const tipsec_context_vista_t* context, __in UINT64 local_saId, __in tipsec_spi_t remote_spi, __in BOOLEAN toInbound);

int tipsec_flush_all(const tipsec_context_vista_t* context);
void DeleteSaContextAndFilters(__in HANDLE engine, __in UINT64 inFilterId, __in UINT64 outFilterId, __in UINT64 saId);

/**@ingroup tipsec_vista_group
*/
int tipsec_start(tipsec_context_t* ctx)
{
	tipsec_context_vista_t* ctx_vista = TIPSEC_CONTEXT_VISTA(ctx);
	int ret = -1;

	if(!ctx_vista){
		ret = -1;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_vista)->started){
		TSK_DEBUG_WARN("The IPSec context already started.");
		ret = -2;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_vista)->state != state_full){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	/* VERY IMPORTANT: The SA context functions must be called in a specific order:
			(http://msdn.microsoft.com/en-us/library/bb540652(VS.85).aspx).
		
		IPsecSaContextCreate0 
		IPsecSaContextGetSpi0 
		IPsecSaContextAddInbound0 
		IPsecSaContextAddOutbound0
	*/

	/* US <- PC */
	if((ret = tipsec_boundSA(ctx_vista, ctx_vista->saId_us, TIPSEC_CONTEXT(ctx_vista)->spi_us, TRUE))){
		TSK_DEBUG_ERROR("Failed to setup [US <- PC] SA.");
		goto bail;
	}
	/* UC <- PS */
	if((ret = tipsec_boundSA(ctx_vista, ctx_vista->saId_uc, TIPSEC_CONTEXT(ctx_vista)->spi_uc, TRUE))){
		TSK_DEBUG_ERROR("Failed to setup [UC <- PS] SA.");
		goto bail;
	}

	/* UC -> PS */
	if((ret = tipsec_boundSA(ctx_vista, ctx_vista->saId_uc, TIPSEC_CONTEXT(ctx_vista)->spi_ps, FALSE))){
		TSK_DEBUG_ERROR("Failed to setup [UC -> PS] SA.");
		goto bail;
	}
	/* US -> PC */
	if((ret = tipsec_boundSA(ctx_vista, ctx_vista->saId_us, TIPSEC_CONTEXT(ctx_vista)->spi_pc, FALSE))){
		TSK_DEBUG_ERROR("Failed to setup [US -> PC] SA.");
		goto bail;
	}

	// Set the state to active.
	TIPSEC_CONTEXT(ctx_vista)->state = state_active;

	TIPSEC_CONTEXT(ctx_vista)->started = 1;

bail:
	return ret;
}

/**@ingroup tipsec_vista_group
*/
int tipsec_set_local(tipsec_context_t* ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	tipsec_context_vista_t* ctx_vista = TIPSEC_CONTEXT_VISTA(ctx);
	int ret;

	if(!ctx_vista){
		ret = -1;
		goto bail;
	}

	if(!addr_local || !port_uc || !port_us){
		ret = -2;
		goto bail;
	}

	if(!TIPSEC_CONTEXT(ctx_vista)->initialized){
		TSK_DEBUG_ERROR("IPSec engine not initialized.");
		ret = -3;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_vista)->state != state_initial){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -4;
		goto bail;
	}

	TSK_FREE(TIPSEC_CONTEXT(ctx_vista)->addr_local);
	TSK_FREE(TIPSEC_CONTEXT(ctx_vista)->addr_remote);

	/* Set local IP */
	if(TIPSEC_CONTEXT(ctx_vista)->use_ipv6){
		TIPSEC_CONTEXT(ctx_vista)->addr_local = tsk_calloc(16, sizeof(uint8_t));
		TIPSEC_CONTEXT(ctx_vista)->addr_remote = tsk_calloc(16, sizeof(uint8_t));

		if ((ret = inet_pton(AF_INET6, addr_local, TIPSEC_CONTEXT(ctx_vista)->addr_local)) != 1 ){
			TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_local, ret);
			goto bail;
		} else ret = 0;
		if ((ret = inet_pton(AF_INET6, addr_remote, TIPSEC_CONTEXT(ctx_vista)->addr_remote)) != 1 ){
			TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_remote, ret);
			goto bail;
		} else ret = 0;
	}
	else{
		TIPSEC_CONTEXT(ctx_vista)->addr_local = tsk_calloc(4, sizeof(uint8_t));
		TIPSEC_CONTEXT(ctx_vista)->addr_remote = tsk_calloc(4, sizeof(uint8_t));

		if ((ret = inet_pton(AF_INET, addr_local, TIPSEC_CONTEXT(ctx_vista)->addr_local)) != 1 ){
			TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_local, ret);
			goto bail;
		} 
		else {
			*((UINT32*)TIPSEC_CONTEXT(ctx_vista)->addr_local) = ntohl(*((UINT32*)TIPSEC_CONTEXT(ctx_vista)->addr_local));
			ret = 0;
		}
		if ((ret = inet_pton(AF_INET, addr_remote, TIPSEC_CONTEXT(ctx_vista)->addr_remote)) != 1 ){
			TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_remote, ret);
			goto bail;
		} 
		else{
			*((UINT32*)TIPSEC_CONTEXT(ctx_vista)->addr_remote) = ntohl(*((UINT32*)TIPSEC_CONTEXT(ctx_vista)->addr_remote));
			ret = 0;
		}
	}

	/* Set ports */
	TIPSEC_CONTEXT(ctx_vista)->port_uc = port_uc;
	TIPSEC_CONTEXT(ctx_vista)->port_us = port_us;
	
	if((ret = tipsec_create_localSA(ctx_vista, TIPSEC_CONTEXT(ctx_vista)->port_uc, &TIPSEC_CONTEXT(ctx_vista)->spi_uc, &ctx_vista->saId_uc))){
		goto bail;
	}

	if((ret = tipsec_create_localSA(ctx_vista, TIPSEC_CONTEXT(ctx_vista)->port_us, &TIPSEC_CONTEXT(ctx_vista)->spi_us, &ctx_vista->saId_us))){
		goto bail;
	}

	TIPSEC_CONTEXT(ctx_vista)->state = state_inbound;

bail:

	return ret;
}

/**@ingroup tipsec_vista_group
*/
int tipsec_set_keys(tipsec_context_t* ctx, const tipsec_key_t* ik, const tipsec_key_t* ck)
{
	if(!ctx || !ik || !ck){
		return -1;
	}

	/* Compute ik and ck */
	TIPSEC_CONTEXT(ctx)->ik = tsk_calloc(1, sizeof(FWP_BYTE_BLOB));
	TIPSEC_CONTEXT(ctx)->ck = tsk_calloc(1, sizeof(FWP_BYTE_BLOB));

	((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ik)->data = tsk_calloc(1, TIPSEC_IK_LEN);
	memcpy(((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ik)->data, ik, TIPSEC_KEY_LEN);
	((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ik)->size = TIPSEC_KEY_LEN;

	((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ck)->data = tsk_calloc(1, TIPSEC_CK_LEN);
	memcpy(((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ck)->data, ck, TIPSEC_KEY_LEN);
	((PFWP_BYTE_BLOB)TIPSEC_CONTEXT(ctx)->ck)->size = TIPSEC_KEY_LEN;

	return 0;
}

/**@ingroup tipsec_vista_group
*/
int tipsec_set_remote(tipsec_context_t* ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	tipsec_context_vista_t* ctx_vista = TIPSEC_CONTEXT_VISTA(ctx);
	int ret = -1;

	if(!ctx_vista){
		ret = -1;
		goto bail;
	}

	if(!lifetime || !port_pc || !port_ps){
		ret = -2;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_vista)->state != state_inbound){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	/* Set Lifetime */
	TIPSEC_CONTEXT(ctx_vista)->lifetime = lifetime;

	/* Set ports */
	TIPSEC_CONTEXT(ctx_vista)->port_ps = port_ps;
	TIPSEC_CONTEXT(ctx_vista)->port_pc = port_pc;

	/* Set spis */
	TIPSEC_CONTEXT(ctx_vista)->spi_ps = spi_ps;
	TIPSEC_CONTEXT(ctx_vista)->spi_pc = spi_pc;

	TIPSEC_CONTEXT(ctx_vista)->state = state_full;

	ret = 0;

bail:
	return ret;
}

/**@ingroup tipsec_vista_group
*/
int tipsec_stop(tipsec_context_t* ctx)
{
	tipsec_context_vista_t* ctx_vista = TIPSEC_CONTEXT_VISTA(ctx);
	int ret = -1;

	if(!ctx_vista){
		ret = -1;
		goto bail;
	}

	if(!TIPSEC_CONTEXT(ctx_vista)->started){
		TSK_DEBUG_WARN("The IPSec context not started.");
		ret = -2;
		goto bail;
	}

	/* Flush (delete) all SAs associated to tinyIPSEC */
	tipsec_flush_all(ctx_vista);

	TIPSEC_CONTEXT(ctx_vista)->started = 0;
	TIPSEC_CONTEXT(ctx_vista)->state = state_initial;

bail:
	return ret;
}

int tipsec_create_localSA(__in const tipsec_context_vista_t* context, __in tipsec_port_t local_port, __out tipsec_spi_t *spi, UINT64 *saId)
{
	DWORD result = NO_ERROR;
	UINT64 tmpInFilterId = 0, tmpOutFilterId = 0, tmpSaId = 0;
	FWPM_FILTER0 filter;
	IPSEC_TRAFFIC0 outTraffic;
	IPSEC_GETSPI0 getSpi;
	int ret = -1;
	FWPM_FILTER_CONDITION0 conds[4];
	conds[0].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
	conds[0].matchType = FWP_MATCH_EQUAL;

	*spi = 0;
	*saId = 0;

	if(TIPSEC_CONTEXT(context)->use_ipv6){
		conds[0].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
		conds[0].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)TIPSEC_CONTEXT(context)->addr_local;
	}
	else{
		conds[0].conditionValue.type = FWP_UINT32;
		conds[0].conditionValue.uint32 = *((UINT32*)TIPSEC_CONTEXT(context)->addr_local);
	}
	conds[1].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
	conds[1].matchType = FWP_MATCH_EQUAL;
	if(TIPSEC_CONTEXT(context)->use_ipv6){
		conds[1].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
		conds[1].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)TIPSEC_CONTEXT(context)->addr_remote;
	}
	else{
		conds[1].conditionValue.type = FWP_UINT32;
		conds[1].conditionValue.uint32 = *((UINT32*)TIPSEC_CONTEXT(context)->addr_remote);
	}

	conds[2].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
	conds[2].matchType = FWP_MATCH_EQUAL;
	conds[2].conditionValue.type = FWP_UINT16;
	conds[2].conditionValue.uint16 = local_port;

	conds[3].fieldKey = FWPM_CONDITION_IP_PROTOCOL;
	conds[3].matchType = FWP_MATCH_EQUAL;
	conds[3].conditionValue.type = FWP_UINT8;
	conds[3].conditionValue.uint16 = TINYIPSEC_VISTA_GET_IPPROTO(TIPSEC_CONTEXT(context)->ipproto);

	// Fill in the common fields shared by both filters.
	memset(&filter, 0, sizeof(filter));
	// For MUI compatibility, object names should be indirect strings. See
	// SHLoadIndirectString for details.
	filter.displayData.name = (PWSTR)TINYIPSEC_FILTER_NAME;
	// Link all objects to our provider. When multiple providers are installed
	// on a computer, this makes it easy to determine who added what.
	filter.providerKey = (GUID*)TINYIPSEC_PROVIDER_KEY;
	filter.numFilterConditions = 4;
	filter.filterCondition = conds;
	filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
	filter.flags = FWPM_FILTER_FLAG_NONE;
	//filter.weight.type = FWP_EMPTY;

	// Add the inbound filter.
	filter.layerKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_LAYER_INBOUND_TRANSPORT_V6 : FWPM_LAYER_INBOUND_TRANSPORT_V4;
	if(TIPSEC_CONTEXT(context)->mode == mode_tun){
		filter.action.calloutKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V4;
	}
	else{
		filter.action.calloutKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V4;
	}
	if((result = FwpmFilterAdd0(context->engine, &filter, NULL, &tmpInFilterId)) != ERROR_SUCCESS){
		TSK_DEBUG_ERROR("FwpmFilterAdd0 (inbound) failed with error code [%x]", result);
		goto CLEANUP;
	}

	// Add the outbound filter.
	filter.layerKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_LAYER_OUTBOUND_TRANSPORT_V6 : FWPM_LAYER_OUTBOUND_TRANSPORT_V4;
	if(TIPSEC_CONTEXT(context)->mode == mode_tun){
		filter.action.calloutKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V4;
	}
	else{
		filter.action.calloutKey = (TIPSEC_CONTEXT(context)->use_ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V4;
	}
	if((result = FwpmFilterAdd0(context->engine, &filter, NULL, &tmpOutFilterId)) != ERROR_SUCCESS){
		TSK_DEBUG_ERROR("FwpmFilterAdd0(outbound) failed with error code [%x]", result);
		goto CLEANUP;
	}

	// Create the SA context using the outbound traffic descriptor.
	memset(&outTraffic, 0, sizeof(outTraffic));
	outTraffic.ipVersion = TINYIPSEC_VISTA_GET_IPVER(TIPSEC_CONTEXT(context)->use_ipv6);
	if(TIPSEC_CONTEXT(context)->use_ipv6){
		memcpy(outTraffic.localV6Address, TIPSEC_CONTEXT(context)->addr_local, 16);
		memcpy(outTraffic.remoteV6Address, TIPSEC_CONTEXT(context)->addr_remote, 16);
	}
	else{
		outTraffic.localV4Address = *((ULONG*)TIPSEC_CONTEXT(context)->addr_local);
		outTraffic.remoteV4Address = *((ULONG*)TIPSEC_CONTEXT(context)->addr_remote);
	}
	outTraffic.trafficType = TINYIPSEC_VISTA_GET_MODE(TIPSEC_CONTEXT(context)->mode);
	outTraffic.ipsecFilterId = tmpOutFilterId;
	if((result = IPsecSaContextCreate0(context->engine, &outTraffic, NULL, &tmpSaId)) != ERROR_SUCCESS){
		TSK_DEBUG_ERROR("IPsecSaContextCreate0 failed with error code [%x]", result);
		goto CLEANUP;
	}
	
	// Get the inbound SPI using the inbound traffic descriptor.
	memset(&getSpi, 0, sizeof(getSpi));
	getSpi.inboundIpsecTraffic.ipVersion = TINYIPSEC_VISTA_GET_IPVER(TIPSEC_CONTEXT(context)->use_ipv6);
	if(TIPSEC_CONTEXT(context)->use_ipv6){
		memcpy(getSpi.inboundIpsecTraffic.localV6Address, TIPSEC_CONTEXT(context)->addr_local, 16);
		memcpy(getSpi.inboundIpsecTraffic.remoteV6Address, TIPSEC_CONTEXT(context)->addr_remote, 16);
	}
	else{
		getSpi.inboundIpsecTraffic.localV4Address = *((ULONG*)TIPSEC_CONTEXT(context)->addr_local);
		getSpi.inboundIpsecTraffic.remoteV4Address = *((ULONG*)TIPSEC_CONTEXT(context)->addr_remote);
	}
	getSpi.inboundIpsecTraffic.trafficType = TINYIPSEC_VISTA_GET_MODE(TIPSEC_CONTEXT(context)->mode);
	getSpi.inboundIpsecTraffic.ipsecFilterId = tmpInFilterId;
	getSpi.ipVersion = TINYIPSEC_VISTA_GET_IPVER(TIPSEC_CONTEXT(context)->use_ipv6);
	if((result = IPsecSaContextGetSpi0(context->engine, tmpSaId, &getSpi, spi))){
		TSK_DEBUG_ERROR("IPsecSaContextGetSpi0 failed with error code [%x]", result);
		goto CLEANUP;
	}

	//// Return the various LUIDs to the caller, so he can clean up.
	//*inFilterId = tmpInFilterId;
	//*outFilterId = tmpOutFilterId;
	*saId = tmpSaId;
	
CLEANUP:
	if (result != NO_ERROR){
		DeleteSaContextAndFilters(context->engine, tmpInFilterId, tmpOutFilterId, tmpSaId);
	}else ret = 0;
	
	return ret;
}

int tipsec_boundSA(__in const tipsec_context_vista_t* context, __in UINT64 local_saId, __in tipsec_spi_t remote_spi, __in BOOLEAN toInbound)
{
	UINT32 i=0, j=0;
	DWORD result = NO_ERROR;
	IPSEC_SA0 sa;
	IPSEC_SA_BUNDLE0 bundle;
	IPSEC_SA_AUTH_INFORMATION0 authInfo;
	PFWP_BYTE_BLOB ik = (PFWP_BYTE_BLOB)TIPSEC_CONTEXT(context)->ik;
	PFWP_BYTE_BLOB ck = (PFWP_BYTE_BLOB)TIPSEC_CONTEXT(context)->ck;
	
	memset(&sa, 0, sizeof(sa));
	sa.spi = remote_spi;
	sa.saTransformType = TINYIPSEC_VISTA_GET_PROTO(TIPSEC_CONTEXT(context)->protocol);


	//
	//	Keys padding
	//
	if(TIPSEC_CONTEXT(context)->alg == algo_hmac_sha_1_96){
		if(ik->size < TIPSEC_IK_LEN){
			for(i = ik->size; i<TIPSEC_KEY_LEN; i++){
				ik->data[i] = 0x00; /* Already done by "tsk_calloc" but ... */
			}
			ik->size = TIPSEC_IK_LEN;
		}
	}
	if(TIPSEC_CONTEXT(context)->ealg == ealg_des_ede3_cbc){
		if(ck->size < TIPSEC_CK_LEN){
			for(i = ck->size; i<TIPSEC_CK_LEN; i++){
				ck->data[i] = ck->data[j++];
			}
			ck->size = TIPSEC_CK_LEN;
		}
	}

	//
	//	In all case create Authentication info
	//
	memset(&authInfo, 0, sizeof(authInfo));
	authInfo.authTransform.authTransformId = TINYIPSEC_VISTA_GET_ALGO(TIPSEC_CONTEXT(context)->alg);
	authInfo.authKey = *ik;

	if( sa.saTransformType == IPSEC_TRANSFORM_AH ){
		sa.ahInformation = &authInfo;
	}
	else if( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH ){
		sa.espAuthInformation = &authInfo;
	}
	else if( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER ){
		IPSEC_SA_CIPHER_INFORMATION0 cipherInfo;
		IPSEC_SA_AUTH_AND_CIPHER_INFORMATION0 cipherAuthInfo;

		memset(&cipherInfo, 0, sizeof(cipherInfo));
		cipherInfo.cipherTransform.cipherTransformId = TINYIPSEC_VISTA_GET_EALGO(TIPSEC_CONTEXT(context)->ealg);
		cipherInfo.cipherKey = *ck;
		
		memset(&cipherAuthInfo, 0, sizeof(cipherAuthInfo));
		cipherAuthInfo.saAuthInformation = authInfo;
		cipherAuthInfo.saCipherInformation = cipherInfo;

		sa.espAuthAndCipherInformation = &cipherAuthInfo;
	}
	
	memset(&bundle, 0, sizeof(bundle));
	bundle.numSAs = 1;
	bundle.saList = &sa;
	bundle.ipVersion = TINYIPSEC_VISTA_GET_IPVER(TIPSEC_CONTEXT(context)->use_ipv6);
	bundle.lifetime.lifetimeSeconds = (UINT32)((TIPSEC_CONTEXT(context)->lifetime > TINYIPSEC_SA_MAX_LIFETIME) ? TINYIPSEC_SA_MAX_LIFETIME : TIPSEC_CONTEXT(context)->lifetime);

	/* From remote to local (inbound) ? */
	if(toInbound){
		if((result = IPsecSaContextAddInbound0(context->engine, local_saId, &bundle)) != ERROR_SUCCESS){
			TSK_DEBUG_ERROR("IPsecSaContextAddInbound0 failed with error code [%x]", result);
			goto CLEANUP;
		}
	}
	else{
		if((result = IPsecSaContextAddOutbound0(context->engine, local_saId, &bundle)) != ERROR_SUCCESS){
			TSK_DEBUG_ERROR("IPsecSaContextAddOutbound0 failed with error code [%x]", result);
			goto CLEANUP;
		}
	}	

CLEANUP:
	return (result == ERROR_SUCCESS) ? 0 : -1;
}


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
         TSK_DEBUG_ERROR("IPsecSaContextDeleteById0 = 0x%08X\n", result);
      }
   }
   if (outFilterId != 0)
   {
      result = FwpmFilterDeleteById0(engine, outFilterId);
      if (result != ERROR_SUCCESS)
      {
         TSK_DEBUG_ERROR("FwpmFilterDeleteById0 = 0x%08X\n", result);
      }
   }
   if (inFilterId != 0)
   {
      result = FwpmFilterDeleteById0(engine, inFilterId);
      if (result != ERROR_SUCCESS)
      {
         TSK_DEBUG_ERROR("FwpmFilterDeleteById0 = 0x%08X\n", result);
      }
   }
}

int tipsec_flush_all(const tipsec_context_vista_t* context)
{
	UINT32 i;
	int ret = -1;

	if(context)
	{
		HANDLE enumHandle = NULL;
		IPSEC_SA_DETAILS0** entries = NULL;
		UINT32 numEntriesReturned = 0;
		DWORD result;

		if((result = IPsecSaCreateEnumHandle0(context->engine, NULL, &enumHandle)) != ERROR_SUCCESS){
			TSK_DEBUG_ERROR("IPsecSaCreateEnumHandle0 failed with error code [%x].", result);
			goto CLEANUP;
		}
		
		if((result = IPsecSaEnum0(context->engine, enumHandle, TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST, &entries, &numEntriesReturned)) != ERROR_SUCCESS){
			TSK_DEBUG_ERROR("IPsecSaEnum0 failed with error code [%x].", result);
			goto CLEANUP;
		}

		for(i = 0; i<numEntriesReturned; i++)
		{
			IPSEC_SA_DETAILS0* entry = (entries)[i];
			if( !wcscmp(entry->transportFilter->displayData.name, TINYIPSEC_FILTER_NAME))
			{
				if((result = FwpmFilterDeleteById0(context->engine, entry->transportFilter->filterId)) != ERROR_SUCCESS){
					TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x].", result);
					goto CLEANUP;
				}
			}
		}

		if((result = IPsecSaDestroyEnumHandle0(context->engine, enumHandle)) != ERROR_SUCCESS){
			TSK_DEBUG_ERROR("IPsecSaDestroyEnumHandle0 failed with error code [%x].", result);
			goto CLEANUP;
		}
		
		TSK_DEBUG_INFO("All SAs have been flushed.");
		ret = 0;

	CLEANUP:
		FwpmFreeMemory0((void**)entries);
	}

	return ret;
}

//=================================================================================================
//	IPSec context object definition
//
static tsk_object_t* tipsec_context_ctor(tsk_object_t * self, va_list * app)
{
	tipsec_context_vista_t *context = self;
	if(context){
		DWORD code;
	
		TIPSEC_CONTEXT(context)->ipproto = va_arg(*app, tipsec_ipproto_t);
		TIPSEC_CONTEXT(context)->use_ipv6 = va_arg(*app, int);
		TIPSEC_CONTEXT(context)->mode = va_arg(*app, tipsec_mode_t);
		TIPSEC_CONTEXT(context)->ealg = va_arg(*app, tipsec_ealgorithm_t);
		TIPSEC_CONTEXT(context)->alg = va_arg(*app, tipsec_algorithm_t);
		TIPSEC_CONTEXT(context)->protocol = va_arg(*app, tipsec_protocol_t);

		/* Open engine */
		if((code = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &context->engine))){
			TIPSEC_CONTEXT(context)->initialized = tsk_false;
			TSK_DEBUG_ERROR("FwpmEngineOpen0 failed with error code [%x].", code);
		}
		else{
			TIPSEC_CONTEXT(context)->initialized = tsk_true;
		}
		
		TIPSEC_CONTEXT(context)->state = state_initial;
	}
	return self;
}

static tsk_object_t* tipsec_context_dtor(tsk_object_t * self)
{ 
	tipsec_context_vista_t *context = self;
	if(context)
	{
		DWORD code;

		if(TIPSEC_CONTEXT(context)->started){
			tipsec_stop(TIPSEC_CONTEXT(context));
		}
		
		/* Close engine */
		if((code = FwpmEngineClose0(context->engine))){
			TSK_DEBUG_ERROR("FwpmEngineClose0 failed with error code [%x].", code);
		}

		TSK_FREE(TIPSEC_CONTEXT(context)->addr_local);
		TSK_FREE(TIPSEC_CONTEXT(context)->addr_remote);
		
		TSK_FREE(TIPSEC_CONTEXT(context)->ik);
		TSK_FREE(TIPSEC_CONTEXT(context)->ck);
	}

	return self;
}

static int tipsec_context_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return-1;
}

static const tsk_object_def_t tipsec_context_def_s = 
{
	sizeof(tipsec_context_vista_t),
	tipsec_context_ctor, 
	tipsec_context_dtor,
	tipsec_context_cmp, 
};
const void *tipsec_context_def_t = &tipsec_context_def_s;


#endif /* HAVE_IPSEC_VISTA */
