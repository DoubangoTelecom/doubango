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
#ifndef TINYWRAP_MEDIA_CONTENT_H
#define TINYWRAP_MEDIA_CONTENT_H

#include "tinymedia.h"
#include "Common.h"

class MediaContentCPIM;

class MediaContent
{
public:
#if !defined(SWIG)
	MediaContent(tmedia_content_t* content);
#endif
	virtual ~MediaContent();

public:
	const char* getType();
	virtual unsigned getDataLength();
	virtual unsigned getData(void* pOutput, unsigned nMaxsize);
	
	// SWIG %newobject()
	static MediaContent* parse(const void* pData, unsigned nSize, const char* pType);
	static MediaContentCPIM* parse(const void* pData, unsigned nSize);

	virtual unsigned getPayloadLength() = 0;
	virtual unsigned getPayload(void* pOutput, unsigned nMaxsize) = 0;

protected:
	tmedia_content_t* m_pContent;

private:
	tsk_buffer_t* m_pData;
};


/* ============ message/CPIM ================= */
class MediaContentCPIM : public MediaContent
{
public:
#if !defined(SWIG)
	MediaContentCPIM(tmedia_content_t* pContent);
#endif
	virtual ~MediaContentCPIM();

public:
	virtual unsigned getPayloadLength();
	virtual unsigned getPayload(void* pOutput, unsigned nMaxsize);
#if !defined(SWIG)
	const void* getPayloadPtr();
#endif
	const char* getHeaderValue(const char* pName);
};

#endif /*TINYWRAP_MEDIA_CONTENT_H*/
