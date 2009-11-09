#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include "sak.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
#	define strdup _strdup
#endif

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


/* test heap*/
void test_heap()
{
	int i = 0;
	tsk_heap_t heap;

	/* initialize our memory heap */
	tsk_heap_init(&heap);

	/* heap(1) */
	for(i=0; i<10;i++)
	{
		char* test = tsk_strdup(&heap, "testing the heap (1)");
	}
	tsk_heap_cleanup(&heap);

	/* heap(2)*/
	{
		char* test = tsk_strdup(&heap, "testing the heap (2)");
		tsk_free(&heap, &test);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/* linked lists */
	test_basic_list();
	test_complex_list();

	/* heap */
	while(1)test_heap();

	printf("\n\n");

	return 0;
}

