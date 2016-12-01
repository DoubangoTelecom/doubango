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
/**@file txcap_node.c
 * @brief XCAP nodes.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinyxcap/txcap_node.h"

#include "tsk_string.h"
#include "tsk_buffer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

/**@ingroup txcap_selector_group
* Gets the Node Url.
* @param auid_id The id of the AUID (e.g 'resource-lists').
* @param ... Node selection steps. You must use @a TXCAP_SELECTOR_NODE_SET*() macros to set these steps.
* The list of parameters must end with @ref TXCAP_SELECTOR_NODE_SET_NULL() even if there is no step.<br>
* @retval The Url of the node (e.g. 'resource-lists/list[2]').
*
* @code
char* node = txcap_selector_get_node("resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", "sip:bob@example.com"),
		TXCAP_SELECTOR_NODE_SET_NULL());

		TSK_FREE(node);
* @endcode
*
* @sa @ref txcap_selector_get_node_2<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2<br>@ref txcap_selector_get_url
*/
char* txcap_selector_get_node(const char* auid_id, ...)
{
    char* ret = tsk_null;
    va_list ap;

    va_start(ap, auid_id);
    ret = txcap_selector_get_node_2(auid_id, &ap);
    va_end(ap);

    return ret;
}

/**@ingroup txcap_selector_group
* Gets the Node Url.
* @param auid_id The id of the AUID (e.g 'resource-lists').
* @param app Node selection steps. You must use @a TXCAP_SELECTOR_NODE_SET*() macros to set these steps.
* The list of parameters must end with @ref TXCAP_SELECTOR_NODE_SET_NULL() even if there is no step.<br>
* @retval The Url of the node (e.g. 'resource-lists/list[2]').
*
* @code
* @endcode
*
* @sa @ref txcap_selector_get_node<br>@ref txcap_selector_get_document<br> @ref txcap_selector_get_document_2<br>@ref txcap_selector_get_url
*/
char* txcap_selector_get_node_2(const char* auid_id, va_list* app)
{
    char* ret = tsk_null;
    char* namespace = tsk_null;
    tsk_buffer_t* buffer = tsk_buffer_create_null();
    txcap_selector_param_type_t step;

    while((step = va_arg(*app, txcap_selector_param_type_t)) != xcapp_node_null) {
        switch(step) {
        case xcapp_node_name: {
            /* (const char*)QNAME_STR */
            const char* QNAME_STR = va_arg(*app, const char*);
            if(tsk_buffer_append_2(buffer, "/%s", QNAME_STR)) {
                goto bail;
            }
            break;
        }
        case xcapp_node_pos: {
            /* (const char*)QNAME_STR, (unsigned int)POS_UINT */
            const char* QNAME_STR = va_arg(*app, const char*);
            unsigned int POS_UINT = va_arg(*app, unsigned int);
            tsk_buffer_append_2(buffer, "/%s%%5B%u%%5D",
                                QNAME_STR, POS_UINT);
            break;
        }
        case xcapp_node_attribute: {
            /* (const char*)QNAME_STR, (const char*)ATT_QNAME_STR, (const char*)ATT_VALUE_STR */
            const char* QNAME_STR = va_arg(*app, const char*);
            const char* ATT_QNAME_STR = va_arg(*app, const char*);
            const char* ATT_VALUE_STR = va_arg(*app, const char*);
            tsk_buffer_append_2(buffer, "/%s%%5B@%s=%%22%s%%22%%5D",
                                QNAME_STR, ATT_QNAME_STR, ATT_VALUE_STR);
            break;
        }
        case xcapp_node_pos_n_attribute: {
            /* (const char*)QNAME_STR, (unsigned int)POS_UINT, (const char*)ATT_QNAME_STR, (const char*)ATT_VALUE_STR */
            const char* QNAME_STR = va_arg(*app, const char*);
            unsigned int POS_UINT = va_arg(*app, unsigned int);
            const char* ATT_QNAME_STR = va_arg(*app, const char*);
            const char* ATT_VALUE_STR = va_arg(*app, const char*);
            tsk_buffer_append_2(buffer, "/%s%%5B%u%%5D%%5B@%s=%%22%s%%22%%5D",
                                QNAME_STR, POS_UINT, ATT_QNAME_STR, ATT_VALUE_STR);
            break;
        }
        case xcapp_node_namespace: {
            /* (const char*)PREFIX_STR, (const char*)VALUE_STR */
            const char* PREFIX_STR = va_arg(*app, const char*);
            const char* VALUE_STR = va_arg(*app, const char*);
            char* temp = tsk_null;
            tsk_sprintf(&temp, "%sxmlns(%s=%%22%s%%22)",
                        namespace?"":"%3F",PREFIX_STR, VALUE_STR);
            tsk_strcat(&namespace, temp);
            TSK_FREE(temp);
            break;
        }
        default: {
            TSK_DEBUG_ERROR("NOT SUPPORTED.");
            goto bail;
        }
        } /* switch */
    } /* while */

    /* append the namespace */
    if(namespace) {
        tsk_buffer_append(buffer, namespace, tsk_strlen(namespace));
        TSK_FREE(namespace);
    }

bail:
    if(TSK_BUFFER_DATA(buffer) && TSK_BUFFER_SIZE(buffer)) {
        ret = tsk_strndup(TSK_BUFFER_DATA(buffer), TSK_BUFFER_SIZE(buffer));
    }
    TSK_OBJECT_SAFE_FREE(buffer);
    return ret;
}
