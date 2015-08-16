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
#include "plugin_cuda_utils.h"

#include "tsk_debug.h"

#include <NVEncoderAPI.h>
#include <cuda.h>
#include <cuda_runtime_api.h>

bool CudaUtils::g_bStarted = false;
bool CudaUtils::g_bH264Checked = false;
bool CudaUtils::g_bH264Supported = false;
int CudaUtils::g_nCores = 0;

HRESULT CudaUtils::Startup()
{
	if(!g_bStarted)
	{
		CUresult cuResult = CUDA_SUCCESS;
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(SUCCEEDED(hr) || hr == 0x80010106) // 0x80010106 when called from managed code (e.g. Boghe) - More info: http://support.microsoft.com/kb/824480
		{
			if((cuResult = cuInit(0)) != CUDA_SUCCESS)
			{
				TSK_DEBUG_ERROR("cuInit() failed with error code = %08x", cuResult);
				hr = E_FAIL;
			}
			else
			{	
				hr = S_OK;
			}
		}
		g_bStarted = true;
		return hr;
	}
	return S_OK;
}

HRESULT CudaUtils::Shutdown()
{
	// cuDeinit();
	return S_OK;
}

bool CudaUtils::IsH264Supported()
{
	if(g_bH264Checked)
	{
		return g_bH264Supported;
	}

	HRESULT hr = S_OK;

	CHECK_HR(hr = Startup());

	g_bH264Checked = true;
	
	NVEncoder pEncoder = NULL;

	CHECK_HR(hr = NVGetHWEncodeCaps());
	CHECK_HR(hr = NVCreateEncoder(&pEncoder));
	// Both Base and Main profiles *must* be supported
	CHECK_HR(hr = NVIsSupportedCodecProfile(pEncoder, NV_CODEC_TYPE_H264, NVVE_H264_PROFILE_BASELINE));
	CHECK_HR(hr = NVIsSupportedCodecProfile(pEncoder, NV_CODEC_TYPE_H264, NVVE_H264_PROFILE_MAIN));
	
	g_bH264Supported = true;
	
bail:
	if(pEncoder)
	{
		NVDestroyEncoder(pEncoder);
		pEncoder = NULL;
	}

	return g_bH264Supported;
}

int CudaUtils::ConvertSMVer2Cores(int nMajor, int nMinor)
{
	if(g_nCores != 0)
	{
		return g_nCores;
	}

	// Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
    typedef struct
    {
        int SM; // 0xMm (hexidecimal notation), M = SM Major version, and m = SM minor version
        int Cores;
    } sSMtoCores;

    sSMtoCores nGpuArchCoresPerSM[] =
    {
        { 0x10,  8 }, // Tesla Generation (SM 1.0) G80 class
        { 0x11,  8 }, // Tesla Generation (SM 1.1) G8x class
        { 0x12,  8 }, // Tesla Generation (SM 1.2) G9x class
        { 0x13,  8 }, // Tesla Generation (SM 1.3) GT200 class
        { 0x20, 32 }, // Fermi Generation (SM 2.0) GF100 class
        { 0x21, 48 }, // Fermi Generation (SM 2.1) GF10x class
        { 0x30, 192}, // Kepler Generation (SM 3.0) GK10x class
        { 0x35, 192}, // Kepler Generation (SM 3.5) GK11x class
    };

    int index = 0;

    while (nGpuArchCoresPerSM[index].SM != -1)
    {
        if (nGpuArchCoresPerSM[index].SM == ((nMajor << 4) + nMinor))
        {
            g_nCores = nGpuArchCoresPerSM[index].Cores;
			break;
        }

        index++;
    }

    // If we don't find the values, we default use the previous one to run properly
    TSK_DEBUG_INFO("MapSMtoCores for SM %d.%d is undefined.  Default to use %d Cores/SM", nMajor, nMinor, nGpuArchCoresPerSM[7].Cores);
    g_nCores = nGpuArchCoresPerSM[7].Cores;

	return g_nCores;
}

int CudaUtils::GetMaxGflopsDeviceId()
{
	int device_count = 0;
	cudaGetDeviceCount( &device_count );

	cudaDeviceProp device_properties;
	int max_gflops_device = 0;
	int max_gflops = 0;
	
	int current_device = 0;
	cudaGetDeviceProperties( &device_properties, current_device );
	max_gflops = device_properties.multiProcessorCount * device_properties.clockRate;
	++current_device;

	while( current_device < device_count )
	{
		cudaGetDeviceProperties( &device_properties, current_device );
		int gflops = device_properties.multiProcessorCount * device_properties.clockRate;
		if( gflops > max_gflops )
		{
			max_gflops        = gflops;
			max_gflops_device = current_device;
		}
		++current_device;
	}

	return max_gflops_device;
}