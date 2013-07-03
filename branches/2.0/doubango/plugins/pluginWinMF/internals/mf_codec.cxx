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
#include "mf_codec.h"
#include "mf_utils.h"

#include "tinymedia/tmedia_common.h"

#include "tsk_debug.h"

#include <Codecapi.h>
#include <assert.h>

// NV12 is the only format supported by all HW encoders and decoders
#if !defined(kMFCodecUncompressedFormat)
#	define kMFCodecUncompressedFormat MFVideoFormat_NV12
#endif

//
//	MFCodec
//

MFCodec::MFCodec(MFCodecId_t eId, MFCodecType_t eType)
: m_eId(eId)
, m_eType(eType)
, m_pMFT(NULL)
, m_pOutputType(NULL)
, m_pInputType(NULL)
, m_dwInputID(0)
, m_dwOutputID(0)
{
	MFUtils::Startup();

	HRESULT hr = S_OK;

	switch(eId)
	{
		case MFCodecId_H264Base:
		case MFCodecId_H264Main:
			{
				m_eMediaType = MFCodecMediaType_Video;
				m_guidCompressedFormat = MFVideoFormat_H264;
				break;
			}
		case MFCodecId_AAC:
			{
				m_eMediaType = MFCodecMediaType_Audio;
				m_guidCompressedFormat = MFAudioFormat_AAC;
				break;
			}
		default:
			{
				assert(false);
				break;
			}
	}
	CHECK_HR(hr = MFCreateMediaType(&m_pOutputType));
	CHECK_HR(hr = MFCreateMediaType(&m_pInputType));
	CHECK_HR(hr = MFUtils::GetBestCodec(
		(m_eType == MFCodecType_Encoder), // Encoder ?
		(m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio, // Media Type
		GUID_NULL, // Input
		(m_eType == MFCodecType_Encoder) ? m_guidCompressedFormat : kMFCodecUncompressedFormat, // Output
		&m_pMFT));

bail:
	if(!IsValid())
	{
		TSK_DEBUG_ERROR("Failed to create codec with id = %d", m_eId);
	}
}	

MFCodec::~MFCodec()
{
	SafeRelease(&m_pMFT);
    SafeRelease(&m_pOutputType);
	SafeRelease(&m_pInputType);
}

bool MFCodec::IsValid()
{
	return (m_pMFT && m_pOutputType && m_pInputType);
}

bool MFCodec::IsReady()
{
	return IsValid(); // FIXME
}

enum tmedia_chroma_e MFCodec::GetUncompressedChroma()
{
	if(kMFCodecUncompressedFormat == MFVideoFormat_NV12)
	{
		return tmedia_chroma_nv12;
	}
	assert(false);
	return tmedia_chroma_none;
}

//
//	MFCodecVideo
//

MFCodecVideo::MFCodecVideo(MFCodecId_t eId, MFCodecType_t eType)
: MFCodec(eId, eType)
{
	assert(m_eMediaType == MFCodecMediaType_Video);
}

MFCodecVideo::~MFCodecVideo()
{

}

HRESULT MFCodecVideo::Initialize(
			UINT32 nFrameRate,
			UINT32 nWidth,
			UINT32 nHeight,
			UINT32 nOutputBitRate /*= 0*/
		)
{
	assert(IsValid());
		
	HRESULT hr = S_OK;

	// make sure identifiers are zero-based (other layouts not supported yet)
	hr = m_pMFT->GetStreamIDs(1, &m_dwInputID, 1, &m_dwOutputID);
    if (hr == E_NOTIMPL)
    {
        m_dwInputID = 0;
        m_dwOutputID = 0;
        hr = S_OK;
    }
    else if (FAILED(hr))
    {
		TSK_DEBUG_ERROR("The stream identifiers are not zero-based");
        return hr;
    }

	CHECK_HR(hr = m_pOutputType->SetGUID(MF_MT_MAJOR_TYPE, (m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio));
	CHECK_HR(hr = m_pInputType->SetGUID(MF_MT_MAJOR_TYPE, (m_eMediaType == MFCodecMediaType_Video) ? MFMediaType_Video : MFMediaType_Audio));

	CHECK_HR(hr = m_pOutputType->SetGUID(MF_MT_SUBTYPE, (m_eType == MFCodecType_Encoder) ? m_guidCompressedFormat : kMFCodecUncompressedFormat));
	CHECK_HR(hr = m_pInputType->SetGUID(MF_MT_SUBTYPE, (m_eType == MFCodecType_Encoder) ? kMFCodecUncompressedFormat : m_guidCompressedFormat));
	
	if(nOutputBitRate > 0 && m_eType == MFCodecType_Encoder)
	{
		CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_AVG_BITRATE, nOutputBitRate));
	}
	     
    CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
	CHECK_HR(hr = m_pInputType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

    CHECK_HR(hr = MFSetAttributeSize(m_pOutputType, MF_MT_FRAME_SIZE, nWidth, nHeight));
	CHECK_HR(hr = MFSetAttributeSize(m_pInputType, MF_MT_FRAME_SIZE, nWidth, nHeight));

    CHECK_HR(hr = MFSetAttributeRatio(m_pOutputType, MF_MT_FRAME_RATE, nFrameRate, 1));
	CHECK_HR(hr = MFSetAttributeRatio(m_pInputType, MF_MT_FRAME_RATE, nFrameRate, 1));

    CHECK_HR(hr = MFSetAttributeRatio(m_pOutputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
	CHECK_HR(hr = MFSetAttributeRatio(m_pInputType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));

	// Output format must be set before input
	CHECK_HR(hr = m_pMFT->SetOutputType(m_dwOutputID, m_pOutputType, 0));
	CHECK_HR(hr = m_pMFT->SetInputType(m_dwInputID, m_pInputType, 0));

bail:
	return hr;
}

//
//	MFCodecVideo
//
MFCodecVideoH264::MFCodecVideoH264(MFCodecId_t eId, MFCodecType_t eType)
: MFCodecVideo(eId, eType)
{
	assert(eId == MFCodecId_H264Base || eId == MFCodecId_H264Main);
	
	HRESULT hr = S_OK;

	if(m_pOutputType)
	{
		CHECK_HR(hr = m_pOutputType->SetUINT32(MF_MT_MPEG2_PROFILE, (m_eId == MFCodecId_H264Base) ? eAVEncH264VProfile_Base : eAVEncH264VProfile_Main));
	}

bail:
	assert(SUCCEEDED(hr));
}

MFCodecVideoH264::~MFCodecVideoH264()
{
	
}

MFCodecVideoH264* MFCodecVideoH264::CreateCodecH264Base(MFCodecType_t eType)
{
	return new MFCodecVideoH264(MFCodecId_H264Base, eType);
}

MFCodecVideoH264* MFCodecVideoH264::CreateCodecH264Main(MFCodecType_t eType)
{
	return new MFCodecVideoH264(MFCodecId_H264Main, eType);
}
