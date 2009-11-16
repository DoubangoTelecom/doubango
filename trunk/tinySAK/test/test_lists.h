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
#ifndef _TEST_LISTS_H_
#define _TEST_LISTS_H_

/* testing basic linked list */
void test_basic_list()
{
	tsk_list_t *list = 0;
	tsk_list_item_t *item = 0;

	/* create list */
	TSK_LIST_CREATE(list);

	/* add items to the list */
	TSK_LIST_ITEM_CREATE(item);
	item->data = (void*)strdup("First item");
	tsk_list_add_item(list, &item);

	TSK_LIST_ITEM_CREATE(item);
	item->data = (void*)strdup("Second item");
	tsk_list_add_item(list, &item);

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		char* item_data = ((char*)item->data);
		printf("test_basic_list/// --> [%s]\n", item_data);
	}

	/* delete all items in the list */
	TSK_LIST_SAFE_FREE(list);
}

/* test complex linked list */
typedef struct person_s { char *id; char *name; } person_t;
void person_free(void** person) { 
	person_t** pp = (person_t**)person; 
	free((*pp)->id); 
	free((*pp)->name);
	free((*pp));
	(*pp)=0;
	printf("test_advanced_list/// [person_free]\n");
}
int find_person_by_name(const tsk_list_item_t* item, const void* name)
{
	person_t* item_data = (person_t*)(item->data);
	printf("test_advanced_list/// [find_person2]\n");
	return !strcmp(item_data->name, (const char*)name);
}

void test_complex_list()
{
	tsk_list_t *list = 0;
	tsk_list_item_t *item = 0;

	/* create list */
	TSK_LIST_CREATE(list);

	/* add items to the list */
	{
		person_t *person1 = (person_t*)malloc(sizeof *person1);
		person1->id = strdup("1");
		person1->name = strdup("person1");
		tsk_list_add_data(list, ((void**) &person1), person_free);
	}
	{
		person_t *person2 = (person_t*)malloc(sizeof *person2);
		person2->id = strdup("2");
		person2->name = strdup("person2");
		tsk_list_add_data(list, ((void**) &person2), person_free);
	}

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		person_t* item_data = ((person_t*)item->data);
		printf("test_complex_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
	}

	/* Find person named "person2" */
	{
		const tsk_list_item_t *item_const = tsk_list_find_item(list, find_person_by_name, "person2");
		{
			const person_t* item_data_const = ((const person_t*)item_const->data);
			printf("test_complex_list/// --> person with name==\"person2\" and id=\"%s\"", item_data_const->id);
		}
	}

	/* delete all items in the list */
	TSK_LIST_SAFE_FREE(list);
}

#endif /* _TEST_LISTS_H_ */