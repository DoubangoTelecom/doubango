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
#include "mf_utils.h"

#include "tsk_debug.h"

#include <assert.h>

bool MFUtils::g_bStarted = false;
const TOPOID MFUtils::g_ullTopoIdSinkMain = 555;
const TOPOID MFUtils::g_ullTopoIdSinkPreview = 666;
const TOPOID MFUtils::g_ullTopoIdSource = 777;

HRESULT MFUtils::Startup()
{
	if(!g_bStarted)
	{
		HRESULT hr = MFStartup(MF_VERSION);
		assert((g_bStarted = SUCCEEDED(hr)));
		return hr;
	}
	return S_OK;
}

HRESULT MFUtils::Shutdown()
{
	if(g_bStarted)
	{
		g_bStarted = false;
		return S_OK;
	}
	return S_OK;
}

//-------------------------------------------------------------------
// CreatePCMAudioType
//
// Creates a media type that describes an uncompressed PCM audio
// format.
//-------------------------------------------------------------------

HRESULT MFUtils::CreatePCMAudioType(
    UINT32 sampleRate,        // Samples per second
    UINT32 bitsPerSample,     // Bits per sample
    UINT32 cChannels,         // Number of channels
    IMFMediaType **ppType     // Receives a pointer to the media type.
    )
{
    HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    // Calculate derived values.
    UINT32 blockAlign = cChannels * (bitsPerSample / 8);
    UINT32 bytesPerSecond = blockAlign * sampleRate;

    // Create the empty media type.
    hr = MFCreateMediaType(&pType);

    // Set attributes on the type.
    if (SUCCEEDED(hr))
    {
        hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, cChannels);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sampleRate);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, blockAlign);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, bytesPerSecond);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, bitsPerSample);
    }

    if (SUCCEEDED(hr))
    {
        hr = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
    }

    if (SUCCEEDED(hr))
    {
        // Return the type to the caller.
        *ppType = pType;
        (*ppType)->AddRef();
    }

    SafeRelease(&pType);
    return hr;
}


//-------------------------------------------------------------------
// CreateVideoType
//
// Creates a media type that describes a video subtype
// format.
//-------------------------------------------------------------------
HRESULT MFUtils::CreateVideoType(
		const GUID* subType, // video subType
		IMFMediaType **ppType,     // Receives a pointer to the media type.
		UINT32 unWidth, // Video width (0 to ignore)
		UINT32 unHeight // Video height (0 to ignore)
	)
{
	HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    CHECK_HR(hr = MFCreateMediaType(&pType));

    CHECK_HR(hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));

    CHECK_HR(hr = pType->SetGUID(MF_MT_SUBTYPE, *subType));

	CHECK_HR(hr = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE)); // UnCompressed

	CHECK_HR(hr = pType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));

	if(unWidth > 0 && unHeight > 0)
	{
		CHECK_HR(hr = MFSetAttributeSize(pType, MF_MT_FRAME_SIZE, unWidth, unHeight));
	}
	
    *ppType = pType;
    (*ppType)->AddRef();

bail:
    SafeRelease(&pType);
    return hr;
}

//-------------------------------------------------------------------
// Name: ValidateVideoFormat
// Description: Validates a media type for this sink.
//-------------------------------------------------------------------
HRESULT MFUtils::ValidateVideoFormat(IMFMediaType *pmt)
{
	GUID major_type = GUID_NULL;
    GUID subtype = GUID_NULL;
    MFVideoInterlaceMode interlace = MFVideoInterlace_Unknown;
    UINT32 val = 0;
    BOOL bFoundMatchingSubtype = FALSE;

    HRESULT hr = S_OK;

    // Major type must be video.
    CHECK_HR(hr = pmt->GetGUID(MF_MT_MAJOR_TYPE, &major_type));

    if (major_type != MFMediaType_Video)
    {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }

    // Subtype must be one of the subtypes in our global list.

    // Get the subtype GUID.
    CHECK_HR(hr = pmt->GetGUID(MF_MT_SUBTYPE, &subtype));

#if 0
    // Look for the subtype in our list of accepted types.
	for (DWORD i = 0; i < g_NumVideoSubtypes; i++)
    {
        if (subtype == *g_VideoSubtypes[i])
        {
            bFoundMatchingSubtype = TRUE;
            break;
        }
    }
    if (!bFoundMatchingSubtype)
    {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }
#endif

    // Video must be progressive frames.
    CHECK_HR(hr = pmt->GetUINT32(MF_MT_INTERLACE_MODE, (UINT32*)&interlace));
    if (interlace != MFVideoInterlace_Progressive)
    {
        CHECK_HR(hr = MF_E_INVALIDMEDIATYPE);
    }

bail:
    return hr;
}

HRESULT MFUtils::CreateMediaSample(
	DWORD cbData, // Maximum buffer size
	IMFSample **ppSample // Receives the sample
)
{
	assert(ppSample);

	HRESULT hr = S_OK;

    IMFSample *pSample = NULL;
    IMFMediaBuffer *pBuffer = NULL;

    CHECK_HR(hr = MFCreateSample(&pSample));    
    CHECK_HR(hr = MFCreateMemoryBuffer(cbData, &pBuffer));    
    CHECK_HR(hr = pSample->AddBuffer(pBuffer));
    
    *ppSample = pSample;
    (*ppSample)->AddRef();

bail:
    SafeRelease(&pSample);
    SafeRelease(&pBuffer);
    return hr;
}

// Gets the best encoder and decoder. Up to the caller to release the returned pointer
HRESULT MFUtils::GetBestCodec(
	bool bEncoder, // Whether we request an encoder or not (TRUE=encoder, FALSE=decoder)
	const GUID& mediaType, // The MediaType
	const GUID& inputFormat, // The input MediaFormat (e.g. MFVideoFormat_NV12)
	const GUID& outputFormat, // The output MediaFormat (e.g. MFVideoFormat_H264)
	IMFTransform **ppMFT // Receives the decoder/encoder transform
	)
{
	assert(ppMFT);
	assert(mediaType == MFMediaType_Video || mediaType == MFMediaType_Audio); // only audio and video codecs are support for now

	*ppMFT = NULL;

#if 0
	if(outputFormat == MFVideoFormat_H264)
	{
		// Force using IntelQuickSync for testing
		hr = CoCreateInstance(bEncoder ? CLSID_MF_H264EncFilter : CLSID_MF_H264DecFilter, NULL, 
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppMFT));
		if(SUCCEED(hr) && *ppMFT)
		{
			return hr;
		}
	}
#endif

	HRESULT hr = S_OK;
	UINT32 count = 0;

	IMFActivate **ppActivate = NULL;	

	MFT_REGISTER_TYPE_INFO infoInput = { mediaType, inputFormat };
	MFT_REGISTER_TYPE_INFO infoOutput = { mediaType, outputFormat };

	UINT32 unFlags = MFT_ENUM_FLAG_HARDWARE |
                 MFT_ENUM_FLAG_SYNCMFT  | 
                 MFT_ENUM_FLAG_LOCALMFT | 
                 MFT_ENUM_FLAG_SORTANDFILTER;

	hr = MFTEnumEx(
		(mediaType == MFMediaType_Video) ? (bEncoder ? MFT_CATEGORY_VIDEO_ENCODER : MFT_CATEGORY_VIDEO_DECODER) : (bEncoder ? MFT_CATEGORY_AUDIO_ENCODER : MFT_CATEGORY_AUDIO_DECODER),
		unFlags,
		(inputFormat == GUID_NULL) ? NULL : &infoInput,      // Input type
		(outputFormat == GUID_NULL) ? NULL : &infoOutput,       // Output type
		&ppActivate,
		&count
		);

	for(UINT32 i = 0; i < count; ++i)
	{
		hr = ppActivate[i]->ActivateObject(IID_PPV_ARGS(ppMFT));
		if(SUCCEEDED(hr) && *ppMFT) // For now we just get the first one. FIXME: Give HW encoders/decoders higher priority.
		{
			break;
		}
		SafeRelease(ppMFT);
	}

	for (UINT32 i = 0; i < count; i++)
	{
		ppActivate[i]->Release();
	}
	CoTaskMemFree(ppActivate);

	return *ppMFT ? S_OK : MF_E_NOT_FOUND;
}

HRESULT MFUtils::GetBestVideoProcessor(
	const GUID& inputFormat, // The input MediaFormat (e.g. MFVideoFormat_I420)
	const GUID& outputFormat, // The output MediaFormat (e.g. MFVideoFormat_NV12)
	IMFTransform **ppProcessor // Receives the video processor
	)
{
	assert(ppProcessor);

	*ppProcessor = NULL;

	HRESULT hr = S_OK;
	UINT32 count = 0;

	IMFActivate **ppActivate = NULL;	

	MFT_REGISTER_TYPE_INFO infoInput = { MFMediaType_Video, inputFormat };
	MFT_REGISTER_TYPE_INFO infoOutput = { MFMediaType_Video, outputFormat };

	UINT32 unFlags = MFT_ENUM_FLAG_HARDWARE |
                 MFT_ENUM_FLAG_SYNCMFT  | 
                 MFT_ENUM_FLAG_LOCALMFT | 
                 MFT_ENUM_FLAG_SORTANDFILTER;

	hr = MFTEnumEx(
		MFT_CATEGORY_VIDEO_PROCESSOR,
		unFlags,
		&infoInput,      // Input type
		&infoOutput,       // Output type
		&ppActivate,
		&count
		);

	for(UINT32 i = 0; i < count; ++i)
	{
		hr = ppActivate[i]->ActivateObject(IID_PPV_ARGS(ppProcessor));
		if(SUCCEEDED(hr) && *ppProcessor)
		{
			break;
		}
		SafeRelease(ppProcessor);
	}

	for (UINT32 i = 0; i < count; i++)
	{
		ppActivate[i]->Release();
	}
	CoTaskMemFree(ppActivate);

	return *ppProcessor ? S_OK : MF_E_NOT_FOUND;
}

// Add an transform node to a topology.
HRESULT MFUtils::AddTransformNode(
    IMFTopology *pTopology,     // Topology.
    IMFTransform *pMFT,     // MFT.
    DWORD dwId,                 // Identifier of the stream sink.
    IMFTopologyNode **ppNode   // Receives the node pointer.
	)
{
	*ppNode = NULL;

    IMFTopologyNode *pNode = NULL;
	HRESULT hr = S_OK;
    
    // Create the node.
    CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_TRANSFORM_NODE, &pNode));
    // Set the object pointer.
    CHECK_HR(hr = pNode->SetObject(pMFT));
	CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId));
	CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE));
    // Add the node to the topology.
    CHECK_HR(hr = pTopology->AddNode(pNode));

    // Return the pointer to the caller.
    *ppNode = pNode;
    (*ppNode)->AddRef();
    
bail:
    SafeRelease(&pNode);
    return hr;
}

// Sets the IMFStreamSink pointer on an output node.
HRESULT MFUtils::BindOutputNode(
	IMFTopologyNode *pNode // The Node
	)
{
	assert(pNode);

	HRESULT hr = S_OK;
	IUnknown *pNodeObject = NULL;
    IMFActivate *pActivate = NULL;
    IMFStreamSink *pStream = NULL;
    IMFMediaSink *pSink = NULL;

    // Get the node's object pointer.
    CHECK_HR(hr = pNode->GetObject(&pNodeObject));

    // The object pointer should be one of the following:
    // 1. An activation object for the media sink.
    // 2. The stream sink.

    // If it's #2, then we're already done.

    // First, check if it's an activation object.
    CHECK_HR(hr = pNodeObject->QueryInterface(IID_PPV_ARGS(&pActivate)));

    if (SUCCEEDED(hr))
    {
        DWORD dwStreamID = 0;

        // The object pointer is an activation object. 
        
        // Try to create the media sink.
        hr = pActivate->ActivateObject(IID_PPV_ARGS(&pSink));

        // Look up the stream ID. (Default to zero.)

        if (SUCCEEDED(hr))
        {
           dwStreamID = MFGetAttributeUINT32(pNode, MF_TOPONODE_STREAMID, 0);
        }

        // Now try to get or create the stream sink.

        // Check if the media sink already has a stream sink with the requested ID.

        if (SUCCEEDED(hr))
        {
            hr = pSink->GetStreamSinkById(dwStreamID, &pStream);
            if (FAILED(hr))
            {
                // Try to add a new stream sink.
                hr = pSink->AddStreamSink(dwStreamID, NULL, &pStream);
            }
        }

        // Replace the node's object pointer with the stream sink. 
        if (SUCCEEDED(hr))
        {
            hr = pNode->SetObject(pStream);
        }
    }
    else
    {
        // Not an activation object. Is it a stream sink?
        hr = pNodeObject->QueryInterface(IID_PPV_ARGS(&pStream));
    }

bail:
    SafeRelease(&pNodeObject);
    SafeRelease(&pActivate);
    SafeRelease(&pStream);
    SafeRelease(&pSink);
    return hr;
}

// Add an output node to a topology.
HRESULT MFUtils::AddOutputNode(
	IMFTopology *pTopology,     // Topology.
	IMFActivate *pActivate,     // Media sink activation object.
	DWORD dwId,                 // Identifier of the stream sink.
	IMFTopologyNode **ppNode)   // Receives the node pointer
{
	IMFTopologyNode *pNode = NULL;

	HRESULT hr = S_OK;
	CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode));
	CHECK_HR(hr = pNode->SetObject(pActivate));
	CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId));
	CHECK_HR(hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE));
	CHECK_HR(hr = pTopology->AddNode(pNode));

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

bail:
	SafeRelease(&pNode);
	return hr;
}

// Add a source node to a topology
HRESULT MFUtils::AddSourceNode(
	IMFTopology *pTopology,           // Topology.
	IMFMediaSource *pSource,          // Media source.
	IMFPresentationDescriptor *pPD,   // Presentation descriptor.
	IMFStreamDescriptor *pSD,         // Stream descriptor.
	IMFTopologyNode **ppNode          // Receives the node pointer.
	)
{
	IMFTopologyNode *pNode = NULL;

	HRESULT hr = S_OK;
	CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode));
	CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource));
	CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD));
	CHECK_HR(hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD));
	CHECK_HR(hr = pTopology->AddNode(pNode));

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

bail:
	SafeRelease(&pNode);
	return hr;
}

// Create the topology
//
// [source] -> (Transform) -> [SinkMain]
//         \-> (SinkPreview)
//
HRESULT MFUtils::CreateTopology(
	IMFMediaSource *pSource, // Media source
	IMFTransform *pTransform, // Transform filter (e.g. encoder or decoder) to insert between the source and Sink. NULL is valid.
	IMFActivate *pSinkActivateMain, // Main sink (e.g. sample grabber or EVR).
	IMFActivate *pSinkActivatePreview, // Preview sink. Optional. Could be NULL.
	const GUID& mediaType, // The MediaType
	IMFTopology **ppTopo // Receives the newly created topology
	)
{
	IMFTopology *pTopology = NULL;
	IMFPresentationDescriptor *pPD = NULL;
	IMFStreamDescriptor *pSD = NULL;
	IMFMediaTypeHandler *pHandler = NULL;
	IMFTopologyNode *pNodeSource = NULL;
	IMFTopologyNode *pNodeSinkMain = NULL;
	IMFTopologyNode *pNodeSinkPreview = NULL;
	IMFTopologyNode *pNodeTransform = NULL;
	IMFTopologyNode *pNodeTee = NULL;
	IMFMediaType *pMediaType = NULL;

	HRESULT hr = S_OK;
	DWORD cStreams = 0;
	BOOL bSourceFound = FALSE;

	CHECK_HR(hr = MFCreateTopology(&pTopology));
	CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
	CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

	for (DWORD i = 0; i < cStreams; i++)
	{
		BOOL fSelected = FALSE;
		GUID majorType;

		CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));
		CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
		CHECK_HR(hr = pHandler->GetMajorType(&majorType));

		if (majorType == mediaType && fSelected)
		{
			CHECK_HR(hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pNodeSource));
			CHECK_HR(hr = pNodeSource->SetTopoNodeID(MFUtils::g_ullTopoIdSource));
			CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivateMain, 0, &pNodeSinkMain));
			CHECK_HR(hr = pNodeSinkMain->SetTopoNodeID(MFUtils::g_ullTopoIdSinkMain));
			CHECK_HR(hr = MFUtils::BindOutputNode(pNodeSinkMain)); // To avoid MF_E_TOPO_SINK_ACTIVATES_UNSUPPORTED
			if(pSinkActivatePreview)
			{
				CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivatePreview, 0, &pNodeSinkPreview));
				CHECK_HR(hr = pNodeSinkPreview->SetTopoNodeID(MFUtils::g_ullTopoIdSinkPreview));
				CHECK_HR(hr = MFUtils::BindOutputNode(pNodeSinkPreview));

				CHECK_HR(hr = MFCreateTopologyNode(MF_TOPOLOGY_TEE_NODE, &pNodeTee));
				CHECK_HR(hr = pTopology->AddNode(pNodeTee));
			}

			if(pTransform)
			{
				CHECK_HR(hr = AddTransformNode(pTopology, pTransform, 0, &pNodeTransform));			
				
				if(pNodeTee)
				{	
					// Connect(Source -> Tee)
					CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeTee, 0));
					// Connect(Tee -> Transform)
					CHECK_HR(hr = pNodeTee->ConnectOutput(0, pNodeTransform, 0));
					// Connect(Transform -> SinkMain)
					CHECK_HR(hr = pNodeTransform->ConnectOutput(0, pNodeSinkMain, 0));
					// Connect(Tee -> SinkPreview)
					CHECK_HR(hr = pNodeTee->ConnectOutput(1, pNodeSinkPreview, 0));
				}
				else
				{
					// Connect (Source -> Transform)
					CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeTransform, 0));
					// Connect(Transform -> SinkMain)
					CHECK_HR(hr = pNodeTransform->ConnectOutput(0, pNodeSinkMain, 0));
				}
			}
			else
			{	
				if(pNodeTee)
				{
					// Connect(Source -> Tee)
					CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeTee, 0));
					// Connect(Tee -> SinkMain)
					CHECK_HR(hr = pNodeTee->ConnectOutput(0, pNodeSinkMain, 0));
					// Connect(Tee -> SinkPreview)
					CHECK_HR(hr = pNodeTee->ConnectOutput(1, pNodeSinkPreview, 0));
				}
				else
				{
					// Connect(Source -> SinkMain)
					CHECK_HR(hr = pNodeSource->ConnectOutput(0, pNodeSinkMain, 0));
				}
			}
			bSourceFound = TRUE;
			break;
		}
		else
		{
			CHECK_HR(hr = pPD->DeselectStream(i));
		}
		SafeRelease(&pSD);
		SafeRelease(&pHandler);
	}

	*ppTopo = pTopology;
	(*ppTopo)->AddRef();

bail:
	SafeRelease(&pTopology);
	SafeRelease(&pNodeSource);
	SafeRelease(&pNodeSinkMain);
	SafeRelease(&pNodeSinkPreview);
	SafeRelease(&pNodeTransform);
	SafeRelease(&pNodeTee);
	SafeRelease(&pPD);
	SafeRelease(&pSD);
	SafeRelease(&pHandler);
	SafeRelease(&pMediaType);

	if(!bSourceFound)
	{
		TSK_DEBUG_ERROR("No source node found");
		return E_NOT_SET;
	}

	return hr;
}

// Creates a fully loaded topology from the input partial topology.
HRESULT MFUtils::ResolveTopology(
	IMFTopology *pInputTopo, // A pointer to the IMFTopology interface of the partial topology to be resolved.
	IMFTopology **ppOutputTopo, // Receives a pointer to the IMFTopology interface of the completed topology. The caller must release the interface.
	IMFTopology *pCurrentTopo /*= NULL*/ // A pointer to the IMFTopology interface of the previous full topology. The topology loader can re-use objects from this topology in the new topology. This parameter can be NULL.
	)
{
	assert(ppOutputTopo && pInputTopo);
	
	HRESULT hr = S_OK;
	IMFTopoLoader* pTopoLoader = NULL;
	
	*ppOutputTopo = NULL;
	
	CHECK_HR(hr = MFCreateTopoLoader(&pTopoLoader));
	CHECK_HR(hr = pTopoLoader->Load(pInputTopo, ppOutputTopo, pCurrentTopo));
	
bail:
	SafeRelease(&pTopoLoader);
	return hr;
}

HRESULT MFUtils::FindNodeObject(
	IMFTopology *pInputTopo, // The Topology containing the node to find
	TOPOID qwTopoNodeID, //The identifier for the node
	void** ppObject // Receives the Object
	)
{
	assert(pInputTopo && ppObject);

	*ppObject = NULL;

	IMFTopologyNode *pNode = NULL;
	HRESULT hr = S_OK;

	CHECK_HR(hr = pInputTopo->GetNodeByID(qwTopoNodeID, &pNode));
	CHECK_HR(hr = pNode->GetObject((IUnknown**)ppObject));

bail:
	SafeRelease(&pNode);
	return hr;
}

//  Create an activation object for a renderer, based on the stream media type.
HRESULT MFUtils::CreateMediaSinkActivate(
    IMFStreamDescriptor *pSourceSD,     // Pointer to the stream descriptor.
    HWND hVideoWindow,                  // Handle to the video clipping window.
    IMFActivate **ppActivate
)
{
	HRESULT hr = S_OK;
    IMFMediaTypeHandler *pHandler = NULL;
    IMFActivate *pActivate = NULL;

    // Get the media type handler for the stream.
    CHECK_HR(hr = pSourceSD->GetMediaTypeHandler(&pHandler));
    // Get the major media type.
    GUID guidMajorType;
    CHECK_HR(hr = pHandler->GetMajorType(&guidMajorType));
 
    // Create an IMFActivate object for the renderer, based on the media type.
    if (MFMediaType_Audio == guidMajorType)
    {
        // Create the audio renderer.
        CHECK_HR(hr = MFCreateAudioRendererActivate(&pActivate));
    }
    else if (MFMediaType_Video == guidMajorType)
    {
        // Create the video renderer.
        CHECK_HR(hr = MFCreateVideoRendererActivate(hVideoWindow, &pActivate));
    }
    else
    {
        // Unknown stream type. 
        hr = E_FAIL;
        // Optionally, you could deselect this stream instead of failing.
    }
    if (FAILED(hr))
    {
        goto bail;
    }
 
    // Return IMFActivate pointer to caller.
    *ppActivate = pActivate;
    (*ppActivate)->AddRef();

bail:
    SafeRelease(&pHandler);
    SafeRelease(&pActivate);
    return hr;
}

// Set source output media type
HRESULT MFUtils::SetMediaType(
    IMFMediaSource *pSource,        // Media source.
	IMFMediaType* pMediaType // Media Type.
	)
{
	assert(pSource && pMediaType);

	IMFPresentationDescriptor *pPD = NULL;
	IMFStreamDescriptor *pSD = NULL;
	IMFMediaTypeHandler *pHandler = NULL;

	HRESULT hr = S_OK;
	DWORD cStreams = 0;
	GUID inputMajorType;

	CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
	CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));
	CHECK_HR(hr = pMediaType->GetMajorType(&inputMajorType));

	for (DWORD i = 0; i < cStreams; i++)
	{
		BOOL fSelected = FALSE;
		GUID majorType;

		CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD));
		CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
		CHECK_HR(hr = pHandler->GetMajorType(&majorType));

		if (majorType == inputMajorType && fSelected)
		{
			CHECK_HR(hr = pHandler->SetCurrentMediaType(pMediaType));
		}
		else
		{
			CHECK_HR(hr = pPD->DeselectStream(i));
		}
		SafeRelease(&pSD);
		SafeRelease(&pHandler);
	}


bail:
	SafeRelease(&pPD);
	SafeRelease(&pSD);
	SafeRelease(&pHandler);

	return hr;
}

HRESULT MFUtils::SetVideoWindow(
    IMFTopology *pTopology,         // Topology.
    IMFMediaSource *pSource,        // Media source.
    HWND hVideoWnd                 // Window for video playback.
	)
{
	HRESULT hr = S_OK;
	IMFStreamDescriptor *pSD = NULL;
	IMFPresentationDescriptor *pPD = NULL;
    IMFActivate         *pSinkActivate = NULL;
    IMFTopologyNode     *pSourceNode = NULL;
    IMFTopologyNode     *pOutputNode = NULL;
	DWORD cStreams = 0, iStream;
		
	CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
	CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

	for(iStream = 0; iStream < cStreams; ++iStream)
	{
		BOOL fSelected = FALSE;

		CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD));

		if (fSelected)
		{
			// Create the media sink activation object.
			CHECK_HR(hr = CreateMediaSinkActivate(pSD, hVideoWnd, &pSinkActivate));
			// Add a source node for this stream.
			CHECK_HR(hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode));
			// Create the output node for the renderer.
			CHECK_HR(hr = AddOutputNode(pTopology, pSinkActivate, 0, &pOutputNode));
			// Connect the source node to the output node.
			CHECK_HR(hr = pSourceNode->ConnectOutput(0, pOutputNode, 0));
		}
		// else: If not selected, don't add the branch. 
	}

bail:
	SafeRelease(&pPD);
    SafeRelease(&pSD);
    SafeRelease(&pSinkActivate);
    SafeRelease(&pSourceNode);
    SafeRelease(&pOutputNode);
    return hr;
}

// Run the session
HRESULT MFUtils::RunSession(
	IMFMediaSession *pSession, // Session to run
	IMFTopology *pTopology // The toppology
	)
{
	assert(pSession && pTopology);

	IMFMediaEvent *pEvent = NULL;

	PROPVARIANT var;
	PropVariantInit(&var);

	MediaEventType met;
	HRESULT hrStatus = S_OK;
	HRESULT hr = S_OK;
	CHECK_HR(hr = pSession->SetTopology(0, pTopology));
	CHECK_HR(hr = pSession->Start(&GUID_NULL, &var));

	// Check first event
	hr = pSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
	if(hr == MF_E_NO_EVENTS_AVAILABLE){
		hr = S_OK;
		goto bail;
	}
	CHECK_HR(hr = pEvent->GetStatus(&hrStatus));
	if (FAILED(hrStatus))
	{
		CHECK_HR(hr = pEvent->GetType(&met));
		TSK_DEBUG_ERROR("Session error: 0x%x (event id: %d)\n", hrStatus, met);
		hr = hrStatus;
		goto bail;
	}

bail:
	SafeRelease(&pEvent);
	return hr;
}

// Stop session
HRESULT MFUtils::ShutdownSession(
	IMFMediaSession *pSession, // The Session
	IMFMediaSource *pSource // Source to shutdown (optional)
	)
{
	// MUST be source then session
	if(pSource){
		pSource->Shutdown();
	}
	if(pSession){
		pSession->Shutdown();
	}
	return S_OK;
}

// Pause session
HRESULT MFUtils::PauseSession(
	IMFMediaSession *pSession, // The session
	IMFMediaSource *pSource // Source to pause (optional)
	)
{
	if(!pSession){
		return E_INVALIDARG;
	}
	if(pSource){
		pSource->Pause();
	}
	return pSession->Pause();
}

// Returns -1 if none is supported
INT MFUtils::GetSupportedSubTypeIndex(
	IMFMediaSource *pSource, // The source
	const GUID& mediaType, // The MediaType
	const VideoSubTypeGuidPair* subTypes, UINT subTypesCount // List of preferred subtypes (in ascending order)
	)
{
	assert(pSource);

	IMFPresentationDescriptor *pPD = NULL;
	IMFStreamDescriptor *pSD = NULL;
	IMFMediaTypeHandler *pHandler = NULL;
	IMFMediaType *pMediaType = NULL;

	INT nIndex = -1;
	HRESULT hr = S_OK;
	DWORD cStreams = 0, cMediaTypesCount;
	GUID majorType, subType;
	BOOL fSelected;
	
	CHECK_HR(hr = pSource->CreatePresentationDescriptor(&pPD));
	CHECK_HR(hr = pPD->GetStreamDescriptorCount(&cStreams));

	for (UINT subTypesIndex = 0; subTypesIndex < subTypesCount && nIndex == -1; ++subTypesIndex)
	{
		for (DWORD cStreamIndex = 0; cStreamIndex < cStreams && nIndex == -1; ++cStreamIndex)
		{
			fSelected = FALSE;
			CHECK_HR(hr = pPD->GetStreamDescriptorByIndex(cStreamIndex, &fSelected, &pSD));
			if(fSelected)
			{
				CHECK_HR(hr = pSD->GetMediaTypeHandler(&pHandler));
				CHECK_HR(hr = pHandler->GetMajorType(&majorType));
				if(majorType == mediaType)
				{
					CHECK_HR(hr = pHandler->GetMediaTypeCount(&cMediaTypesCount));
					for(DWORD cMediaTypesIndex = 0; cMediaTypesIndex < cMediaTypesCount && nIndex == -1; ++cMediaTypesIndex)
					{
						CHECK_HR(hr = pHandler->GetMediaTypeByIndex(cMediaTypesIndex, &pMediaType));
						CHECK_HR(hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &subType));
						if (subTypes[subTypesIndex].fourcc == subType)
						{
							nIndex = subTypesIndex;
							break;
						}
						SafeRelease(&pMediaType);
					}
				}
			}
			
			SafeRelease(&pSD);
			SafeRelease(&pHandler);
		}
	}

bail:
	SafeRelease(&pMediaType);
	SafeRelease(&pPD);
	SafeRelease(&pSD);
	SafeRelease(&pHandler);

	return nIndex;
}

HWND MFUtils::GetConsoleHwnd(void)
{
   #define MY_BUFSIZE 1024 // Buffer size for console window titles.
   HWND hwndFound;         // This is what is returned to the caller.
   TCHAR pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
                                       // WindowTitle.
   TCHAR pszOldWindowTitle[MY_BUFSIZE]; // Contains original
                                       // WindowTitle.

   // Fetch current window title.
   GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

   // Format a "unique" NewWindowTitle.
   wsprintf(pszNewWindowTitle,TEXT("%d/%d"),
               GetTickCount(),
               GetCurrentProcessId());

   // Change current window title.
   SetConsoleTitle(pszNewWindowTitle);

   // Ensure window title has been updated.
   Sleep(40);

   // Look for NewWindowTitle.
   hwndFound=FindWindow(NULL, pszNewWindowTitle);

   // Restore original window title.
   SetConsoleTitle(pszOldWindowTitle);

   return(hwndFound);
}

inline LONG Width(const RECT& r)
{
    return r.right - r.left;
}

inline LONG Height(const RECT& r)
{
    return r.bottom - r.top;
}

//-----------------------------------------------------------------------------
// CorrectAspectRatio
//
// Converts a rectangle from the source's pixel aspect ratio (PAR) to 1:1 PAR.
// Returns the corrected rectangle.
//
// For example, a 720 x 486 rect with a PAR of 9:10, when converted to 1x1 PAR,
// is stretched to 720 x 540.
// Copyright (C) Microsoft
//-----------------------------------------------------------------------------

RECT MFUtils::CorrectAspectRatio(const RECT& src, const MFRatio& srcPAR)
{
    // Start with a rectangle the same size as src, but offset to the origin (0,0).
    RECT rc = {0, 0, src.right - src.left, src.bottom - src.top};

    if ((srcPAR.Numerator != 1) || (srcPAR.Denominator != 1))
    {
        // Correct for the source's PAR.

        if (srcPAR.Numerator > srcPAR.Denominator)
        {
            // The source has "wide" pixels, so stretch the width.
            rc.right = MulDiv(rc.right, srcPAR.Numerator, srcPAR.Denominator);
        }
        else if (srcPAR.Numerator < srcPAR.Denominator)
        {
            // The source has "tall" pixels, so stretch the height.
            rc.bottom = MulDiv(rc.bottom, srcPAR.Denominator, srcPAR.Numerator);
        }
        // else: PAR is 1:1, which is a no-op.
    }
    return rc;
}

//-------------------------------------------------------------------
// LetterBoxDstRect
//
// Takes a src rectangle and constructs the largest possible
// destination rectangle within the specifed destination rectangle
// such thatthe video maintains its current shape.
//
// This function assumes that pels are the same shape within both the
// source and destination rectangles.
// Copyright (C) Microsoft
//-------------------------------------------------------------------

RECT MFUtils::LetterBoxRect(const RECT& rcSrc, const RECT& rcDst)
{
    // figure out src/dest scale ratios
    int iSrcWidth  = Width(rcSrc);
    int iSrcHeight = Height(rcSrc);

    int iDstWidth  = Width(rcDst);
    int iDstHeight = Height(rcDst);

    int iDstLBWidth;
    int iDstLBHeight;

    if (MulDiv(iSrcWidth, iDstHeight, iSrcHeight) <= iDstWidth) {

        // Column letter boxing ("pillar box")

        iDstLBWidth  = MulDiv(iDstHeight, iSrcWidth, iSrcHeight);
        iDstLBHeight = iDstHeight;
    }
    else {

        // Row letter boxing.

        iDstLBWidth  = iDstWidth;
        iDstLBHeight = MulDiv(iDstWidth, iSrcHeight, iSrcWidth);
    }


    // Create a centered rectangle within the current destination rect

    RECT rc;

    LONG left = rcDst.left + ((iDstWidth - iDstLBWidth) / 2);
    LONG top = rcDst.top + ((iDstHeight - iDstLBHeight) / 2);

    SetRect(&rc, left, top, left + iDstLBWidth, top + iDstLBHeight);

    return rc;
}