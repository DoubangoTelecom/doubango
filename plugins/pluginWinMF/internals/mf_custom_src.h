/* 
* Copyright (C) Microsoft Corporation. All rights reserved.
* Copyright (C) 2013 Mamadou DIOP
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
#ifndef PLUGIN_WIN_MF_CUSTOM_SOURCE_H
#define PLUGIN_WIN_MF_CUSTOM_SOURCE_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <windows.h>
#include <assert.h>

#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mferror.h>
#include <shlwapi.h>

class CMFStreamSource;
class CMFSource;

LONGLONG AudioDurationFromBufferSize(const WAVEFORMATEX *pWav, DWORD cbAudioDataSize);


//////////////////////////////////////////////////////////////////////////
//  CMFSource
//  Description: Media source object.
//////////////////////////////////////////////////////////////////////////

class CMFSource : public IMFMediaSource
{
    friend class CMFStreamSource;

public:
    static HRESULT CreateInstance(REFIID iid, void **ppSource);
	static HRESULT CreateInstanceEx(REFIID iid, void **ppSource, IMFMediaType *pMediaType);

	// IMFCustomSource
	HRESULT CopyVideoBuffer(UINT32 nWidth, UINT32 nHeight, const void* pBufferPtr, UINT32 nBufferSize);

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFMediaEventGenerator
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback,IUnknown* punkState);
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent);
    STDMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent);
    STDMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue);

    // IMFMediaSource
    STDMETHODIMP CreatePresentationDescriptor(IMFPresentationDescriptor** ppPresentationDescriptor);
    STDMETHODIMP GetCharacteristics(DWORD* pdwCharacteristics);
    STDMETHODIMP Pause();
    STDMETHODIMP Shutdown();
    STDMETHODIMP Start(
        IMFPresentationDescriptor* pPresentationDescriptor,
        const GUID* pguidTimeFormat,
        const PROPVARIANT* pvarStartPosition
    );
    STDMETHODIMP Stop();

private:

    enum State
    {
        STATE_STOPPED,
        STATE_PAUSED,
        STATE_STARTED
    };


    // Constructor is private - client should use static CreateInstance method.
    CMFSource(HRESULT &hr, IMFMediaType *pMediaType);
    virtual ~CMFSource();

    HRESULT CheckShutdown() const
    {
        if (m_IsShutdown)
        {
            return MF_E_SHUTDOWN;
        }
        else
        {
            return S_OK;
        }
    }

    HRESULT     CreatePresentationDescriptor();
    HRESULT     QueueNewStreamEvent(IMFPresentationDescriptor *pPD);
    HRESULT     CreateCMFStreamSource(IMFStreamDescriptor *pSD);
    HRESULT     ValidatePresentationDescriptor(IMFPresentationDescriptor *pPD);

    LONGLONG    GetCurrentPosition() const;
    State       GetState() const { return m_state; }

    IMFMediaEventQueue          *m_pEventQueue;             // Event generator helper
    IMFPresentationDescriptor   *m_pPresentationDescriptor; // Default presentation

    CMFStreamSource				*m_pStream;                 // Media stream. Can be NULL is no stream is selected.

    long                        m_nRefCount;                // reference count
    CRITICAL_SECTION            m_critSec;
    BOOL                        m_IsShutdown;               // Flag to indicate if Shutdown() method was called.
    State                       m_state;                    // Current state (running, stopped, paused)

	IMFMediaType				*m_pMediaType;				// The supported mediaType
};


class SampleQueue
{
protected:

    // Nodes in the linked list
    struct Node
    {
        Node *prev;
        Node *next;
        IMFSample*  item;

        Node() : prev(NULL), next(NULL)
        {
        }

        Node(IMFSample* item) : prev(NULL), next(NULL)
        {
            this->item = item;
        }

        IMFSample* Item() const { return item; }
    };


protected:
    Node    m_anchor;  // Anchor node for the linked list.

public:

    SampleQueue()
    {
        m_anchor.next = &m_anchor;
        m_anchor.prev = &m_anchor;
    }

    virtual ~SampleQueue()
    {
        Clear();
    }

    HRESULT Queue(IMFSample* item)
    {
        if (item == NULL)
        {
            return E_POINTER;
        }

        Node *pNode = new (std::nothrow) Node(item);
        if (pNode == NULL)
        {
            return E_OUTOFMEMORY;
        }

        item->AddRef();

        Node *pBefore = m_anchor.prev;

        Node *pAfter = pBefore->next;

        pBefore->next = pNode;
        pAfter->prev = pNode;

        pNode->prev = pBefore;
        pNode->next = pAfter;

        return S_OK;

    }

    HRESULT Dequeue(IMFSample* *ppItem)
    {
        if (IsEmpty())
        {
            return E_FAIL;
        }
        if (ppItem == NULL)
        {
            return E_POINTER;
        }

        Node *pNode = m_anchor.next;

        // The next node's previous is this node's previous.
        pNode->next->prev = m_anchor.next->prev;

        // The previous node's next is this node's next.
        pNode->prev->next = pNode->next;

        *ppItem = pNode->item;
        delete pNode;

        return S_OK;
    }

    BOOL IsEmpty() const { return m_anchor.next == &m_anchor; }

    void Clear()
    {
        Node *n = m_anchor.next;

        // Delete the nodes
        while (n != &m_anchor)
        {
            if (n->item)
            {
                n->item->Release();
            }

            Node *tmp = n->next;
            delete n;
            n = tmp;
        }

        // Reset the anchor to point at itself
        m_anchor.next = &m_anchor;
        m_anchor.prev = &m_anchor;
    }

};



//////////////////////////////////////////////////////////////////////////
//  CMFStreamSource
//  Description: Media stream object.
//////////////////////////////////////////////////////////////////////////


class CMFStreamSource : public IMFMediaStream
{
    friend class CMFSource;

public:

	// IMFCustomSource
	HRESULT CopyVideoBuffer(UINT32 nWidth, UINT32 nHeight, const void* pBufferPtr, UINT32 nBufferSize);

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFMediaEventGenerator
    STDMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback,IUnknown* punkState);
    STDMETHODIMP EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent);
    STDMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent);
    STDMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue);

    // IMFMediaStream
    STDMETHODIMP GetMediaSource(IMFMediaSource** ppMediaSource);
    STDMETHODIMP GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor);
    STDMETHODIMP RequestSample(IUnknown* pToken);

private:

    CMFStreamSource(CMFSource *pSource, IMFStreamDescriptor *pSD, HRESULT& hr);
    ~CMFStreamSource();


    HRESULT CheckShutdown() const
    {
        if (m_IsShutdown)
        {
            return MF_E_SHUTDOWN;
        }
        else
        {
            return S_OK;
        }
    }

	HRESULT		InitializeParams();
    HRESULT     Shutdown();
    HRESULT     CreateSample(IMFSample **pSample);
    HRESULT     DeliverSample(IMFSample *pSample);
    HRESULT     DeliverQueuedSamples();
    HRESULT     Flush();

    LONGLONG    GetCurrentPosition() const { return m_rtCurrentPosition; }
    HRESULT     SetPosition(LONGLONG rtNewPosition);
    HRESULT     CheckEndOfStream();


    long                        m_nRefCount;            // reference count
    CRITICAL_SECTION            m_critSec;
    BOOL                        m_IsShutdown;           // Flag to indicate if source's Shutdown() method was called.
    LONGLONG                    m_rtCurrentPosition;    // Current position in the stream, in 100-ns units
	UINT64						m_rtDuration;			// Sample duration, in 100-ns units
    BOOL                        m_discontinuity;        // Is the next sample a discontinuity?
    BOOL                        m_EOS;                  // Did we reach the end of the stream?

    IMFMediaEventQueue          *m_pEventQueue;         // Event generator helper.
    CMFSource                   *m_pSource;             // Parent media source
    IMFStreamDescriptor         *m_pStreamDescriptor;   // Stream descriptor for this stream.

    SampleQueue                  m_sampleQueue;          // Queue for samples while paused.
	GUID						m_guidMajorType;		// major media type (e.g. MFMediaType_Video or MFMediaType_Audio)
	GUID						m_guidSubType;			// Media subtype (e.g. MFVideoFormat_RGB32 or MFVideoFormat_H264)
	IMFMediaBuffer				*m_pMediaBuffer;			// Pointer to the data to deliver
	UINT32						m_nBufferSize;			// Size of the data to deliver 
	 
	struct
	{
		UINT32 nWidth;
		UINT32 nHeigh;
		UINT32 nFps;
	} 
	m_structVideoParams;
};


#endif /* PLUGIN_WIN_MF_CUSTOM_SOURCE_H */
