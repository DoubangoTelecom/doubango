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
#ifndef PLUGIN_DSHOW_DSOUTPUTFILTER_H
#define PLUGIN_DSHOW_DSOUTPUTFILTER_H

#include "plugin_dshow_config.h"

#include "internals/DSBufferWriter.h"
#include <streams.h>

class DSOutputStream;

// {17D9D5CB-850D-4339-B72A-F72D084D8D64}
TDSHOW_DEFINE_GUID(CLSID_TdshowOutputFilter,
                   0x17d9d5cb, 0x850d, 0x4339, 0xb7, 0x2a, 0xf7, 0x2d, 0x8, 0x4d, 0x8d, 0x64);

class DSOutputFilter : public CSource, public DSBufferWriter
{
public:
    DSOutputFilter(LPUNKNOWN pUnk, HRESULT *phr);
    virtual ~DSOutputFilter();

    //static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
    DECLARE_IUNKNOWN;

    virtual void setBuffer(void *pointer, int size);
    virtual inline HRESULT setImageFormat(UINT width, UINT height);
    virtual bool getImageFormat(UINT &width, UINT &height);

    virtual void setFps(int fps_);
    virtual void showOverlay(int value);

    virtual void getMediaType(AM_MEDIA_TYPE* &pmt);
    virtual HRESULT setMediaType(const AM_MEDIA_TYPE* pmt);

    void reset();

#ifdef _WIN32_WCE
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
#endif

//protected:
#ifdef _WIN32_WCE
    /*STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
    	 CheckPointer(ppv, E_POINTER);

    	if (riid == IID_IBaseFilter
    		|| riid == IID_IBaseFilter
    		|| riid == IID_IUnknown
    		|| riid == IID_IMediaFilter
    		)
    	{
    		return GetInterface((IBaseFilter *) this, ppv);
    	}
    	else
    	{
    		*ppv = NULL;
    		return E_NOINTERFACE;
    	}
    };

    STDMETHODIMP_(ULONG) AddRef() {
    	//return GetOwner()->AddRef();
    	//return 1;
    	return (ULONG)InterlockedIncrement(&m_cRef);
    };

    STDMETHODIMP_(ULONG) Release() {
        LONG lRefCount = InterlockedDecrement(&m_cRef);
    	if(m_cRef < 1) delete this;
    	return (ULONG)m_cRef;
    };

    STDMETHODIMP_(ULONG) NonDelegatingAddRef()
    {
    	return InterlockedIncrement(&m_cRef);
    }*/
#endif
    /*
    	STDMETHODIMP_(ULONG) NonDelegatingRelease()
    	{
    		if(InterlockedDecrement(&m_cRef) == 0)
    		{
    			delete this;
    			return 0;
    		}
    		return m_cRef;
    	}*/

private:
    DSOutputStream *outputStream;

    friend class DSOutputStream;
};

#endif
