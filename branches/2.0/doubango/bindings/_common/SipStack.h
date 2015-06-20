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
#ifndef TINYWRAP_SIPSTACK_H
#define TINYWRAP_SIPSTACK_H

#include "tinyWRAP_config.h"

#include "SipCallback.h"
#include "SafeObject.h"

#include "tinydav/tdav.h"
#include "tinysip.h"
#include "tinyhttp.h"

class DDebugCallback;

class TINYWRAP_API SipStack: public SafeObject
{
public: /* ctor() and dtor() */
	SipStack(SipCallback* pCallback, const char* realm_uri, const char* impi_uri, const char* impu_uri);
    ~SipStack();

public: /* API functions */
	bool start();
	bool setDebugCallback(DDebugCallback* pCallback);
	bool setDisplayName(const char* display_name);
	bool setRealm(const char* realm_uri);
	bool setIMPI(const char* impi);
	bool setIMPU(const char* impu_uri);
	bool setPassword(const char* password);
	bool setAMF(const char* amf);
	bool setOperatorId(const char* opid);
	bool setProxyCSCF(const char* fqdn, unsigned short port, const char* transport, const char* ipversion);
	bool setLocalIP(const char* ip, const char* transport=tsk_null);
	bool setLocalPort(unsigned short port, const char* transport=tsk_null);
	bool setEarlyIMS(bool enabled);
	bool addHeader(const char* name, const char* value);
	bool removeHeader(const char* name);
	bool addDnsServer(const char* ip);
	bool setDnsDiscovery(bool enabled);
	bool setAoR(const char* ip, int port);
#if !defined(SWIG)
	bool setMode(enum tsip_stack_mode_e mode);
#endif

	bool setSigCompParams(unsigned dms, unsigned sms, unsigned cpb, bool enablePresDict);
	bool addSigCompCompartment(const char* compId);
	bool removeSigCompCompartment(const char* compId);
	
	bool setSTUNEnabledForICE(bool enabled);  // @deprecated
	bool setSTUNServer(const char* hostname, unsigned short port);  // @deprecated
	bool setSTUNCred(const char* login, const char* password);  // @deprecated
	bool setSTUNEnabled(bool enabled);

	bool setTLSSecAgree(bool enabled);
	bool setSSLCertificates(const char* privKey, const char* pubKey, const char* caKey, bool verify = false);
	bool setSSLCretificates(const char* privKey, const char* pubKey, const char* caKey, bool verify = false); /*@deprecated: typo */
	bool setIPSecSecAgree(bool enabled);
	bool setIPSecParameters(const char* algo, const char* ealgo, const char* mode, const char* proto);
	
	char* dnsENUM(const char* service, const char* e164num, const char* domain);
	char* dnsNaptrSrv(const char* domain, const char* service, unsigned short *OUTPUT);
	char* dnsSrv(const char* service, unsigned short* OUTPUT);

	bool setMaxFDs(unsigned max_fds);

	char* getLocalIPnPort(const char* protocol, unsigned short* OUTPUT);

	char* getPreferredIdentity();

	bool isValid();
	bool stop();
	
	static bool initialize();
	static bool deInitialize();
	static void setCodecs(tdav_codec_id_t codecs);
	static void setCodecs_2(int64_t codecs); // For stupid languages
	static bool setCodecPriority(tdav_codec_id_t codec_id, int priority);
	static bool setCodecPriority_2(int codec, int priority);// For stupid languages
	static bool isCodecSupported(tdav_codec_id_t codec_id);
	static bool isIPSecSupported();

public: /* Public helper function */
#if !defined(SWIG)
	inline tsip_stack_handle_t* getHandle()const{
		return m_pHandle;
	}
	inline SipCallback* getCallback()const{
		return m_pCallback;
	}
	inline DDebugCallback* getDebugCallback() const{
		return m_pDebugCallback;
	}
#endif

private:
	SipCallback* m_pCallback;
	DDebugCallback* m_pDebugCallback;
	tsip_stack_handle_t* m_pHandle;

	static bool g_bInitialized;
};

#endif /* TINYWRAP_SIPSTACK_H */
