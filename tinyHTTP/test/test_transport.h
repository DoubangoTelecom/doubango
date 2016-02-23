#ifndef _TEST_HTTP_TRANSPORT_H_
#define _TEST_HTTP_TRANSPORT_H_

#define TEST_HTTP_CAT_(A, B) A ## B
#define TEST_HTTP_CAT(A, B) TEST_HTTP_CAT_(A, B)
#define TEST_HTTP_STRING_(A) #A
#define TEST_HTTP_STRING(A) TEST_HTTP_STRING_(A)

#define TEST_HTTP_LOCAL_IP				"0.0.0.0"
#define TEST_HTTP_LOCAL_PORT			8080
#define TEST_HTTP_REMOTE_IP				"google.com"
#define TEST_HTTP_REMOTE_PORT			80
#define TEST_HTTP_MIN_STREAM_CHUNCK_SIZE	0x32
#define TEST_HTTP_GET "GET / HTTP/1.1\r\n" \
	"Host: " TEST_HTTP_REMOTE_IP ":" TEST_HTTP_STRING(TEST_HTTP_REMOTE_PORT) "\r\n" \
	"Connection: keep-alive\r\n" \
	"Cache-Control: max-age=0\r\n" \
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n" \
	"User-Agent: Mozilla/5.0 (Windows NT 6.0) AppleWebKit/537.36 (KHTML, like Gecko)\r\n" \
	"Doubango 2.0\r\n" \
	"Accept-Encoding: gzip,deflate,sdch\r\n" \
	"Accept-Language: en-US,en;q=0.8\r\n" \
	"\r\n" \
 
static int test_http_transport_callback(const tnet_transport_event_t* e);

/************************************************
* test_http_peer_t
************************************************/
typedef struct test_http_peer_s {
    TSK_DECLARE_OBJECT;
    tnet_fd_t fd;
    tsk_buffer_t* buff;
}
test_http_peer_t;
typedef tsk_list_t test_http_peers_L_t;

static tsk_object_t* test_http_peer_ctor(tsk_object_t * self, va_list * app)
{
    return self;
}
static tsk_object_t* test_http_peer_dtor(tsk_object_t * self)
{
    test_http_peer_t *peer = self;
    if (peer) {
        TSK_OBJECT_SAFE_FREE(peer->buff);
    }

    return self;
}
static const tsk_object_def_t test_http_peer_def_s = {
    sizeof(test_http_peer_t),
    test_http_peer_ctor,
    test_http_peer_dtor,
    tsk_null,
};

static test_http_peer_t* test_http_peer_create(tnet_fd_t fd)
{
    test_http_peer_t* peer = tsk_object_new(&test_http_peer_def_s);
    peer->fd = fd;
    peer->buff = tsk_buffer_create_null();
    return peer;
}

static int test_http_peer_pred_fd_cmp(const tsk_list_item_t* item, const void* data)
{
    return ((test_http_peer_t*)item->data)->fd - *((const tnet_fd_t*)data);
}


/************************************************
* test_http_transport_t
************************************************/
typedef struct test_http_transport_s {
    TSK_DECLARE_OBJECT;

    test_http_peers_L_t* peers;
    tnet_transport_handle_t* handle;
}
test_http_transport_t;

static tsk_object_t* test_http_transport_ctor(tsk_object_t * self, va_list * app)
{
    test_http_transport_t *transport = self;
    if (transport) {
    }
    return self;
}
static tsk_object_t* test_http_transport_dtor(tsk_object_t * self)
{
    test_http_transport_t *transport = self;
    if(transport) {
        TSK_OBJECT_SAFE_FREE(transport->handle);
        TSK_OBJECT_SAFE_FREE(transport->peers);
    }

    return self;
}
static const tsk_object_def_t test_http_transport_def_s = {
    sizeof(test_http_transport_t),
    test_http_transport_ctor,
    test_http_transport_dtor,
    tsk_null,
};

static test_http_transport_t* test_http_transport_create(const char* local_ip, tnet_port_t local_port, enum tnet_socket_type_e socket_type)
{
    test_http_transport_t *transport = tsk_object_new(&test_http_transport_def_s);
    if (transport) {
        transport->peers = tsk_list_create();
        transport->handle = tnet_transport_create(local_ip, local_port, socket_type, "HTTP Transport");
        if (!transport->handle) {
            TSK_OBJECT_SAFE_FREE(transport);
            return tsk_null;
        }
        tnet_transport_set_callback(transport->handle, test_http_transport_callback, transport);
    }
    return transport;
}

static int test_http_transport_start(test_http_transport_t* self)
{
    if (self) {
        return tnet_transport_start(self->handle);
    }
    return -1;
}

// Up to the caller to free the returned object using TSK_OBJECT_SAFREE(peer)
static test_http_peer_t* test_http_transport_connect_to(test_http_transport_t* self, const char* dst_host, tnet_port_t dst_port)
{
    tnet_fd_t fd = tnet_transport_connectto_2(self->handle, dst_host, dst_port);
    if (fd > 0) {
        return test_http_peer_create(fd);
    }
    return tsk_null;
}

static tsk_size_t test_http_transport_send_data(test_http_transport_t* self, tnet_fd_t fd, const void* data_ptr, tsk_size_t data_size)
{
    if (self && self->handle && data_ptr && data_size) {
        return tnet_transport_send(self->handle, fd, data_ptr, data_size);
    }
    return 0;
}

static void test_http_transport_add_peer(test_http_transport_t* self, test_http_peer_t* peer)
{
    tsk_list_lock(self->peers);
    tsk_list_push_back_data(self->peers, &peer);
    tsk_list_unlock(self->peers);
}

static void test_http_transport_remove_peer_by_fd(test_http_transport_t* self, tnet_fd_t fd)
{
    tsk_list_lock(self->peers);
    tsk_list_remove_item_by_pred(self->peers, test_http_peer_pred_fd_cmp, &fd);
    tsk_list_unlock(self->peers);
}

static const test_http_peer_t* test_http_transport_find_peer_by_fd(test_http_transport_t* self, tnet_fd_t fd)
{
    const tsk_list_item_t* item;
    tsk_list_lock(self->peers);
    item = tsk_list_find_item_by_pred(self->peers, test_http_peer_pred_fd_cmp, &fd);
    tsk_list_unlock(self->peers);
    if (item) {
        return (const test_http_peer_t*)item->data;
    }
    return tsk_null;
}

static void test_http_transport_process_incoming_msg(test_http_transport_t* self, const thttp_message_t* msg, tnet_fd_t fd)
{
    if (THTTP_MESSAGE_IS_REQUEST(msg)) {
        if (tsk_striequals(msg->line.request.method, "GET")) {
            char* result = tsk_null;
            const char* content = "<html><body>Hello world!</body></html>";
            int len = tsk_sprintf(
                          (char**)&result,
                          "HTTP/1.1 %u %s\r\n"
                          "Server: My test server \r\n"
                          "Access-Control-Allow-Origin: *\r\n"
                          "Content-Length: %u\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: Close\r\n"
                          "\r\n"
                          "%s", 200, "OK", tsk_strlen(content), content);
            tnet_transport_send(self, fd, result, len);
            TSK_FREE(result);
        }
    }
}

static int test_http_transport_callback(const tnet_transport_event_t* e)
{
    test_http_transport_t* transport = (test_http_transport_t*)e->callback_data;
    const test_http_peer_t* _peer;
    thttp_message_t *message = tsk_null;
    int endOfheaders = -1;
    tsk_ragel_state_t state;
    tsk_bool_t have_all_content = tsk_false;
    int ret;
    switch (e->type) {
    case event_closed: {
        test_http_transport_remove_peer_by_fd(transport, e->local_fd);
        return 0;
    }

    case event_connected:
    case event_accepted: {
        _peer = test_http_transport_find_peer_by_fd(transport, e->local_fd);
        if (!_peer) {
            _peer = test_http_peer_create(e->local_fd);
            test_http_transport_add_peer(transport, (test_http_peer_t*)_peer);
        }
        return 0;
    }

    case event_data: {
        TSK_DEBUG_INFO("\n\nRECV: %.*s\n\n", e->size, (const char*)e->data);
        break;
    }
    default:
        return 0;
    }


    _peer = test_http_transport_find_peer_by_fd(transport, e->local_fd);
    if(!_peer) {
        TSK_DEBUG_ERROR("Data event but no peer found!");
        return -1;
    }

    /* Append new content. */
    tsk_buffer_append(_peer->buff, e->data, e->size);

    /* Check if we have all HTTP headers. */
parse_buffer:
    if ((endOfheaders = tsk_strindexOf(TSK_BUFFER_DATA(_peer->buff), TSK_BUFFER_SIZE(_peer->buff), "\r\n\r\n"/*2CRLF*/)) < 0) {
        TSK_DEBUG_INFO("No all HTTP headers in the TCP buffer.");
        goto bail;
    }

    /* If we are here this mean that we have all HTTP headers.
    *	==> Parse the HTTP message without the content.
    */
    tsk_ragel_state_init(&state, TSK_BUFFER_DATA(_peer->buff), endOfheaders + 4/*2CRLF*/);
    if (!(ret = thttp_message_parse(&state, &message, tsk_false/* do not extract the content */))) {
        const thttp_header_Transfer_Encoding_t* transfer_Encoding;

        /* chunked? */
        if((transfer_Encoding = (const thttp_header_Transfer_Encoding_t*)thttp_message_get_header(message, thttp_htype_Transfer_Encoding)) && tsk_striequals(transfer_Encoding->encoding, "chunked")) {
            const char* start = (const char*)(TSK_BUFFER_TO_U8(_peer->buff) + (endOfheaders + 4/*2CRLF*/));
            const char* end = (const char*)(TSK_BUFFER_TO_U8(_peer->buff) + TSK_BUFFER_SIZE(_peer->buff));
            int index;

            TSK_DEBUG_INFO("CHUNKED transfer.");
            while(start < end) {
                /* RFC 2616 - 19.4.6 Introduction of Transfer-Encoding */
                // read chunk-size, chunk-extension (if any) and CRLF
                tsk_size_t chunk_size = (tsk_size_t)tsk_atox(start);
                if((index = tsk_strindexOf(start, (end-start), "\r\n")) >=0) {
                    start += index + 2/*CRLF*/;
                }
                else {
                    TSK_DEBUG_INFO("Parsing chunked data has failed.");
                    break;
                }

                if(chunk_size == 0 && ((start + 2) <= end) && *start == '\r' && *(start+ 1) == '\n') {
                    int parsed_len = (start - (const char*)(TSK_BUFFER_TO_U8(_peer->buff))) + 2/*CRLF*/;
                    tsk_buffer_remove(_peer->buff, 0, parsed_len);
                    have_all_content = tsk_true;
                    break;
                }

                thttp_message_append_content(message, start, chunk_size);
                start += chunk_size + 2/*CRLF*/;
            }
        }
        else {
            tsk_size_t clen = THTTP_MESSAGE_CONTENT_LENGTH(message); /* MUST have content-length header. */
            if(clen == 0) { /* No content */
                tsk_buffer_remove(_peer->buff, 0, (endOfheaders + 4/*2CRLF*/)); /* Remove HTTP headers and CRLF ==> must never happen */
                have_all_content = tsk_true;
            }
            else { /* There is a content */
                if((endOfheaders + 4/*2CRLF*/ + clen) > TSK_BUFFER_SIZE(_peer->buff)) { /* There is content but not all the content. */
                    TSK_DEBUG_INFO("No all HTTP content in the TCP buffer.");
                    goto bail;
                }
                else {
                    /* Add the content to the message. */
                    thttp_message_add_content(message, tsk_null, TSK_BUFFER_TO_U8(_peer->buff) + endOfheaders + 4/*2CRLF*/, clen);
                    /* Remove HTTP headers, CRLF and the content. */
                    tsk_buffer_remove(_peer->buff, 0, (endOfheaders + 4/*2CRLF*/ + clen));
                    have_all_content = tsk_true;
                }
            }
        }
    }

    /* Alert the dialog (FSM) */
    if(message) {
        if (have_all_content) { /* only if we have all data */
            test_http_transport_process_incoming_msg(transport, message, e->local_fd);
            /* Parse next chunck */
            if (TSK_BUFFER_SIZE(_peer->buff) >= TEST_HTTP_MIN_STREAM_CHUNCK_SIZE) {
                TSK_OBJECT_SAFE_FREE(message);
                goto parse_buffer;
            }
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(message);

    return 0;
}


/************************************************
* Main
************************************************/
static void test_transport()
{
    test_http_transport_t* p_transport = tsk_null;
    test_http_peer_t* peer = tsk_null;
    int ret;
    static const char* __get_msg = TEST_HTTP_GET;

    p_transport = test_http_transport_create(TEST_HTTP_LOCAL_IP, TEST_HTTP_LOCAL_PORT, tnet_socket_type_tcp_ipv4);
    if (!p_transport) {
        TSK_DEBUG_ERROR("Failed to HTTP transport");
        goto bail;
    }

    ret = test_http_transport_start(p_transport);
    if (ret) {
        TSK_DEBUG_ERROR("Failed to start HTTP transport with error code = %d", ret);
        goto bail;
    }

    getchar();

    // Send data to google.com
    peer = test_http_transport_connect_to(p_transport, TEST_HTTP_REMOTE_IP, TEST_HTTP_REMOTE_PORT);
    ret = tnet_sockfd_waitUntilWritable(peer->fd, 1000); // you should use the callback function instead of blocking the process
    if (ret) {
        TSK_DEBUG_ERROR("Failed to connect to(%s,%d) with error code = %d", TEST_HTTP_REMOTE_IP, TEST_HTTP_REMOTE_PORT, ret);
        goto bail;
    }
    ret = test_http_transport_send_data(p_transport, peer->fd, __get_msg, tsk_strlen(__get_msg));
    TSK_DEBUG_INFO("Sent %d bytes", ret);

    getchar();


bail:
    TSK_OBJECT_SAFE_FREE(p_transport); // stop server and free memory
    TSK_OBJECT_SAFE_FREE(peer);
}

#endif /* _TEST_HTTP_TRANSPORT_H_ */
