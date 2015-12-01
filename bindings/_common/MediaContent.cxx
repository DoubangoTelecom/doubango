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
#include "MediaContent.h"



MediaContent::MediaContent(tmedia_content_t* pContent)
: m_pData(tsk_null)
{
	m_pContent = (tmedia_content_t*)tsk_object_ref(pContent);
}

MediaContent::~MediaContent()
{
	TSK_OBJECT_SAFE_FREE(m_pContent);
	TSK_OBJECT_SAFE_FREE(m_pData);
}

const char* MediaContent::getType()
{
	if(m_pContent){
		return m_pContent->type;
	}
	return tsk_null;
}

unsigned MediaContent::getDataLength()
{
	if(!m_pContent){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(!m_pData){
		m_pData = tmedia_content_get_data(m_pContent);
	}

	return (m_pData ? m_pData->size : 0);
}

unsigned MediaContent::getData(void* pOutput, unsigned nMaxsize)
{
	unsigned nRetsize = 0;

	if(!m_pContent){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(!m_pData){
		m_pData = tmedia_content_get_data(m_pContent);
	}	
		
	if(pOutput && nMaxsize && m_pData){
		nRetsize = (m_pData->size > nMaxsize) ? nMaxsize : m_pData->size;
		memcpy(pOutput, m_pData->data, nRetsize);
	}

	return nRetsize;
}

MediaContent* MediaContent::parse(const void* pData, unsigned nSize, const char* pType)
{
	MediaContent* pMediaContent = tsk_null;

	tmedia_content_t* pContent = tmedia_content_parse(pData, (tsk_size_t)nSize, pType);
	if(pContent){
		if(TMEDIA_CONTENT_IS_CPIM(pContent)){
			pMediaContent = new MediaContentCPIM(pContent);
		}
		else if(TMEDIA_CONTENT_IS_DUMMY(pContent)){
			// Todo
		}
		TSK_OBJECT_SAFE_FREE(pContent);
	}

	return pMediaContent;
}

MediaContentCPIM* MediaContent::parse(const void* pData, unsigned nSize)
{
	MediaContent* pMediaContent;
	if((pMediaContent = MediaContent::parse(pData, nSize, TMEDIA_CONTENT_CPIM_TYPE))){
		return dyn_cast<MediaContentCPIM*>(pMediaContent);
	}
	return tsk_null;
}

/* ============ message/CPIM ================= */

MediaContentCPIM::MediaContentCPIM(tmedia_content_t* pContent)
: MediaContent(pContent)
{
}

MediaContentCPIM::~MediaContentCPIM()
{
}

unsigned MediaContentCPIM::getPayloadLength()
{
	if(!m_pContent || !TMEDIA_CONTENT_IS_CPIM(m_pContent)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	return (TMEDIA_CONTENT_CPIM(m_pContent)->e ? TMEDIA_CONTENT_CPIM(m_pContent)->e->size : 0);
}

unsigned MediaContentCPIM::getPayload(void* pOutput, unsigned nMaxsize)
{
	unsigned nRetsize = 0;

	if(!m_pContent || !TMEDIA_CONTENT_IS_CPIM(m_pContent)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(pOutput && nMaxsize && TMEDIA_CONTENT_CPIM(m_pContent)->e){
		nRetsize = (TMEDIA_CONTENT_CPIM(m_pContent)->e->size > nMaxsize) ? nMaxsize : TMEDIA_CONTENT_CPIM(m_pContent)->e->size;
		memcpy(pOutput, TMEDIA_CONTENT_CPIM(m_pContent)->e->data, nRetsize);
	}

	return nRetsize;
}

const void* MediaContentCPIM::getPayloadPtr(){
	if(!m_pContent || !TMEDIA_CONTENT_IS_CPIM(m_pContent)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return tsk_null;
	}
	
	return TMEDIA_CONTENT_CPIM(m_pContent)->e ? TMEDIA_CONTENT_CPIM(m_pContent)->e->data : tsk_null;
}

const char* MediaContentCPIM::getHeaderValue(const char* name)
{
	const tmedia_content_cpim_t* cpim;
	const tsk_list_item_t* item;

	if(!m_pContent || !TMEDIA_CONTENT_IS_CPIM(m_pContent)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return tsk_null;
	}

	cpim = TMEDIA_CONTENT_CPIM(m_pContent);
	tsk_list_foreach(item, cpim->h_headers){
		if(tsk_striequals(name, TMEDIA_CONTENT_HEADER(item->data)->name)){
			return TMEDIA_CONTENT_HEADER(item->data)->value;
		}
	}
	tsk_list_foreach(item, cpim->m_headers){
		if(tsk_striequals(name, TMEDIA_CONTENT_HEADER(item->data)->name)){
			return TMEDIA_CONTENT_HEADER(item->data)->value;
		}
	}

	return tsk_null;
}