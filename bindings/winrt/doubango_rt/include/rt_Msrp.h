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

class MsrpCallback;
class MsrpMessage;
class MsrpEvent;

namespace doubango_rt
{
namespace BackEnd
{
ref class rtMsrpSession;

public ref class rtMsrpByteRange sealed
{
internal:
    rtMsrpByteRange(int64 start, int64 end, int64 total):_start(start),_end(end),_total(total) {}
public:
    property int64 Start { int64 get() {
        return _start;
    };
                         }
    property int64 End { int64 get() {
        return _end;
    };
                       }
    property int64 Total { int64 get() {
        return _total;
    };
                         }
private:
    int64 _start, _end, _total;
};

public ref class rtMsrpMessage sealed
{
public:
    virtual ~rtMsrpMessage();
internal:
    rtMsrpMessage(struct tmsrp_message_s *message);

public:
    bool isRequest();
    short getCode();
    Platform::String^ getPhrase();
    rt_tmsrp_request_type_t getRequestType();
#if COM_VISIBLE
    rtMsrpByteRange^ getByteRange();
#else
    void getByteRange(Platform::IntPtr start, Platform::IntPtr end, Platform::IntPtr total);
#endif
    bool isLastChunck();
    bool isFirstChunck();
    bool isSuccessReport();
    Platform::String^ getMsrpHeaderValue(Platform::String^ name);
    Platform::String^ getMsrpHeaderParamValue(Platform::String^ name, Platform::String^ param);
    unsigned getMsrpContentLength();
#if COM_VISIBLE
    Platform::String^ getMsrpContent(unsigned maxsize);
#else
    unsigned getMsrpContent(Platform::IntPtr output, unsigned maxsize);
#endif

private:
    MsrpMessage* m_pMsrpMessage;
};

public ref class rtMsrpEvent sealed
{
internal:
    rtMsrpEvent(const struct tmsrp_event_s *event);
public:
    virtual ~rtMsrpEvent();

    rt_tmsrp_event_type_t getType();
    rtMsrpSession^ getSipSession();
    rtMsrpMessage^ getMessage();

private:
    MsrpEvent* m_pMsrpEvent;
};

public interface class rtIMsrpCallback
{
    virtual int OnEvent(rtMsrpEvent^ pEvent);
};
public ref class rtMsrpCallback sealed
{
internal:
    rtMsrpCallback(rtIMsrpCallback^ pI);
    const MsrpCallback* getWrappedCallback() {
        return m_pCallback;
    }
public:
    virtual ~rtMsrpCallback();

private:
    MsrpCallback* m_pCallback;
    rtIMsrpCallback^ m_pI;
};
}
}