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
#include "rt_SipStack.h"
#include "rt_SipCallback.h"
#include "rt_DDebug.h"
#include "rt_String.h"
#include "rt_Enums.h"

#include "SipStack.h"


using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

rtSipStack::rtSipStack(rtISipCallback^ callback, Platform::String^ realmUri, Platform::String^ impiString, Platform::String^ impuUri)
{
    m_pSipCallback = ref new rtSipCallback(callback);
    m_pSipStack = new SipStack(
        const_cast<SipCallback*>(m_pSipCallback->getWrappedCallback()),
        rtString::toUtf8(realmUri).data(),
        rtString::toUtf8(impiString).data(),
        rtString::toUtf8(impuUri).data()
    );
}

rtSipStack::~rtSipStack()
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    rtSafeDelete(m_pSipStack);
    rtSafeDelete(m_pSipCallback);
    rtSafeDelete(m_pDebugCallback);
}

bool rtSipStack::start()
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->start();
}

bool rtSipStack::setDebugCallback(rtIDDebugCallback^ pCallback)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    m_pSipStack->setDebugCallback(tsk_null);
    rtSafeDelete(m_pDebugCallback);
    if(!pCallback) {
        return true;
    }

    m_pDebugCallback = ref new rtDDebugCallback(pCallback);
    return m_pSipStack->setDebugCallback(const_cast<DDebugCallback*>(m_pDebugCallback->getWrappedCallback()));
}

bool rtSipStack::setDisplayName(Platform::String^ display_name)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setDisplayName(rtString::toUtf8(display_name).data());
}

bool rtSipStack::setRealm(Platform::String^ realm_uri)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setRealm(rtString::toUtf8(realm_uri).data());
}

bool rtSipStack::setIMPI(Platform::String^ impi)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setIMPI(rtString::toUtf8(impi).data());
}

bool rtSipStack::setIMPU(Platform::String^ impu_uri)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setIMPU(rtString::toUtf8(impu_uri).data());
}

bool rtSipStack::setPassword(Platform::String^ password)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setPassword(rtString::toUtf8(password).data());
}

bool rtSipStack::setAMF(Platform::String^ amf)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setAMF(rtString::toUtf8(amf).data());
}

bool rtSipStack::setOperatorId(Platform::String^ opid)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setOperatorId(rtString::toUtf8(opid).data());
}


bool rtSipStack::setProxyCSCF(Platform::String^ fqdn, unsigned short port, Platform::String^ transport, Platform::String^ ipversion)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setProxyCSCF(
               rtString::toUtf8(fqdn).data(),
               port,
               rtString::toUtf8(transport).data(),
               rtString::toUtf8(ipversion).data()
           );
}
bool rtSipStack::setLocalIP(Platform::String^ ip, Platform::String^ transport)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setLocalIP(rtString::toUtf8(ip).data(), rtString::toUtf8(transport).data());
}

bool rtSipStack::setLocalIP(Platform::String^ ip)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setLocalIP(rtString::toUtf8(ip).data());
}

bool rtSipStack::setLocalPort(unsigned short port, Platform::String^ transport)
{
    return m_pSipStack->setLocalPort(port, rtString::toUtf8(transport).data());
}

bool rtSipStack::setLocalPort(unsigned short port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setLocalPort(port);
}

bool rtSipStack::setEarlyIMS(bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setEarlyIMS(enabled);
}

bool rtSipStack::addHeader(Platform::String^ name, Platform::String^ value)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->addHeader(rtString::toUtf8(name).data(), rtString::toUtf8(value).data());
}

bool rtSipStack::removeHeader(Platform::String^ name)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->removeHeader(rtString::toUtf8(name).data());
}

bool rtSipStack::addDnsServer(Platform::String^ ip)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->removeHeader(rtString::toUtf8(ip).data());
}

bool rtSipStack::setDnsDiscovery(bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setDnsDiscovery(enabled);
}

bool rtSipStack::setAoR(Platform::String^ ip, int port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setAoR(rtString::toUtf8(ip).data(), port);
}

bool rtSipStack::setSigCompParams(unsigned dms, unsigned sms, unsigned cpb, bool enablePresDict)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setSigCompParams(dms, sms, cpb, enablePresDict);
}

bool rtSipStack::addSigCompCompartment(Platform::String^ compId)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->addSigCompCompartment(rtString::toUtf8(compId).data());
}

bool rtSipStack::removeSigCompCompartment(Platform::String^ compId)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->removeSigCompCompartment(rtString::toUtf8(compId).data());
}

bool rtSipStack::setSTUNServer(Platform::String^ ip, unsigned short port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setSTUNServer(rtString::toUtf8(ip).data(), port);
}

bool rtSipStack::setSTUNCred(Platform::String^ login, Platform::String^ password)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setSTUNCred(rtString::toUtf8(login).data(), rtString::toUtf8(password).data());
}

bool rtSipStack::setTLSSecAgree(bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setTLSSecAgree(enabled);
}

bool rtSipStack::setSSLCertificates(Platform::String^ privKey, Platform::String^ pubKey, Platform::String^ caKey, bool verify)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setSSLCertificates(rtString::toUtf8(privKey).data(), rtString::toUtf8(pubKey).data(), rtString::toUtf8(caKey).data(), verify);
}

bool rtSipStack::setSSLCertificates(Platform::String^ privKey, Platform::String^ pubKey, Platform::String^ caKey)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return setSSLCertificates(privKey, pubKey, caKey, false);
}

bool rtSipStack::setIPSecSecAgree(bool enabled)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setIPSecSecAgree(enabled);
}

bool rtSipStack::setIPSecParameters(Platform::String^ algo, Platform::String^ ealgo, Platform::String^ mode, Platform::String^ proto)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->setIPSecParameters(rtString::toUtf8(algo).data(), rtString::toUtf8(ealgo).data(), rtString::toUtf8(mode).data(), rtString::toUtf8(proto).data());
}

Platform::String^ rtSipStack::dnsENUM(Platform::String^ service, Platform::String^ e164num, Platform::String^ domain)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return rtString::toString(m_pSipStack->dnsENUM(rtString::toUtf8(service).data(), rtString::toUtf8(e164num).data(), rtString::toUtf8(domain).data()));
}

#if COM_VISIBLE
rtDnsResult^ rtSipStack::dnsNaptrSrv(String^ domain, String^ service)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    unsigned short _port = 0;
    String^ address = rtString::toString(m_pSipStack->dnsNaptrSrv(rtString::toUtf8(domain).data(), rtString::toUtf8(service).data(), &_port));
    return ref new rtDnsResult(_port, address);
}

rtDnsResult^ rtSipStack::dnsSrv(String^ service)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    unsigned short _port = 0;
    String^ address = rtString::toString(m_pSipStack->dnsSrv(rtString::toUtf8(service).data(), &_port));
    return ref new rtDnsResult(_port, address);
}

String^ rtSipStack::getLocalIP(String^ protocol)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    char* _ip = m_pSipStack->getLocalIPnPort(rtString::toUtf8(protocol).data(), tsk_null);
    String^ ip = rtString::toString(_ip);
    TSK_FREE(_ip);
    return ip;
}

uint16 rtSipStack::getLocalPort(String^ protocol)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    uint16 port = 0;
    char* _ip = m_pSipStack->getLocalIPnPort(rtString::toUtf8(protocol).data(), &port);
    TSK_FREE(_ip);
    return port;
}
#else
String^ rtSipStack::dnsNaptrSrv(String^ domain, String^ service, IntPtr port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return rtString::toString(m_pSipStack->dnsNaptrSrv(rtString::toUtf8(domain).data(), rtString::toUtf8(service).data(), ((unsigned short*)(void*)port)));
}

String^ rtSipStack::dnsSrv(String^ service, IntPtr port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return rtString::toString(m_pSipStack->dnsSrv(rtString::toUtf8(service).data(), ((unsigned short*)(void*)port)));
}

Platform::String^ rtSipStack::getLocalIPnPort(String^ protocol, IntPtr port)
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    char* _ip = m_pSipStack->getLocalIPnPort(rtString::toUtf8(protocol).data(), ((unsigned short*)(void*)port));
    String^ ip = rtString::toString();
    TSK_FREE(_ip);
    return ip;
}
#endif /* COM_VISIBLE */

Platform::String^ rtSipStack::getPreferredIdentity()
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return rtString::toString(m_pSipStack->getPreferredIdentity());
}

bool rtSipStack::isValid()
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->isValid();
}

bool rtSipStack::stop()
{
    std::lock_guard<std::recursive_mutex> lock(mLock);

    return m_pSipStack->stop();
}

bool rtSipStack::initialize()
{
    return SipStack::initialize();
}

bool rtSipStack::deInitialize()
{
    return SipStack::deInitialize();
}

void rtSipStack::setCodecs(enum class rt_tdav_codec_id_t codecs)
{
    return SipStack::setCodecs((tdav_codec_id_t) codecs);
}

bool rtSipStack::setCodecPriority(enum class rt_tdav_codec_id_t codec_id, int priority)
{
    return SipStack::setCodecPriority((tdav_codec_id_t)codec_id, priority);
}

bool rtSipStack::isCodecSupported(enum class rt_tdav_codec_id_t codec_id)
{
    return SipStack::isCodecSupported((tdav_codec_id_t) codec_id);
}



