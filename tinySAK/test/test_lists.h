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


// (well-defined object declaration)
typedef struct student_s {
    TSK_DECLARE_OBJECT;

    char *id;
    char *name;
}
student_t;
// (constructor)
static tsk_object_t* student_ctor(tsk_object_t* self, va_list * app)
{
    student_t* student = (tsk_object_t*)self;
    if(student) {
    }
    return self;
}
// (destructor)
static tsk_object_t* student_dtor(tsk_object_t* self)
{
    student_t* student = (tsk_object_t*)self;
    TSK_FREE(student->id);
    TSK_FREE(student->name);
    return self;
}
// (case insensitive comparator)
static int student_icmp(const tsk_object_t *self, const tsk_object_t *object)
{
    const student_t* student1 = (const tsk_object_t*)self;
    const student_t* student2 = (const tsk_object_t*)object;

    if(!student1 || !student2) {
        return 0;// must never happen
    }

    return tsk_stricmp(student1->id, student2->id);
}
// (well-defined object declaration)
static const tsk_object_def_t student_def_s = {
    sizeof(student_t),
    student_ctor,
    student_dtor,
    student_icmp
};
// create a stun object
static student_t* student_create(const char* id, const char* name)
{
    student_t* student;
    if((student = (student_t*)tsk_object_new(&student_def_s))) {
        student->id = tsk_strdup(id);
        student->name = tsk_strdup(name);
    }
    return student;
}

// predicate function to find a student by name
static int pred_find_student_by_name(const tsk_list_item_t *item, const void *name)
{
    if(item && item->data) {
        student_t *student = item->data;
        return tsk_striequals(student->name, name);
    }
    return -1;
}

// predicate function to find a student by id
static int pred_find_student_by_id(const tsk_list_item_t *item, const void *id)
{
    if(item && item->data) {
        student_t *student = item->data;
        return tsk_striequals(student->id, id);
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
    tsk_list_foreach(item, list) {
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
    tsk_list_t *list = tsk_list_create();
    tsk_list_item_t *item = 0;

    /* add items to the list */
    {
        student_t *student2 = student_create("2", "student2");
        tsk_list_push_ascending_data(list, ((void**) &student2));
    }
    {
        student_t *student6 = student_create("6", "student6");
        tsk_list_push_ascending_data(list, ((void**) &student6));
    }
    {
        student_t *student1 = student_create("1", "student1");
        tsk_list_push_ascending_data(list, ((void**) &student1));
    }
    {
        student_t *student6 = student_create("6", "student6");
        tsk_list_push_ascending_data(list, ((void**) &student6));
    }
    {
        student_t *student6 = student_create("6", "student6");
        tsk_list_push_ascending_data(list, ((void**) &student6));
    }
    {
        student_t *student2 = student_create("2", "student2");
        tsk_list_push_ascending_data(list, ((void**) &student2));
    }
    {
        student_t *student2 = student_create("2", "student2");
        tsk_list_push_ascending_data(list, ((void**) &student2));
    }
    {
        student_t *student5 = student_create("5", "student5");
        tsk_list_push_ascending_data(list, ((void**) &student5));
    }
    {
        student_t *student4 = student_create("4", "student4");
        tsk_list_push_ascending_data(list, ((void**) &student4));
    }
    {
        student_t *student1 = student_create("1", "student1");
        tsk_list_push_ascending_data(list, ((void**) &student1));
    }
    {
        student_t *student1 = student_create("1", "student1");
        tsk_list_push_ascending_data(list, ((void**) &student1));
    }
    {
        student_t *student3 = student_create("3", "student3");
        tsk_list_push_ascending_data(list, ((void**) &student3));
    }
    {
        student_t *student6 = student_create("6", "student6");
        tsk_list_push_ascending_data(list, ((void**) &student6));
    }
    {
        student_t *student1 = student_create("1", "student1");
        tsk_list_push_ascending_data(list, ((void**) &student1));
    }
    {
        student_t *student3 = student_create("3", "student3");
        tsk_list_push_ascending_data(list, ((void**) &student3));
    }
    {
        student_t *student6 = student_create("6", "student6");
        tsk_list_push_ascending_data(list, ((void**) &student6));
    }

    /* dump all items */
    tsk_list_foreach(item, list) {
        student_t* item_data = item->data;
        TSK_DEBUG_INFO("test_filtered_list/// --> [id=%s and name=%s]", item_data->id, item_data->name);
    }

    /* delete all items in the list */
    TSK_OBJECT_SAFE_FREE(list);
}

void test_complex_list()
{
    tsk_list_t *list = tsk_list_create();
    tsk_list_item_t *item = 0;

    /* add items to the list */
    {
        student_t *student1 = student_create("1", "student1");
        tsk_list_push_back_data(list, ((void**) &student1));
    }
    {
        student_t *student2 = student_create("2", "student2");
        tsk_list_push_front_data(list, ((void**) &student2));
    }
    {
        student_t *student3 = student_create("3", "student3");
        tsk_list_push_front_data(list, ((void**) &student3));
    }

    /* dump all items */
    tsk_list_foreach(item, list) {
        student_t* item_data = item->data;
        TSK_DEBUG_INFO("test_complex_list/// --> [id=%s and name=%s]", item_data->id, item_data->name);
    }

    /* Find student using tsk_object* */
    {
        student_t *student_to_find = student_create("1", "student1");
        const tsk_list_item_t *item_const = tsk_list_find_item_by_data(list, student_to_find);
        {
            const student_t* item_data_const = item_const->data;
            TSK_DEBUG_INFO("test_complex_list/// using tsk_object --> student with name==\"student1\" and id=\"%s\"", item_data_const->id);
        }
        TSK_OBJECT_SAFE_FREE(student_to_find);
    }

    /* Find student named "student2" using predicate */
    {
        const tsk_list_item_t *item_const = tsk_list_find_item_by_pred(list, pred_find_student_by_name, "student2");
        {
            const student_t* item_data_const = item_const->data;
            TSK_DEBUG_INFO("test_complex_list/// using predicate --> student with name==\"student2\" and id=\"%s\"", item_data_const->id);
        }
    }

    /* delete all items in the list */
    TSK_OBJECT_SAFE_FREE(list);
}

#endif /* _TEST_LISTS_H_ */
