/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef PLUGIN_DSHOW_DSSCREENCAST_H
#define PLUGIN_DSHOW_DSSCREENCAST_H

#include "plugin_dshow_config.h"
#include "internals/DSBaseCaptureGraph.h"

#include <control.h>
#include <vector>
#include <qedit.h>


class CPushSourceDesktop;

class DSScreenCastGraph : public DSBaseCaptureGraph
{
public:
	DSScreenCastGraph(ISampleGrabberCB* callback, HRESULT *hr);
	virtual ~DSScreenCastGraph();

	std::vector<DSCaptureFormat> *getFormats() { return &this->supportedFormats; };

	virtual HRESULT setSource(const std::string &devicePath) { return S_OK; }
	HRESULT setParameters(DSCaptureFormat *format, int framerate);

	HRESULT connect();
	HRESULT disconnect();

	HRESULT start();
	HRESULT stop();
	HRESULT pause();
	bool isRunning();
	bool isPaused();

	HRESULT getConnectedMediaType(AM_MEDIA_TYPE *mediaType);

	virtual std::string getDeviceId() const { return std::string("screencast"); }

private:
	HRESULT createCaptureGraph();

private:
	ISampleGrabberCB				*grabberCallback;

	ICaptureGraphBuilder2			*captureGraphBuilder;
	IGraphBuilder					*graphBuilder;

	CPushSourceDesktop				*sourceFilter;
	IBaseFilter						*nullRendererFilter;
	IBaseFilter						*sampleGrabberFilter;

	ISampleGrabber					*grabberController;

	IMediaControl					*mediaController;
	IMediaEventEx					*mediaEventController;

	std::vector<DSCaptureFormat>	supportedFormats;
	DSCaptureFormat					*captureFormat;

	bool							running;
	bool							paused;
};

#endif /* PLUGIN_DSHOW_DSSCREENCAST_H */
/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef PLUGIN_DSHOW_DSSCREENCAST_H
#define PLUGIN_DSHOW_DSSCREENCAST_H

#include "plugin_dshow_config.h"
#include "internals/DSBaseCaptureGraph.h"

#include <control.h>
#include <vector>
#include <qedit.h>


class CPushSourceDesktop;

class DSScreenCastGraph : public DSBaseCaptureGraph
{
public:
	DSScreenCastGraph(ISampleGrabberCB* callback, HRESULT *hr);
	virtual ~DSScreenCastGraph();

	std::vector<DSCaptureFormat> *getFormats() { return &this->supportedFormats; };

	virtual HRESULT setSource(const std::string &devicePath) { return S_OK; }
	HRESULT setParameters(DSCaptureFormat *format, int framerate);

	HRESULT connect();
	HRESULT disconnect();

	HRESULT start();
	HRESULT stop();
	HRESULT pause();
	bool isRunning();
	bool isPaused();

	HRESULT getConnectedMediaType(AM_MEDIA_TYPE *mediaType);

	virtual std::string getDeviceId() const { return std::string("screencast"); }

private:
	HRESULT createCaptureGraph();

private:
	ISampleGrabberCB				*grabberCallback;

	ICaptureGraphBuilder2			*captureGraphBuilder;
	IGraphBuilder					*graphBuilder;

	CPushSourceDesktop				*sourceFilter;
	IBaseFilter						*nullRendererFilter;
	IBaseFilter						*sampleGrabberFilter;

	ISampleGrabber					*grabberController;

	IMediaControl					*mediaController;
	IMediaEventEx					*mediaEventController;

	std::vector<DSCaptureFormat>	supportedFormats;
	DSCaptureFormat					*captureFormat;

	bool							running;
	bool							paused;
};

#endif /* PLUGIN_DSHOW_DSSCREENCAST_H */
