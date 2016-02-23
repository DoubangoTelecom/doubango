/* Copyright (C) 2014 Mamadou DIOP
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
#ifndef PLUGIN_DSHOW_DSBASECAPTUREGRAPH_H
#define PLUGIN_DSHOW_DSBASECAPTUREGRAPH_H

#include "plugin_dshow_config.h"

#include <vector>
#include <control.h>
#include "internals/DSCaptureFormat.h"

#if defined(_WIN32_WCE)
#	include <internals/wince/DSSampleGrabber.h>
#	include <internals/wince/DSNullFilter.h>
#	include <internals/wince/DSISampleGrabberCB.h>
#else
#	include <qedit.h>
#endif

class DSBaseCaptureGraph
{
public:
#if defined(_WIN32_WCE)
    DSBaseCaptureGraph(DSISampleGrabberCB* callback, HRESULT *hr) {}
#else
    DSBaseCaptureGraph(ISampleGrabberCB* callback, HRESULT *hr) {}
#endif
    virtual ~DSBaseCaptureGraph() {}

    virtual std::vector<DSCaptureFormat> *getFormats() = 0;

    virtual HRESULT setSource(const std::string &devicePath) = 0;
    virtual HRESULT setParameters(DSCaptureFormat *format, int framerate) = 0;

    virtual HRESULT connect() = 0;
    virtual HRESULT disconnect() = 0;

    virtual HRESULT start() = 0;
    virtual HRESULT stop() = 0;
    virtual HRESULT pause() = 0;
    virtual bool isRunning() = 0;
    virtual bool isPaused() = 0;

    virtual std::string getDeviceId() const = 0;

    virtual HRESULT getConnectedMediaType(AM_MEDIA_TYPE *mediaType) = 0;
};

#endif /* PLUGIN_DSHOW_DSBASECAPTUREGRAPH_H */
