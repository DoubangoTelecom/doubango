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
#include "rt_SipUri.h"
#include "rt_String.h"

#include "SipUri.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;


rtSipUri::rtSipUri(Platform::String^ uriString, Platform::String^ displayName)
{
    m_pSipUri = new SipUri(
        rtString::toUtf8(uriString).data(),
        rtString::toUtf8(displayName).data());
}

rtSipUri::rtSipUri(Platform::String^ uriString)
{
    m_pSipUri = new SipUri(rtString::toUtf8(uriString).data());
}

rtSipUri::~rtSipUri()
{
    rtSafeDelete(m_pSipUri);
}

bool rtSipUri::isValid(Platform::String^ uri)
{
    return SipUri::isValid(rtString::toUtf8(uri).data());
}

// MIDL4069: Static members and instance members cannot have the same name on a runtime class. isValid

#if COM_VISIBLE
bool rtSipUri::isValid_()
#else
bool rtSipUri::isValid()
#endif
{
    return (m_pSipUri && m_pSipUri->isValid());
}

Platform::String^ rtSipUri::getScheme()
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getScheme()) : nullptr;
}

Platform::String^ rtSipUri::getHost()
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getHost()) : nullptr;
}

unsigned short rtSipUri::getPort()
{
    return m_pSipUri ? m_pSipUri->getPort() : 0;
}

Platform::String^ rtSipUri::getUserName()
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getUserName()) : nullptr;
}

Platform::String^ rtSipUri::getPassword()
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getPassword()) : nullptr;
}

Platform::String^ rtSipUri::getDisplayName()
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getDisplayName()) : nullptr;
}

Platform::String^ rtSipUri::getParamValue(Platform::String^ name)
{
    return m_pSipUri ? rtString::toString(m_pSipUri->getParamValue(rtString::toUtf8(name).data())) : nullptr;
}

void rtSipUri::setDisplayName(Platform::String^ displayName)
{
    if(m_pSipUri) {
        m_pSipUri->setDisplayName(rtString::toUtf8(displayName).data());
    }
}

