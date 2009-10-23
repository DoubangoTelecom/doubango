/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#include "xdm_rls.h"
#include "xdm_utils.h"

#define RLS_RETURN_IF_INVALID(rls) if(!rls || !(rls->docPtr)) return 0;

//static const char* xdm_rls_ns = "urn:ietf:params:xml:ns:rls-services";

#define RLS_XML_HEADER	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
						"<rls-services xmlns=\"urn:ietf:params:xml:ns:rls-services\">"
#define RLS_XML_FOOTER	"</rls-services>"

/* init service */
void xdm_rls_service_init(xdm_rls_service_t *service)
{
	memset(service, 0, sizeof(xdm_rls_service_t));
}


/* set service's uri and resource-list */
void xdm_rls_service_set(xdm_rls_service_t *service, const char* uri, const char* resource_list)
{
	if(service)
	{		
		xdm_strupdate(&(service->uri), uri);
		xdm_strupdate(&(service->resource_list), resource_list);
	}
}

/* add package to the existing list */
void xdm_rls_service_add_package(xdm_rls_service_t *service, const char* package)
{
	xdm_list_item_t *item = 0;
	if(service)
	{
		if(!(service->packages)) 
		{
			XDM_LIST_CREATE(service->packages);
		}
		
		XDM_LIST_ITEM_CREATE(item);
		item->data = (void*)xdm_strdup(package);
		xdm_list_add_item(service->packages, &item);
	}
}

/* free service */
void xdm_rls_service_free(void **_service)
{
	xdm_rls_service_t **service = ((xdm_rls_service_t**)_service);

	XDM_SAFE_FREE((*service)->uri);
	XDM_SAFE_FREE((*service)->resource_list);
	XDM_LIST_SAFE_FREE((*service)->packages);

	free(*_service);
	(*_service) = 0;
}

/* xml<->service binding*/
/* ATTENTION: use 'XDM_RLS_SERVICE_SAFE_FREE' function to free the returned object */
xdm_rls_service_t* xdm_rls_service_from_xml(const xmlNodePtr node)
{
	xmlNodePtr node2 = 0;
	xdm_rls_service_t *rls_service = 0;
	xdm_list_item_t* item = 0;

	if(xdm_xml_find_node(node, "service", nft_none))
	{
		XDM_RLS_SERVICE_CREATE(rls_service);
				
		/* uri */
		node2 = xdm_xml_select_node(node, 
			XDM_XML_NODE_SELECT_ATT_VALUE("service", "uri"),
			XDM_XML_NODE_SELECT_END());
		rls_service->uri = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* resource-list */
		node2 = xdm_xml_select_node(node, XDM_XML_NODE_SELECT_BY_NAME("service"),
			XDM_XML_NODE_SELECT_BY_NAME("resource-list"),
			XDM_XML_NODE_SELECT_END());
		rls_service->resource_list = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2));

		/* packages */
		node2 = xdm_xml_select_node(node, XDM_XML_NODE_SELECT_BY_NAME("service"),
			XDM_XML_NODE_SELECT_BY_NAME("packages"),
			XDM_XML_NODE_SELECT_CONTENT(),
			XDM_XML_NODE_SELECT_END());
		/* select first package */
		if(!xdm_xml_find_node(node2, "package", nft_none)) node2 = xdm_xml_find_node(node2, "package", nft_next);
		if(node2)
		{
			XDM_LIST_CREATE(rls_service->packages);
			do
			{
				XDM_LIST_ITEM_CREATE(item);
				item->data = xdm_strdup(XDM_XML_NODE_SAFE_GET_TEXTVALUE(node2->children));
				xdm_list_add_item(rls_service->packages, &item);
			}
			while(node2 = xdm_xml_find_node(node2, "service", nft_next));
		}
	}else return 0;

	return rls_service;
}

/* create rls context */
xdm_rls_t* xdm_rls_create(const char* buffer, size_t size)
{
	if(buffer && size)
	{
		xdm_rls_t* rls = (xdm_rls_t*)malloc(sizeof(xdm_rls_t));
		memset(rls, 0, sizeof(xdm_rls_t));
		rls->docPtr = xmlParseMemory(buffer, (int)size);

		return rls;
	}

	return 0;
}

/* get all services (xdm_rls_service_t)*/
xdm_rls_service_L_t* xdm_rls_get_all_services(const xdm_rls_t* rls)
{
	xdm_rls_service_t* rls_service = 0;
	xdm_rls_service_L_t* list = 0;
	xmlNodePtr node = 0;

	RLS_RETURN_IF_INVALID(rls);
	
	/* root */
	node = xdm_xml_select_node(rls->docPtr->children,
		XDM_XML_NODE_SELECT_BY_NAME("rls-services"),
		XDM_XML_NODE_SELECT_END());

	/* select first service */
	if(!xdm_xml_find_node(node, "service", nft_none)) node = xdm_xml_find_node(node, "service", nft_next);
	if(node)
	{
		XDM_LIST_CREATE(list);
		do
		{
			rls_service = xdm_rls_service_from_xml(node);
			xdm_list_add_data(list, ((void**) &rls_service), xdm_rls_service_free);
		}
		while(node = xdm_xml_find_node(node, "service", nft_next));
	}
	return list;
}

/* serialize service */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rls_service_serialize(const xdm_rls_service_t *service)
{
	char* service_str = 0;
	char* package_str = 0;
	xdm_list_item_t* item = 0;

	if(!service) return 0;

	/* packages */
	xdm_list_foreach(item, service->packages)
	{
		char* curr = 0;
		xdm_sprintf(&curr, "<package>%s</package>", ((char*)item->data));
		xdm_strcat(&package_str, curr);
		XDM_SAFE_FREE(curr);
	}
	/* service */
	xdm_sprintf(&service_str,
				"<service uri=\"%s\">"
					"<resource-list>%s</resource-list>"
					"<packages>"
						"%s"
					"</packages>"
				"</service>",
				service->uri, service->resource_list, package_str);
	XDM_SAFE_FREE(package_str);
	return service_str;
}

/* serialize services with xml header */
/* ATTENTION: use 'XDM_SAFE_FREE' macro to free the returned string */
char* xdm_rls_rls_serialize(const xdm_list_t *services)
{
	xdm_list_item_t* item = 0;
	char* services_str = 0;

	if(!services) return 0;

	/* xml header */
	xdm_strcat(&services_str, RLS_XML_HEADER);

	xdm_list_foreach(item, services)
	{
		/* get service */
		xdm_rls_service_t *service = ((xdm_rls_service_t*)item->data);
		char* service_str = xdm_rls_service_serialize(service);
		xdm_strcat(&services_str, service_str);
		XDM_FREE(service_str);
	}
	
	/* xml footer */
	xdm_strcat(&services_str, RLS_XML_FOOTER);

	return services_str;
}

/* free rls context */
void xdm_rls_free(xdm_rls_t **rls)
{
	if(*rls)
	{	
		xmlFreeDoc((*rls)->docPtr);
		
		free(*rls);
		(*rls)=0;
	}
}

#undef RLS_RETURN_IF_INVALID

#undef RLS_XML_HEADER
#undef RLS_XML_FOOTER