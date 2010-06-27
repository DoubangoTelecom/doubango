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
#ifndef DSDISPLAYGRAPH_H
#define DSDISPLAYGRAPH_H

#include <tinydshow_config.h>

#include <control.h>

#include <tinydshow/VideoFrame.h>
#include <tinydshow/DSOutputFilter.h>
#include <tinydshow/DSDisplayOverlay.h>

#if defined(VMR9) || defined(VMR9_WINDOWLESS)
#include <D3D9.h>
#include <vmr9.h>
#endif


class DSDisplayGraph
{
public:
	DSDisplayGraph(HRESULT *hr);
	virtual ~DSDisplayGraph();

	int getDisplayFps() { return this->fps; };
	void setDisplayFps(int fps_);

	bool getImageFormat(UINT &width, UINT &height);
	bool setImageFormat(UINT width, UINT height);

	HRESULT connect();
	HRESULT disconnect();

	HRESULT start();
	HRESULT stop();
	bool isRunning();

	IMediaEventEx			*getMediaEvent()		{ return this->mediaEvent; };
	IVideoWindow			*getVideoWindow()		{ return this->videoWindow; };
	DSOutputFilter			*getSourceFilter()		{ return this->sourceFilter; };

#if defined(VMR)
	IVMRMixerBitmap			*getMixerBitmap()		{ return this->mixerBitmap; };
#elif defined(VMR9)
	IVMRMixerBitmap9		*getMixerBitmap()		{ return this->mixerBitmap; };
#elif defined(VMR9_WINDOWLESS)
	IVMRMixerBitmap9		*getMixerBitmap()		{ return this->mixerBitmap; };
	IVMRMixerControl9		*getMixerControl()		{ return this->mixerControl; };
	IVMRWindowlessControl9	*getWindowlessControl()	{ return this->windowlessControl; };
#endif

	void handleFrame(const void* data, int w, int h);

private:
	HRESULT createDisplayGraph();

private:
	IGraphBuilder					*graphBuilder;

	DSOutputFilter					*sourceFilter;
	IBaseFilter						*colorspaceConverterFilter;
	IBaseFilter						*videoRendererFilter;

	IMediaControl					*mediaController;
	IMediaEventEx					*mediaEvent;
	IVideoWindow					*videoWindow;

#if defined(VMR)
	IVMRMixerBitmap					*mixerBitmap;
	IVMRFilterConfig				*filterConfig;
#elif defined(VMR9)
	IVMRMixerBitmap9				*mixerBitmap;
	IVMRMixerControl9				*mixerControl;
	IVMRFilterConfig9				*filterConfig;
#elif defined(VMR9_WINDOWLESS)
	IVMRMixerBitmap9				*mixerBitmap;
	IVMRMixerControl9				*mixerControl;
	IVMRFilterConfig9				*filterConfig;
	IVMRWindowlessControl9			*windowlessControl;
#endif

	bool running;
	int fps;
};

#endif
