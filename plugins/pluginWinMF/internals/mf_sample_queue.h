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
#ifndef PLUGIN_WIN_MF_SAMPLE_QUEUE_H
#define PLUGIN_WIN_MF_SAMPLE_QUEUE_H

#include "../plugin_win_mf_config.h"

#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <shlwapi.h>

class MFSampleQueue : public IUnknown
{
protected:

    // Nodes in the linked list
    struct Node {
        Node *prev;
        Node *next;
        IMFSample*  item;

        Node() : prev(NULL), next(NULL) {
        }

        Node(IMFSample* item) : prev(NULL), next(NULL) {
            this->item = item;
        }

        IMFSample* Item() const {
            return item;
        }
    };


protected:
    Node						m_anchor;
    long						m_nCount;
    CRITICAL_SECTION            m_critSec;

private:
    long						m_nRefCount;

public:

    MFSampleQueue();
    virtual ~MFSampleQueue();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    HRESULT Queue(IMFSample* item);
    HRESULT Dequeue(IMFSample**ppItem);
    HRESULT Clear();

    inline BOOL IsEmpty() const {
        return m_anchor.next == &m_anchor;
    }
    inline long Count() {
        return m_nCount;
    }
};


#endif /* PLUGIN_WIN_MF_SAMPLE_QUEUE_H */