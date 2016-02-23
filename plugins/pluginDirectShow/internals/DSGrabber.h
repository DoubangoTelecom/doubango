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
#ifndef PLUGIN_DSHOW_DIRECTSHOW_GRABBER_H
#define PLUGIN_DSHOW_DIRECTSHOW_GRABBER_H

#include "plugin_dshow_config.h"

#include "internals/DSBaseCaptureGraph.h"
#include "internals/VideoFrame.h"

#include "tinymedia/tmedia_producer.h"

#include "tsk_mutex.h"

class DSDisplay;

#if defined(_WIN32_WCE)
#	include "internals/wince/DSISampleGrabberCB.h"
#endif


class DSGrabber : public
#if defined(_WIN32_WCE)
    DSISampleGrabberCB
#else
    ISampleGrabberCB
#endif
{
public:
    DSGrabber(HRESULT *hr, BOOL screenCast);
    virtual ~DSGrabber();

    void setCallback(tmedia_producer_enc_cb_f callback, const void* callback_data);
    void setCaptureDevice(const std::string &devicePath);

    virtual void start();
    virtual void pause();
    virtual void stop();

    virtual bool setCaptureParameters(int format, int f);
    virtual bool setCaptureParameters(int w, int h, int f);

    virtual void setPluginFirefox(bool value);

    virtual int getFramerate();
    virtual HRESULT getConnectedMediaType(AM_MEDIA_TYPE *mediaType);

    virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample);
    virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    DSDisplay *preview;

private:
    int					width;
    int					height;
    int					fps;

    DSBaseCaptureGraph		*graph;

    //VideoFrame			*currentFrame;
    BITMAPINFOHEADER	bitmapInfo;
    BYTE				*buffer;

    tsk_mutex_handle_t	*mutex_buffer;

    BOOL				first_buffer;
    BOOL				screenCast;

    const void* plugin_cb_data;
    tmedia_producer_enc_cb_f plugin_cb;
};

#endif
