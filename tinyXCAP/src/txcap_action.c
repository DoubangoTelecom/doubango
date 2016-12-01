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
/**@file txcap_action.c
 * @brief XCAP actions.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "tinyxcap/txcap_action.h"

#include "tsk_options.h"
#include "tsk_params.h"
#include "tinyxcap/txcap_selector.h"

/**@defgroup txcap_action_group XDMC (Sending Requests)
*/

extern char* __txcap_selector_get_url(const txcap_stack_handle_t* stack, const char* auid_id, va_list* app);


/** Sends HTTP/HTTPS request to the XDMS.
*/
int txcap_action_perform(txcap_stack_handle_t* stack, txcap_action_type_t type, txcap_action_target_t target, ...)
{
    tsk_options_L_t* options = tsk_null;
    tsk_params_L_t* headers = tsk_null;
    char* urlstring = tsk_null;
    thttp_action_t* action;
    thttp_dialog_t* dialog;
    int ret = -1;
    txcap_action_param_type_t curr;
    const char* method = "GET";
    const char* mime_type = tsk_null;
    const char* AUID_STR = tsk_null;
    const void* PAY_PTR = tsk_null;
    size_t PAY_SIZE = 0;
    va_list ap;
    txcap_stack_t* xcap_stack = stack;

    if(!xcap_stack) {
        goto bail;
    }
    else {
        xcap_stack = tsk_object_ref(stack);
        options = tsk_list_create();
        headers = tsk_list_create();
    }

    va_start(ap, target);
    while((curr = va_arg(ap, txcap_action_param_type_t)) != txcap_apt_null) {
        switch(curr) {
        case txcap_apt_option: {
            /*(thttp_action_option_t)ID_INT, (const char*)VALUE_STR*/
            thttp_action_option_t ID_INT = va_arg(ap, thttp_action_option_t);
            const char* VALUE_STR = va_arg(ap, const char *);
            tsk_options_add_option(&options, ID_INT, VALUE_STR);
            break;
        }
        case txcap_apt_header: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* NAME_STR = va_arg(ap, const char *);
            const char* VALUE_STR = va_arg(ap, const char *);
            if(VALUE_STR == (const char*)-1) {
                tsk_params_remove_param(headers, NAME_STR);
            }
            else {
                tsk_params_add_param(&headers, NAME_STR, VALUE_STR);
            }
            break;
        }
        case txcap_apt_payload: {
            /*(const void*)PAY_PTR, (size_t)PAY_SIZE*/
            PAY_PTR = va_arg(ap, const void *);
            PAY_SIZE = va_arg(ap, size_t);
            break;
        }
        case txcap_apt_selector: {
            /*(const char*)AUID_STR, __VA_ARGS__*/
            AUID_STR = va_arg(ap, const char *);
            if(!urlstring) {
                urlstring = __txcap_selector_get_url(stack, AUID_STR, &ap);
            }
            break;
        }
        case txcap_apt_urlstring: {
            /* (const char*)URI_STR */
            const char* URI_STR = va_arg(ap, const char *);
            if(!urlstring) {
                urlstring = tsk_strdup(URI_STR);
            }
            break;
        }
        default: {
            TSK_DEBUG_ERROR("NOT SUPPORTED.");
            goto done;
        }
        }/* switch */
    } /* while */

done:
    va_end(ap);

    /* checks urlstring */
    if(!urlstring) {
        TSK_DEBUG_ERROR("Failed to compute XCAP URL");
        goto bail;
    }

    /* HTTP method */
    switch(type) {
    case txcap_atp_create:
    case txcap_atp_replace:
        method = "PUT";
        break;
    case txcap_atp_fetch:
        method = "GET";
        break;
    case txcap_atp_delete:
        method = "DELETE";
        break;
    }

    /*content-type*/
    switch(target) {
    case txcap_atg_element:
        mime_type = TXCAP_MIME_TYPE_ELEMENT;
        break;
    case txcap_atg_document: {
        if(AUID_STR) {
            txcap_auid_t* auid;
            if((auid = txcap_auid_get_by_id(xcap_stack->auids, AUID_STR))) {
                mime_type = (const char*)auid->mime_type; // (cont char*) do not strdup()
                TSK_OBJECT_SAFE_FREE(auid);
            }
            else {
                TSK_DEBUG_WARN("Failed to find auid with id=%s", AUID_STR);
            }
        }
        break;
    }
    case txcap_atg_attribute:
        mime_type = TXCAP_MIME_TYPE_ATTRIBUTE;
        break;
    }

    /* HTTP action */
    if((action = thttp_action_create(thttp_atype_o_request, urlstring, method, tsk_null))) {
        const tsk_list_item_t* item;
        if((dialog = thttp_dialog_new(xcap_stack->http_session))) {
            /* copy options */
            tsk_list_foreach(item, options) {
                tsk_options_add_option(&action->options, ((const tsk_option_t*)item->data)->id, ((const tsk_option_t*)item->data)->value);
            }
            /* copy action-level headers */
            tsk_list_foreach(item, headers) {
                tsk_params_add_param(&action->headers, ((const tsk_param_t*)item->data)->name, ((const tsk_param_t*)item->data)->value);
            }
            /* copy session-level headers ==> will be added by thttp_dialog_send_request()*/
            //--tsk_list_pushback_list(action->headers, ((thttp_session_t*)xcap_stack->http_session)->headers);

            /* Content-Type */
            if(mime_type) {
                if(!tsk_params_have_param(action->headers, "Content-Type")) {
                    tsk_params_add_param(&action->headers, "Content-Type", mime_type);
                }
            }

            /* payload */
            if(PAY_PTR && PAY_SIZE) {
                action->payload = tsk_buffer_create(PAY_PTR, PAY_SIZE);
            }

            /* performs */
            ret = thttp_dialog_fsm_act(dialog, action->type, tsk_null, action);
            tsk_object_unref(dialog);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create new HTTP/HTTPS dialog.");
            ret = -2;
        }
        TSK_OBJECT_SAFE_FREE(action);
    }

bail:
    TSK_FREE(urlstring);
    TSK_OBJECT_SAFE_FREE(options);
    TSK_OBJECT_SAFE_FREE(headers);
    tsk_object_unref(xcap_stack);
    return ret;
}

