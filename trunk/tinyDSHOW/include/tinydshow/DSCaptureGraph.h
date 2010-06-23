/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
*
*/
#ifndef DSCAPTUREGRAPH_H
#define DSCAPTUREGRAPH_H

#include <tinydshow_config.h>
#include <vector>
#include <control.h>
#include <tinydshow/DSCaptureFormat.h>
#include <tinydshow/DSFrameRateFilter.h>

#ifdef _WIN32_WCE
#	include <tinydshow/wce/DSSampleGrabber.h>
#	include <tinydshow/wce/DSInxbNullFilter.h>
#	include <tinydshow/wce/InxbISampleGrabberCB.h>
#else
#	include <qedit.h>
#endif


class DSCaptureGraph
{
public:
#ifdef _WIN32_WCE
	DSCaptureGraph(InxbISampleGrabberCB* callback, HRESULT *hr);
#else
	DSCaptureGraph(ISampleGrabberCB* callback, HRESULT *hr);
#endif
	virtual ~DSCaptureGraph();

	std::vector<DSCaptureFormat> *getFormats() { return &this->supportedFormats; };

	HRESULT setSource(const std::string &devicePath);
	HRESULT setParameters(DSCaptureFormat *format, int framerate);

	HRESULT connect();
	HRESULT disconnect();

	HRESULT start();
	HRESULT stop();
	bool isRunning();

	std::string getDeviceId() const { return this->deviceId; };

	HRESULT getConnectedMediaType(AM_MEDIA_TYPE *mediaType);

private:
	HRESULT createCaptureGraph();

private:
#ifdef _WIN32_WCE
	InxbISampleGrabberCB			*grabberCallback;
#else
	ISampleGrabberCB				*grabberCallback;
#endif

	ICaptureGraphBuilder2			*captureGraphBuilder;
	IGraphBuilder					*graphBuilder;

	IBaseFilter						*sourceFilter;
	IBaseFilter						*nullRendererFilter;
	IBaseFilter						*sampleGrabberFilter;
	
#ifdef _WIN32_WCE
	IBaseFilter						*colorConvertor565; //http://msdn.microsoft.com/en-us/library/aa926076.aspx
#else
	IBaseFilter						*decompressorFilter;
	DSFrameRateFilter				*frameRateFilter;
#endif

#ifdef _WIN32_WCE
	DSSampleGrabber					*grabberController;
#else
	ISampleGrabber					*grabberController;
#endif

	IMediaControl					*mediaController;
	IMediaEventEx					*mediaEventController;

	IAMStreamConfig					*streamConfiguration;

	std::vector<DSCaptureFormat>	supportedFormats;
	DSCaptureFormat					*captureFormat;

	bool							running;
	std::string						deviceId;
};

#endif
