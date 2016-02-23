/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#pragma once

#include <mutex>

class SipStack;

namespace doubango_rt
{
namespace BackEnd
{
ref class rtSipCallback;
interface class rtISipCallback;
ref class rtDDebugCallback;
interface class rtIDDebugCallback;

enum class rt_tdav_codec_id_t;

public ref class rtDnsResult sealed
{
internal:
    rtDnsResult(uint16 port, Platform::String^ address):_port(port), _address(address) {}
public:
    property uint16 Port { uint16 get() {
        return _port;
    };
                         }
    property Platform::String^ Address { Platform::String^ get() {
        return _address;
    };
                                       }
private:
    uint16 _port;
    Platform::String^ _address;
};

public ref class rtSipStack sealed
{
public:
    virtual ~rtSipStack();

internal:
    rtSipStack(rtISipCallback^ callback, Platform::String^ realmUri, Platform::String^ impiString, Platform::String^ impuUri);
    const SipStack* getWrappedStack() {
        return m_pSipStack;
    }

public:
    bool start();
    bool setDebugCallback(rtIDDebugCallback^ pCallback);
    bool setDisplayName(Platform::String^ display_name);
    bool setRealm(Platform::String^ realm_uri);
    bool setIMPI(Platform::String^ impi);
    bool setIMPU(Platform::String^ impu_uri);
    bool setPassword(Platform::String^ password);
    bool setAMF(Platform::String^ amf);
    bool setOperatorId(Platform::String^ opid);
    bool setProxyCSCF(Platform::String^ fqdn, unsigned short port, Platform::String^ transport, Platform::String^ ipversion);
    bool setLocalIP(Platform::String^ ip, Platform::String^ transport);
    bool setLocalIP(Platform::String^ ip);
    bool setLocalPort(unsigned short port, Platform::String^ transport);
    bool setLocalPort(unsigned short port);
    bool setEarlyIMS(bool enabled);
    bool addHeader(Platform::String^ name, Platform::String^ value);
    bool removeHeader(Platform::String^ name);
    bool addDnsServer(Platform::String^ ip);
    bool setDnsDiscovery(bool enabled);
    bool setAoR(Platform::String^ ip, int port);
    bool setSigCompParams(unsigned dms, unsigned sms, unsigned cpb, bool enablePresDict);
    bool addSigCompCompartment(Platform::String^ compId);
    bool removeSigCompCompartment(Platform::String^ compId);

    bool setSTUNServer(Platform::String^ ip, unsigned short port);
    bool setSTUNCred(Platform::String^ login, Platform::String^ password);

    bool setTLSSecAgree(bool enabled);
    bool setSSLCertificates(Platform::String^ privKey, Platform::String^ pubKey, Platform::String^ caKey, bool verify);
    bool setSSLCertificates(Platform::String^ privKey, Platform::String^ pubKey, Platform::String^ caKey);
    bool setIPSecSecAgree(bool enabled);
    bool setIPSecParameters(Platform::String^ algo, Platform::String^ ealgo, Platform::String^ mode, Platform::String^ proto);

    Platform::String^ dnsENUM(Platform::String^ service, Platform::String^ e164num, Platform::String^ domain);
#if COM_VISIBLE
    rtDnsResult^ dnsNaptrSrv(Platform::String^ domain, Platform::String^ service);
    rtDnsResult^ dnsSrv(Platform::String^ service);
    Platform::String^ getLocalIP(Platform::String^ protocol);
    uint16 getLocalPort(Platform::String^ protocol);
#else
    Platform::String^ dnsNaptrSrv(Platform::String^ domain, Platform::String^ service, Platform::IntPtr port);
    Platform::String^ dnsSrv(Platform::String^ service, Platform::IntPtr port);
    Platform::String^ getLocalIPnPort(Platform::String^ protocol, Platform::IntPtr port);
#endif

    Platform::String^ getPreferredIdentity();

    bool isValid();
    bool stop();

    static bool initialize();
    static bool deInitialize();
    static void setCodecs(enum class rt_tdav_codec_id_t codecs);
    static bool setCodecPriority(enum class rt_tdav_codec_id_t codec_id, int priority);
    static bool isCodecSupported(enum class rt_tdav_codec_id_t codec_id);

private:
    SipStack* m_pSipStack;
    rtSipCallback^ m_pSipCallback;
    rtDDebugCallback^ m_pDebugCallback;
    std::recursive_mutex mLock;
};
}
}