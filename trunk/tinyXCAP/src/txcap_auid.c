/*
* Copyright (C) 2009 Mamadou Diop.
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
/**@file txcap_auid.h
 * @brief XCAP AUID manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyXCAP/txcap_auid.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"


typedef struct auid_s
{
	txcap_auid_type_t type;
	const char* id;
	const char* mime_type;
	const char* ns;
	const char* document_name;
	tsk_bool_t global;
}
auid_t;

/** List of all default auids
*/
static const auid_t __txcap_auids[] =
{
	/*== xcap-caps ==*/{	
		tauid_ietf_xcap_caps, 
		"xcap-caps", 
		"application/xcap-caps+xml", 
		"urn:ietf:params:xml:ns:xcap-caps",
		"index", 
		tsk_true 
	},
	
	/*== resource-lists ==*/{
		tauid_ietf_resource_lists, 
		"resource-lists", 
		"application/resource-lists+xml", 
		"urn:ietf:params:xml:ns:resource-lists",
		"index", 
		tsk_false 
	},

	/*== rls-services ==*/{
		tauid_ietf_rls_services, 
		"rls-services", 
		"application/rls-services+xml", 
		"urn:ietf:params:xml:ns:resource-lists",
		"index", 
		tsk_false 
	},

	/*== pres-rules ==*/{
		tauid_ietf_pres_rules, 
		"pres-rules", 
		"application/auth-policy+xml", 
		"urn:ietf:params:xml:ns:pres-rules",
		"index", 
		tsk_false 
	},

	/*== org.openmobilealliance.pres-rules ==*/{
		tauid_oma_pres_rules, 
		"org.openmobilealliance.pres-rules", 
		"application/auth-policy+xml", 
		"urn:ietf:params:xml:ns:common-policy",
		"pres-rules", 
		tsk_false 
	},

	/*== directory ==*/{
		tauid_ietf_directory, 
		"directory", 
		"application/directory+xml", 
		"urn:ietf:params:xml:ns:xcap-directory",
		"directory.xml", 
		tsk_false 
	},

	/*== org.openmobilealliance.xcap-directory ==*/{
		tauid_oma_directory, 
		"org.openmobilealliance.xcap-directory", 
		"application/vnd.oma.xcap-directory+xml", 
		"urn:oma:xml:xdm:xcap-directory",
		"directory.xml", 
		tsk_false
	},

	/*== org.openmobilealliance.pres-content ==*/{
		tauid_oma_pres_content, 
		"org.openmobilealliance.pres-content", 
		"application/vnd.oma.pres-content+xml", 
		"urn:oma:xml:prs:pres-content",
		"oma_status-icon/rcs_status_icon", 
		tsk_false
	},

	/*== org.openmobilealliance.conv-history ==*/{
		tauid_oma_conv_history, 
		"org.openmobilealliance.conv-history",
		"application/vnd.oma.im.history-list+xml",
		"urn:oma:xml:im:history-list",
		"conv-history",
		tsk_false
	},

	/*== org.openmobilealliance.deferred-list ==*/{
		tauid_oma_deferred_list, 
		"org.openmobilealliance.deferred-list", 
		"application/vnd.oma.im.deferred-list+xml", 
		"urn:oma:xml:im:history-list",
		"deferred-list", 
		tsk_false
	},

	/*== org.openmobilealliance.group-usage-list ==*/{
		tauid_oma_shared_groups, 
		"org.openmobilealliance.group-usage-list", 
		"application/vnd.oma.group-usage-list+xml", 
		"rn:ietf:params:xml:ns:resource-lists",
		"index", 
		tsk_false
	},
};

/** Predicate function used to find an option by id.
*/
static int pred_find_auid_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data){
		txcap_auid_t *auid = item->data;
		return tsk_stricmp(auid->id, (const char*)id);
	}
	return -1;
}



/** Initialze the list of auids with default values from __txcap_auids
* auids must be null;
*/
int txcap_auids_init(txcap_auids_L_t** auids)
{
	size_t i;
	size_t count;
	if(!auids){
		TSK_DEBUG_ERROR("invalid parameter.");
		return -1;
	}
	else if(*auids){
		TSK_DEBUG_WARN("auids already initialized.");
	}
	else{
		*auids = TSK_LIST_CREATE();
	}
	
	count = sizeof(__txcap_auids)/sizeof(auid_t);
	for(i = 0; i<count; i++){
		txcap_auid_t* auid = TXCAP_AUID_CREATE(__txcap_auids[i].type,
			__txcap_auids[i].id,
			__txcap_auids[i].mime_type,
			__txcap_auids[i].ns,
			__txcap_auids[i].document_name,
			__txcap_auids[i].global);
		tsk_list_push_back_data(*auids, (void**)&auid);
	}

	return 0;
}

/** 
* Registers a new AUID. If the AUID already exist (case-insensitive comparison on the id),
* then it will be updated with the new supplied values.
* @param auids The destination list.
* @param id The id of the new AUID to add (e.g. xcap-caps).
* @param mime_type The MIME-Type of the new AUID to add (e.g. application/xcap-caps+xml).
* @param ns The Namespace of the new AUID to add (e.g. urn:ietf:params:xml:ns:xcap-caps).
* @param document_name The name of the new AUID to add (e.g. index).
* @param is_global Indicates whether the AUID scope is global or not (user).
* @retval Zero if succeed and non-zero error code otherwise.
*/
int txcap_auid_register(txcap_auids_L_t* auids, const char* id, const char* mime_type, const char* ns, const char* document_name, tsk_bool_t is_global)
{
	const tsk_list_item_t* item;
	int ret = -1;

	if(!auids || !id){
		return -1;
	}

	if((item = tsk_list_find_item_by_pred(auids, pred_find_auid_by_id, id))){
		tsk_strupdate(&((txcap_auid_t*)item->data)->mime_type, mime_type);
		tsk_strupdate(&((txcap_auid_t*)item->data)->ns, ns);
		tsk_strupdate(&((txcap_auid_t*)item->data)->document_name, document_name);
		((txcap_auid_t*)item->data)->global = is_global;
		ret = 0;
	}
	else{
		txcap_auid_t* auid;
		if((auid = TXCAP_AUID_CREATE(tauid_dummy, id, mime_type, ns, document_name, is_global))){
			tsk_list_push_back_data(auids, (void**)&auid);
			ret = 0;
		}else{
			ret = -2;
		}
	}

	return ret;
}

/**
* Finds an AUID by id (case-insensitive).
* @param auids List of AUIDs from which to find the AUID.
* @param id The @a id of the AUID to find.
* @retval An AUID with the matching id or null if does not exist.
* It's up to you to free the returned object.
*/
txcap_auid_t* txcap_auid_get_by_id(txcap_auids_L_t* auids, const char* id)
{
	const txcap_auid_t* ret = tsk_null;
	const tsk_list_item_t* item;
	
	if(!auids){
		return tsk_null;
	}
	
	if((item = tsk_list_find_item_by_pred(auids, pred_find_auid_by_id, id))){
		return tsk_object_ref((void*)item->data);
	}
	else{
		return tsk_null;
	}

}

//=================================================================================================
//	AUID object definition
//
static tsk_object_t* txcap_auid_create(tsk_object_t * self, va_list * app)
{
	txcap_auid_t *auid = self;
	if(auid){
		auid->type = va_arg(*app, txcap_auid_type_t);
		auid->id = tsk_strdup( va_arg(*app, const char*) );
		auid->mime_type = tsk_strdup( va_arg(*app, const char*) );
		auid->ns = tsk_strdup( va_arg(*app, const char*) );
		auid->document_name = tsk_strdup( va_arg(*app, const char*) );
		auid->global = va_arg(*app, tsk_bool_t);
	}
	return self;
}

static tsk_object_t* txcap_auid_destroy(tsk_object_t * self)
{ 
	txcap_auid_t *auid = self;
	if(auid){
		TSK_FREE(auid->id);
		TSK_FREE(auid->mime_type);
		TSK_FREE(auid->ns);
		TSK_FREE(auid->document_name);
	}
	
	return self;
}

static int txcap_auid_cmp(const tsk_object_t *_a1, const tsk_object_t *_a2)
{
	const txcap_auid_t *a1 = _a1;
	const txcap_auid_t *a2 = _a2;

	if(a1 && a2){
		return tsk_stricmp(a1->id, a2->id);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t txcap_auid_def_s = 
{
	sizeof(txcap_auid_t),
	txcap_auid_create, 
	txcap_auid_destroy,
	txcap_auid_cmp, 
};
const tsk_object_def_t *txcap_auid_def_t = &txcap_auid_def_s;

