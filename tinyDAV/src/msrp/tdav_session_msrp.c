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

/**@file tdav_session_msrp.c
 * @brief The Message Session Relay Protocol (MSRP) session.
 * Used for both Message (RFC 4975) and file transfer (RFC 5547).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/msrp/tdav_session_msrp.h"

#if !defined(HAVE_TINYMSRP) || HAVE_TINYMSRP

#include "tsk_memory.h" /* TSK_FREE */

#define TDAV_MSRP_CONNECT_TIMEOUT	2000

static void send_pending_file(tdav_session_msrp_t *session);
static void send_bodiless(tdav_session_msrp_t *msrp);

/*
	* http://tools.ietf.org/html/draft-ietf-simple-msrp-acm-09
	* http://tools.ietf.org/html/draft-ietf-simple-msrp-sessmatch-06
	* http://www.openmobilealliance.org/technical/release_program/docs/SIMPLE_IM/V1_0-20100322-C/OMA-TS-SIMPLE_IM-V1_0-20100322-C.pdf
*/

int tdav_msrp_event_proxy_cb(tmsrp_event_t* _event/*!Not the owner of the object*/)
{
    tdav_session_msrp_t* msrp;
    int ret = 0;

    if(!_event || !_event->callback_data) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    msrp = tsk_object_ref((void*)_event->callback_data);
    if(TMEDIA_SESSION_MSRP(msrp)->callback.func) {
        _event->callback_data = TMEDIA_SESSION_MSRP(msrp)->callback.data; // steal callback data
        ret = TMEDIA_SESSION_MSRP(msrp)->callback.func(_event); // call callback function()
    }
    tsk_object_unref(msrp);

    return ret;
}

int tdav_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
    const tdav_session_msrp_t *session = e->callback_data;
    tmsrp_receiver_t* receiver;
    int ret = -1;

#define TMSRP_ALERT_USER(type) \
	{ \
		tdav_session_msrp_t *msrp = tsk_object_ref((void*)session); \
		tmsrp_event_t* _event = tmsrp_event_create(msrp, tsk_false, type, tsk_null); \
		tdav_msrp_event_proxy_cb(_event); \
		TSK_OBJECT_SAFE_FREE(_event); \
		tsk_object_unref(msrp); \
	}

    switch(e->type) {
    case event_data: {
        break;
    }
    case event_accepted:
        if(!session->connectedFD) {
            tdav_session_msrp_t *msrp = tsk_object_ref((void*)session);
            /* We are passive ==> update connection info */
            msrp->connectedFD = e->local_fd;
            tmsrp_sender_set_fd(msrp->sender, msrp->connectedFD);
            tmsrp_receiver_set_fd(msrp->receiver, msrp->connectedFD);
            msrp->fresh_conn = tsk_true;
            /* Send Bodiless request */
            if(msrp->send_bodiless) {
                send_bodiless(msrp);
                msrp->send_bodiless = tsk_false;
            }
            /* Alert user */
            TMSRP_ALERT_USER(tmsrp_event_type_connected);

            tsk_object_unref(msrp);
        }
        break;
    case event_closed:
        if(e->local_fd == session->connectedFD) {
            TSK_DEBUG_INFO("MSRP Socket closed");
            TMSRP_ALERT_USER(tmsrp_event_type_disconnected);
        }
        return 0;

    case event_connected: {
        tdav_session_msrp_t *msrp = tsk_object_ref((void*)session);
        if(e->local_fd == msrp->connectedFD) {
            msrp->fresh_conn = tsk_true;
            /* Send Bodiless request */
            if(msrp->send_bodiless) {
                send_bodiless(msrp);
                msrp->send_bodiless = tsk_false;
            }
            /* Alert user */
            TMSRP_ALERT_USER(tmsrp_event_type_connected);
        }
        tsk_object_unref(msrp);
    }
    break;
    default: {
        return 0;
    }
    }

    if(e->data && (receiver = tsk_object_ref((void*)session->receiver))) {
        ret = tmsrp_receiver_recv(receiver, e->data, e->size);
        tsk_object_unref(receiver);
    }

    if(session->fresh_conn) {
        tdav_session_msrp_t *msrp = tsk_object_ref((void*)session);
        /* send pending file - and data?*/
        if(session->offerer) {
            send_pending_file(msrp);
        }
        msrp->fresh_conn = tsk_false;
        tsk_object_unref(msrp);
    }

    return ret;
}

static void send_pending_file(tdav_session_msrp_t *msrp)
{
    if(msrp && msrp->file.path && !msrp->file.sent) {
        msrp->file.sent = tsk_true;
        tsmrp_sender_send_file(msrp->sender, msrp->file.path);
    }
}

static void send_bodiless(tdav_session_msrp_t *msrp)
{
    tmsrp_request_t* BODILESS;
    if(msrp->config->To_Path && msrp->config->From_Path) {
        if((BODILESS = tmsrp_create_bodiless(msrp->config->To_Path->uri, msrp->config->From_Path->uri))) {
            char* str;
            if((str = tmsrp_message_tostring(BODILESS))) {
                if(!tnet_sockfd_send(msrp->connectedFD, str, tsk_strlen(str), 0)) {
                    TSK_DEBUG_WARN("Failed to send bodiless request.");
                }
                TSK_FREE(str);
            }

            TSK_OBJECT_SAFE_FREE(BODILESS);
        }
    }
}

static tdav_msrp_setup_t setup_from_string(const char* setup)
{
    tdav_msrp_setup_t ret = msrp_setup_active;

    if(setup) {
        if(tsk_strequals(setup, "holdconn")) {
            ret = msrp_setup_holdconn;
        }
        else if(tsk_strequals(setup, "passive")) {
            ret = msrp_setup_passive;
        }
        else if(tsk_strequals(setup, "actpass")) {
            ret = msrp_setup_actpass;
        }
        else {
            ret = msrp_setup_active;
        }
    }
    return ret;
}

static const char* setup_to_string(tdav_msrp_setup_t setup)
{
    switch(setup) {
    case msrp_setup_active:
        return "active";
    case msrp_setup_passive:
        return "passive";
    case msrp_setup_actpass:
        return "actpass";
    case msrp_setup_holdconn:
        return "holdconn";
    }
    return "active";
}

static int init_neg_types(tdav_session_msrp_t* msrp, const tsdp_header_M_t* m)
{
    const tsdp_header_A_t* A;

    if((A = tsdp_header_M_findA(m, "accept-types"))) {
        char *saveptr = NULL;
        char* atype = tsk_strtok_r((char*)A->value, " ", &saveptr);
        const char* default_atype = atype;
        while(atype) {
            if(tsk_striequals(atype, "message/CPIM")) {
                tsk_strupdate(&msrp->neg_accept_type, atype);
                if((A = tsdp_header_M_findA(m, "accept-wrapped-types"))) {
                    char* awtype = tsk_strtok_r((char*)A->value, " ", &saveptr);
                    tsk_strupdate(&msrp->neg_accept_w_type, awtype); // first
                }
                break;
            }
            atype = tsk_strtok_r(tsk_null, " ", &saveptr);
        }

        if(!msrp->neg_accept_type) {
            tsk_strupdate(&msrp->neg_accept_type, default_atype);
        }

        return 0;
    }
    return -1;
}

static int populate_lo(tdav_session_msrp_t* self, tsk_bool_t initial)
{
    if(!self || !TMEDIA_SESSION(self)->M.lo) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(initial) {
        const char* att_dir = tsk_null;
        tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                  TSDP_HEADER_A_VA_ARGS("setup", setup_to_string(self->setup)),

                                  tsk_null
                                 );

        if(self->accept_types || self->neg_accept_type) {
            /* a=accept-types:message/CPIM application/octet-stream */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("accept-types", self->accept_types ? self->accept_types : self->neg_accept_type),
                                      tsk_null);
        }
        if(self->accept_w_types || self->neg_accept_w_type) {
            /* a=accept-wrapped-types:application/octet-stream */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("accept-wrapped-types", self->accept_w_types ? self->accept_w_types : self->neg_accept_w_type),
                                      tsk_null);
        }

        /* direction */
        switch(self->dir) {
        case tdav_msrp_dir_sendonly:
            att_dir = "sendonly";
            break;
        case tdav_msrp_dir_recvonly:
            att_dir = "recvonly";
            break;
        case tdav_msrp_dir_sendrecv:
        case tdav_msrp_dir_none: {
            att_dir = "sendrecv";
            break;
        }
        }
        tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                  TSDP_HEADER_A_VA_ARGS(att_dir, tsk_null),
                                  tsk_null);

        /*=== File Transfer ===*/
        if(self->file.path) {
            /* Compute default 'file-selector' */
            if(!self->file.selector && !TMEDIA_SESSION(self)->M.ro) {
                int index = tsk_strLastIndexOf(self->file.path, tsk_strlen(self->file.path), "\\");
                if(index == -1) {
                    index = tsk_strLastIndexOf(self->file.path, tsk_strlen(self->file.path), "/");
                }
                index++;
                tsk_sprintf(&self->file.selector, "name:\"%s\" type:application/octet-stream", (self->file.path + index));
            }
            /* Compute default 'file-transfer-id' */
            if(!self->file.transfer_id && !TMEDIA_SESSION(self)->M.ro) {
                tsk_istr_t rand_string;
                tsk_strrandom(&rand_string);
                self->file.transfer_id = tsk_strdup(rand_string);
            }
        }

        if(self->file.selector) {
            /* a=file-selector:name:"test.pcap" type:application/octet-stream size:11376 hash:sha-1:8D:55:24:2B:F4:F9:9B:A2:54:A3:5B:91:00:15:9E:A3:D4:48:D7:DF */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("file-selector", self->file.selector),
                                      tsk_null);
        }
        if(self->file.transfer_id) {
            /* a=file-transfer-id:vscxggbwkfnqduxwfnfozvsrtkjprepg */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("file-transfer-id", self->file.transfer_id),
                                      tsk_null);
        }
        if(self->file.disposition) {
            /* a=file-disposition:attachment */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("file-disposition", self->file.disposition),
                                      tsk_null);
        }
        if(self->file.date) {
            /* a=file-date:creation:2010-02-13T17:50:31.763Z */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("file-date", self->file.date),
                                      tsk_null);
        }
        if(self->file.icon) {
            /* a=file-icon:cid:test@doubango.org */
            tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("file-icon", self->file.icon),
                                      tsk_null);
        }
    }

    return 0;
}

static tsk_bool_t match_offer(const tdav_session_msrp_t* self, const tsdp_header_M_t* M)
{
    return tsk_true;
}

/* ============ Plugin interface ================= */

int tdav_session_msrp_set(tmedia_session_t* self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_session_msrp_t* msrp;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("tdav_session_msrp_set");

    msrp = (tdav_session_msrp_t*)self;

    if(param->value_type == tmedia_pvt_pchar) {
        if(tsk_striequals(param->key, "remote-ip")) {
            // only if no ip associated to the "m=" line
            if(param->value && !msrp->remote_ip) {
                msrp->remote_ip = tsk_strdup(param->value);
            }
        }
        else if(tsk_striequals(param->key, "local-ip")) {
            tsk_strupdate(&msrp->local_ip, param->value);
        }
        else if(tsk_striequals(param->key, "use-ipv6")) {
            msrp->useIPv6 = tsk_striequals(param->value, "ipv6");
        }
        else if(tsk_striequals(param->key, "accept-types")) {
            tsk_strupdate(&msrp->accept_types, param->value);
        }
        else if(tsk_striequals(param->key, "accept-wrapped-types")) {
            tsk_strupdate(&msrp->accept_w_types, param->value);
        }

        /* Configuration */
        else if(tsk_striequals(param->key, "Failure-Report")) {
            msrp->config->Failure_Report = tsk_striequals(param->value, "yes");
        }
        else if(tsk_striequals(param->key, "Success-Report")) {
            msrp->config->Success_Report = tsk_striequals(param->value, "yes");
        }

        /* File Transfer */
        else if(tsk_striequals(param->key, "file-path") && !tsk_strnullORempty((const char*)param->value)) {
            tsk_strupdate(&msrp->file.path, param->value);
        }
        else if(tsk_striequals(param->key, "file-selector")) {
            tsk_strupdate(&msrp->file.selector, param->value);
        }
        else if(tsk_striequals(param->key, "file-disposition")) {
            tsk_strupdate(&msrp->file.disposition, param->value);
        }
        else if(tsk_striequals(param->key, "file-date")) {
            tsk_strupdate(&msrp->file.date, param->value);
        }
        else if(tsk_striequals(param->key, "file-icon")) {
            tsk_strupdate(&msrp->file.icon, param->value);
        }
        else if(tsk_striequals(param->key, "file-transfer-id")) {
            tsk_strupdate(&msrp->file.transfer_id, param->value);
        }
    }
    else if(param->value_type == tmedia_pvt_pobject) {
        if(tsk_striequals(param->key, "natt-ctx")) {
            TSK_OBJECT_SAFE_FREE(msrp->natt_ctx);
            msrp->natt_ctx = tsk_object_ref(param->value);
        }
    }
    else if(param->value_type == tmedia_pvt_int64 || param->value_type == tmedia_pvt_int32) {
        if(tsk_striequals(param->key, "chunck-duration")) {
            msrp->chunck_duration = TSK_TO_UINT32((uint8_t*)param->value);
            if(msrp->sender) {
                msrp->sender->chunck_duration = msrp->chunck_duration;
            }
        }
    }

    return ret;
}

int tdav_session_msrp_get(tmedia_session_t* self, tmedia_param_t* param)
{
    return -1;
}

int tdav_session_msrp_prepare(tmedia_session_t* self)
{
    tdav_session_msrp_t* msrp;
    tnet_socket_type_t socket_type = tnet_socket_type_tcp_ipv4;
    int ret = 0;

#if 1
    tnet_port_t local_port = TNET_SOCKET_PORT_ANY;
#else
    tnet_port_t local_port = 2000;
#endif

    TSK_DEBUG_INFO("tdav_session_msrp_prepare");

    msrp = (tdav_session_msrp_t*)self;

    /* set local port */
    if(!msrp->transport) {
        if(msrp->useIPv6) {
            TNET_SOCKET_TYPE_SET_IPV6Only(socket_type);
        }

        if(!msrp->local_ip) {
            tnet_host_t local;
            tnet_gethostname(&local);
            msrp->transport = tnet_transport_create(local, local_port, socket_type, "MSRP/MSRPS transport");
        }
        else {
            msrp->transport = tnet_transport_create(msrp->local_ip, local_port, socket_type, "MSRP/MSRPS transport");
        }
        /* Set NAT context */
        if(msrp->natt_ctx) {
            tnet_transport_set_natt_ctx(msrp->transport, msrp->natt_ctx);
        }
    }

    /* sender will be prepared in tdav_session_msrp_start() */
    /* receiver will be prepared in tdav_session_msrp_start() */

    return ret;
}

int tdav_session_msrp_start(tmedia_session_t* self)
{
    tdav_session_msrp_t* msrp;
    int ret;

    TSK_DEBUG_INFO("tdav_session_msrp_start");

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    msrp = (tdav_session_msrp_t*)self;

    /* start the transport */
    if((ret = tnet_transport_start(msrp->transport))) {
        goto bail;
    }

    switch(msrp->setup) {
    case msrp_setup_active:
    case msrp_setup_actpass: {
        //
        //	ACTIVE
        //
        TSK_DEBUG_INFO("connectto(%s:%d)", msrp->remote_ip, msrp->remote_port);
        if((msrp->connectedFD = tnet_transport_connectto_2(msrp->transport, msrp->remote_ip, msrp->remote_port)) == TNET_INVALID_FD) {
            TSK_DEBUG_ERROR("Failed to connect to the remote party");
            ret = -2;
            goto bail;
        }
        else {
            //TSK_DEBUG_INFO("Msrp connected FD=%d", msrp->connectedFD);
            //if((ret = tnet_sockfd_waitUntilWritable(msrp->connectedFD, TDAV_MSRP_CONNECT_TIMEOUT)) && msrp->offerer){
            //	TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected to (%s:%d).", TDAV_MSRP_CONNECT_TIMEOUT, msrp->remote_ip, msrp->remote_port);
            //	goto bail;
            //}
            /*	draft-denis-simple-msrp-comedia-02 - 4.2.3. Setting up the connection
            	Once the TCP session is established, and if the answerer was the
            	active connection endpoint, it MUST send an MSRP request.  In
            	particular, if it has no pending data to send, it MUST send an empty
            	MSRP SEND request.  That is necessary for the other endpoint to
            	authenticate this TCP session.

            	...RFC 4975 - 7.1
            */
            msrp->send_bodiless = tsk_true;
        }
        break;
    }
    default: {
        //
        //	PASSIVE
        //
        break;
    }
    }

    // create and start the receiver
    if(!msrp->receiver) {
        if((msrp->receiver = tmsrp_receiver_create(msrp->config, msrp->connectedFD))) {
            tnet_transport_set_callback(msrp->transport, TNET_TRANSPORT_CB_F(tdav_transport_layer_stream_cb), msrp);
            if((ret = tmsrp_receiver_start(msrp->receiver, msrp, tdav_msrp_event_proxy_cb))) {
                TSK_DEBUG_ERROR("Failed to start the MSRP receiver");
                goto bail;
            }
        }
    }

    // create and start the sender
    if(!msrp->sender) {
        if((msrp->sender = tmsrp_sender_create(msrp->config, msrp->connectedFD))) {
            msrp->sender->chunck_duration = msrp->chunck_duration;
            if((ret = tmsrp_sender_start(msrp->sender))) {
                TSK_DEBUG_ERROR("Failed to start the MSRP sender");
                goto bail;
            }
        }
    }

bail:
    return ret;
}

int tdav_session_msrp_pause(tmedia_session_t* self)
{
    TSK_DEBUG_ERROR("Not Implemented");
    return -1;
}

int tdav_session_msrp_stop(tmedia_session_t* self)
{
    tdav_session_msrp_t* msrp;
    int ret;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    msrp = (tdav_session_msrp_t*)self;

    if(msrp->sender) {
        if((ret = tmsrp_sender_stop(msrp->sender))) {
            TSK_DEBUG_ERROR("Failed to stop the MSRP sender");
        }
    }
    if(msrp->receiver) {
        if((ret = tmsrp_receiver_stop(msrp->receiver))) {
            TSK_DEBUG_ERROR("Failed to stop the MSRP receiver");
        }
    }

    if(msrp->transport) {
        if((ret = tnet_transport_shutdown(msrp->transport))) {
            TSK_DEBUG_ERROR("Failed to stop the MSRP transport");
        }
    }

    return 0;
}

const tsdp_header_M_t* tdav_session_msrp_get_lo(tmedia_session_t* self)
{
    tdav_session_msrp_t* msrp;
    tsk_bool_t changed = tsk_false;

    const char* proto = "TCP/MSRP";
    const char* sheme = "msrp";

    TSK_DEBUG_INFO("tdav_session_msrp_get_lo");

    if(!self || !self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    msrp = (tdav_session_msrp_t*)self;

    if(!msrp->transport) {
        TSK_DEBUG_ERROR("Not prepared");
        return tsk_null;
    }

    if(/*TNET_SOCKET_TYPE_IS_TLS(type)*/ tsk_false) {
        proto = "TCP/TLS/MSRP";
        sheme = "msrps";
    }

    if(self->ro_changed && self->M.lo) {
        /* Codecs */
        tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "fmtp");
        tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "rtpmap");
        tsk_list_clear_items(self->M.lo->FMTs);

        /* QoS */
        tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "curr");
        tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "des");
        tsdp_header_A_removeAll_by_field(self->M.lo->Attributes, "conf");
    }

    changed = (self->ro_changed || !self->M.lo);

    if(!self->M.lo) {
        tsk_istr_t id;
        char* path = tsk_null;
        tnet_ip_t ip = "0.0.0.0";
        tnet_port_t port = 0;

        tsk_strrandom(&id);
        tnet_transport_get_public_ip_n_port(msrp->transport, msrp->transport->master->fd, &ip, &port);
        tsk_sprintf(&path, "%s://%s:%u/%s;tcp", sheme, ip, port, id); //tcp is ok even if tls is used.

        if((self->M.lo = tsdp_header_M_create(self->plugin->media, port, proto))) {
            tmsrp_uri_t* uri;

            tsdp_header_M_add_headers(self->M.lo,
                                      TSDP_FMT_VA_ARGS("*"),
                                      TSDP_HEADER_C_VA_ARGS("IN", msrp->useIPv6 ? "IP6" : "IP4", ip),
                                      TSDP_HEADER_A_VA_ARGS("path", path),
                                      tsk_null);

            if((uri = tmsrp_uri_parse(path, tsk_strlen(path)))) {
                TSK_OBJECT_SAFE_FREE(msrp->config->From_Path);
                msrp->config->From_Path = tmsrp_header_From_Path_create(uri);
                TSK_OBJECT_SAFE_FREE(uri);
            }
        }
        TSK_FREE(path);

        if(self->M.ro) { /* We are probably about to send 2xx INVITE(sdp) */
            /*	[OMA-TS-SIMPLE_IM-V1_0-20100322-C] - 5.8.1 Negotiate direction of the MSRP connection setup
            	Offer      Answer
            	________________
            	active     passive / holdconn
            	passive    active / holdconn
            	actpass    active / passive / holdconn
            	holdconn   holdconn
            */
            const tsdp_header_A_t* A;
            if((A = tsdp_header_M_findA(self->M.ro, "setup"))) {
                tdav_msrp_setup_t setup = setup_from_string(A->value);
                switch(setup) {
                case msrp_setup_passive:
                case msrp_setup_actpass: // Because of NAT issues "actpass -(always)-> active"
                    msrp->setup = msrp_setup_active;
                    break;
                case msrp_setup_active:
                    msrp->setup = msrp_setup_passive;
                    break;
                default:
                    break;
                }
                tsdp_header_M_add_headers(self->M.lo,
                                          TSDP_HEADER_A_VA_ARGS("connection", "new"),
                                          tsk_null
                                         );
            }
            msrp->offerer = tsk_false;
        }
        else { /* We are probably about to send initial INVITE */
            tsdp_header_M_add_headers(self->M.lo,
                                      TSDP_HEADER_A_VA_ARGS("connection", "new"),
                                      tsk_null
                                     );
            msrp->offerer = tsk_true;
        }

        /* direction */
        if(msrp->dir == tdav_msrp_dir_none) {
            msrp->dir = msrp->file.path ? tdav_msrp_dir_sendonly : tdav_msrp_dir_sendrecv;
        }

        /* Other SDP fields */
        populate_lo(msrp, tsk_true);
    } // !lo


    return self->M.lo;
}

int tdav_session_msrp_set_ro(tmedia_session_t* self, const tsdp_header_M_t* m)
{
    tdav_session_msrp_t* msrp;
    const tsdp_header_A_t* A;
    tsk_bool_t answer;

    TSK_DEBUG_INFO("tdav_session_msrp_set_ro");

    if(!self || !m) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    msrp = (tdav_session_msrp_t*)self;

    // answer or initial offer?
    answer = (self->M.lo != tsk_null);

    /* update remote offer */
    TSK_OBJECT_SAFE_FREE(self->M.ro);
    self->M.ro = tsk_object_ref((void*)m);

    if(self->M.lo) {
        if((match_offer(msrp, m))) {

        }
        else {
            TSK_DEBUG_ERROR("MSRP offer doesn't match");
            return -1;
        }
        /* QoS */
        if(self->qos) {
            tmedia_qos_tline_t* ro_tline;
            if(self->M.ro && (ro_tline = tmedia_qos_tline_from_sdp(self->M.ro))) {
                tmedia_qos_tline_set_ro(self->qos, ro_tline);
                TSK_OBJECT_SAFE_FREE(ro_tline);
            }
        }
    }

    /* To-Path */
    if((A = tsdp_header_M_findA(m, "path"))) {
        tmsrp_uri_t* uri;
        if((uri = tmsrp_uri_parse(A->value, tsk_strlen(A->value)))) {
            TSK_OBJECT_SAFE_FREE(msrp->config->To_Path);
            msrp->config->To_Path = tmsrp_header_To_Path_create(uri);
            TSK_OBJECT_SAFE_FREE(uri);
        }
    }

    // OMA-TS-SIMPLE_IM-V1_0-20080903-C - 5.8.1 Negotiate direction of the MSRP connection setup
    if((A = tsdp_header_M_findA(m, "setup"))) {
        tdav_msrp_setup_t setup = setup_from_string(A->value);
        switch(setup) {
        case msrp_setup_actpass:
        case msrp_setup_passive:
            msrp->setup = msrp_setup_active;
            break;
        case msrp_setup_active:
            msrp->setup = msrp_setup_passive;
            break;
        default:
            break;
        }
    }

    /* direction attribute */
    if(msrp->dir == tdav_msrp_dir_none) {
        if((A = tsdp_header_M_findA(m, "sendonly"))) {
            msrp->dir = tdav_msrp_dir_recvonly;
        }
        else if((A = tsdp_header_M_findA(m, "sendrecv"))) {
            msrp->dir = tdav_msrp_dir_sendrecv;
        }
        else if((A = tsdp_header_M_findA(m, "recvonly"))) {
            msrp->dir = tdav_msrp_dir_sendonly;
        }
    }

    /* Neg parameters */
    init_neg_types(msrp, m);


    /* [OMA-TS-SIMPLE_IM-V1_0-20100322-C] - 5.8.2 Support of Application Level Gateway */

    /* get connection associated to this media line
    * If the connnection is global, then the manager will call tdav_session_audio_set() */
    if(m->C && m->C->addr && !msrp->remote_ip) {
        tsk_strupdate(&msrp->remote_ip, m->C->addr);
        msrp->useIPv6 = tsk_striequals(m->C->addrtype, "IP6");
    }
    /* set remote port */
    msrp->remote_port = m->port;

    return 0;
}

/* ============ Public functions ================= */
int tdav_session_msrp_send_file(tmedia_session_msrp_t* self, const char* path, va_list *app)
{
    tdav_session_msrp_t* msrp;
    int ret;

    if(!path || !(msrp = (tdav_session_msrp_t*)self) || !msrp->sender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    //if(1 || !msrp->file.selector){
    //	/* File transfer without selector line
    //	- a=file-selector:name:"test.pcap" type:application/octet-stream size:20312
    //	*/
    //	FILE* file = fopen(path, "rb");
    //	const char* fname = path + tsk_strlen(path);
    //	while(fname && (fname> path) && *fname != '\\' && *fname != '/'){
    //		fname--;
    //	}

    //	if(file){
    //		tsk_size_t size = 0;
    //		fseek(file, 0L, SEEK_END);
    //		size = ftell(file);
    //		fclose(file);

    //		tsk_sprintf(&msrp->file.selector, "name:\"%s\" type:application/octet-stream size:%u",
    //			fname, size);
    //		tsdp_header_M_add_headers(TMEDIA_SESSION(self)->M.lo,
    //			TSDP_HEADER_A_VA_ARGS("file-selector", msrp->file.selector),
    //			tsk_null);
    //	}
    //}

    ret = tsmrp_sender_send_file(msrp->sender, path);

    return ret;
}

int tdav_session_msrp_send_message(tmedia_session_msrp_t* self, const void* data, tsk_size_t size, const tmedia_params_L_t *params)
{
    const tdav_session_msrp_t* msrp;
    const tmedia_param_t* param;
    int ret;
    const tsk_list_item_t* item;
    const char* content_type = tsk_null;
    const char* w_content_type = tsk_null;

    if(!data || !size || !(msrp = (tdav_session_msrp_t*)self) || !msrp->sender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_list_foreach(item, params) {
        if((param = TMEDIA_PARAM(item->data))) {
            if((param->media_type & tmedia_msrp) == param->media_type
                    && param->plugin_type == tmedia_ppt_session
                    && param->value_type == tmedia_pvt_pchar) {

                if(tsk_striequals(param->key, "content-type")) {
                    content_type = (const char*)param->value;
                }
                else if(tsk_striequals(param->key, "w-content-type")) {
                    w_content_type = (const char*)param->value;
                }
            }
        }
    }

    if(content_type || w_content_type) { // user-defined content-types
        ret = tsmrp_sender_send_data(msrp->sender, data, size, content_type, w_content_type);
    }
    else { // neg. content-types
        ret = tsmrp_sender_send_data(msrp->sender, data, size,
                                     msrp->neg_accept_type, msrp->neg_accept_w_type
                                    );
    }

    return ret;
}





//=================================================================================================
//	Session MSRp Plugin object definition
//
/* constructor */
static tsk_object_t* tdav_session_msrp_ctor(tsk_object_t * self, va_list * app)
{
    tdav_session_msrp_t *session = self;
    if(session) {
        /* init base: called by tmedia_session_create() */
        /* init self */
        TMEDIA_SESSION_MSRP(session)->send_file = tdav_session_msrp_send_file;
        TMEDIA_SESSION_MSRP(session)->send_message = tdav_session_msrp_send_message;

        session->config = tmsrp_config_create();
        session->setup = msrp_setup_actpass; /* draft-denis-simple-msrp-comedia-02 - 4.1.1. Sending the offer */
        session->dir = tdav_msrp_dir_none;
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_session_msrp_dtor(tsk_object_t * self)
{
    tdav_session_msrp_t *session = self;
    if(session) {

        /* deinit self (rtp manager should be destroyed after the producer) */
        TSK_OBJECT_SAFE_FREE(session->config);

        TSK_OBJECT_SAFE_FREE(session->receiver);
        TSK_OBJECT_SAFE_FREE(session->sender);
        TSK_OBJECT_SAFE_FREE(session->transport);

        TSK_FREE(session->remote_ip);
        TSK_FREE(session->local_ip);

        TSK_FREE(session->neg_accept_type);
        TSK_FREE(session->neg_accept_w_type);
        TSK_FREE(session->accept_types);
        TSK_FREE(session->accept_w_types);

        /* File */
        TSK_FREE(session->file.path);
        TSK_FREE(session->file.selector);
        TSK_FREE(session->file.disposition);
        TSK_FREE(session->file.date);
        TSK_FREE(session->file.icon);
        TSK_FREE(session->file.transfer_id);

        /* NAT Traversal context */
        TSK_OBJECT_SAFE_FREE(session->natt_ctx);

        /* deinit base */
        tmedia_session_deinit(self);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_session_msrp_def_s = {
    sizeof(tdav_session_msrp_t),
    tdav_session_msrp_ctor,
    tdav_session_msrp_dtor,
    tmedia_session_cmp,
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_msrp_plugin_def_s = {
    &tdav_session_msrp_def_s,

    tmedia_msrp,
    "message",

    tdav_session_msrp_set,
    tdav_session_msrp_get,
    tdav_session_msrp_prepare,
    tdav_session_msrp_start,
    tdav_session_msrp_pause,
    tdav_session_msrp_stop,

    /* Audio part */
    { tsk_null },

    tdav_session_msrp_get_lo,
    tdav_session_msrp_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_msrp_plugin_def_t = &tdav_session_msrp_plugin_def_s;

#endif /* !defined(HAVE_TINYMSRP) || HAVE_TINYMSRP */