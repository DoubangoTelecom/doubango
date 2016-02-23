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

class SipCallback;
class SipCallbackProxy;

namespace doubango_rt
{
namespace BackEnd
{
ref class rtDialogEvent;
ref class rtStackEvent;

ref class rtInviteEvent;
ref class rtMessagingEvent;
ref class rtInfoEvent;
ref class rtOptionsEvent;
ref class rtPublicationEvent;
ref class rtRegistrationEvent;
ref class rtSubscriptionEvent;

public interface class rtISipCallback
{
    virtual int OnDialogEvent(rtDialogEvent^ e);
    virtual int OnStackEvent(rtStackEvent^ e);

    virtual int OnInviteEvent(rtInviteEvent^ e);
    virtual int OnMessagingEvent(rtMessagingEvent^ e);
    virtual int OnInfoEvent(rtInfoEvent^ e);
    virtual int OnOptionsEvent(rtOptionsEvent^ e);
    virtual int OnPublicationEvent(rtPublicationEvent^ e);
    virtual int OnRegistrationEvent(rtRegistrationEvent^ e);
    virtual int OnSubscriptionEvent(rtSubscriptionEvent^ e);
};

public ref class rtSipCallback sealed
{
internal:
    rtSipCallback(rtISipCallback^ pI);
public:
    virtual ~rtSipCallback();
    friend class SipCallbackProxy;

internal:
    const SipCallback* getWrappedCallback();

private:
    rtISipCallback^ m_pI;
    SipCallbackProxy* m_pCallback;
};
}
}


