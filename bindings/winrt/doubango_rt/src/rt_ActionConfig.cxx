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
#include "rt_ActionConfig.h"
#include "rt_String.h"

#include "ActionConfig.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

rtActionConfig::rtActionConfig()
{
    m_pActionConfig = new ActionConfig();
}

rtActionConfig::~rtActionConfig()
{
    rtSafeDelete(m_pActionConfig);
}

bool rtActionConfig::addHeader(String^ name, String^ value)
{
    return m_pActionConfig->addHeader(rtString::toUtf8(name).data(), rtString::toUtf8(value).data());
}

bool rtActionConfig::setActiveMedia(rt_twrap_media_type_t type)
{
    return m_pActionConfig->setActiveMedia((twrap_media_type_t)type);
}

#if COM_VISIBLE
bool rtActionConfig::addPayload(Platform::String^ payload)
{
    static std::vector<char> _payload = rtString::toUtf8(payload);
    return m_pActionConfig->addPayload(_payload.data(), _payload.size());
}
#else
bool rtActionConfig::addPayload(IntPtr payload, unsigned len)
{
    return m_pActionConfig->addPayload((const void*)payload, len);
}
#endif /* COM_VISIBLE */

rtActionConfig^ rtActionConfig::setResponseLine(short code, String^ phrase)
{
    m_pActionConfig->setResponseLine(code, rtString::toUtf8(phrase).data());
    return this;
}

rtActionConfig^ rtActionConfig::setMediaString(rt_twrap_media_type_t type, String^ key, String^ value)
{
    m_pActionConfig->setMediaString((twrap_media_type_t)type, rtString::toUtf8(key).data(),  rtString::toUtf8(value).data());
    return this;
}

rtActionConfig^ rtActionConfig::setMediaInt(rt_twrap_media_type_t type, String^ key, int value)
{
    m_pActionConfig->setMediaInt((twrap_media_type_t)type, rtString::toUtf8(key).data(),  value);
    return this;
}