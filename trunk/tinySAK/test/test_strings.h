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
#ifndef _TEST_STRINGS_H_
#define _TEST_STRINGS_H_

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

#endif /* _TEST_STRINGS_H_ */