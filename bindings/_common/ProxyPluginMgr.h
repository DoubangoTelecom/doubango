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
#ifndef TINYWRAP_PROXY_PLUGIN_MGR_H
#define TINYWRAP_PROXY_PLUGIN_MGR_H

#include "tinymedia.h"
#include "Common.h"

class ProxyPlugin;
class ProxyConsumer;
class ProxyAudioConsumer;
class ProxyVideoConsumer;
class ProxyAudioProducer;
class ProxyVideoProducer;
class ProxyPluginMgrCallback;

typedef enum twrap_proxy_plugin_type_e
{
	twrap_proxy_plugin_audio_producer,
	twrap_proxy_plugin_video_producer,
	twrap_proxy_plugin_audio_consumer,
	twrap_proxy_plugin_video_consumer,
}
twrap_proxy_plugin_type_t;

/* ============ ProxyPluginMgr Class ================= */

typedef tsk_list_t twrap_proxy_plungins_L_t; // contains "twrap_proxy_plungin_t" elements

class ProxyPluginMgr
{
private:
	ProxyPluginMgr(ProxyPluginMgrCallback* callback);
public:
	virtual ~ProxyPluginMgr();

	// SWIG %newobject
	static ProxyPluginMgr* createInstance(ProxyPluginMgrCallback* pCallback);
#if !defined(SWIG)
	static void destroyInstance(ProxyPluginMgr** ppInstance);
#endif
	static ProxyPluginMgr* getInstance();

#if !defined(SWIG)
	static uint64_t getUniqueId();

	int addPlugin(ProxyPlugin**);
	const ProxyPlugin* findPlugin(tsk_object_t* wrapped_plugin);
	int removePlugin(uint64_t id);
	int removePlugin(ProxyPlugin**);

	inline ProxyPluginMgrCallback* getCallback(){ return this->callback; }
#endif

	const ProxyPlugin* findPlugin(uint64_t id);

	const ProxyAudioConsumer* findAudioConsumer(uint64_t id);
	const ProxyVideoConsumer* findVideoConsumer(uint64_t id);
	const ProxyAudioProducer* findAudioProducer(uint64_t id);
	const ProxyVideoProducer* findVideoProducer(uint64_t id);

private:
	static ProxyPluginMgr* instance;
	ProxyPluginMgrCallback* callback;

	twrap_proxy_plungins_L_t* plugins;
};


/* ============ ProxyPluginMgrCallback Class ================= */
class ProxyPluginMgrCallback
{
public:
	ProxyPluginMgrCallback() { }
	virtual ~ProxyPluginMgrCallback() { }

	virtual int OnPluginCreated(uint64_t id, enum twrap_proxy_plugin_type_e type) { return -1; }
	virtual int OnPluginDestroyed(uint64_t id, enum twrap_proxy_plugin_type_e type) { return -1; }
};

/* ============ ProxyPlugin Class ================= */
class ProxyPlugin
{
public:
#if !defined SWIG
	ProxyPlugin(twrap_proxy_plugin_type_t _type) { 
		this->type=_type;
		this->id = ProxyPluginMgr::getUniqueId();
	}
#endif
	virtual ~ProxyPlugin() {}
	
#if !defined(SWIG)
	virtual bool operator ==(const ProxyPlugin &plugin)const{
		return this->getId() == plugin.getId();
	}
	virtual inline bool isWrapping(tsk_object_t* wrapped_plugin) = 0;
	virtual inline uint64_t getMediaSessionId() = 0;
#endif

	inline twrap_proxy_plugin_type_t getType()const{ return this->type; }
	inline uint64_t getId()const{ return this->id; }

protected:
	uint64_t id;
	twrap_proxy_plugin_type_t type;
};

#endif /* TINYWRAP_PROXY_PLUGIN_MGR_H */
