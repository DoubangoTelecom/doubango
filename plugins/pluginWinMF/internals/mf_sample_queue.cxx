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
#include "mf_sample_queue.h"

#include <assert.h>

MFSampleQueue::MFSampleQueue()
: m_nRefCount(1)
, m_nCount(0)
{
	InitializeCriticalSection(&m_critSec);

	 m_anchor.next = &m_anchor;
     m_anchor.prev = &m_anchor;
}

MFSampleQueue::~MFSampleQueue()
{
	assert(m_nRefCount == 0);

	Clear();

	DeleteCriticalSection(&m_critSec);
}

STDMETHODIMP MFSampleQueue::QueryInterface(REFIID iid, void** ppv)
{
	return E_NOTIMPL;
}

STDMETHODIMP_(ULONG) MFSampleQueue::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

STDMETHODIMP_(ULONG) MFSampleQueue::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if (uCount == 0)
    {
        delete this;
    }
    // For thread safety, return a temporary variable.
    return uCount;
}

HRESULT MFSampleQueue::Queue(IMFSample* item)
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

	EnterCriticalSection(&m_critSec);

    Node *pBefore = m_anchor.prev;

    Node *pAfter = pBefore->next;

    pBefore->next = pNode;
    pAfter->prev = pNode;

    pNode->prev = pBefore;
    pNode->next = pAfter;

	m_nCount++;

	LeaveCriticalSection(&m_critSec);

    return S_OK;
}

HRESULT MFSampleQueue::Dequeue(IMFSample* *ppItem)
{
	if (ppItem == NULL)
    {
        return E_POINTER;
    }

	EnterCriticalSection(&m_critSec);

	if (IsEmpty())
    {
		LeaveCriticalSection(&m_critSec);
        return E_FAIL;
    }

    Node *pNode = m_anchor.next;

    // The next node's previous is this node's previous.
    pNode->next->prev = m_anchor.next->prev;

    // The previous node's next is this node's next.
    pNode->prev->next = pNode->next;

    *ppItem = pNode->item;
    delete pNode;

	m_nCount--;

	LeaveCriticalSection(&m_critSec);

    return S_OK;
}

HRESULT MFSampleQueue::Clear()
{
	EnterCriticalSection(&m_critSec);

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

	m_nCount = 0;

	LeaveCriticalSection(&m_critSec);

	return S_OK;
}
