/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
 *
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

/**@file thttp_session.c
 * @brief HTTP/HTTPS session.
 *
 */
#include "tinyhttp/thttp_session.h"

#include "thttp.h"
#include "tinyhttp/thttp_action.h"

#include "tinyhttp/headers/thttp_header_Dummy.h"
#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"

/**@defgroup thttp_session_group HTTP Session
 */

int thttp_session_signal(thttp_session_t *self, thttp_action_type_t atype);

/**Sets parameters.
 */
int __thttp_session_set(thttp_session_t *self, va_list* app)
{
    thttp_session_param_type_t curr;

    if(!self) {
        return -1;
    }

    while((curr=va_arg(*app, thttp_session_param_type_t)) != httpp_null) {
        switch(curr) {
        case httpp_option: {
            /* (thttp_session_option_t)ID_ENUM, (const char*)VALUE_STR */
            thttp_session_option_t id = va_arg(*app, thttp_session_option_t);
            const char* value = va_arg(*app, const char *);
            tsk_options_add_option(&self->options, id, value);
            break;
        }

        case httpp_header: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* name = va_arg(*app, const char *);
            const char* value = va_arg(*app, const char *);
            if(value == ((const char*)-1)) { /* UNSET */
                tsk_params_remove_param(self->headers, name);
            }
            else { /* SET */
                tsk_params_add_param(&self->headers, name, value);
            }
            break;
        }

        case httpp_cred: {
            /* (const char*)USERNAME_STR, (const char*)PASSWORD_STR */
            tsk_strupdate(&self->cred.usename, va_arg(*app, const char *));
            tsk_strupdate(&self->cred.password, va_arg(*app, const char *));
            break;
        }

        case httpp_userdata: {
            /* (const void*)USERDATA_PTR */
            self->userdata = va_arg(*app, const void *);
            break;
        }

        default: {
            /* va_list will be unsafe => exit */
            TSK_DEBUG_ERROR("NOT SUPPORTED.");
            goto bail;
        }
        } /* sxitch */
    } /* while */
    return 0;

bail:
    return -2;
}

/**@ingroup thttp_session_group
 * Creates new session.
 * @param stack The HTTP/HTTPS @a stack to use. The @a stack shall be created using @ref thttp_stack_create.
 * @param ... Any @b THTTP_SESSION_SET_*() macros. MUST ends with @ref THTTP_SESSION_SET_NULL().
 * @retval A pointer to the newly created session.
 * A session is a well-defined object.
 *
 * @code
 thttp_session_handle_t * session = thttp_session_create(stack,
	// session-level parameters
	THTTP_SESSION_SET_PARAM("timeout", "6000"),

	// session-level headers
	THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),

	THTTP_SESSION_SET_NULL());
 * @endcode
 *
 * @sa @ref thttp_session_set
 */
thttp_session_handle_t* thttp_session_create(const thttp_stack_handle_t* stack, ...)
{
    thttp_session_handle_t* ret = tsk_null;

    if((ret = tsk_object_new(thttp_session_def_t, stack))) {
        va_list ap;
        va_start(ap, stack);
        if(__thttp_session_set(ret, &ap)) {
            TSK_OBJECT_SAFE_FREE(ret);
        }
        va_end(ap);
    }
    else {
        TSK_DEBUG_ERROR("failed to create new HTTP/HTTPS session.");
    }
    return ret;
}

/**@ingroup thttp_session_group
 * Updates the session parameters.
 * @param self The session to update. The session shall be created using @ref thttp_session_create().
 * @param ... Any @b THTTP_SESSION_SET_*() macros. MUST ends with @ref THTTP_SESSION_SET_NULL().
 * @retval Zero if succeed and non zero error code otherwise.
 *
 * @code
 int ret = thttp_session_set(session,
	// session-level parameters
	THTTP_SESSION_SET_OPTION(THTTP_SESSION_OPTION_TIMEOUT, "6000"),

	// session-level headers
	THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_SESSION_SET_HEADER("User-Agent", "doubango 1.0"),

	THTTP_SESSION_SET_NULL());
 * @endcode
 *
 * @sa @ref thttp_session_create
 */
int thttp_session_set(thttp_session_handle_t *self, ...)
{
    if(self) {
        int ret;
        va_list ap;

        thttp_session_t *session = self;

        if(session->id == THTTP_SESSION_INVALID_ID) {
            TSK_DEBUG_ERROR("Using invalid session.");
            return -2;
        }

        va_start(ap, self);
        ret = __thttp_session_set(session, &ap);
        va_end(ap);
        return ret;
    }

    return -1;
}

/**@ingroup thttp_session_group
 * Gets the session id.
 * @param self The session for which to get the id.
 * @retval The id of the session.
 */
thttp_session_id_t thttp_session_get_id(const thttp_session_handle_t *self)
{
    const thttp_session_t *session = self;
    if(session) {
        return session->id;
    }
    return THTTP_SESSION_INVALID_ID;
}

/**@ingroup thttp_session_group
 * Gets the user context (user/application data).
 * @param self A pointer to the session from which to get the context.
 * @retval A pointer to the context. Previously defined by using @ref THTTP_SESSION_SET_USERDATA() macro.
 * @sa @ref THTTP_SESSION_SET_USERDATA()
 */
const void* thttp_session_get_userdata(const thttp_session_handle_t *self)
{
    if(self) {
        return ((const thttp_session_t*)self)->userdata;
    }
    return tsk_null;
}

int thttp_session_closefd(thttp_session_handle_t *_self)
{
    int ret = 0;
    thttp_session_t* self = _self;

    if(self->fd != TNET_INVALID_FD) {
        if((ret = tnet_transport_remove_socket(self->stack->transport, &self->fd))) {
            ret = tnet_sockfd_close(&self->fd);
        }
    }

    return ret;
}

/** Updates authentications headers.
 */
int thttp_session_update_challenges(thttp_session_t *self, const thttp_response_t* response, tsk_bool_t answered)
{
    int ret = 0;
    tsk_size_t i;

    tsk_list_item_t *item;

    thttp_challenge_t *challenge;

    const thttp_header_WWW_Authenticate_t *WWW_Authenticate;
    const thttp_header_Proxy_Authenticate_t *Proxy_Authenticate;

    tsk_safeobj_lock(self);

    /* RFC 2617 - Digest Operation

     *	(A) The client response to a WWW-Authenticate challenge for a protection
     space starts an authentication session with that protection space.
     The authentication session lasts until the client receives another
     WWW-Authenticate challenge from any server in the protection space.

     (B) The server may return a 401 response with a new nonce value, causing the client
     to retry the request; by specifying stale=TRUE with this response,
     the server tells the client to retry with the new nonce, but without
     prompting for a new username and password.
     */
    /* RFC 2617 - 1.2 Access Authentication Framework
     The realm directive (case-insensitive) is required for all authentication schemes that issue a challenge.
     */

    /* FIXME: As we perform the same task ==> Use only one loop.
     */

    for(i =0; (WWW_Authenticate = (const thttp_header_WWW_Authenticate_t*)thttp_message_get_headerAt(response, thttp_htype_WWW_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, WWW_Authenticate->realm) && (WWW_Authenticate->stale || !answered)) {
                /*== (B) ==*/
                if ((ret = thttp_challenge_update(challenge,
                                                  WWW_Authenticate->scheme,
                                                  WWW_Authenticate->realm,
                                                  WWW_Authenticate->nonce,
                                                  WWW_Authenticate->opaque,
                                                  WWW_Authenticate->algorithm,
                                                  WWW_Authenticate->qop)) != 0) {
                    goto bail;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                ret = -1;
                goto bail;
            }
        }

        if(isnew) {
            if((challenge = thttp_challenge_create(tsk_false, /* Not proxy */
                                                   WWW_Authenticate->scheme,
                                                   WWW_Authenticate->realm,
                                                   WWW_Authenticate->nonce,
                                                   WWW_Authenticate->opaque,
                                                   WWW_Authenticate->algorithm,
                                                   WWW_Authenticate->qop))) {
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                ret = -1;
                goto bail;
            }
        }
    }

    for(i=0; (Proxy_Authenticate = (const thttp_header_Proxy_Authenticate_t*)thttp_message_get_headerAt(response, thttp_htype_Proxy_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(!challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale || !answered)) {
                /*== (B) ==*/
                if ((ret = thttp_challenge_update(challenge,
                                                  Proxy_Authenticate->scheme,
                                                  Proxy_Authenticate->realm,
                                                  Proxy_Authenticate->nonce,
                                                  Proxy_Authenticate->opaque,
                                                  Proxy_Authenticate->algorithm,
                                                  Proxy_Authenticate->qop)) != 0) {
                    goto bail;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                ret = -1;
                goto bail;
            }
        }

        if(isnew) {
            if((challenge = thttp_challenge_create(tsk_true, /* Proxy */
                                                   Proxy_Authenticate->scheme,
                                                   Proxy_Authenticate->realm,
                                                   Proxy_Authenticate->nonce,
                                                   Proxy_Authenticate->opaque,
                                                   Proxy_Authenticate->algorithm,
                                                   Proxy_Authenticate->qop))) {
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                ret = -1;
                goto bail;
            }
        }
    }

bail:
    tsk_safeobj_unlock(self);
    return ret;

}

/* internal function */
int thttp_session_signal(thttp_session_t *self, thttp_action_type_t atype)
{
    tsk_list_item_t *item;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(self);
again:
    tsk_list_foreach(item, self->dialogs) {
        item = tsk_object_ref(item);
        thttp_dialog_fsm_act((thttp_dialog_t*)item->data, atype, tsk_null, tsk_null);
        /* As the above action could terminate the dialog (which means change the content of self->dialogs)
         * => list becomes unsafe */
        if(!(item = tsk_object_unref(item))) {
            goto again;
        }
    }

    switch(atype) {
    case thttp_thttp_atype_closed:
        self->fd = TNET_INVALID_FD;
        break;
    default:
        break;
    }

    tsk_safeobj_unlock(self);

    return 0;
}

/** Signals to all dialogs that the connection have been closed. */
int thttp_session_signal_closed(thttp_session_t *self)
{
    return thttp_session_signal(self, thttp_thttp_atype_closed);
}

/** Signals to all dialogss that we got an error */
int thttp_session_signal_error(thttp_session_t *self)
{
    return thttp_session_signal(self, thttp_atype_error);
}


/** Retrieves a session by fd */
thttp_session_t* thttp_session_get_by_fd(thttp_sessions_L_t* sessions, tnet_fd_t fd)
{
    thttp_session_t* ret = tsk_null;
    const tsk_list_item_t *item;

    if(!sessions) {
        goto bail;
    }

    tsk_list_foreach(item, sessions) {
        if(((thttp_session_t*)item->data)->fd == fd) {
            ret = tsk_object_ref(item->data);
            goto bail;
        }
    }

bail:
    return ret;
}




//========================================================
//	HTTP SESSION object definition
//
static tsk_object_t* _thttp_session_create(tsk_object_t * self, va_list * app)
{
    thttp_session_t *session = self;
    static thttp_session_id_t unique_id = THTTP_SESSION_INVALID_ID;
    if(session) {
        tsk_safeobj_init(session);

        session->stack = va_arg(*app, const thttp_stack_handle_t*);
        session->options = tsk_list_create();
        session->headers = tsk_list_create();
        session->challenges = tsk_list_create();
        session->dialogs = tsk_list_create();
        session->fd = TNET_INVALID_FD;

        session->id = THTTP_SESSION_INVALID_ID;

        /* add the session to the stack */
        if(session->stack) {
            session->id = ++unique_id;
            tsk_list_push_back_data(session->stack->sessions, (void**)&session);
        }
    }

    return self;
}

static tsk_object_t* thttp_session_destroy(tsk_object_t * self)
{
    thttp_session_t *session = self;
    if(session) {
        TSK_DEBUG_INFO("*** HTTP/HTTPS Session destroyed ***");

        /* remove from the stack */
        if(session->stack) {
            tsk_list_remove_item_by_data(session->stack->sessions, session);
        }

        TSK_OBJECT_SAFE_FREE(session->options);
        TSK_OBJECT_SAFE_FREE(session->headers);
        TSK_OBJECT_SAFE_FREE(session->challenges);
        TSK_OBJECT_SAFE_FREE(session->dialogs);

        // cred
        TSK_FREE(session->cred.usename);
        TSK_FREE(session->cred.password);

        // fd
        if(session->fd != TNET_INVALID_FD) {
            if(tnet_transport_remove_socket(session->stack->transport, &session->fd)) {
                tnet_sockfd_close(&session->fd);
            }
        }

        tsk_safeobj_deinit(session);
    }
    return self;
}

static int thttp_session_cmp(const tsk_object_t *_session1, const tsk_object_t *_session2)
{
    const thttp_session_t *session1 = _session1;
    const thttp_session_t *session2 = _session2;

    if(session1 && session2) {
        return (int)(session1->id-session2->id);
    }
    return -1;
}

static const tsk_object_def_t thttp_session_def_s = {
    sizeof(thttp_session_t),
    _thttp_session_create,
    thttp_session_destroy,
    thttp_session_cmp,
};
const tsk_object_def_t *thttp_session_def_t = &thttp_session_def_s;
