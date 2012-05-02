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
#include <tinydshow/DSUtils.h>

#include <atlbase.h>
#include <atlstr.h>

#include "tsk_debug.h"

HWND GetMainWindow()
{
	HWND hWnd;
	if(!(hWnd = GetActiveWindow())){
		if(!(hWnd = GetForegroundWindow())){
			if(!(hWnd = GetConsoleWindow())){
				return NULL;
			}
		}
	}
	return hWnd;
}

bool IsMainThread()
{	
	HWND hWnd = GetMainWindow();
	if(hWnd){
		DWORD mainTid = GetWindowThreadProcessId(hWnd, NULL);
		DWORD currentTid = GetCurrentThreadId();
		return (mainTid == currentTid);
	}
	return false;
}

IPin *GetPin(IBaseFilter *filter, PIN_DIRECTION direction)
{
	IEnumPins	*enumPins = NULL;
	IPin		*pin = NULL;

	HRESULT hr = filter->EnumPins(&enumPins);
	if(!enumPins){
		return NULL;
	}

	for(;;){
		ULONG fetched = 0;
		PIN_DIRECTION pinDir = PIN_DIRECTION(-1);
		pin = NULL;

		if (FAILED(enumPins->Next(1, &pin, &fetched))){
			enumPins->Release();
			return NULL;
		}

		if (fetched == 1 && pin){
			pin->QueryDirection(&pinDir);
			if(pinDir == direction){
				break;
			}
			pin->Release();
		}
	}

	enumPins->Release();
	return pin;
}

HRESULT ConnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination, AM_MEDIA_TYPE *mediaType)
{
	HRESULT hr;

	IPin *outPin = GetPin(source, PINDIR_OUTPUT);
	IPin *inPin = GetPin(destination, PINDIR_INPUT);

	if (mediaType != NULL){
		hr = graphBuilder->ConnectDirect(outPin, inPin, mediaType);
	}
	else{
		hr = graphBuilder->Connect(outPin, inPin);
	}

	SAFE_RELEASE(outPin);
	SAFE_RELEASE(inPin);

	return hr;
}

HRESULT DisconnectFilters(IGraphBuilder *graphBuilder, IBaseFilter *source, IBaseFilter *destination)
{
	HRESULT hr;

	IPin *outPin = GetPin(source, PINDIR_OUTPUT);
	IPin *inPin = GetPin(destination, PINDIR_INPUT);

	if (inPin){
		hr = graphBuilder->Disconnect(inPin);
	}

	if (outPin){
		hr = graphBuilder->Disconnect(outPin);
	}

	SAFE_RELEASE(outPin);
	SAFE_RELEASE(inPin);

	return hr;
}

bool DisconnectAllFilters(IGraphBuilder *graphBuilder)
{
	CComPtr<IEnumFilters> filterEnum = NULL;
	CComPtr<IBaseFilter> currentFilter = NULL;
	ULONG fetched;
	HRESULT hr;

	hr = graphBuilder->EnumFilters(&filterEnum);
	if (FAILED(hr)) return false;

	while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK){
		hr = DisconnectFilters(graphBuilder, currentFilter, currentFilter);
	}

	filterEnum.Release();
	return true;
}

bool RemoveAllFilters(IGraphBuilder *graphBuilder)
{
	CComPtr<IEnumFilters> filterEnum = NULL;
	CComPtr<IBaseFilter> currentFilter = NULL;
	ULONG fetched;
	HRESULT hr;

	hr = graphBuilder->EnumFilters(&filterEnum);
	if (FAILED(hr)) return false;

	while(filterEnum->Next(1, &currentFilter, &fetched) == S_OK){
		hr = graphBuilder->RemoveFilter(currentFilter);
		if (FAILED(hr)){
			filterEnum.Release();
			return false;
		}
		currentFilter.Release();
		filterEnum->Reset();
	}

	filterEnum.Release();
	return true;
}


#include <tinydshow/DSDisplay.h>
#include <tinydshow/DSGrabber.h>

#define WM_CREATE_DISPLAY_ON_UI_THREAD				(WM_USER + 101)
#define WM_CREATE_GRABBER_ON_UI_THREAD				(WM_CREATE_DISPLAY_ON_UI_THREAD + 1)
#define WM_CREATE_ON_UI_THREAD_TIMEOUT		1000

// C Callback that dispatch event to create display on UI thread
static LRESULT CALLBACK __create__WndProcWindow(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HANDLE* event = reinterpret_cast<HANDLE*>(wParam);

	if(event && lParam){
		switch(uMsg){
			case WM_CREATE_DISPLAY_ON_UI_THREAD:
				{
					HRESULT hr;
					DSDisplay** ppDisplay = reinterpret_cast<DSDisplay**>(lParam);
					*ppDisplay = new DSDisplay(&hr);
					SetEvent(event);
					break;
				}
			case WM_CREATE_GRABBER_ON_UI_THREAD:
				{
					HRESULT hr;
					DSGrabber** ppGrabber = reinterpret_cast<DSGrabber**>(lParam);
					*ppGrabber = new DSGrabber(&hr);
					SetEvent(event);
					break;
				}
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int createOnUIThead(HWND hWnd, void** ppRet, bool display)
{
	if(!ppRet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(IsMainThread()){
		HRESULT hr;
		if(display) *ppRet = new DSDisplay(&hr);
		else *ppRet = new DSGrabber(&hr);
		if(FAILED(hr)){
			TSK_DEBUG_ERROR("Failed to created DirectShow %s", display ? "Display" : "Grabber");
			SAFE_DELETE_PTR(*ppRet);
			return -2;
		}
		return 0;
	}
	else{

		HANDLE event = NULL;
		int ret = 0;
		DWORD retWait, retryCount = 3;		

		if(!hWnd){
			if(!(hWnd = FindWindowA(NULL, "Boghe - IMS/RCS Client"))){
				if(!(hWnd = GetMainWindow())){
					TSK_DEBUG_ERROR("No Window handle could be used");
					return -2;
				}
			}
		}

		WNDPROC wndProc = (WNDPROC) SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG) __create__WndProcWindow);
		if(!wndProc){
			TSK_DEBUG_ERROR("SetWindowLongPtr() failed");
			return -3;
		}

		if(!(event = CreateEvent(NULL, TRUE, FALSE, NULL))){
			TSK_DEBUG_ERROR("Failed to create new event");
			ret = -4; goto bail;
		}

		if(!PostMessageA(hWnd, display ? WM_CREATE_DISPLAY_ON_UI_THREAD : WM_CREATE_GRABBER_ON_UI_THREAD, reinterpret_cast<WPARAM>(event), reinterpret_cast<LPARAM>(ppRet))){
			TSK_DEBUG_ERROR("PostMessageA() failed");
			ret = -5; goto bail;
		}
		
		do{
			retWait = WaitForSingleObject(event, WM_CREATE_ON_UI_THREAD_TIMEOUT);
		}
		while(retryCount-- > 0 && (retWait == WAIT_TIMEOUT));

	bail:
		// restore
		if(hWnd && wndProc){
			SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG)wndProc);
		}
		if(event){
			CloseHandle(event);
		}

		return ret;
	}
}