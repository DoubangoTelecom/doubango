/*
* Copyright (C) 2009-2010 Mamadou Diop.
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



MediaContent::MediaContent(tmedia_content_t* _content)
: data(tsk_null)
{
	this->content = (tmedia_content_t*)tsk_object_ref(_content);
}

MediaContent::~MediaContent()
{
	TSK_OBJECT_SAFE_FREE(this->content);
	TSK_OBJECT_SAFE_FREE(this->data);
}

const char* MediaContent::getType()
{
	if(this->content){
		return this->content->type;
	}
	return tsk_null;
}

unsigned MediaContent::getDataLength()
{
	if(!this->content){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(!this->data){
		this->data = tmedia_content_get_data(this->content);
	}

	return (this->data ? this->data->size : 0);
}

unsigned MediaContent::getData(void* output, unsigned maxsize)
{
	unsigned retsize = 0;

	if(!this->content){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(!this->data){
		this->data = tmedia_content_get_data(this->content);
	}	
		
	if(output && maxsize && this->data){
		retsize = (this->data->size > maxsize) ? maxsize : this->data->size;
		memcpy(output, this->data->data, retsize);
	}

	return retsize;
}

MediaContent* MediaContent::parse(const void* data, unsigned size, const char* type)
{
	MediaContent* mediaContent = tsk_null;

	tmedia_content_t* _content = tmedia_content_parse(data, (tsk_size_t)size, type);
	if(_content){
		if(TMEDIA_CONTENT_IS_CPIM(_content)){
			mediaContent = new MediaContentCPIM(_content);
		}
		else if(TMEDIA_CONTENT_IS_DUMMY(_content)){
			// Todo
		}
		TSK_OBJECT_SAFE_FREE(_content);
	}

	return mediaContent;
}

MediaContentCPIM* MediaContent::parse(const void* data, unsigned size)
{
	MediaContent* mediaContent;
	if((mediaContent = MediaContent::parse(data, size, TMEDIA_CONTENT_CPIM_TYPE))){
		return dyn_cast<MediaContentCPIM*>(mediaContent);
	}
	return tsk_null;
}

/* ============ message/CPIM ================= */

MediaContentCPIM::MediaContentCPIM(tmedia_content_t* _content)
: MediaContent(_content)
{
}

MediaContentCPIM::~MediaContentCPIM()
{
}

unsigned MediaContentCPIM::getPayloadLength()
{
	if(!this->content || !TMEDIA_CONTENT_IS_CPIM(this->content)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	return (TMEDIA_CONTENT_CPIM(this->content)->e ? TMEDIA_CONTENT_CPIM(this->content)->e->size : 0);
}

unsigned MediaContentCPIM::getPayload(void* output, unsigned maxsize)
{
	unsigned retsize = 0;

	if(!this->content || !TMEDIA_CONTENT_IS_CPIM(this->content)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return 0;
	}

	if(output && maxsize && TMEDIA_CONTENT_CPIM(this->content)->e){
		retsize = (TMEDIA_CONTENT_CPIM(this->content)->e->size > maxsize) ? maxsize : TMEDIA_CONTENT_CPIM(this->content)->e->size;
		memcpy(output, TMEDIA_CONTENT_CPIM(this->content)->e->data, retsize);
	}

	return retsize;
}

const char* MediaContentCPIM::getHeaderValue(const char* name)
{
	const tmedia_content_cpim_t* cpim;
	const tsk_list_item_t* item;

	if(!this->content || !TMEDIA_CONTENT_IS_CPIM(this->content)){
		TSK_DEBUG_ERROR("Invalid internal object");
		return tsk_null;
	}

	cpim = TMEDIA_CONTENT_CPIM(this->content);
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