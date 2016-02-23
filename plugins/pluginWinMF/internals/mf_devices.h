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
#ifndef PLUGIN_WIN_MF_DEVICES_H
#define PLUGIN_WIN_MF_DEVICES_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>

//
//  DeviceList [Declaration]
//
class DeviceList
{
    UINT32      m_cDevices;
    IMFActivate **m_ppDevices;

public:
    DeviceList();
    virtual  ~DeviceList();

    UINT32  Count()const;
    void Clear();
    HRESULT GetDeviceAtIndex(UINT32 index, IMFActivate **ppActivate);
    HRESULT GetDeviceBest(IMFActivate **ppActivate, WCHAR *pszName = NULL);
    HRESULT GetDeviceName(UINT32 index, WCHAR **ppszName);

protected:
    HRESULT EnumerateDevices(const GUID& sourceType);
};

class DeviceListAudio : public DeviceList
{
public:
    HRESULT EnumerateDevices();
};

class DeviceListVideo : public DeviceList
{
public:
    HRESULT EnumerateDevices();
};

#endif /* PLUGIN_WIN_MF_DEVICES_H */
