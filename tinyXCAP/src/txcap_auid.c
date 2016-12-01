/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
/**@file txcap_auid.c
 * @brief XCAP AUID manager.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinyxcap/txcap_auid.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"


typedef struct auid_s {
    txcap_auid_type_t type;
    const char* id;
    const char* mime_type;
    const char* ns;
    const char* document_name;
    tsk_bool_t global;
}
auid_t;

/** List of all default auids */
static const auid_t __txcap_auids[] = {
    /*== xcap-caps ==*/{
        tauid_ietf_xcap_caps,
        TXCAP_AUID_IETF_XCAP_CAPS_ID,
        TXCAP_AUID_IETF_XCAP_CAPS_MIME_TYPE,
        TXCAP_AUID_IETF_XCAP_CAPS_NS,
        TXCAP_AUID_IETF_XCAP_CAPS_DOC,
        tsk_true
    },

    /*== resource-lists ==*/{
        tauid_ietf_resource_lists,
        TXCAP_AUID_IETF_RESOURCE_LISTS_ID,
        TXCAP_AUID_IETF_RESOURCE_LISTS_MIME_TYPE,
        TXCAP_AUID_IETF_RESOURCE_LISTS_NS,
        TXCAP_AUID_IETF_RESOURCE_LISTS_DOC,
        tsk_false
    },

    /*== rls-services ==*/{
        tauid_ietf_rls_services,
        TXCAP_AUID_IETF_RLS_SERVICES_ID,
        TXCAP_AUID_IETF_RLS_SERVICES_MIME_TYPE,
        TXCAP_AUID_IETF_RLS_SERVICES_NS,
        TXCAP_AUID_IETF_RLS_SERVICES_DOC,
        tsk_false
    },

    /*== pres-rules ==*/{
        tauid_ietf_pres_rules,
        TXCAP_AUID_IETF_PRES_RULES_ID,
        TXCAP_AUID_IETF_PRES_RULES_MIME_TYPE,
        TXCAP_AUID_IETF_PRES_RULES_NS,
        TXCAP_AUID_IETF_PRES_RULES_DOC,
        tsk_false
    },

    /*== pidf-manipulation ==*/{
        tauid_ietf_pres_rules,
        TXCAP_AUID_IETF_PIDF_MANIPULATION_ID,
        TXCAP_AUID_IETF_PIDF_MANIPULATION_MIME_TYPE,
        TXCAP_AUID_IETF_PIDF_MANIPULATION_NS,
        TXCAP_AUID_IETF_PIDF_MANIPULATION_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.pres-rules ==*/{
        tauid_oma_pres_rules,
        TXCAP_AUID_OMA_PRES_RULES_ID,
        TXCAP_AUID_OMA_PRES_RULES_MIME_TYPE,
        TXCAP_AUID_OMA_PRES_RULES_NS,
        TXCAP_AUID_OMA_PRES_RULES_DOC,
        tsk_false
    },

    /*== directory ==*/{
        tauid_ietf_directory,
        TXCAP_AUID_IETF_DIRECTORY_ID,
        TXCAP_AUID_IETF_DIRECTORY_MIME_TYPE,
        TXCAP_AUID_IETF_DIRECTORY_NS,
        TXCAP_AUID_IETF_DIRECTORY_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.xcap-directory ==*/{
        tauid_oma_directory,
        TXCAP_AUID_OMA_DIRECTORY_ID,
        TXCAP_AUID_OMA_DIRECTORY_MIME_TYPE,
        TXCAP_AUID_OMA_DIRECTORY_NS,
        TXCAP_AUID_OMA_DIRECTORY_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.pres-content ==*/{
        tauid_oma_pres_content,
        TXCAP_AUID_OMA_PRES_CONTENT_ID,
        TXCAP_AUID_OMA_PRES_CONTENT_MIME_TYPE,
        TXCAP_AUID_OMA_PRES_CONTENT_NS,
        TXCAP_AUID_OMA_PRES_CONTENT_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.conv-history ==*/{
        tauid_oma_conv_history,
        TXCAP_AUID_OMA_CONV_HISTORY_ID,
        TXCAP_AUID_OMA_CONV_HISTORY_MIME_TYPE,
        TXCAP_AUID_OMA_CONV_HISTORY_NS,
        TXCAP_AUID_OMA_CONV_HISTORY_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.deferred-list ==*/{
        tauid_oma_deferred_list,
        TXCAP_AUID_OMA_DEFERRED_LIST_ID,
        TXCAP_AUID_OMA_DEFERRED_LIST_MIME_TYPE,
        TXCAP_AUID_OMA_DEFERRED_LIST_NS,
        TXCAP_AUID_OMA_DEFERRED_LIST_DOC,
        tsk_false
    },

    /*== org.openmobilealliance.group-usage-list ==*/{
        tauid_oma_shared_groups,
        TXCAP_AUID_OMA_SHARED_GROUPS_ID,
        TXCAP_AUID_OMA_SHARED_GROUPS_MIME_TYPE,
        TXCAP_AUID_OMA_SHARED_GROUPS_NS,
        TXCAP_AUID_OMA_SHARED_GROUPS_DOC,
        tsk_false
    },
};

txcap_auid_t* txcap_auid_create(txcap_auid_type_t type, const char* id, const char* mime_type, const char* ns, const char* document_name, tsk_bool_t is_global)
{
    return tsk_object_new(txcap_auid_def_t, type, id, mime_type, ns, document_name, is_global);
}

/** Predicate function used to find an option by id.
*/
static int pred_find_auid_by_id(const tsk_list_item_t *item, const void *id)
{
    if(item && item->data) {
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
    if(!auids) {
        TSK_DEBUG_ERROR("invalid parameter.");
        return -1;
    }
    else if(*auids) {
        TSK_DEBUG_WARN("auids already initialized.");
    }
    else {
        *auids = tsk_list_create();
    }

    count = sizeof(__txcap_auids)/sizeof(auid_t);
    for(i = 0; i<count; i++) {
        txcap_auid_t* auid = txcap_auid_create(__txcap_auids[i].type,
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

    if(!auids || !id) {
        return -1;
    }

    if((item = tsk_list_find_item_by_pred(auids, pred_find_auid_by_id, id))) {
        tsk_strupdate(&((txcap_auid_t*)item->data)->mime_type, mime_type);
        tsk_strupdate(&((txcap_auid_t*)item->data)->ns, ns);
        tsk_strupdate(&((txcap_auid_t*)item->data)->document_name, document_name);
        ((txcap_auid_t*)item->data)->global = is_global;
        ret = 0;
    }
    else {
        txcap_auid_t* auid;
        if((auid = txcap_auid_create(tauid_dummy, id, mime_type, ns, document_name, is_global))) {
            tsk_list_push_back_data(auids, (void**)&auid);
            ret = 0;
        }
        else {
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
    //const txcap_auid_t* ret = tsk_null;
    const tsk_list_item_t* item;

    if(!auids) {
        return tsk_null;
    }

    if((item = tsk_list_find_item_by_pred(auids, pred_find_auid_by_id, id))) {
        return tsk_object_ref((void*)item->data);
    }
    else {
        return tsk_null;
    }

}

//=================================================================================================
//	AUID object definition
//
static tsk_object_t* txcap_auid_ctor(tsk_object_t * self, va_list * app)
{
    txcap_auid_t *auid = self;
    if(auid) {
        auid->type = va_arg(*app, txcap_auid_type_t);
        auid->id = tsk_strdup( va_arg(*app, const char*) );
        auid->mime_type = tsk_strdup( va_arg(*app, const char*) );
        auid->ns = tsk_strdup( va_arg(*app, const char*) );
        auid->document_name = tsk_strdup( va_arg(*app, const char*) );
        auid->global = va_arg(*app, tsk_bool_t);
    }
    return self;
}

static tsk_object_t* txcap_auid_dtor(tsk_object_t * self)
{
    txcap_auid_t *auid = self;
    if(auid) {
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

    if(a1 && a2) {
        return tsk_stricmp(a1->id, a2->id);
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t txcap_auid_def_s = {
    sizeof(txcap_auid_t),
    txcap_auid_ctor,
    txcap_auid_dtor,
    txcap_auid_cmp,
};
const tsk_object_def_t *txcap_auid_def_t = &txcap_auid_def_s;

