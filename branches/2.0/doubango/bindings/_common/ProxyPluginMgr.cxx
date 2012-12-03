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
#include "ProxyPluginMgr.h"

#include "ProxyConsumer.h"
#include "ProxyProducer.h"

//
//	"twrap_proxy_plugin_t" Declarations
//
typedef struct twrap_proxy_plugin_s
{
	TSK_DECLARE_OBJECT;
	ProxyPlugin* plugin;
}
twrap_proxy_plugin_t;
#define TWRAP_PROXY_PLUGIN(self) ((twrap_proxy_plugin_t*)(self))
static int pred_find_plugin_by_value(const tsk_list_item_t *item, const void *proxyPlugin);
static twrap_proxy_plugin_t* twrap_proxy_plugin_create(ProxyPlugin** plugin);


//
//	"ProxyPluginMgr" Class Implementation
//
ProxyPluginMgr* ProxyPluginMgr::instance = tsk_null;
static uint64_t __uniqueId = 0;

ProxyPluginMgr::ProxyPluginMgr(ProxyPluginMgrCallback* _callback)
:callback(_callback)
{
	this->plugins = tsk_list_create();
	if(!this->callback){
		TSK_DEBUG_WARN("Callback function is Null => You will have big problems as we won't check it before call");
	}
}

ProxyPluginMgr::~ProxyPluginMgr()
{
	if(this == ProxyPluginMgr::instance){
		ProxyPluginMgr::instance = tsk_null;
	}
	TSK_OBJECT_SAFE_FREE(this->plugins);
}

ProxyPluginMgr* ProxyPluginMgr::createInstance(ProxyPluginMgrCallback* pCallback)
{
	if(!ProxyPluginMgr::instance){
		ProxyPluginMgr::instance = new ProxyPluginMgr(pCallback);
	}
	else{
		TSK_DEBUG_WARN("Plugin instance already exist");
		ProxyPluginMgr::instance->callback = pCallback;
	}
	return ProxyPluginMgr::instance;
}

void ProxyPluginMgr::destroyInstance(ProxyPluginMgr** ppInstance)
{
	if(ppInstance && *ppInstance){
		bool bMatch = ProxyPluginMgr::instance && (*ppInstance == ProxyPluginMgr::instance);
		delete *ppInstance, *ppInstance = tsk_null;
		if(bMatch){
			ProxyPluginMgr::instance = tsk_null;
		}
	}
}

ProxyPluginMgr* ProxyPluginMgr::getInstance()
{
	if(!ProxyPluginMgr::instance){
		TSK_DEBUG_ERROR("No instance of the manager could be found");
	}
	return ProxyPluginMgr::instance;
}

uint64_t ProxyPluginMgr::getUniqueId()
{
	return ++__uniqueId;
}

int ProxyPluginMgr::addPlugin(ProxyPlugin** plugin)
{
	twrap_proxy_plugin_t* twrap_plugin;
	int ret = -1;

	tsk_list_lock(this->plugins);

	if(!plugin || !*plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		goto bail;
	}

	if(tsk_list_find_item_by_pred(this->plugins, pred_find_plugin_by_value, *plugin)){
		TSK_DEBUG_ERROR("Plugin already exist");
		goto bail;
	}
	
	if((twrap_plugin = twrap_proxy_plugin_create(plugin))){
		tsk_list_push_back_data(this->plugins, (void**)&twrap_plugin);
		ret = 0;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create plugin");
		goto bail;
	}

bail:
	tsk_list_unlock(this->plugins);

	return ret;
}

int ProxyPluginMgr::removePlugin(ProxyPlugin** plugin)
{
	if(!plugin || !*plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return this->removePlugin((*plugin)->getId());
}

const ProxyPlugin* ProxyPluginMgr::findPlugin(uint64_t id)
{
	ProxyPlugin* ret = tsk_null;

	tsk_list_item_t* item;

	tsk_list_lock(this->plugins);
	tsk_list_foreach(item, this->plugins){
		if(TWRAP_PROXY_PLUGIN(item->data)->plugin->getId() == id){
			ret = TWRAP_PROXY_PLUGIN(item->data)->plugin;
			break;
		}
	}
	tsk_list_unlock(this->plugins);

	return ret;
}

const ProxyPlugin* ProxyPluginMgr::findPlugin(tsk_object_t* wrapped_plugin)
{
	ProxyPlugin* ret = tsk_null;

	tsk_list_item_t* item;

	if(!wrapped_plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	tsk_list_lock(this->plugins);
	tsk_list_foreach(item, this->plugins){
		if(TWRAP_PROXY_PLUGIN(item->data)->plugin->isWrapping(wrapped_plugin)){
			ret = TWRAP_PROXY_PLUGIN(item->data)->plugin;
			break;
		}
	}
	tsk_list_unlock(this->plugins);

	return ret;
}

int ProxyPluginMgr::removePlugin(uint64_t id)
{
	tsk_list_item_t* item;

	tsk_list_lock(this->plugins);

	tsk_list_foreach(item, this->plugins){
		if(TWRAP_PROXY_PLUGIN(item->data)->plugin->getId() == id){
			tsk_list_remove_item(this->plugins, item);
			break;
		}
	}

	tsk_list_unlock(this->plugins);
	return 0;
}

const ProxyAudioConsumer* ProxyPluginMgr::findAudioConsumer(uint64_t id)
{
	const ProxyPlugin* audioConsumer = this->findPlugin(id);
	if(audioConsumer && audioConsumer->getType() == twrap_proxy_plugin_audio_consumer){
		return dyn_cast<const ProxyAudioConsumer*>(audioConsumer);
	}
	return tsk_null;
}

const ProxyVideoConsumer* ProxyPluginMgr::findVideoConsumer(uint64_t id)
{
	const ProxyPlugin* videoConsumer = this->findPlugin(id);
	if(videoConsumer && videoConsumer->getType() == twrap_proxy_plugin_video_consumer){
		return dyn_cast<const ProxyVideoConsumer*>(videoConsumer);
	}
	return tsk_null;
}

const ProxyAudioProducer* ProxyPluginMgr::findAudioProducer(uint64_t id)
{
	const ProxyPlugin* audioProducer = this->findPlugin(id);
	if(audioProducer && audioProducer->getType() == twrap_proxy_plugin_audio_producer){
		return dyn_cast<const ProxyAudioProducer*>(audioProducer);
	}
	return tsk_null;
}

const ProxyVideoProducer* ProxyPluginMgr::findVideoProducer(uint64_t id)
{
	const ProxyPlugin* videoProducer = this->findPlugin(id);
	if(videoProducer && videoProducer->getType() == twrap_proxy_plugin_video_producer){
		return dyn_cast<const ProxyVideoProducer*>(videoProducer);
	}
	return tsk_null;
}


//
//	"twrap_proxy_plugin_t" Implementations
//
static tsk_object_t* twrap_proxy_plugin_ctor(tsk_object_t * self, va_list * app)
{
	twrap_proxy_plugin_t *_self = dyn_cast<twrap_proxy_plugin_t *>(TWRAP_PROXY_PLUGIN(self));
	if(_self){
	}
	return self;
}

static tsk_object_t* twrap_proxy_plugin_dtor(tsk_object_t * self)
{ 
	twrap_proxy_plugin_t *_self = dyn_cast<twrap_proxy_plugin_t *>(TWRAP_PROXY_PLUGIN(self));
	if(_self){
		if(_self->plugin){
			delete _self->plugin, _self->plugin = tsk_null;
		}
	}

	return self;
}

static int twrap_proxy_plugin_cmp(const tsk_object_t *_c1, const tsk_object_t *_c2)
{
	const twrap_proxy_plugin_t *c1 = dyn_cast<const twrap_proxy_plugin_t *>(TWRAP_PROXY_PLUGIN(_c1));
	const twrap_proxy_plugin_t *c2 = dyn_cast<const twrap_proxy_plugin_t *>(TWRAP_PROXY_PLUGIN(_c2));

	if(c1 && c2){
		return (c1->plugin == c2->plugin); // See "ProxyPlugin::operator =="
	}
	else if(!c1 && !c2) return 0;
	else return -1;
}

static const tsk_object_def_t twrap_proxy_plugin_def_s = 
{
	sizeof(twrap_proxy_plugin_t),
	twrap_proxy_plugin_ctor, 
	twrap_proxy_plugin_dtor,
	twrap_proxy_plugin_cmp, 
};
const tsk_object_def_t *twrap_proxy_plugin_def_t = &twrap_proxy_plugin_def_s;

static int pred_find_plugin_by_value(const tsk_list_item_t *item, const void *proxyPlugin)
{
	if(item && item->data){
		const twrap_proxy_plugin_t *twrap_plugin = dyn_cast<const twrap_proxy_plugin_t *>(TWRAP_PROXY_PLUGIN(item->data));
		return (twrap_plugin->plugin == dyn_cast<const ProxyPlugin *>((const ProxyPlugin*)proxyPlugin)) ? 0 : -1;
	}
	return -1;
}

static twrap_proxy_plugin_t* twrap_proxy_plugin_create(ProxyPlugin** plugin)
{
	if(!plugin || !*plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	twrap_proxy_plugin_t* twrap_plugin = (twrap_proxy_plugin_t*)tsk_object_new(twrap_proxy_plugin_def_t);
	if(!twrap_plugin){
		TSK_DEBUG_ERROR("Failed to create new instance of 'twrap_proxy_plugin_t'");
		return tsk_null;
	}

	twrap_plugin->plugin = *plugin, 
		*plugin = tsk_null;
	return twrap_plugin;
}

