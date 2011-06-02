/*
* Copyright (C) 2009 Mamadou Diop.
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
#ifndef _TEST_LISTS_H_
#define _TEST_LISTS_H_


//========================================================
//	Person object definition
//
typedef struct student_s 
{	
	TSK_DECLARE_OBJECT;

	char *id;
	char *name;
} 
student_t;

static void* student_ctor(void * self, va_list * app)
{
	student_t* student = self;
	student->id = tsk_strdup(va_arg(*app, const char *));
	student->name = tsk_strdup(va_arg(*app, const char *));
	return self;
}

static void* student_dtor(void *self)
{
	student_t* student = self;
	tsk_free((void**)&(student->id));
	tsk_free((void**)&(student->name));
	return self;
}

static int student_icmp(const void *self, const void *object)
{	
	const student_t* student1 = self;
	const student_t* student2 = object;

	if(!student1 || !student2){
		return 0;
	}

	return tsk_stricmp(student1->id, student2->id);
}

static const tsk_object_def_t student_def_s =
{
	sizeof(student_t),	
	student_ctor,
	student_dtor,
	student_icmp
};
const void *student_def_t = &student_def_s;
#define STUDENT_CREATE(id, name)		tsk_object_new(student_def_t, id, name)
#define STUDENT_SAFE_FREE(self)	tsk_object_unref(self)

static int pred_find_student_by_name(const tsk_list_item_t *item, const void *name)
{
	if(item && item->data){
		student_t *student = item->data;
		return tsk_striequals(student->name, name);
	}
	return -1;
}

/* testing basic linked list */
void test_basic_list()
{
	tsk_list_t *list = tsk_list_create();
	tsk_list_item_t *item = tsk_list_item_create();
	
	/* add items to the list */
	item->data = tsk_string_create("First item");
	tsk_list_push_front_item(list, &item);

	item = tsk_list_item_create();
	item->data = tsk_string_create("Second item");
	tsk_list_push_back_item(list, &item);

	item = tsk_list_item_create();
	item->data = tsk_string_create("Third item");
	tsk_list_push_front_item(list, &item);

	item = tsk_list_item_create();
	item->data = tsk_string_create("Fourth item");
	tsk_list_push_back_item(list, &item);

	/* dump all items */
	tsk_list_foreach(item, list){
		tsk_string_t* item_data = ((tsk_string_t*)item->data);
		printf("test_basic_list/// --> [%s]\n", item_data->value);
	}	

	tsk_list_remove_item(list, list->tail);
	tsk_list_remove_item(list, list->tail);
	tsk_list_remove_item(list, list->tail);
	tsk_list_remove_item(list, list->tail);

	/* delete all items in the list */
	TSK_OBJECT_SAFE_FREE(list);
}

void test_filtered_list()
{
#define PUSH_FILTERED(list, data) tsk_list_push_ascending_data(list, data);

	tsk_list_t *list = tsk_list_create();
	tsk_list_item_t *item = 0;

	/* add items to the list */
	{
		student_t *student2 = STUDENT_CREATE("2", "student2");
		PUSH_FILTERED(list, ((void**) &student2));
	}
	{
		student_t *student6 = STUDENT_CREATE("6", "student6");
		PUSH_FILTERED(list, ((void**) &student6));
	}
	{
		student_t *student1 = STUDENT_CREATE("1", "student1");
		PUSH_FILTERED(list, ((void**) &student1));
	}
	{
		student_t *student6 = STUDENT_CREATE("6", "student6");
		PUSH_FILTERED(list, ((void**) &student6));
	}
	{
		student_t *student6 = STUDENT_CREATE("6", "student6");
		PUSH_FILTERED(list, ((void**) &student6));
	}
	{
		student_t *student2 = STUDENT_CREATE("2", "student2");
		PUSH_FILTERED(list, ((void**) &student2));
	}
	{
		student_t *student2 = STUDENT_CREATE("2", "student2");
		PUSH_FILTERED(list, ((void**) &student2));
	}
	{
		student_t *student5 = STUDENT_CREATE("5", "student5");
		PUSH_FILTERED(list, ((void**) &student5));
	}
	{
		student_t *student4 = STUDENT_CREATE("4", "student4");
		PUSH_FILTERED(list, ((void**) &student4));
	}
	{
		student_t *student1 = STUDENT_CREATE("1", "student1");
		PUSH_FILTERED(list, ((void**) &student1));
	}
	{
		student_t *student1 = STUDENT_CREATE("1", "student1");
		PUSH_FILTERED(list, ((void**) &student1));
	}
	{
		student_t *student3 = STUDENT_CREATE("3", "student3");
		PUSH_FILTERED(list, ((void**) &student3));
	}
	{
		student_t *student6 = STUDENT_CREATE("6", "student6");
		PUSH_FILTERED(list, ((void**) &student6));
	}
	{
		student_t *student1 = STUDENT_CREATE("1", "student1");
		PUSH_FILTERED(list, ((void**) &student1));
	}
	{
		student_t *student3 = STUDENT_CREATE("3", "student3");
		PUSH_FILTERED(list, ((void**) &student3));
	}
	{
		student_t *student6 = STUDENT_CREATE("6", "student6");
		PUSH_FILTERED(list, ((void**) &student6));
	}

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		student_t* item_data = item->data;
		printf("test_filtered_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
	}

	/* delete all items in the list */
	TSK_OBJECT_SAFE_FREE(list);

#undef PUSH_FILTERED
}

void test_complex_list()
{
	tsk_list_t *list = tsk_list_create();
	tsk_list_item_t *item = 0;

	/* add items to the list */
	{
		student_t *student1 = STUDENT_CREATE("1", "student1");
		tsk_list_push_back_data(list, ((void**) &student1));
	}
	{
		student_t *student2 = STUDENT_CREATE("2", "student2");
		tsk_list_push_front_data(list, ((void**) &student2));
	}
	{
		student_t *student3 = STUDENT_CREATE("3", "student3");
		tsk_list_push_front_data(list, ((void**) &student3));
	}

	/* dump all items */
	tsk_list_foreach(item, list)
	{
		student_t* item_data = item->data;
		printf("test_complex_list/// --> [id=%s and name=%s]\n", item_data->id, item_data->name);
	}

	/* Find student using tsk_object* */
	{
		student_t *student_to_find = STUDENT_CREATE("1", "student1");
		const tsk_list_item_t *item_const = tsk_list_find_item_by_data(list, student_to_find);
		{
			const student_t* item_data_const = item_const->data;
			printf("test_complex_list/// using tsk_object --> student with name==\"student1\" and id=\"%s\"", item_data_const->id);
		}
		STUDENT_SAFE_FREE(student_to_find);
	}

	/* Find student named "student2" using predicate */
	{
		const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(list, pred_find_student_by_name, "student2");
		{
			const student_t* item_data_const = item_const->data;
			printf("test_complex_list/// using predicate --> student with name==\"student2\" and id=\"%s\"", item_data_const->id);
		}
	}

	/* delete all items in the list */
	TSK_OBJECT_SAFE_FREE(list);
}

#endif /* _TEST_LISTS_H_ */
