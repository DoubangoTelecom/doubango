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
/**@file txcap_auid.h
 * @brief XCAP AUID manager.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYXCAP_TXCAP_AUID_H
#define TINYXCAP_TXCAP_AUID_H

#include "tinyxcap_config.h"

#include "tsk_list.h"

TXCAP_BEGIN_DECLS

/*== xcap-caps ==*/
#define TXCAP_AUID_IETF_XCAP_CAPS_ID "xcap-caps"
#define TXCAP_AUID_IETF_XCAP_CAPS_MIME_TYPE "application/xcap-caps+xml"
#define TXCAP_AUID_IETF_XCAP_CAPS_NS "urn:ietf:params:xml:ns:xcap-caps"
#define TXCAP_AUID_IETF_XCAP_CAPS_DOC "index"

/*== resource-lists ==*/
#define TXCAP_AUID_IETF_RESOURCE_LISTS_ID "resource-lists"
#define TXCAP_AUID_IETF_RESOURCE_LISTS_MIME_TYPE "application/resource-lists+xml"
#define TXCAP_AUID_IETF_RESOURCE_LISTS_NS "urn:ietf:params:xml:ns:resource-lists"
#define TXCAP_AUID_IETF_RESOURCE_LISTS_DOC "index"

/*== rls-services ==*/
#define TXCAP_AUID_IETF_RLS_SERVICES_ID "rls-services"
#define TXCAP_AUID_IETF_RLS_SERVICES_MIME_TYPE "application/rls-services+xml"
#define TXCAP_AUID_IETF_RLS_SERVICES_NS "urn:ietf:params:xml:ns:resource-lists"
#define TXCAP_AUID_IETF_RLS_SERVICES_DOC "index"

/*== pres-rules ==*/
#define TXCAP_AUID_IETF_PRES_RULES_ID "pres-rules"
#define TXCAP_AUID_IETF_PRES_RULES_MIME_TYPE "application/auth-policy+xml"
#define TXCAP_AUID_IETF_PRES_RULES_NS "urn:ietf:params:xml:ns:pres-rules"
#define TXCAP_AUID_IETF_PRES_RULES_DOC "index"

/*== pidf-manipulation ==*/
#define TXCAP_AUID_IETF_PIDF_MANIPULATION_ID "pidf-manipulation"
#define TXCAP_AUID_IETF_PIDF_MANIPULATION_MIME_TYPE "application/pidf+xml"
#define TXCAP_AUID_IETF_PIDF_MANIPULATION_NS "urn:ietf:params:xml:ns:pidf"
#define TXCAP_AUID_IETF_PIDF_MANIPULATION_DOC "index"

/*== org.openmobilealliance.pres-rules ==*/
#define TXCAP_AUID_OMA_PRES_RULES_ID "org.openmobilealliance.pres-rules"
#define TXCAP_AUID_OMA_PRES_RULES_MIME_TYPE "application/auth-policy+xml"
#define TXCAP_AUID_OMA_PRES_RULES_NS "urn:ietf:params:xml:ns:common-policy"
#define TXCAP_AUID_OMA_PRES_RULES_DOC "pres-rules"

/*== directory ==*/
#define TXCAP_AUID_IETF_DIRECTORY_ID "directory"
#define TXCAP_AUID_IETF_DIRECTORY_MIME_TYPE "application/directory+xml"
#define TXCAP_AUID_IETF_DIRECTORY_NS "urn:ietf:params:xml:ns:xcap-directory"
#define TXCAP_AUID_IETF_DIRECTORY_DOC "directory.xml"

/*== org.openmobilealliance.xcap-directory ==*/
#define TXCAP_AUID_OMA_DIRECTORY_ID "org.openmobilealliance.xcap-directory"
#define TXCAP_AUID_OMA_DIRECTORY_MIME_TYPE "application/vnd.oma.xcap-directory+xml"
#define TXCAP_AUID_OMA_DIRECTORY_NS "urn:oma:xml:xdm:xcap-directory"
#define TXCAP_AUID_OMA_DIRECTORY_DOC "directory.xml"

/*== org.openmobilealliance.pres-content ==*/
#define TXCAP_AUID_OMA_PRES_CONTENT_ID "org.openmobilealliance.pres-content"
#define TXCAP_AUID_OMA_PRES_CONTENT_MIME_TYPE "application/vnd.oma.pres-content+xml"
#define TXCAP_AUID_OMA_PRES_CONTENT_NS "urn:oma:xml:prs:pres-content"
#define TXCAP_AUID_OMA_PRES_CONTENT_DOC "oma_status-icon/rcs_status_icon"

/*== org.openmobilealliance.conv-history ==*/
#define TXCAP_AUID_OMA_CONV_HISTORY_ID "org.openmobilealliance.conv-history"
#define TXCAP_AUID_OMA_CONV_HISTORY_MIME_TYPE "application/vnd.oma.im.history-list+xml"
#define TXCAP_AUID_OMA_CONV_HISTORY_NS "urn:oma:xml:im:history-list"
#define TXCAP_AUID_OMA_CONV_HISTORY_DOC "conv-history"

/*== org.openmobilealliance.deferred-list ==*/
#define TXCAP_AUID_OMA_DEFERRED_LIST_ID "org.openmobilealliance.deferred-list"
#define TXCAP_AUID_OMA_DEFERRED_LIST_MIME_TYPE "application/vnd.oma.im.deferred-list+xml"
#define TXCAP_AUID_OMA_DEFERRED_LIST_NS "urn:oma:xml:im:history-list"
#define TXCAP_AUID_OMA_DEFERRED_LIST_DOC "deferred-list"

/*== org.openmobilealliance.group-usage-list ==*/
#define TXCAP_AUID_OMA_SHARED_GROUPS_ID "org.openmobilealliance.group-usage-list"
#define TXCAP_AUID_OMA_SHARED_GROUPS_MIME_TYPE "application/vnd.oma.group-usage-list+xml"
#define TXCAP_AUID_OMA_SHARED_GROUPS_NS "urn:ietf:params:xml:ns:resource-lists"
#define TXCAP_AUID_OMA_SHARED_GROUPS_DOC "index"


/** List of supported AUIDs.
*/
typedef enum txcap_auid_type_e {
    tauid_dummy, /**<  any auid */

    tauid_ietf_xcap_caps,			/**< xcap-caps */
    tauid_ietf_resource_lists,	/**< resource-lists */
    tauid_ietf_rls_services,		/**< rls-services */
    tauid_ietf_pres_rules,		/**< pres-rules */
    tauid_ietf_directory,			/**< directory */

    tauid_oma_conv_history,		/**< org.openmobilealliance.conv-history */
    tauid_oma_pres_rules,			/**< org.openmobilealliance.pres-rules */
    tauid_oma_directory,			/**< org.openmobilealliance.xcap-directory */
    tauid_oma_deferred_list,		/**< org.openmobilealliance.deferred-list */
    tauid_oma_pres_content,		/**< org.openmobilealliance.pres-content*/
    tauid_oma_shared_groups		/**< org.openmobilealliance.group-usage-list */
}
txcap_auid_type_t;

/** XCAP AUID.
*/
typedef struct txcap_auid_s {
    TSK_DECLARE_OBJECT;

    txcap_auid_type_t type;
    char* id; /**< The id of the AUID (e.g. 'xcap-caps' or 'rls-services' or 'resource-lists' or ...)*/
    char* mime_type; /**< The default MIME-type of the document associated to this auid. */
    char* ns; /**< The namespace associated to this auid. */
    char* document_name; /**< The default name of the document associated to this auid. */
    tsk_bool_t global; /**< Indicates the auid scope. */
}
txcap_auid_t;

typedef tsk_list_t txcap_auids_L_t;

TINYXCAP_API int txcap_auid_register(txcap_auids_L_t* auids, const char* id, const char* mime_type, const char* ns, const char* document_name, tsk_bool_t is_global);
txcap_auid_t* txcap_auid_get_by_id(txcap_auids_L_t* auids, const char* id);
int txcap_auids_init(txcap_auids_L_t** auids);


TINYXCAP_GEXTERN const tsk_object_def_t *txcap_auid_def_t;

TXCAP_END_DECLS

#endif // TINYXCAP_TXCAP_AUID_H

