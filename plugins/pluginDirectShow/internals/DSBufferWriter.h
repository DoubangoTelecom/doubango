/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#ifndef PLUGIN_DSHOW_DSBUFFERWRITTER_H
#define PLUGIN_DSHOW_DSBUFFERWRITTER_H
/*
// TODO: do it only once
#if !defined(TDSHOW_DEFINE_GUID) && !defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#elif !defined(TDSHOW_DEFINE_GUID) && defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID __declspec(selectany) name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif

// {27AD9929-E4E7-423b-8BDD-8AF5AC894DE0}
TDSHOW_DEFINE_GUID(IID_DSBufferWriter,
			0x27ad9929, 0xe4e7, 0x423b, 0x8b, 0xdd, 0x8a, 0xf5, 0xac, 0x89, 0x4d, 0xe0);
			*/


class DSBufferWriter
#ifndef _WIN32_WCE
    : public IUnknown
#endif
{
public:
    virtual void setBuffer (void* pBuffer, int size) = 0;
    virtual HRESULT setImageFormat(UINT width, UINT height/*, GUID subType, UINT fps*/) = 0;
};

#endif
