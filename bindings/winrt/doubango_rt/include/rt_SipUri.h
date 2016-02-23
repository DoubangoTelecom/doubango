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

#include "rt_Config.h"

class SipUri;

namespace doubango_rt
{
namespace BackEnd
{
public ref class rtSipUri sealed
{
public:
    virtual ~rtSipUri();
    rtSipUri(Platform::String^ uriString, Platform::String^ displayName);
    rtSipUri(Platform::String^ uriString);


public:
    static bool isValid(Platform::String^ uri);

#if COM_VISIBLE
    bool isValid_();
#else
    bool isValid();
#endif
    Platform::String^ getScheme();
    Platform::String^ getHost();
    unsigned short getPort();
    Platform::String^ getUserName();
    Platform::String^ getPassword();
    Platform::String^ getDisplayName();
    Platform::String^ getParamValue(Platform::String^ name);
    void setDisplayName(Platform::String^ displayName);

internal:
    const SipUri* getWrappedUri() {
        return m_pSipUri;
    }

private:
    SipUri* m_pSipUri;
};
}
}