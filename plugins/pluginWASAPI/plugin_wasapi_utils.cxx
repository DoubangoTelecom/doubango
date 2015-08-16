/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
#include "plugin_wasapi_utils.h"

#include "tsk_debug.h"

bool WASAPIUtils::g_bStarted = false;

HRESULT WASAPIUtils::Startup()
{
	if(!g_bStarted)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(SUCCEEDED(hr) || hr == 0x80010106) // 0x80010106 when called from managed code (e.g. Boghe) - More info: http://support.microsoft.com/kb/824480
		{
			hr = S_OK;
		}
		g_bStarted = SUCCEEDED(hr);
		return hr;
	}
	return S_OK;
}

HRESULT WASAPIUtils::Shutdown()
{
	return S_OK;
}

void WASAPIUtils::PrintError(const char* pcFileName, const char* pcFuncName, unsigned nLineNumber, HRESULT hr)
{
	CHAR message[1024] = {0};

#if PLUGIN_WASAPI_UNDER_WINDOWS_RT
	// FormatMessageA not allowed on the Store
	static WCHAR wBuff[1024] = {0};
	FormatMessageW(
		  FORMAT_MESSAGE_FROM_SYSTEM, 
		  tsk_null,
		  hr,
		  0,
		  wBuff, 
		  sizeof(wBuff)-1,
		  tsk_null);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wBuff, wcslen(wBuff), message, sizeof(message) - 1, NULL, NULL);
#else
#ifdef _WIN32_WCE
	FormatMessage
#else
	FormatMessageA
#endif
	(
#if !PLUGIN_WASAPI_UNDER_WINDOWS_RT
	  FORMAT_MESSAGE_ALLOCATE_BUFFER | 
#endif
	  FORMAT_MESSAGE_FROM_SYSTEM, 
	  tsk_null,
	  hr,
	  0,
	  message, 
	  sizeof(message) - 1,
	  tsk_null);
#endif

	TSK_DEBUG_ERROR("[WASAPI] File:%s\n Function=%s\n Line:%u\n Message:%s", pcFileName, pcFuncName, nLineNumber, message);
}