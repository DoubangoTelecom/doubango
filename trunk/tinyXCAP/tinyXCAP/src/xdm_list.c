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
#include "xdm_list.h"
#include "xdm_utils.h"

/* initialize the list */
void xdm_list_init(xdm_list_t* list)
{
	memset(list, 0, sizeof(xdm_list_t));
}

/* add an item to the list */
void xdm_list_add_item(xdm_list_t* list, xdm_list_item_t** item)
{
	(*item)->next  = list->head;
    list->head = *item;
	
	list->count++;
	(*item) = 0;
}

/* add a list to the list */
void xdm_list_add_list(xdm_list_t* destination, xdm_list_t** source)
{
	xdm_list_item_t* next = 0;
	xdm_list_item_t* curr = (*source)->head;

	while(curr)
	{
		next = curr->next;
		xdm_list_add_item(destination, &curr);
		curr = next->next;
	}
	(*source) = 0;
}

/* add data to the list */
void xdm_list_add_data(xdm_list_t* destination, void** data, xdm_list_item_func_free item_func_free)
{
	if(data)
	{
		xdm_list_item_t *item = 0;
		XDM_LIST_ITEM_CREATE(item);
		item->data = *data;
		item->func_free = item_func_free;

		xdm_list_add_item(destination, &item);
		(*data) = 0;
	}
}

/* remove item by position */
void xdm_list_remove_at(int position)
{
}

/* free the list */
void xdm_list_free(xdm_list_t** list)
{
	if(*list)
	{
		xdm_list_item_t* next = 0;
		xdm_list_item_t* curr = (*list)->head;

		while(curr)
		{
			next = curr->next;
			xdm_list_item_free(&curr);
			curr = next;
		}
		free(*list);
		(*list) = 0;
	}
}

/* initialize an item */
void xdm_list_item_init(xdm_list_item_t* item)
{
	memset(item, 0, sizeof(xdm_list_item_t));
}

/* free item */
void xdm_list_item_free(xdm_list_item_t** item)
{
	if(*item)
	{
		if((*item)->func_free)
		{
			(*item)->func_free(&((*item)->data));
		}
		else if((*item)->data)
		{
			free((*item)->data);
		}

		free(*item);
		(*item) = 0;
	}
}