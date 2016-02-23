#if defined(_WIN32_WCE)

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

#include "internals/wince/DSNullFilter.h"

// {7F9F08CF-139F-40b2-A283-01C4EC26A452}
TDSHOW_DEFINE_GUID(CLSID_DSNullFilter,
                   0x7f9f08cf, 0x139f, 0x40b2, 0xa2, 0x83, 0x1, 0xc4, 0xec, 0x26, 0xa4, 0x52);

DSNullFilter::DSNullFilter(LPUNKNOWN punk,HRESULT *phr)
    : CTransInPlaceFilter(TEXT("NullRenderer"), punk, CLSID_DSNullFilter, phr)
{
}

HRESULT DSNullFilter::CheckInputType(const CMediaType *mtIn)
{
    CheckPointer(mtIn,E_POINTER);

    if (*mtIn->FormatType() != FORMAT_VideoInfo) {
        return E_INVALIDARG;
    }

    if ( *mtIn->Type( ) != MEDIATYPE_Video ) {
        return E_INVALIDARG;
    }

    if ( *mtIn->Subtype( ) != MEDIASUBTYPE_RGB24 ) {
        return E_INVALIDARG;
    }

    return NOERROR;
}

HRESULT DSNullFilter::Transform(IMediaSample *pSample)
{
    return NOERROR;
}

#endif /* _WIN32_WCE */
