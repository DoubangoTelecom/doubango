/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tnet_transport.c
 * @brief Network transport layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_transport.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"
#include "tsk_thread.h"

#define DGRAM_MAX_SIZE	8192
#define STREAM_MAX_SIZE	8192

typedef struct tnet_transport_s
{
	tnet_socket_t *server;
	tnet_socket_t *client;

	void *context;

	void* tid[1];

	char *description;
	unsigned running:1;
	unsigned connected:1;
}
tnet_transport_t;

static void *tnet_transport_thread(void *param);


tnet_transport_handle_t* tnet_transport_start(const char* host, uint16_t port, tnet_socket_type_t type, const char* description)
{
	int err;
	tnet_transport_t *transport = tsk_object_new(tnet_transport_def_t, host, port, type, description);
	if(!TNET_SOCKET_IS_VALID(transport->server))
	{
		tsk_object_unref(transport);
		TSK_DEBUG_ERROR("Failed to create new transport.");
		return 0;
	}

	/* Create new Network thread. */
	if(err = tsk_thread_create(&(transport->tid[0]), tnet_transport_thread, transport))
	{
		tsk_object_unref(transport);
		TSK_DEBUG_FATAL("Failed to start transport thread with error: %d\n", err);
		return 0;
	}

	transport->running = 1;
	return transport;
}

int tnet_transport_get_isconnected(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		return transport->connected;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

const char* tnet_transport_get_description(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		return transport->description;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

uint16_t tnet_transport_get_local_port(const tnet_transport_handle_t *handle, int client)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		if(TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type) || !client)
		{
			return transport->server->port;
		}
		else
		{
			return (client && transport->client) ? transport->client->port : 0;
		}
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

const char* tnet_transport_get_local_ip(const tnet_transport_handle_t *handle, int client)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		if(TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type) || !client)
		{
			return transport->server->hostname;
		}
		else
		{
			return (client && transport->client) ? transport->client->hostname : 0;
		}
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return 0;
}

tnet_socket_type_t tnet_transport_get_socket_type(const tnet_transport_handle_t *handle)
{
	if(handle)
	{
		const tnet_transport_t *transport = handle;
		transport->server->type;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL transport object.");
	}
	return tnet_socket_type_invalid;
}



int tnet_transport_shutdown(tnet_transport_handle_t** handle)
{
	*handle = 0;
	return -1;
}






//========================================================
//	Transport object definition
//
static void* tnet_transport_create(void * self, va_list * app)
{
	tnet_transport_t *transport = self;
	if(transport)
	{
		const char *host = va_arg(*app, const char*);
		uint16_t port = va_arg(*app, uint16_t);
		tnet_socket_type_t type = va_arg(*app, tnet_socket_type_t);
		const char *description = va_arg(*app, const char*);

		if(description)
		{
			transport->description = tsk_strdup(description);
		}
		
		transport->server = TNET_SOCKET_CREATE(host, port, type);
		transport->client = 0;
	}
	return self;
}

static void* tnet_transport_destroy(void * self)
{ 
	tnet_transport_t *transport = self;
	if(transport)
	{
		TNET_SOCKET_SAFE_FREE(transport->server);
		TSK_FREE(transport->description);
	}

	return self;
}

static const tsk_object_def_t tnet_transport_def_s = 
{
	sizeof(tnet_transport_t),
	tnet_transport_create, 
	tnet_transport_destroy,
	0, 
};
const void *tnet_transport_def_t = &tnet_transport_def_s;































#if HAVE_POLL
/*****************************************************
*
*	Network manager thread using POLL.
*
*****************************************************/


#elif(TNET_UNDER_WINDOWS) /* !HAVE_POLL */
/*****************************************************
*
*	Network manager thread using WIN32 WSA* functions.
*
*****************************************************/

/*== Socket description */
typedef struct tnet_socket_desc_s
{
	WSABUF wsaBuffer;
	int32_t fd;

	DWORD readCount;
	DWORD writeCount;
}
tnet_socket_desc_t;

/*== Transport context structure definition */
typedef struct tnet_transport_ctx_s
{
	DWORD eventsCount;
	WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
	tnet_socket_desc_t* sockets[WSA_MAXIMUM_WAIT_EVENTS];
}
tnet_transport_ctx_t;

/*== Add socket */
static void tnet_socket_desc_add(int32_t fd, tnet_transport_ctx_t *context)
{
	tnet_socket_desc_t *socket_desc = tsk_calloc(1, sizeof(tnet_socket_desc_t));
	socket_desc->fd = fd;
	
	context->events[context->eventsCount] = WSACreateEvent();
	context->sockets[context->eventsCount] = socket_desc;

	context->eventsCount++;
}

/*== Remove socket */
static void tnet_socket_desc_remove(int index, tnet_transport_ctx_t *context)
{

}

/*=== */
int CALLBACK AcceptCondFunc(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQos, LPQOS lpGQos, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR	*Group, DWORD CallbackData)
{
	tnet_transport_ctx_t *context = (tnet_transport_ctx_t*)CallbackData;
	return context->eventsCount < WSA_MAXIMUM_WAIT_EVENTS ? CF_ACCEPT : CF_REJECT;
}


/* == Register new socket */
int tnet_transport_add_socket(const tnet_transport_handle_t *handle, int32_t fd)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	tnet_transport_ctx_t *context;
	int ret = -1;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	context = (tnet_transport_ctx_t*)transport->context;
	tnet_socket_desc_add(fd, context);
	if(WSAEventSelect(fd, context->events[context->eventsCount - 1], FD_ALL_EVENTS/*FD_READ | FD_CONNECT | FD_CLOSE*/) == SOCKET_ERROR)
	{
		tnet_socket_desc_remove((context->eventsCount - 1), context);
		TNET_PRINT_LAST_ERROR();
		return -1;
	}

	/* Signal */
	WSASetEvent(context->events[0]);

	return 0;
}

/*== Connect stream socket to the specified destination. */
int tnet_transport_connectto(const tnet_transport_handle_t *handle, const char* host, uint16_t port)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	struct sockaddr to;
	int ret = -1;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}

	if(!TNET_SOCKET_TYPE_IS_STREAM(transport->server->type))
	{
		TSK_DEBUG_ERROR("In order to use WSAConnect you must use a stream transport.");
		return ret;
	}

	if(transport->connected || transport->client)
	{
		TSK_DEBUG_ERROR("Transport already connected.");
		return ret;
	}

	/* Init sockaddr fields */
	if(tnet_sockaddr_init(host, port, transport->server->type, &to))
	{
		TSK_DEBUG_ERROR("Invalid host or port.");
		return ret;
	}

	/* Create client socket. */
	transport->client = TNET_SOCKET_CREATE(0, 0, transport->server->type);
	if(!TNET_SOCKET_IS_VALID(transport->client) || tnet_transport_add_socket(handle, transport->client->fd))
	{
		TNET_PRINT_LAST_ERROR();
		TNET_SOCKET_SAFE_FREE(transport->client);
		return ret;
	}

	if((ret = WSAConnect(transport->client->fd, (LPSOCKADDR)&to, sizeof(to), NULL, NULL, NULL, NULL)) == SOCKET_ERROR)
	{
		if((ret = WSAGetLastError()) == WSAEWOULDBLOCK)
		{
			TSK_DEBUG_INFO("WSAEWOULDBLOCK error for WSAConnect operation");
			ret = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR();
			return ret;
		}
	} else ret = 0;
	
	return ret;
}

/*== send stream data to the remote peer*/
int tnet_transport_send(const tnet_transport_handle_t *handle, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	int ret = -1;
	WSABUF wsaBuffer;
	DWORD numberOfBytesSent;

	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid transport handle.");
		return ret;
	}

	if(!transport->connected || !transport->client)
	{
		TSK_DEBUG_ERROR("You MUST connect first.");
		return ret;
	}

	if(!TNET_SOCKET_TYPE_IS_STREAM(transport->client->type))
	{
		TSK_DEBUG_ERROR("In order to use WSASend you must use a stream transport.");
		return ret;
	}

	wsaBuffer.buf = (CHAR*)buf;
	wsaBuffer.len = size;

	if((ret = WSASend(transport->client->fd, &wsaBuffer, 1, &numberOfBytesSent, 0, NULL, NULL)) == SOCKET_ERROR)
	{
		if((ret = WSAGetLastError()) == WSAEWOULDBLOCK)
		{
			TSK_DEBUG_INFO("WSAEWOULDBLOCK error for WSASend operation");
			ret = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR();
			return ret;
		}
	}else ret = 0;

	return ret;
}

/*== send dgarm to the specified destionation */
int tnet_transport_sendto(const tnet_transport_handle_t *handle, const struct sockaddr *to, const void* buf, size_t size)
{
	tnet_transport_t *transport = (tnet_transport_t*)handle;
	WSABUF wsaBuffer;
	DWORD numberOfBytesSent;
	int ret = -1;
	
	if(!transport)
	{
		TSK_DEBUG_ERROR("Invalid server handle.");
		return ret;
	}
	
	if(!TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type))
	{
		TSK_DEBUG_ERROR("In order to use WSASendTo you must use an udp transport.");
		return ret;
	}
	
	wsaBuffer.buf = (CHAR*)buf;
	wsaBuffer.len = size;
	
    if((ret = WSASendTo(transport->server->fd, &wsaBuffer, 1, &numberOfBytesSent, 0, to, sizeof(*to), 0, 0)) == SOCKET_ERROR)
	{
		if((ret = WSAGetLastError()) == WSAEWOULDBLOCK)
		{
			TSK_DEBUG_INFO("WSAEWOULDBLOCK error for WSASendTo operation");
			ret = 0;
		}
		else
		{
			TNET_PRINT_LAST_ERROR();
			return ret;
		}
	} else ret = 0;
	
	TSK_DEBUG_INFO("[%s] %d bytes have been sent using WSASendTo", transport->description, numberOfBytesSent);
	
	return ret;
}

/*=== Main thread */
static void *tnet_transport_thread(void *param)
{
	tnet_transport_t *transport = param;
	tnet_transport_ctx_t *context;
	DWORD evt;
	WSANETWORKEVENTS networkEvents;
	DWORD flags = 0;
	int ret;
	
	context = (tnet_transport_ctx_t*)tsk_calloc(1, sizeof(tnet_transport_ctx_t));
	transport->context = context;

	/* Start listening */
	if(TNET_SOCKET_TYPE_IS_STREAM(transport->server->type))
	{
		if(listen(transport->server->fd, WSA_MAXIMUM_WAIT_EVENTS))
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}
	}

	/* Add the current the transport socket to the context. */
	tnet_socket_desc_add(transport->server->fd, context);
	if(ret = WSAEventSelect(transport->server->fd, context->events[context->eventsCount - 1], TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type) ? FD_READ : FD_ALL_EVENTS/*FD_ACCEPT | FD_READ | FD_CONNECT | FD_CLOSE*/) == SOCKET_ERROR)
	{
		TNET_PRINT_LAST_ERROR();
		goto bail;
	}

	TSK_DEBUG_INFO("Starting [%s] server with IP {%s} on port {%d}...", transport->description, transport->server->hostname, transport->server->port);
	
	while(transport->running)
	{
		WSAEVENT active_event;
		tnet_socket_desc_t* active_socket;
		int index;

		/* Wait for multiple events */
		if((evt = WSAWaitForMultipleEvents(context->eventsCount, context->events, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}

		if(!transport->running)
		{
			goto bail;
		}

		/* Get active event and socket */
		index = (evt - WSA_WAIT_EVENT_0);
		active_event = context->events[index];
		active_socket = context->sockets[index];

		/* Get the network events flags */
		if (WSAEnumNetworkEvents(active_socket->fd, active_event, &networkEvents) == SOCKET_ERROR)
		{
			TNET_PRINT_LAST_ERROR();
			goto bail;
		}

		/*================== FD_ACCEPT ==================*/
		if(networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			int32_t fd;
			
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_ACCEPT", transport->description);

			if(networkEvents.iErrorCode[FD_ACCEPT_BIT])
			{
				TSK_DEBUG_ERROR("ACCEPT FAILED.");
				TNET_PRINT_LAST_ERROR();
				continue;
			}
			
			/* Accept the connection */
			if((fd = WSAAccept(active_socket->fd, NULL, NULL, AcceptCondFunc, (DWORD_PTR)context)) != INVALID_SOCKET)
			{
				/* Add the new fd to the server context */
				tnet_socket_desc_add(fd, context);
				if(WSAEventSelect(fd, context->events[context->eventsCount - 1], FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
				{
					tnet_socket_desc_remove((context->eventsCount - 1), context);
					TNET_PRINT_LAST_ERROR();
					continue;
				}
			}
			else
			{
				TSK_DEBUG_ERROR("ACCEPT FAILED.");
				TNET_PRINT_LAST_ERROR();
				continue;
			}

			


		}

		/*================== FD_CONNECT ==================*/
		if(networkEvents.lNetworkEvents & FD_CONNECT)
		{
			//int32_t fd;

			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CONNECT", transport->description);

			if(networkEvents.iErrorCode[FD_CONNECT_BIT])
			{
				TSK_DEBUG_ERROR("CONNECT FAILED.");
				TNET_PRINT_LAST_ERROR();
				continue;
			}
			else
			{
				if(!transport->connected)
				{
					transport->connected = 1;
				}
			}
		}


		/*================== FD_READ ==================*/
		if(networkEvents.lNetworkEvents & FD_READ)
		{
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_READ", transport->description);

			if(networkEvents.iErrorCode[FD_READ_BIT])
			{
				TSK_DEBUG_ERROR("READ FAILED.");
				TNET_PRINT_LAST_ERROR();
				continue;
			}
			
			/* Create socket's internal buffer. */
			if(!active_socket->wsaBuffer.buf)
			{
				size_t max_buffer_size = TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type) ? DGRAM_MAX_SIZE : STREAM_MAX_SIZE;
				active_socket->wsaBuffer.buf = tsk_calloc(max_buffer_size, sizeof(char));
				active_socket->wsaBuffer.len = max_buffer_size;
			}

			/* Receive the waiting data. */
			if(WSARecv(active_socket->fd, &(active_socket->wsaBuffer), 1, &(active_socket->readCount), &flags, 0, 0) == SOCKET_ERROR)
			{
				if(WSAGetLastError() == WSAEWOULDBLOCK)
				{
					TSK_DEBUG_INFO("WSAEWOULDBLOCK error for READ operation");
				}
				else
				{
					tnet_socket_desc_remove(index, context);
					TNET_PRINT_LAST_ERROR();
					continue;
				}
			}
			else
			{
				TSK_DEBUG_INFO("WSARecv(%d bytes) success on [%s]", active_socket->readCount, transport->description);
			}
		}




		/*================== FD_WRITE ==================*/
		if(networkEvents.lNetworkEvents & FD_WRITE)
		{
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_WRITE", transport->description);

			if(networkEvents.iErrorCode[FD_WRITE_BIT])
			{
				TSK_DEBUG_ERROR("WRITE FAILED.");
				TNET_PRINT_LAST_ERROR();
				continue;
			}

			/*{
				WSAOVERLAPPED RecvOverlapped = {0};
				RecvOverlapped.hEvent = active_event;

				
				//* Create socket's internal buffer.
				if(!active_socket->wsaBuffer.buf)
				{
					size_t max_buffer_size = TNET_SOCKET_TYPE_IS_DGRAM(transport->server->type) ? DGRAM_MAX_SIZE : STREAM_MAX_SIZE;
					active_socket->wsaBuffer.buf = tsk_calloc(max_buffer_size, sizeof(char));
					active_socket->wsaBuffer.len = max_buffer_size;
				}

				//* Receive the waiting data. 
				if(WSARecv(active_socket->fd, &(active_socket->wsaBuffer), 1, &(active_socket->readCount), &flags, &RecvOverlapped, NULL) == SOCKET_ERROR)
				{
					if(WSAGetLastError() == WSAEWOULDBLOCK)
					{
						TSK_DEBUG_INFO("WSAEWOULDBLOCK error for READ operation");
					}
					else
					{
						tnet_socket_desc_remove(index, context);
						TNET_PRINT_LAST_ERROR();
						//continue;
					}
				}
				else
				{
					TSK_DEBUG_INFO("WSARecv(%d bytes) success on [%s]", active_socket->readCount, transport->description);
				}

				//ret = WSAGetOverlappedResult(active_socket->fd, &RecvOverlapped, &active_socket->readCount, FALSE, &flags);
				WSAEventSelect(active_socket->fd, active_event, FD_READ | FD_WRITE | FD_CLOSE);
				WaitForSingleObject( active_event, INFINITE );
			}*/
		}


		


		/*================== FD_CLOSE ==================*/
		if(networkEvents.lNetworkEvents & FD_CLOSE)
		{
			TSK_DEBUG_INFO("NETWORK EVENT FOR SERVER [%s] -- FD_CLOSE", transport->description);
		}

		/* Reset event */
		WSAResetEvent(active_event);

	} /* while(transport->running) */
	

bail:
	//WSACloseEvent
	transport->running = 0;
	return 0;
}


#else
#error "You MUST use Poll or WSA* functions to handle network operations"
#endif /* HAVE_POLL */