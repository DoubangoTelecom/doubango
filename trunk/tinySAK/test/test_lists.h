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

//========================================================
//	Person object definition
//
typedef struct person_s 
{	
	TSK_DECLARE_OBJECT;

	char *id;
	char *name;
} 
person_t;

static void* person_create(void * self, va_list * app)
{
	person_t* person = self;
	person->id = tsk_strdup(va_arg(*app, const char *));
	person->name = tsk_strdup(va_arg(*app, const char *));
	return self;
}

static void* person_destroy(void *self)
{
	person_t* person = self;
	tsk_free(&(person->id));
	tsk_free(&(person->name));
	return self;
}

static int person_icmp(const void *self, const void *object)
{	
	const person_t* person1 = self;
	const person_t* person2 = object;

	if(!person1 || !person2)
	{
		return 0;
	}

	return tsk_stricmp(person1->id, person2->id);
}

static const tsk_object_def_t person_def_s =
{
	sizeof(person_t),	
	person_create,
	person_destroy,
	person_icmp
};
const void *person_def_t = &person_def_s;
#define PERSON_CREATE(id, name)		tsk_object_new(person_def_t, id, name)
#define PERSON_SAFE_FREE(self)	tsk_object_unref(self)

static int pred_find_person_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data)
	{
		person_t *person = item->data;
		return tsk_striequals(person->name, name);
	}
	return -1;
}

/* testing basic linked list */
void test_basic_list()
{
	tsk_list_t *list = TSK_LIST_CREATE();
	tsk_list_item_t *item = TSK_LIST_ITEM_CREATE();
	
	/* add items to the list */
	item->data = TSK_STRING_CREATE("First item");
	tsk_list_push_front_item(list, &item);

	item = TSK_LIST_ITEM_CREATE();
	item->data = TSK_STRING_CREATE("Second item");
	tsk_list_push_back_item(list, &item);

	item = TSK_LIST_ITEM_CREATE();
	item->data = TSK_STRING_CREATE("Third item");
	tsk_list_push_front_item(list, &item);

	item = TSK_LIST_ITEM_CREATE();
	item->data = TSK_STRING_CREATE("Fourth item");
	tsk_list_push_back_item(list, &item);

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		tsk_string_t* item_data = ((tsk_string_t*)item->data);
		printf("test_basic_list/// --> [%s]\n", item_data->value);
	}

	/* delete all items in the list */
	TSK_LIST_SAFE_FREE(list);
}

void test_filtered_list()
{
#define PUSH_FILTERED(list, data) tsk_list_push_ascending_data(list, data);

	tsk_list_t *list = TSK_LIST_CREATE();
	tsk_list_item_t *item = 0;

	/* add items to the list */
	{
		person_t *person2 = PERSON_CREATE("2", "person2");
		PUSH_FILTERED(list, ((void**) &person2));
	}
	{
		person_t *person6 = PERSON_CREATE("6", "person6");
		PUSH_FILTERED(list, ((void**) &person6));
	}
	{
		person_t *person1 = PERSON_CREATE("1", "person1");
		PUSH_FILTERED(list, ((void**) &person1));
	}
	{
		person_t *person6 = PERSON_CREATE("6", "person6");
		PUSH_FILTERED(list, ((void**) &person6));
	}
	{
		person_t *person6 = PERSON_CREATE("6", "person6");
		PUSH_FILTERED(list, ((void**) &person6));
	}
	{
		person_t *person2 = PERSON_CREATE("2", "person2");
		PUSH_FILTERED(list, ((void**) &person2));
	}
	{
		person_t *person2 = PERSON_CREATE("2", "person2");
		PUSH_FILTERED(list, ((void**) &person2));
	}
	{
		person_t *person5 = PERSON_CREATE("5", "person5");
		PUSH_FILTERED(list, ((void**) &person5));
	}
	{
		person_t *person4 = PERSON_CREATE("4", "person4");
		PUSH_FILTERED(list, ((void**) &person4));
	}
	{
		person_t *person1 = PERSON_CREATE("1", "person1");
		PUSH_FILTERED(list, ((void**) &person1));
	}
	{
		person_t *person1 = PERSON_CREATE("1", "person1");
		PUSH_FILTERED(list, ((void**) &person1));
	}
	{
		person_t *person3 = PERSON_CREATE("3", "person3");
		PUSH_FILTERED(list, ((void**) &person3));
	}
	{
		person_t *person6 = PERSON_CREATE("6", "person6");
		PUSH_FILTERED(list, ((void**) &person6));
	}
	{
		person_t *person1 = PERSON_CREATE("1", "person1");
		PUSH_FILTERED(list, ((void**) &person1));
	}
	{
		person_t *person3 = PERSON_CREATE("3", "person3");
		PUSH_FILTERED(list, ((void**) &person3));
	}
	{
		person_t *person6 = PERSON_CREATE("6", "person6");
		PUSH_FILTERED(list, ((void**) &person6));
	}

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		person_t* item_data = item->data;
		printf("test_filtered_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
	}

	/* delete all items in the list */
	TSK_LIST_SAFE_FREE(list);

#undef PUSH_FILTERED
}

void test_complex_list()
{
	tsk_list_t *list = TSK_LIST_CREATE();
	tsk_list_item_t *item = 0;

	/* add items to the list */
	{
		person_t *person1 = PERSON_CREATE("1", "person1");
		tsk_list_push_back_data(list, ((void**) &person1));
	}
	{
		person_t *person2 = PERSON_CREATE("2", "person2");
		tsk_list_push_front_data(list, ((void**) &person2));
	}
	{
		person_t *person3 = PERSON_CREATE("3", "person3");
		tsk_list_push_front_data(list, ((void**) &person3));
	}

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		person_t* item_data = item->data;
		printf("test_complex_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
	}

	/* Find person using tsk_object* */
	{
		person_t *person_to_find = PERSON_CREATE("1", "person1");
		const tsk_list_item_t *item_const = tsk_list_find_item_by_data(list, person_to_find);
		{
			const person_t* item_data_const = item_const->data;
			printf("test_complex_list/// using tsk_object --> person with name==\"person1\" and id=\"%s\"", item_data_const->id);
		}
		PERSON_SAFE_FREE(person_to_find);
	}

	/* Find person named "person2" using predicate */
	{
		const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(list, pred_find_person_by_name, "person2");
		{
			const person_t* item_data_const = item_const->data;
			printf("test_complex_list/// using predicate --> person with name==\"person2\" and id=\"%s\"", item_data_const->id);
		}
	}

	/* delete all items in the list */
	TSK_LIST_SAFE_FREE(list);
}

#endif /* _TEST_LISTS_H_ */