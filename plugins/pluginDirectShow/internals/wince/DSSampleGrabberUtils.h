/* Copyright (C) 2014-2015 Mamadou DIOP
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

#if defined(_WIN32_WCE)

// callback definition
typedef void (CALLBACK *MANAGEDCALLBACKPROC)(BYTE* pdata, long len);

// ISampleGrabber interface definition

// {04951BFF-696A-4ade-828D-42A5F1EDB631}
DEFINE_GUID(IID_ISampleGrabber,
            0x4951bff, 0x696a, 0x4ade, 0x82, 0x8d, 0x42, 0xa5, 0xf1, 0xed, 0xb6, 0x31);

DECLARE_INTERFACE_(ISampleGrabber, IUnknown)
{
    STDMETHOD(SetCallback)(MANAGEDCALLBACKPROC callback) PURE;
};

// {D11DFE19-8864-4a60-B26C-552F9AA472E1}
DEFINE_GUID(CLSID_NullRenderer,
            0xd11dfe19, 0x8864, 0x4a60, 0xb2, 0x6c, 0x55, 0x2f, 0x9a, 0xa4, 0x72, 0xe1);

#endif /* _WIN32_WCE */
