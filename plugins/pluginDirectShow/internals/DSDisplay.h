/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#ifndef PLUGIN_DSHOW_DIRECTSHOW_DISPLAY_H
#define PLUGIN_DSHOW_DIRECTSHOW_DISPLAY_H

#include "plugin_dshow_config.h"

#include "internals/DSDisplayGraph.h"
#include "internals/DSDisplayOverlay.h"

#define WM_FULLSCREEN_SET	(WM_USER + 401)

class DSDisplay
{
public:
	DSDisplay(HRESULT *hr);
	virtual ~DSDisplay();

	virtual void attach(INT64 parent);
	virtual void attach(void *parent);
	virtual void detach(void *parent);
	virtual void detach();
	virtual bool isAttached();

	virtual void start();
	virtual void pause();
	virtual void stop();

	virtual int getWidth();
	virtual int getHeight();
	virtual void setSize(int w, int h);

	virtual bool isFullscreen();
	virtual void setFullscreen(bool value);
	virtual void setPluginFirefox(bool value);

	virtual bool canFullscreen();

	virtual void setFps(int fps_);

	virtual void handleVideoFrame(const void* data, int w, int h);

	LRESULT handleEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void hook();
	void unhook();
	void applyRatio(RECT rect);

private:
	DSDisplayGraph *graph;
	DSDisplayOverlay *overlay;

	int fps;
	int left, top, width, height, imgWidth, imgHeight;

	bool bPluginFirefox;
	bool fullscreen;
	HWND window;
	WNDPROC parentWindowProc;
	
	bool hooked;
};

#endif
