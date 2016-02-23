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
#include "rt_DDebug.h"
#include "rt_String.h"

#include "DDebug.h"

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

class DDebugCallbackProxy : public DDebugCallback
{
public:
    DDebugCallbackProxy(rtDDebugCallback^ pCallback) {
        m_pCallback = pCallback;
    }

    ~DDebugCallbackProxy() {

    }

    virtual int OnDebugInfo(const char* message) {
        return m_pCallback->m_pI->OnDebugInfo(rtString::toString(message));
    }

    virtual int OnDebugWarn(const char* message) {
        return m_pCallback->m_pI->OnDebugWarn(rtString::toString(message));
    }

    virtual int OnDebugError(const char* message) {
        return m_pCallback->m_pI->OnDebugError(rtString::toString(message));
    }

    virtual int OnDebugFatal(const char* message) {
        return m_pCallback->m_pI->OnDebugFatal(rtString::toString(message));
    }

private:
    rtDDebugCallback^ m_pCallback;
};

rtDDebugCallback::rtDDebugCallback(rtIDDebugCallback^ pI)
{
    m_pI = pI;
    m_pCallback = new DDebugCallbackProxy(this);
}

rtDDebugCallback::~rtDDebugCallback()
{
    rtSafeDelete(m_pCallback);
}

const DDebugCallback* rtDDebugCallback::getWrappedCallback()
{
    return dynamic_cast<const DDebugCallback*>(m_pCallback);
}