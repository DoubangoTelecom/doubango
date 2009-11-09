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

	/* heap(1): Ten strings are allocated an all freed when we cleanup the heap  */
	for(i=0; i<10;i++)
	{
		char* test = tsk_strdup(&heap, "testing the heap (1)");
	}
	tsk_heap_cleanup(&heap);

	/* heap(2): pop memory from the heap*/
	{
 		char* test = tsk_strdup(&heap, "testing the heap (2)");
		tsk_free(&heap, &test);

		test = tsk_calloc(&heap, 10, 1);
		tsk_free(&heap, &test);

		test = tsk_malloc(&heap, 10);
		tsk_free(&heap, &test);

		test = tsk_malloc(&heap, 10);
		test = tsk_realloc(&heap, test, 100);
		tsk_free(&heap, &test);
	}
	
	/* heap(3): pop a NULL pointer */
	{
		tsk_free(&heap, 0);
	}

	/* heap(4): allocate and pop from NULL heap */
	{
		char* test = tsk_calloc(0, 10, 1);
		tsk_free(0, &test);

		test = tsk_malloc(0, 10);
		tsk_free(0, &test);

		test = tsk_malloc(0, 10);
		test = tsk_realloc(0, test, 100);
		tsk_free(0, &test);
	}
}

/* test string manipulation */
void test_strings()
{
	char* str = 0;
	tsk_heap_t heap;

	/* initialize our memory heap */
	tsk_heap_init(&heap);

	/* concatenation */
	tsk_strcat(&heap, &str, "first");
	printf("test_strings/// strcat=%s\n", str);

	tsk_strcat(&heap, &str, "second");
	printf("test_strings/// strcat=%s\n", str);
	
	/* sprintf */
	tsk_sprintf(&heap, &str, "%s", "third");
	printf("test_strings/// strcat=%s\n", str);

	/* cleanup */
	tsk_heap_cleanup(&heap);
}

/* url encoding/decoding */
void test_url()
{
	const char* url = "http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D";
	char* str = 0;
	tsk_heap_t heap;

	/* initialize our memory heap */
	tsk_heap_init(&heap);

	/*decode url*/
	str = tsk_url_decode(&heap, url);
	printf("test_url/// decoded url:%s\n", str);

	/*encode url*/
	str = tsk_url_encode(&heap, str);
	printf("test_url/// encoded url:%s\n", str);

	/* cleanup */
	tsk_heap_cleanup(&heap);
}

int _tmain(int argc, _TCHAR* argv[])
{
	while(1)
	{
		/* linked lists */
		test_basic_list();
		printf("\n\n");
		test_complex_list();
		printf("\n\n");

		/* heap */
		test_heap();
		printf("\n\n");

		/* strings */
		test_strings();
		printf("\n\n");

		/* url */
		while(1)test_url();
		printf("\n\n");
	}

	getchar();

	return 0;
}

