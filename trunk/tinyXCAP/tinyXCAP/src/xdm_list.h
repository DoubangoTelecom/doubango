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
#ifndef _TINYXCAP_XDM_LIST_H_
#define _TINYXCAP_XDM_LIST_H_

#include "tinyxcap_config.h"

#define XDM_LIST_CREATE(l)				XDM_XXX_CREATE(l, list)
#define XDM_LIST_SAFE_FREE(l)			XDM_XXX_SAFE_FREE(l, list)

#define XDM_LIST_ITEM_CREATE(item)		XDM_XXX_CREATE(item, list_item)
#define XDM_LIST_ITEM_SAFE_FREE(item)	XDM_XXX_SAFE_FREE(item, list_item)

#define xdm_list_foreach(item, list)	for(item = list?list->head:0; item; item= item->next)

/* function pointer ... */
typedef void (*xdm_list_item_func_free)(void** item);

typedef struct xdm_list_item_s
{
	void* data;
	xdm_list_item_func_free func_free;
	struct xdm_list_item_s* next;
}
xdm_list_item_t;

typedef struct xdm_list_s
{
	xdm_list_item_t* head;
	int count;
}
xdm_list_t;

TINYXCAP_API void xdm_list_init(xdm_list_t* list);
TINYXCAP_API void xdm_list_add_item(xdm_list_t* list, xdm_list_item_t** item);
TINYXCAP_API void xdm_list_add_list(xdm_list_t* destination, xdm_list_t** source);
TINYXCAP_API void xdm_list_add_data(xdm_list_t* destination, void** data, xdm_list_item_func_free item_func_free);
TINYXCAP_API void xdm_list_remove_at(int position);
TINYXCAP_API void xdm_list_free(xdm_list_t** list);

TINYXCAP_API void xdm_list_item_init(xdm_list_item_t* item);
TINYXCAP_API void xdm_list_item_free(xdm_list_item_t** item);


#endif /* _TINYXCAP_XDM_LIST_H_ */