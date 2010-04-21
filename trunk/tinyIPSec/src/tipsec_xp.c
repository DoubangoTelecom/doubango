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

/**@file tipsec_xp.c
 * @brief Windows XP/2003 IPsec implementation using ipsec6 tool.
 * @sa http://technet.microsoft.com/en-us/library/cc787900.aspx
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tipsec_xp.h"

/**@defgroup tipsec_xp_group IPSec XP implementation.
* Supported algo: <b>hmac-md5-96</b> and <b>hmac-sha-1-96</b>.<br>
* Supported ealg: <b>null</b> only.<br>
* Supported mode: <b>tunnel</b> and <b>transport</b>.<br>
* Supported proto: <b>ah</b> and <b>esp</b>.<br>
* Supported IP proto: <b>tcp</b> and <b>udp</b>.<br>
* Supported IP version: <b>IPv6</b> only.
*/

#if HAVE_IPSEC_XP

#include "tsk_debug.h"
#include "tsk_memory.h"


#define TINYIPSEC_XP_GET_ALGO(algo) ((algo ==  algo_hmac_md5_96) ? "HMAC-MD5-96" : "HMAC-SHA1")
#define TINYIPSEC_XP_GET_MODE(mode)	 ((mode == mode_tun) ? "TUNNEL" : "TRANSPORT")
#define TINYIPSEC_XP_GET_PROTO(proto)	 ((proto == proto_ah) ? "AH" : "ESP")
#define TINYIPSEC_XP_GET_IPPROTO(ipproto)	 ((ipproto == ipproto_tcp) ? "TCP" : ((ipproto == ipproto_icmp) ? "ICMP" : "UDP"))

/**@ingroup tipsec_xp_group
* @def TINYIPSEC_IPSEC6_FILE
*/
/**@ingroup tipsec_xp_group
* @def TINYIPSEC_IPSEC6_FILE_KEY
*/
/**@ingroup tipsec_xp_group
* @def TINYIPSEC_IPSEC6_FILE_SAD
*/
/**@ingroup tipsec_xp_group
* @def TINYIPSEC_IPSEC6_FILE_SPD
*/
#define TINYIPSEC_IPSEC6_FILE				"tinyIPSec"
#define TINYIPSEC_IPSEC6_FILE_KEY			TINYIPSEC_IPSEC6_FILE".key"
#define TINYIPSEC_IPSEC6_FILE_SAD			TINYIPSEC_IPSEC6_FILE".sad"
#define TINYIPSEC_IPSEC6_FILE_SPD			TINYIPSEC_IPSEC6_FILE".spd"

#define TINYIPSEC_IPSEC6_TEMPLATE_POLICY "\n"\
"Security Policy List\n"\
"\n"\
"Policy      RemoteIPAddr                                   LocalIPAddr                                    Protocol      RemotePort    LocalPort   IPSecProtocol  IPSecMode   RemoteGWIPAddr                               SABundleIndex  Direction   Action      InterfaceIndex \n"\
"_________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n"\
"%s - %s - %s - %s - %u - %u %s %s %s %s %s %s %s ;\n"\
"%s - %s - %s - %s - %u - %u %s %s %s %s %s %s %s ;\n"\
"_________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n"\
"\n"\
"- = Take selector from policy.\n"\
"+ = Take selector from packet.\n"
#define TINYIPSEC_IPSEC6_TEMPLATE_SA "\n"\
"Security Association List\n"\
"\n"\
"SAEntry   SPI            SADestIPAddr                                 DestIPAddr                                   SrcIPAddr                                    Protocol    DestPort    SrcPort     AuthAlg     KeyFile        Direction   SecPolicyIndex \n"\
"___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n"\
"%s %u %s %s %s %s %s %s %s %s %s %s ;\n"\
"%s %u %s %s %s %s %s %s %s %s %s %s ;\n"\
"%s %u %s %s %s %s %s %s %s %s %s %s ;\n"\
"%s %u %s %s %s %s %s %s %s %s %s %s ;\n"\
"___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n"

/**@ingroup tipsec_xp_group
*/
#define TINYIPSEC_IPSEC6_UCPS_POLICY	"11111983"
/**@ingroup tipsec_xp_group
*/
#define TINYIPSEC_IPSEC6_USPC_POLICY	"22221983"

/**@ingroup tipsec_xp_group
* IPSec context.
*/
typedef struct tipsec_context_xp_s
{
	TINYIPSEC_DECLARE_CONTEXT;
	
}
tipsec_context_xp_t;

/**@ingroup tipsec_xp_group
*/
#define TIPSEC_CONTEXT_XP(ctx) ((tipsec_context_xp_t*)(ctx))

int tipsec_set_IKey(tipsec_context_xp_t* ctx_xp);
int tipsec_set_SPDs(tipsec_context_xp_t* ctx_xp);
int tipsec_set_SAs(tipsec_context_xp_t* ctx_xp);

struct handleInfo{
	HANDLE process;
	HANDLE pipe;
};

int tipsec_run_command(TCHAR *args);
DWORD WINAPI  tipsec_waitForExit(void *arg);

/**@ingroup tipsec_xp_group
*/
int tipsec_start(tipsec_context_t* ctx)
{
	tipsec_context_xp_t* ctx_xp = TIPSEC_CONTEXT_XP(ctx);
	int ret = -1;
	
	if(!ctx_xp){
		ret = -1;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->started){
		TSK_DEBUG_WARN("The IPSec context already started.");
		ret = -2;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_full){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	if((ret = tipsec_set_IKey(ctx_xp))){
		TSK_DEBUG_ERROR("Failed to set IKey.");
		goto bail;
	}

	if((ret = tipsec_set_SPDs(ctx_xp))){
		TSK_DEBUG_ERROR("Failed to set SPDs.");
		goto bail;
	}

	if((ret = tipsec_set_SAs(ctx_xp))){
		TSK_DEBUG_ERROR("Failed to set SAs.");
		goto bail;
	}

	/* delete previous Policies */
	tipsec_run_command(TEXT("d sp "TEXT(TINYIPSEC_IPSEC6_UCPS_POLICY)));
	tipsec_run_command(TEXT("d sp "TEXT(TINYIPSEC_IPSEC6_USPC_POLICY)));

	/* Load new policies */
	if(!(ret = tipsec_run_command(TEXT("l "TEXT(TINYIPSEC_IPSEC6_FILE))))){
		TIPSEC_CONTEXT(ctx_xp)->started = 1;
	}
	
bail:
	/* Remove files */
	remove(TINYIPSEC_IPSEC6_FILE_SPD);
	remove(TINYIPSEC_IPSEC6_FILE_SAD);
	remove(TINYIPSEC_IPSEC6_FILE_KEY);

	return ret;
}

/**@ingroup tipsec_xp_group
*/
int tipsec_set_local(tipsec_context_t* ctx, const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	tipsec_context_xp_t* ctx_xp = TIPSEC_CONTEXT_XP(ctx);
	int ret = -1;

	if(!ctx_xp){
		ret = -1;
		goto bail;
	}

	if(!addr_local || !port_uc || !port_us){
		ret = -2;
		goto bail;
	}

	if(!TIPSEC_CONTEXT(ctx_xp)->initialized){
		TSK_DEBUG_ERROR("IPSec engine not initialized.");
		ret = -3;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_initial){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -4;
		goto bail;
	}
	
	/* Set local/remote IPv6 addresses*/
	tsk_strupdate((char**)&TIPSEC_CONTEXT(ctx_xp)->addr_local, addr_local);
	tsk_strupdate((char**)&TIPSEC_CONTEXT(ctx_xp)->addr_remote, addr_remote);

	/* Set ports */
	TIPSEC_CONTEXT(ctx_xp)->port_uc = port_uc;
	TIPSEC_CONTEXT(ctx_xp)->port_us = port_us;

	/* Set SPIs */
	TIPSEC_CONTEXT(ctx_xp)->spi_uc = (rand() ^ rand());
	TIPSEC_CONTEXT(ctx_xp)->spi_us = (rand() ^ rand());

	TIPSEC_CONTEXT(ctx_xp)->state = state_inbound;

	ret = 0;

bail:
	return ret;
}

/**@ingroup tipsec_xp_group
*/
int tipsec_set_keys(tipsec_context_t* ctx, const tipsec_key_t* ik, const tipsec_key_t* ck)
{
	if(!ctx || !ik || !ck){
		return -1;
	}

	TSK_FREE(ctx->ik);
	TSK_FREE(ctx->ck);

	ctx->ik = tsk_strndup(ik, TIPSEC_KEY_LEN);
	ctx->ck = tsk_strndup(ck, TIPSEC_KEY_LEN); /* XP version of IPSec do not support encryption key but we copy ck (Who know?). */				

	return 0;
}

/**@ingroup tipsec_xp_group
*/
int tipsec_set_remote(tipsec_context_t* ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	tipsec_context_xp_t* ctx_xp = TIPSEC_CONTEXT_XP(ctx);
	int ret = -1;

	if(!ctx_xp){
		ret = -1;
		goto bail;
	}

	if(!lifetime || !port_pc || !port_ps){
		ret = -2;
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_inbound){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	/* Set Lifetime */
	TIPSEC_CONTEXT(ctx_xp)->lifetime = lifetime;

	/* Set ports */
	TIPSEC_CONTEXT(ctx_xp)->port_ps = port_ps;
	TIPSEC_CONTEXT(ctx_xp)->port_pc = port_pc;

	/* Set spis */
	TIPSEC_CONTEXT(ctx_xp)->spi_ps = spi_ps;
	TIPSEC_CONTEXT(ctx_xp)->spi_pc = spi_pc;

	TIPSEC_CONTEXT(ctx_xp)->state = state_full;

	ret = 0;

bail:
	return ret;
}

/**@ingroup tipsec_xp_group
*/
int tipsec_stop(tipsec_context_t* ctx)
{
	tipsec_context_xp_t* ctx_xp = TIPSEC_CONTEXT_XP(ctx);
	int ret = -1;

	/* Load previous Policies */
	ret = tipsec_run_command(TEXT("d sp "TEXT(TINYIPSEC_IPSEC6_UCPS_POLICY)));
	ret = tipsec_run_command(TEXT("d sp "TEXT(TINYIPSEC_IPSEC6_USPC_POLICY)));

	return ret;
}


int tipsec_set_IKey(tipsec_context_xp_t* ctx_xp)
{
	int ret = -1;
	FILE* file = NULL;

	if(!ctx_xp){
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_full){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	if(!(file = fopen(TINYIPSEC_IPSEC6_FILE_KEY, "wb+"))){
		TSK_DEBUG_ERROR("Failed to open file [%s].", TINYIPSEC_IPSEC6_FILE_KEY);
		ret = -4;
		goto bail;
	}

	fwrite(TIPSEC_CONTEXT(ctx_xp)->ik, TIPSEC_KEY_LEN, sizeof(uint8_t), file);

	if(TIPSEC_CONTEXT(ctx_xp)->alg == algo_hmac_md5_96){ /* Pad if HMAC-MD5-96 */
		uint8_t zeros[4];
		memset(zeros, 0, 4);
		fwrite(zeros, 4, sizeof(uint8_t), file);
	}

	ret = 0;

bail:
	if(file){
		fclose(file);
	}

	return ret;
}

int tipsec_set_SPDs(tipsec_context_xp_t* ctx_xp)
{
	int ret = -1;
	FILE* file = NULL;
	char* str = NULL;

	if(!ctx_xp){
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_full){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	if(!(file = fopen(TINYIPSEC_IPSEC6_FILE_SPD, "wb+"))){
		TSK_DEBUG_ERROR("Failed to open file [%s].", TINYIPSEC_IPSEC6_FILE_SPD);
		ret = -4;
		goto bail;
	}

	tsk_sprintf(&str, TINYIPSEC_IPSEC6_TEMPLATE_POLICY,
		/* UC -> PS */
		TINYIPSEC_IPSEC6_UCPS_POLICY,
		TIPSEC_CONTEXT(ctx_xp)->addr_remote,
		TIPSEC_CONTEXT(ctx_xp)->addr_local,
		TINYIPSEC_XP_GET_IPPROTO(TIPSEC_CONTEXT(ctx_xp)->ipproto),
		TIPSEC_CONTEXT(ctx_xp)->port_ps,
		TIPSEC_CONTEXT(ctx_xp)->port_uc,
		TINYIPSEC_XP_GET_PROTO(TIPSEC_CONTEXT(ctx_xp)->protocol),
		TINYIPSEC_XP_GET_MODE(TIPSEC_CONTEXT(ctx_xp)->mode),
		"*", /* RemoteGWIPAddr */
		"NONE", /* SABundleIndex */
		"BIDIRECT", /* Direction */
		"APPLY", /* Action */
		"0", /* InterfaceIndex */

		/* US -> PC */
		TINYIPSEC_IPSEC6_USPC_POLICY,
		TIPSEC_CONTEXT(ctx_xp)->addr_remote,
		TIPSEC_CONTEXT(ctx_xp)->addr_local,
		TINYIPSEC_XP_GET_IPPROTO(TIPSEC_CONTEXT(ctx_xp)->ipproto),
		TIPSEC_CONTEXT(ctx_xp)->port_pc,
		TIPSEC_CONTEXT(ctx_xp)->port_us,
		TINYIPSEC_XP_GET_PROTO(TIPSEC_CONTEXT(ctx_xp)->protocol),
		TINYIPSEC_XP_GET_MODE(TIPSEC_CONTEXT(ctx_xp)->mode),
		"*", /* RemoteGWIPAddr */
		"NONE", /* SABundleIndex */
		"BIDIRECT", /* Direction */
		"APPLY", /* Action */
		"0" /* InterfaceIndex */
		);

	fwrite(str, strlen(str), sizeof(uint8_t), file);
	ret = 0;

bail:
	if(file){
		fclose(file);
	}
	if(str){
		TSK_FREE(str);
	}
	return ret;
}

int tipsec_set_SAs(tipsec_context_xp_t* ctx_xp)
{

	int ret = -1;
	FILE* file = NULL;
	char* str = NULL;

	if(!ctx_xp){
		goto bail;
	}

	if(TIPSEC_CONTEXT(ctx_xp)->state != state_full){
		TSK_DEBUG_ERROR("IPSec context is in the wrong state.");
		ret = -3;
		goto bail;
	}

	if(!(file = fopen(TINYIPSEC_IPSEC6_FILE_SAD, "wb+"))){
		TSK_DEBUG_ERROR("Failed to open file [%s].", TINYIPSEC_IPSEC6_FILE_SAD);
		ret = -4;
		goto bail;
	}

	tsk_sprintf(&str, TINYIPSEC_IPSEC6_TEMPLATE_SA,
		/* PC -> US */
		"1", // SAEntry
		TIPSEC_CONTEXT(ctx_xp)->spi_us, // SPI
		TIPSEC_CONTEXT(ctx_xp)->addr_local, // SADestIPAddr
		"POLICY", // DestIPAddr
		"POLICY", // SrcIPAddr
		"POLICY", // Protocol
		"POLICY", // DestPort
		"POLICY", // SrcPort
		TINYIPSEC_XP_GET_ALGO(TIPSEC_CONTEXT(ctx_xp)->alg), // AuthAlg
		TINYIPSEC_IPSEC6_FILE_KEY, // KeyFile
		"INBOUND", // Direction
		"0", /* SecPolicyIndex */
		
		/* US -> PC */
		"2", // SAEntry
		TIPSEC_CONTEXT(ctx_xp)->spi_pc, // SPI
		TIPSEC_CONTEXT(ctx_xp)->addr_remote, // SADestIPAddr
		"POLICY", // DestIPAddr
		"POLICY", // SrcIPAddr
		"POLICY", // Protocol
		"POLICY", // DestPort
		"POLICY", // SrcPort
		TINYIPSEC_XP_GET_ALGO(TIPSEC_CONTEXT(ctx_xp)->alg), // AuthAlg
		TINYIPSEC_IPSEC6_FILE_KEY, // KeyFile
		"OUTBOUND", // Direction
		"0", /* SecPolicyIndex */

		/* PS -> UC */
		"3", // SAEntry
		TIPSEC_CONTEXT(ctx_xp)->spi_uc, // SPI
		TIPSEC_CONTEXT(ctx_xp)->addr_local, // SADestIPAddr
		"POLICY", // DestIPAddr
		"POLICY", // SrcIPAddr
		"POLICY", // Protocol
		"POLICY", // DestPort
		"POLICY", // SrcPort
		TINYIPSEC_XP_GET_ALGO(TIPSEC_CONTEXT(ctx_xp)->alg), // AuthAlg
		TINYIPSEC_IPSEC6_FILE_KEY, // KeyFile
		"INBOUND", // Direction
		"0", /* SecPolicyIndex */

		/* UC -> PS */
		"4", // SAEntry
		TIPSEC_CONTEXT(ctx_xp)->spi_ps, // SPI
		TIPSEC_CONTEXT(ctx_xp)->addr_remote, // SADestIPAddr
		"POLICY", // DestIPAddr
		"POLICY", // SrcIPAddr
		"POLICY", // Protocol
		"POLICY", // DestPort
		"POLICY", // SrcPort
		TINYIPSEC_XP_GET_ALGO(TIPSEC_CONTEXT(ctx_xp)->alg), // AuthAlg
		TINYIPSEC_IPSEC6_FILE_KEY, // KeyFile
		"OUTBOUND", // Direction
		"0" /* SecPolicyIndex */
		);
		
	fwrite(str, strlen(str), sizeof(uint8_t), file);
	ret = 0;

bail:
	if(file){
		fclose(file);
	}
	if(str){
		TSK_FREE(str);
	}
	return ret;
}

int tipsec_run_command(TCHAR *args)
{
#define TIPSEC_PIPE_BUFFER 1024

	DWORD bread=0,tid=0;
	int ret = -1;
	struct handleInfo hInfo;
	TCHAR _args[MAX_PATH];
	HANDLE writePipe, readPipe, hThread;
	SECURITY_ATTRIBUTES secAttr  = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	char buffer[TIPSEC_PIPE_BUFFER];

	/* Create pipes */
	if((ret = CreatePipe(&readPipe, &writePipe, &secAttr, 0)) == 0) {
		TSK_DEBUG_ERROR("CreatePipe failed with error code [%d].", GetLastError());
		ret = -5;
		goto bail;
	}

	wsprintf(_args, TEXT("\"%s\" %s"), TEXT("ipsec6.exe"), args );

	memset(buffer, 0, TIPSEC_PIPE_BUFFER);
	
	si.cb          =  sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESTDHANDLES;
	si.hStdInput  =  NULL;
	si.hStdOutput =  writePipe;
	si.hStdError  =  NULL;

	
	/* Create process */
	if (CreateProcess(NULL, _args, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0){
		TSK_DEBUG_ERROR("CreateProcess failed with error code [%d].", GetLastError());
		ret = -3;
		goto bail;
	}
		
	hInfo.process = pi.hProcess;
	hInfo.pipe = writePipe;

	/* Create thread */
	if((hThread = CreateThread(NULL, 0, tipsec_waitForExit, &hInfo, 0, &tid)) == NULL) {
		TSK_DEBUG_ERROR("CreateThread failed with error code [%d].", GetLastError());
		ret = -4;
		goto bail;
	}
	
	/* For debugging */
#if defined(DEBUG) || defined(_DEBUG)
	while (ReadFile(readPipe, buffer, TIPSEC_PIPE_BUFFER-2, &bread, NULL))
	{
		if(bread > 0){
			TSK_DEBUG_INFO("IPSEC6 ==> %s\n-------------\n", buffer);
			memset(buffer, 0, TIPSEC_PIPE_BUFFER); /* reset the buffer. */
		}
	}
#endif

	ret = 0;
	
bail:
	return ret;
}

DWORD WINAPI tipsec_waitForExit(void *arg)
{
	struct handleInfo *info = (struct handleInfo *)arg;
	WaitForSingleObject(&(info->process),INFINITE);
	CloseHandle(info->pipe);
	return 0;
}







//=================================================================================================
//	IPSec context object definition
//
static tsk_object_t* tipsec_context_ctor(tsk_object_t * self, va_list * app)
{
	tipsec_context_xp_t *context = self;
	if(context){
		TIPSEC_CONTEXT(context)->ipproto = va_arg(*app, tipsec_ipproto_t);
		TIPSEC_CONTEXT(context)->use_ipv6 = va_arg(*app, int);
		TIPSEC_CONTEXT(context)->mode = va_arg(*app, tipsec_mode_t);
		TIPSEC_CONTEXT(context)->ealg = va_arg(*app, tipsec_ealgorithm_t);
		TIPSEC_CONTEXT(context)->alg = va_arg(*app, tipsec_algorithm_t);
		TIPSEC_CONTEXT(context)->protocol = va_arg(*app, tipsec_protocol_t);

		/* Open engine */
		if(!TIPSEC_CONTEXT(context)->use_ipv6){
			TSK_DEBUG_ERROR("IPSec/IPv4 is not supported on Windows XP.");

			TIPSEC_CONTEXT(context)->initialized = tsk_false;
			goto bail;
		}
		else{
			TIPSEC_CONTEXT(context)->initialized = tsk_true;
		}
		
		TIPSEC_CONTEXT(context)->state = state_initial;
	}
bail:
	return self;
}

static tsk_object_t* tipsec_context_dtor(tsk_object_t * self)
{ 
	tipsec_context_xp_t *context = self;
	if(context)
	{
		if(TIPSEC_CONTEXT(context)->started){
			tipsec_stop(TIPSEC_CONTEXT(context));
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
	sizeof(tipsec_context_xp_t),
	tipsec_context_ctor, 
	tipsec_context_dtor,
	tipsec_context_cmp, 
};
const void *tipsec_context_def_t = &tipsec_context_def_s;


#endif /* HAVE_IPSEC_XP */
