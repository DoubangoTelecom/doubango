/* Copyright (C) 2013-2014 Mamadou DIOP
* Copyright (C) 2013-2014 Doubango Telecom <http://www.doubango.org>
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
*/
#include "plugin_win_ipsec_vista_config.h"

#include "tipsec.h" /* From tinyIPSec project. Requires linking against "tinyIPSec.lib" */

#include "tsk_memory.h"
#include "tsk_object.h"
#include "tsk_debug.h"

#include <ws2tcpip.h>
#include <Fwpmu.h>
#include <Rpc.h>

#if defined(_MSC_VER)
#	pragma comment(lib, "Fwpuclnt.lib")
#	pragma comment(lib, "Rpcrt4.lib")
#endif

typedef FWP_BYTE_BLOB* PFWP_BYTE_BLOB;

/* as WFP do not provide null encryption I define my own*/
static const IPSEC_CIPHER_TRANSFORM_ID0 IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL= {
    (IPSEC_CIPHER_TYPE)NULL,
    (IPSEC_CIPHER_TYPE)NULL
};

#define TINYIPSEC_FILTER_NAME					TEXT("Doubango Telecom tinyIPSec (Windows Vista)")
#define TINYIPSEC_PROVIDER_KEY					NULL

#define TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST		INT_MAX
#define TINYIPSEC_SA_MAX_LIFETIME				172799

#define TINYIPSEC_VISTA_GET_ALGO(algo) (algo ==  tipsec_alg_hmac_md5_96) ? IPSEC_AUTH_TRANSFORM_ID_HMAC_MD5_96 : IPSEC_AUTH_TRANSFORM_ID_HMAC_SHA_1_96
#define TINYIPSEC_VISTA_GET_EALGO(ealg)	 (ealg == tipsec_ealg_des_ede3_cbc) ? IPSEC_CIPHER_TRANSFORM_ID_CBC_3DES : ( (ealg == tipsec_ealg_aes) ? IPSEC_CIPHER_TRANSFORM_ID_AES_128 : IPSEC_CIPHER_TRANSFORM_ID_NULL_NULL )
#define TINYIPSEC_VISTA_GET_MODE(mode)	 (mode == tipsec_mode_tun) ? IPSEC_TRAFFIC_TYPE_TUNNEL : IPSEC_TRAFFIC_TYPE_TRANSPORT
#define TINYIPSEC_VISTA_GET_IPPROTO(ipproto)	 (ipproto == tipsec_ipproto_tcp) ? IPPROTO_TCP : ((ipproto == tipsec_ipproto_icmp) ? IPPROTO_ICMP : IPPROTO_UDP)
#define TINYIPSEC_VISTA_GET_IPVER(ipv6)	 (ipv6) ? FWP_IP_VERSION_V6 : FWP_IP_VERSION_V4
#define TINYIPSEC_VISTA_GET_PROTO(proto, ealg)	 (proto == tipsec_proto_ah) ? IPSEC_TRANSFORM_AH : ( (proto == tipsec_proto_esp) ? (ealg == tipsec_ealg_null ? IPSEC_TRANSFORM_ESP_AUTH : IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER) : IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER );

typedef struct plugin_win_ipsec_vista_ctx_s {
    TIPSEC_DECLARE_CTX;

    tipsec_ctx_t* pc_base;
    UINT64 saId_us;
    UINT64 saId_uc;
    UINT64 filterId_in_us;
    UINT64 filterId_out_us;
    UINT64 filterId_in_uc;
    UINT64 filterId_out_uc;
    WCHAR filter_name[256];

    HANDLE engine;
}
plugin_win_ipsec_vista_ctx_t;

static int _vista_createLocalSA(__in const plugin_win_ipsec_vista_ctx_t* p_ctx, __in tipsec_port_t local_port, __out tipsec_spi_t *spi, __out UINT64 *saId, __out UINT64 *filterId_in, __out UINT64 *filterId_out);
static int _vista_boundSA(__in const plugin_win_ipsec_vista_ctx_t* p_ctx, __in UINT64 local_saId, __in tipsec_spi_t remote_spi, __in BOOLEAN toInbound);

static int _vista_flushAll(const plugin_win_ipsec_vista_ctx_t* p_ctx);
static void _vista_deleteSaContextAndFilters(__in HANDLE engine, __in UINT64 inFilterId, __in UINT64 outFilterId, __in UINT64 saId);

//
//	Plugin implementation
//

static tipsec_error_t _plugin_win_ipsec_vista_ctx_init(tipsec_ctx_t* _p_ctx)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;
    DWORD code;
    UUID uuid;
    RPC_STATUS status;
    static uint64_t __guard = 0;

    if (p_ctx->pc_base->initialized) {
        TSK_DEBUG_ERROR("Already initialized");
        return tipsec_error_invalid_state;
    }

    /* Create filter name */
    status = UuidCreate(&uuid);
    if (status == RPC_S_OK) {
        WCHAR* wszUuid = NULL;
        UuidToStringW(&uuid, (RPC_WSTR*)&wszUuid);
        if (!wszUuid) {
            TSK_DEBUG_ERROR("Failed to convert the UUID");
            return tipsec_error_sys;
        }
        swprintf(p_ctx->filter_name, sizeof(p_ctx->filter_name)/sizeof(p_ctx->filter_name[0]), L"%s//%s//%llu", TINYIPSEC_FILTER_NAME, wszUuid, __guard++);
        RpcStringFree((RPC_WSTR*)&wszUuid);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new UUID");
        return tipsec_error_sys;
    }



    /* Open engine */
    if ((code = FwpmEngineOpen0(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &p_ctx->engine))) {
        p_ctx->pc_base->initialized = tsk_false;
        TSK_DEBUG_ERROR("FwpmEngineOpen0 failed with error code [%x].", code);
        return tipsec_error_sys;
    }
    else {
        p_ctx->pc_base->initialized = tsk_true;
        p_ctx->pc_base->state = tipsec_state_initial;
        return tipsec_error_success;
    }
}

static tipsec_error_t _plugin_win_ipsec_vista_ctx_set_local(tipsec_ctx_t* _p_ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;
    int ret;

    _p_ctx->addr_local = tsk_realloc(_p_ctx->addr_local, _p_ctx->use_ipv6 ? 16 : 4);
    if (!_p_ctx->addr_local) {
        return tipsec_error_outofmemory;
    }
    _p_ctx->addr_remote = tsk_realloc(_p_ctx->addr_remote, _p_ctx->use_ipv6 ? 16 : 4);
    if (!_p_ctx->addr_remote) {
        return tipsec_error_outofmemory;
    }

    /* Set local IP */
    if (_p_ctx->use_ipv6) {
        if ((ret = inet_pton(AF_INET6, addr_local, _p_ctx->addr_local)) != 1 ) {
            TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_local, ret);
            return tipsec_error_sys;
        }
        if ((ret = inet_pton(AF_INET6, addr_remote, _p_ctx->addr_remote)) != 1 ) {
            TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_remote, ret);
            return tipsec_error_sys;
        }
    }
    else {
        if ((ret = inet_pton(AF_INET, addr_local, _p_ctx->addr_local)) != 1 ) {
            TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_local, ret);
            return tipsec_error_sys;
        }
        else {
            *((UINT32*)_p_ctx->addr_local) = ntohl(*((UINT32*)_p_ctx->addr_local));
        }
        if ((ret = inet_pton(AF_INET, addr_remote, _p_ctx->addr_remote)) != 1 ) {
            TSK_DEBUG_ERROR("inet_pton(%s) have failed with error code [%x].", addr_remote, ret);
            return tipsec_error_sys;
        }
        else {
            *((UINT32*)_p_ctx->addr_remote) = ntohl(*((UINT32*)_p_ctx->addr_remote));
        }
    }

    /* Set ports */
    _p_ctx->port_uc = port_uc;
    _p_ctx->port_us = port_us;

    // Create SA1: (UC -> PS)
    if ((ret = _vista_createLocalSA(p_ctx, _p_ctx->port_uc, &_p_ctx->spi_uc, &p_ctx->saId_uc, &p_ctx->filterId_in_uc, &p_ctx->filterId_out_uc))) {
        return tipsec_error_sys;
    }

    // Create SA2: (US <-PC)
    if ((ret = _vista_createLocalSA(p_ctx, _p_ctx->port_us, &_p_ctx->spi_us, &p_ctx->saId_us, &p_ctx->filterId_in_us, &p_ctx->filterId_out_uc))) {
        return tipsec_error_sys;
    }

    _p_ctx->state = tipsec_state_inbound;

    return tipsec_error_success;
}

static tipsec_error_t _plugin_win_ipsec_vista_ctx_set_remote(tipsec_ctx_t* _p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;

    /* Set Lifetime */
    _p_ctx->lifetime = lifetime;

    /* Set ports */
    _p_ctx->port_ps = port_ps;
    _p_ctx->port_pc = port_pc;

    /* Set SPIs */
    _p_ctx->spi_ps = spi_ps;
    _p_ctx->spi_pc = spi_pc;

    _p_ctx->state = tipsec_state_full;

    return tipsec_error_success;
}

static tipsec_error_t _plugin_win_ipsec_vista_ctx_set_keys(tipsec_ctx_t* _p_ctx, const tipsec_key_t* ik, const tipsec_key_t* ck)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;
    PFWP_BYTE_BLOB _ik, _ck;

    /* Compute ik and ck */
    _p_ctx->ik = tsk_realloc(_p_ctx->ik, sizeof(FWP_BYTE_BLOB));
    if (!_p_ctx->ik) {
        return tipsec_error_outofmemory;
    }
    _ik = (PFWP_BYTE_BLOB)_p_ctx->ik;
    _p_ctx->ck = tsk_realloc(_p_ctx->ck, sizeof(FWP_BYTE_BLOB));
    if (!_p_ctx->ck) {
        return tipsec_error_outofmemory;
    }
    _ck = (PFWP_BYTE_BLOB)_p_ctx->ck;

    _ik->data = tsk_calloc(TIPSEC_IK_LEN, 1);
    if (!_ik->data) {
        return tipsec_error_outofmemory;
    }
    memcpy(_ik->data, ik, TIPSEC_KEY_LEN);
    _ik->size = TIPSEC_KEY_LEN;

    _ck->data = tsk_calloc(TIPSEC_CK_LEN, 1);
    if (!_ck->data) {
        return tipsec_error_outofmemory;
    }
    memcpy(_ck->data, ck, TIPSEC_KEY_LEN);
    _ck->size = TIPSEC_KEY_LEN;

    return tipsec_error_success;
}

static tipsec_error_t _plugin_win_ipsec_vista_ctx_start(tipsec_ctx_t* _p_ctx)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;
    int ret;

    /* VERY IMPORTANT: The SA context functions must be called in a specific order:
    		(http://msdn.microsoft.com/en-us/library/bb540652(VS.85).aspx).

    	IPsecSaContextCreate0
    	IPsecSaContextGetSpi0
    	IPsecSaContextAddInbound0
    	IPsecSaContextAddOutbound0
    */

    /* US <- PC */
    if ((ret = _vista_boundSA(p_ctx, p_ctx->saId_us, _p_ctx->spi_us, TRUE))) {
        TSK_DEBUG_ERROR("Failed to setup [US <- PC] SA. Error code = %d", ret);
        return tipsec_error_sys;
    }
    /* UC <- PS */
    if ((ret = _vista_boundSA(p_ctx, p_ctx->saId_uc, _p_ctx->spi_uc, TRUE))) {
        TSK_DEBUG_ERROR("Failed to setup [UC <- PS] SA. Error code = %d", ret);
        return tipsec_error_sys;
    }

    /* UC -> PS */
    if ((ret = _vista_boundSA(p_ctx, p_ctx->saId_uc, _p_ctx->spi_ps, FALSE))) {
        TSK_DEBUG_ERROR("Failed to setup [UC -> PS] SA. Error code = %d", ret);
        return tipsec_error_sys;
    }
    /* US -> PC */
    if ((ret = _vista_boundSA(p_ctx, p_ctx->saId_us, _p_ctx->spi_pc, FALSE))) {
        TSK_DEBUG_ERROR("Failed to setup [US -> PC] SA. Error code = %d", ret);
        return tipsec_error_sys;
    }

    _p_ctx->state = tipsec_state_active;
    _p_ctx->started = 1;

    return tipsec_error_success;
}

static tipsec_error_t _plugin_win_ipsec_vista_ctx_stop(tipsec_ctx_t* _p_ctx)
{
    plugin_win_ipsec_vista_ctx_t* p_ctx = (plugin_win_ipsec_vista_ctx_t*)_p_ctx;
    tipsec_error_t err = tipsec_error_success;

    //if (!_p_ctx->started) {
    //    return tipsec_error_success;
    //}

    /* Flush (delete) all SAs associated to tinyIPSEC */
    _vista_flushAll(p_ctx);

    _p_ctx->started = 0;
    _p_ctx->state = tipsec_state_initial;

    return tipsec_error_success;
}

//
//	Private functions
//
static int _vista_createLocalSA(__in const plugin_win_ipsec_vista_ctx_t* p_ctx, __in tipsec_port_t local_port, __out tipsec_spi_t *spi, __out UINT64 *saId, __out UINT64 *filterId_in, __out UINT64 *filterId_out)
{
    DWORD result = NO_ERROR;
    UINT64 tmpInFilterId = 0, tmpOutFilterId = 0, tmpSaId = 0;
    FWPM_FILTER0 filter;
    IPSEC_TRAFFIC0 outTraffic;
    IPSEC_GETSPI0 getSpi;
    int ret = -1;
    FWPM_FILTER_CONDITION0 conds[6];
    UINT32 numFilterConditions = 3;

    *spi = 0;
    *saId = 0;
    *filterId_in = 0;
    *filterId_out = 0;

    conds[0].fieldKey = FWPM_CONDITION_IP_LOCAL_ADDRESS;
    conds[0].matchType = FWP_MATCH_EQUAL;
    conds[1].fieldKey = FWPM_CONDITION_IP_REMOTE_ADDRESS;
    conds[1].matchType = FWP_MATCH_EQUAL;
    if (p_ctx->pc_base->use_ipv6) {
        conds[0].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
        conds[0].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)p_ctx->pc_base->addr_local;
        conds[1].conditionValue.type = FWP_BYTE_ARRAY16_TYPE;
        conds[1].conditionValue.byteArray16 = (FWP_BYTE_ARRAY16*)p_ctx->pc_base->addr_remote;
    }
    else {
        conds[0].conditionValue.type = FWP_UINT32;
        conds[0].conditionValue.uint32 = *((UINT32*)p_ctx->pc_base->addr_local);
        conds[1].conditionValue.type = FWP_UINT32;
        conds[1].conditionValue.uint32 = *((UINT32*)p_ctx->pc_base->addr_remote);
    }

    conds[2].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
    conds[2].matchType = FWP_MATCH_EQUAL;
    conds[2].conditionValue.type = FWP_UINT16;
    conds[2].conditionValue.uint16 = local_port;

    if (p_ctx->pc_base->ipproto != tipsec_ipproto_all) {
        conds[numFilterConditions].fieldKey = FWPM_CONDITION_IP_PROTOCOL;
        conds[numFilterConditions].matchType = FWP_MATCH_EQUAL;
        conds[numFilterConditions].conditionValue.type = FWP_UINT8;
        conds[numFilterConditions].conditionValue.uint8 = TINYIPSEC_VISTA_GET_IPPROTO(p_ctx->pc_base->ipproto);
        ++numFilterConditions;
    }

    // Fill in the common fields shared by both filters.
    memset(&filter, 0, sizeof(filter));
    // For MUI compatibility, object names should be indirect strings. See
    // SHLoadIndirectString for details.
    filter.displayData.name = (PWCHAR)p_ctx->filter_name;
    // Link all objects to our provider. When multiple providers are installed
    // on a computer, this makes it easy to determine who added what.
    filter.providerKey = (GUID*)TINYIPSEC_PROVIDER_KEY;
    filter.numFilterConditions = numFilterConditions;
    filter.filterCondition = conds;
    filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
    filter.flags = FWPM_FILTER_FLAG_NONE;
    filter.weight.type = FWP_EMPTY;

    // Add the inbound filter.
    filter.layerKey = (p_ctx->pc_base->use_ipv6) ? FWPM_LAYER_INBOUND_TRANSPORT_V6 : FWPM_LAYER_INBOUND_TRANSPORT_V4;
    if (p_ctx->pc_base->mode == tipsec_mode_tun) {
        filter.action.calloutKey = (p_ctx->pc_base->use_ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TUNNEL_V4;
    }
    else {
        filter.action.calloutKey = (p_ctx->pc_base->use_ipv6) ? FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_INBOUND_TRANSPORT_V4;
    }
    if ((result = FwpmFilterAdd0(p_ctx->engine, &filter, NULL, &tmpInFilterId)) != ERROR_SUCCESS) {
        TSK_DEBUG_ERROR("FwpmFilterAdd0 (inbound) failed with error code [%x]", result);
        goto CLEANUP;
    }

    // Add the outbound filter.
    filter.layerKey = (p_ctx->pc_base->use_ipv6) ? FWPM_LAYER_OUTBOUND_TRANSPORT_V6 : FWPM_LAYER_OUTBOUND_TRANSPORT_V4;
    if (p_ctx->pc_base->mode == tipsec_mode_tun) {
        filter.action.calloutKey = (p_ctx->pc_base->use_ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TUNNEL_V4;
    }
    else {
        filter.action.calloutKey = (p_ctx->pc_base->use_ipv6) ? FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V6 : FWPM_CALLOUT_IPSEC_OUTBOUND_TRANSPORT_V4;
    }
    if ((result = FwpmFilterAdd0(p_ctx->engine, &filter, NULL, &tmpOutFilterId)) != ERROR_SUCCESS) {
        TSK_DEBUG_ERROR("FwpmFilterAdd0(outbound) failed with error code [%x]", result);
        goto CLEANUP;
    }

    // Create the SA context using the outbound traffic descriptor.
    memset(&outTraffic, 0, sizeof(outTraffic));
    outTraffic.ipVersion = TINYIPSEC_VISTA_GET_IPVER(p_ctx->pc_base->use_ipv6);
    if (p_ctx->pc_base->use_ipv6) {
        memcpy(outTraffic.localV6Address, p_ctx->pc_base->addr_local, 16);
        memcpy(outTraffic.remoteV6Address, p_ctx->pc_base->addr_remote, 16);
    }
    else {
        outTraffic.localV4Address = *((UINT32*)p_ctx->pc_base->addr_local);
        outTraffic.remoteV4Address = *((UINT32*)p_ctx->pc_base->addr_remote);
    }
    outTraffic.trafficType = TINYIPSEC_VISTA_GET_MODE(p_ctx->pc_base->mode);
    outTraffic.ipsecFilterId = tmpOutFilterId;
    if ((result = IPsecSaContextCreate0(p_ctx->engine, &outTraffic, NULL, &tmpSaId)) != ERROR_SUCCESS) {
        TSK_DEBUG_ERROR("IPsecSaContextCreate0 failed with error code [%x]", result);
        goto CLEANUP;
    }

    // Get the inbound SPI using the inbound traffic descriptor.
    memset(&getSpi, 0, sizeof(getSpi));
    getSpi.inboundIpsecTraffic.ipVersion = TINYIPSEC_VISTA_GET_IPVER(p_ctx->pc_base->use_ipv6);
    if (p_ctx->pc_base->use_ipv6) {
        memcpy(getSpi.inboundIpsecTraffic.localV6Address, p_ctx->pc_base->addr_local, 16);
        memcpy(getSpi.inboundIpsecTraffic.remoteV6Address, p_ctx->pc_base->addr_remote, 16);
    }
    else {
        getSpi.inboundIpsecTraffic.localV4Address = *((UINT32*)p_ctx->pc_base->addr_local);
        getSpi.inboundIpsecTraffic.remoteV4Address = *((UINT32*)p_ctx->pc_base->addr_remote);
    }
    getSpi.inboundIpsecTraffic.trafficType = TINYIPSEC_VISTA_GET_MODE(p_ctx->pc_base->mode);
    getSpi.inboundIpsecTraffic.ipsecFilterId = tmpInFilterId;
    getSpi.ipVersion = TINYIPSEC_VISTA_GET_IPVER(p_ctx->pc_base->use_ipv6);
    if ((result = IPsecSaContextGetSpi0(p_ctx->engine, tmpSaId, &getSpi, spi))) {
        TSK_DEBUG_ERROR("IPsecSaContextGetSpi0 failed with error code [%x]", result);
        goto CLEANUP;
    }

    //// Return the various LUIDs to the caller, so he can clean up.
    *filterId_in = tmpInFilterId;
    *filterId_out = tmpOutFilterId;
    *saId = tmpSaId;

CLEANUP:
    if (result != NO_ERROR) {
        _vista_deleteSaContextAndFilters(p_ctx->engine, tmpInFilterId, tmpOutFilterId, tmpSaId);
    }
    else {
        ret = 0;
    }

    return ret;
}

static int _vista_boundSA(__in const plugin_win_ipsec_vista_ctx_t* p_ctx, __in UINT64 local_saId, __in tipsec_spi_t remote_spi, __in BOOLEAN toInbound)
{
    UINT32 i=0, j=0;
    DWORD result = NO_ERROR;
    IPSEC_SA0 sa;
    IPSEC_SA_BUNDLE0 bundle;
    IPSEC_SA_AUTH_INFORMATION0 authInfo; // must be global because use as reference (X = &authInfo)
    IPSEC_SA_AUTH_AND_CIPHER_INFORMATION0 cipherAuthInfo; // must be global because use as reference (X = &cipherAuthInfo)
    PFWP_BYTE_BLOB ik = (PFWP_BYTE_BLOB)p_ctx->pc_base->ik;
    PFWP_BYTE_BLOB ck = (PFWP_BYTE_BLOB)p_ctx->pc_base->ck;

    memset(&sa, 0, sizeof(sa));
    sa.spi = remote_spi;
    sa.saTransformType = TINYIPSEC_VISTA_GET_PROTO(p_ctx->pc_base->protocol, p_ctx->pc_base->ealg);

    //
    //	Keys padding
    //
    if (p_ctx->pc_base->alg == tipsec_alg_hmac_sha_1_96) {
        if (ik->size < TIPSEC_IK_LEN) {
            for(i = ik->size; i < TIPSEC_KEY_LEN; i++) {
                ik->data[i] = 0x00; /* Already done by "tsk_calloc" but ... */
            }
            ik->size = TIPSEC_IK_LEN;
        }
    }
    if (p_ctx->pc_base->ealg == tipsec_ealg_des_ede3_cbc) {
        if (ck->size < TIPSEC_CK_LEN) {
            for (i = ck->size; i<TIPSEC_CK_LEN; i++) {
                ck->data[i] = ck->data[j++];
            }
            ck->size = TIPSEC_CK_LEN;
        }
    }

    //
    //	In all case create Authentication info
    //
    memset(&authInfo, 0, sizeof(authInfo));
    authInfo.authTransform.authTransformId = TINYIPSEC_VISTA_GET_ALGO(p_ctx->pc_base->alg);
    authInfo.authKey = *ik;

    if ( sa.saTransformType == IPSEC_TRANSFORM_AH ) {
        sa.ahInformation = &authInfo;
    }
    else if ( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH ) {
        sa.espAuthInformation = &authInfo;
    }
    else if ( sa.saTransformType == IPSEC_TRANSFORM_ESP_CIPHER ) {
        IPSEC_SA_CIPHER_INFORMATION0 cipherInfo;

        memset(&cipherInfo, 0, sizeof(cipherInfo));
        cipherInfo.cipherTransform.cipherTransformId = TINYIPSEC_VISTA_GET_EALGO(p_ctx->pc_base->ealg);
        cipherInfo.cipherKey = *ck;

        sa.espCipherInformation = &cipherInfo;
    }
    else if ( sa.saTransformType == IPSEC_TRANSFORM_ESP_AUTH_AND_CIPHER ) {
        IPSEC_SA_CIPHER_INFORMATION0 cipherInfo;

        memset(&cipherInfo, 0, sizeof(cipherInfo));
        cipherInfo.cipherTransform.cipherTransformId = TINYIPSEC_VISTA_GET_EALGO(p_ctx->pc_base->ealg);
        cipherInfo.cipherKey = *ck;

        memset(&cipherAuthInfo, 0, sizeof(cipherAuthInfo));
        cipherAuthInfo.saAuthInformation = authInfo;
        cipherAuthInfo.saCipherInformation = cipherInfo;

        sa.espAuthAndCipherInformation = &cipherAuthInfo;
    }

    memset(&bundle, 0, sizeof(bundle));
    bundle.numSAs = 1;
    bundle.saList = &sa;
    bundle.ipVersion = TINYIPSEC_VISTA_GET_IPVER(p_ctx->pc_base->use_ipv6);
    bundle.lifetime.lifetimeSeconds = (UINT32)((p_ctx->pc_base->lifetime > TINYIPSEC_SA_MAX_LIFETIME) ? TINYIPSEC_SA_MAX_LIFETIME : p_ctx->pc_base->lifetime);

    /* From remote to local (inbound) ? */
    if (toInbound) {
        if((result = IPsecSaContextAddInbound0(p_ctx->engine, local_saId, &bundle)) != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("IPsecSaContextAddInbound0 failed with error code [%x]", result);
            goto CLEANUP;
        }
    }
    else {
        if ((result = IPsecSaContextAddOutbound0(p_ctx->engine, local_saId, &bundle)) != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("IPsecSaContextAddOutbound0 failed with error code [%x]", result);
            goto CLEANUP;
        }
    }

CLEANUP:
    return (result == ERROR_SUCCESS) ? 0 : -1;
}

static int _vista_flushAll(const plugin_win_ipsec_vista_ctx_t* p_ctx)
{
#if 1
    int ret = -1;
    if (p_ctx && p_ctx->engine) {
        DWORD result;
        result = FwpmFilterDeleteById0(p_ctx->engine, p_ctx->filterId_in_uc);
        if (result != ERROR_SUCCESS && result != FWP_E_FILTER_NOT_FOUND) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x]", result);
        }
        result = FwpmFilterDeleteById0(p_ctx->engine, p_ctx->filterId_in_us);
        if (result != ERROR_SUCCESS && result != FWP_E_FILTER_NOT_FOUND) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x]", result);
        }
        result = FwpmFilterDeleteById0(p_ctx->engine, p_ctx->filterId_out_uc);
        if (result != ERROR_SUCCESS && result != FWP_E_FILTER_NOT_FOUND) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x]", result);
        }
        result = FwpmFilterDeleteById0(p_ctx->engine, p_ctx->filterId_out_us);
        if (result != ERROR_SUCCESS && result != FWP_E_FILTER_NOT_FOUND) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x]", result);
        }
        return 0;
    }
    //
    return ret;
#else
    UINT32 i;
    int ret = -1;

    if (p_ctx && p_ctx->engine) {
        HANDLE enumHandle = NULL;
        IPSEC_SA_DETAILS0** entries = NULL;
        UINT32 numEntriesReturned = 0;
        DWORD result;

        if ((result = IPsecSaCreateEnumHandle0(p_ctx->engine, NULL, &enumHandle)) != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("IPsecSaCreateEnumHandle0 failed with error code [%x].", result);
            goto CLEANUP;
        }

        if ((result = IPsecSaEnum0(p_ctx->engine, enumHandle, TINYIPSEC_SA_NUM_ENTRIES_TO_REQUEST, &entries, &numEntriesReturned)) != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("IPsecSaEnum0 failed with error code [%x].", result);
            goto CLEANUP;
        }

        for (i = 0; i<numEntriesReturned; i++) {
            IPSEC_SA_DETAILS0* entry = (entries)[i];
            if ( !wcscmp(entry->transportFilter->displayData.name, p_ctx->filter_name)) {
                if ((result = FwpmFilterDeleteById0(p_ctx->engine, entry->transportFilter->filterId)) != ERROR_SUCCESS) {
                    TSK_DEBUG_ERROR("FwpmFilterDeleteById0 failed with error code [%x].", result);
                    goto CLEANUP;
                }
            }
        }

        TSK_DEBUG_INFO("All SAs have been flushed.");
        ret = 0;

CLEANUP:
        if (entries) {
            FwpmFreeMemory0((void**)entries);
        }
        if (enumHandle) {
            if ((result = IPsecSaDestroyEnumHandle0(p_ctx->engine, enumHandle)) != ERROR_SUCCESS) {
                TSK_DEBUG_ERROR("IPsecSaDestroyEnumHandle0 failed with error code [%x].", result);
            }
        }
    }

    return ret;
#endif
}

static void _vista_deleteSaContextAndFilters(__in HANDLE engine, __in UINT64 inFilterId, __in UINT64 outFilterId, __in UINT64 saId)
{
    DWORD result;

    // Allow the LUIDs to be zero, so we can use this function to cleanup
    // partial results.
    if (saId != 0) {
        result = IPsecSaContextDeleteById0(engine, saId);
        if (result != ERROR_SUCCESS) {
            // There's not much we can do if delete fails, so continue trying to
            // clean up the remaining objects.
            TSK_DEBUG_ERROR("IPsecSaContextDeleteById0 = 0x%08X\n", result);
        }
    }
    if (outFilterId != 0) {
        result = FwpmFilterDeleteById0(engine, outFilterId);
        if (result != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 = 0x%08X\n", result);
        }
    }
    if (inFilterId != 0) {
        result = FwpmFilterDeleteById0(engine, inFilterId);
        if (result != ERROR_SUCCESS) {
            TSK_DEBUG_ERROR("FwpmFilterDeleteById0 = 0x%08X\n", result);
        }
    }
}

//
//	Windows Vista IPSec Plugin definition
//

/* constructor */
static tsk_object_t* _plugin_win_ipsec_vista_ctx_ctor(tsk_object_t * self, va_list * app)
{
    plugin_win_ipsec_vista_ctx_t *p_ctx = (plugin_win_ipsec_vista_ctx_t *)self;
    if (p_ctx) {
        p_ctx->pc_base = TIPSEC_CTX(p_ctx);
    }
    return self;
}
/* destructor */
static tsk_object_t* _plugin_win_ipsec_vista_ctx_dtor(tsk_object_t * self)
{
    plugin_win_ipsec_vista_ctx_t *p_ctx = (plugin_win_ipsec_vista_ctx_t *)self;
    if (p_ctx) {
        DWORD code;

        if (p_ctx->pc_base->started) {
            tipsec_ctx_stop(p_ctx->pc_base);
        }

        /* Close engine */
        if (p_ctx->engine) {
            if ((code = FwpmEngineClose0(p_ctx->engine))) {
                TSK_DEBUG_ERROR("FwpmEngineClose0 failed with error code [%x].", code);
            }
        }

        TSK_FREE(p_ctx->pc_base->addr_local);
        TSK_FREE(p_ctx->pc_base->addr_remote);

        if (p_ctx->pc_base->ik) {
            TSK_FREE(((PFWP_BYTE_BLOB)p_ctx->pc_base->ik)->data);
            TSK_FREE(p_ctx->pc_base->ik);
        }
        if (p_ctx->pc_base->ck) {
            TSK_FREE(((PFWP_BYTE_BLOB)p_ctx->pc_base->ck)->data);
            TSK_FREE(p_ctx->pc_base->ck);
        }

        TSK_DEBUG_INFO("*** Windows Vista IPSec plugin (Windows Filtering Platform) context destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_win_ipsec_vista_ctx_def_s = {
    sizeof(plugin_win_ipsec_vista_ctx_t),
    _plugin_win_ipsec_vista_ctx_ctor,
    _plugin_win_ipsec_vista_ctx_dtor,
    tsk_null,
};
/* plugin definition*/
static const tipsec_plugin_def_t plugin_win_ipsec_vista_plugin_def_s = {
    &plugin_win_ipsec_vista_ctx_def_s,

    tipsec_impl_type_vista,
    "Windows Vista IPSec (Windows Filtering Platform)",

    _plugin_win_ipsec_vista_ctx_init,
    _plugin_win_ipsec_vista_ctx_set_local,
    _plugin_win_ipsec_vista_ctx_set_remote,
    _plugin_win_ipsec_vista_ctx_set_keys,
    _plugin_win_ipsec_vista_ctx_start,
    _plugin_win_ipsec_vista_ctx_stop,
};
const tipsec_plugin_def_t *plugin_win_ipsec_vista_plugin_def_t = &plugin_win_ipsec_vista_plugin_def_s;
