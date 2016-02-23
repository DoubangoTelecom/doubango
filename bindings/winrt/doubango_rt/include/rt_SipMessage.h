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

#include "rt_Enums.h"

class SdpMessage;
class SipMessage;

struct tsdp_message_s;
struct tsip_message_s;

namespace doubango_rt
{
namespace BackEnd
{
public ref class rtSdpMessage sealed
{
internal:
    rtSdpMessage();
    rtSdpMessage(struct tsdp_message_s *message);
public:
    virtual ~rtSdpMessage();

public:
    Platform::String^ getSdpHeaderValue(Platform::String^ media, Platform::String^ name, unsigned index);
    Platform::String^ getSdpHeaderValue(Platform::String^ media, Platform::String^ name);
    Platform::String^ getSdpHeaderAValue(Platform::String^ media, Platform::String^ attributeName);

private:
    SdpMessage* m_pSdpMessage;
};

public ref class rtSipMessage sealed
{
internal:
    rtSipMessage();
    rtSipMessage(struct tsip_message_s* message);
public:
    virtual ~rtSipMessage();

public:
    bool isResponse();
    rt_tsip_request_type_t getRequestType();
    short getResponseCode();
    Platform::String^ getResponsePhrase();
    Platform::String^ getSipHeaderValue(Platform::String^ name, unsigned index);
    Platform::String^ getSipHeaderValue(Platform::String^ name);
    Platform::String^ getSipHeaderParamValue(Platform::String^ name, Platform::String^ param, unsigned index);
    Platform::String^ getSipHeaderParamValue(Platform::String^ name, Platform::String^ param);
    unsigned getSipContentLength();
#if COM_VISIBLE
    Platform::String^ getSipContent();
#else
    unsigned getSipContent(Platform::IntPtr output, unsigned maxsize);
#endif
    rtSdpMessage^ getSdpMessage();

private:
    SipMessage* m_pSipMessage;
};
}
}
