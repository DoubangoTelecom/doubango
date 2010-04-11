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

/**@file tsk_uuid.c
 * @brief Universally Unique Identifier (UUID version 5) implementation (RFC 4122).
 *		  This implementation is not fully conform to RFC 4122 but could be safely used to generate random UUIDs.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_uuid.h"

#include "tsk_sha1.h"
#include "tsk_string.h"
#include "tsk_time.h"

#include <stdlib.h>
#include <string.h>

/**@defgroup tsk_uuid_group niversally Unique Identifier (UUID version 5) implementation (RFC 4122).
*/

/**@ingroup tsk_uuid_group
*/
int tsk_uuidgenerate(tsk_uuidstring_t *result)
{
	/* From wikipedia
	*	Version 5 UUIDs use a scheme with SHA-1 hashing, otherwise it is the same idea as in version 3. 
	*	RFC 4122 states that version 5 is preferred over version 3 name based UUIDs. 
	*	Note that the 160 bit SHA-1 hash is truncated to 128 bits to make the length work out.
	*/
	tsk_sha1string_t sha1result;
	tsk_istr_t epoch;
	unsigned i, k;
	static char HEX[] = "0123456789abcdef";

	tsk_itoa(tsk_time_epoch(), &epoch);
	tsk_sha1compute(epoch, sizeof(epoch), &sha1result);

	/* XOR the SHA-1 result with random numbers. */
	for(i=0; i<(TSK_UUID_DIGEST_SIZE*2); i+=4){
		*((uint32_t*)&sha1result[i]) ^= rand();
		
		for(k=0; k<sizeof(uint32_t); k++){
			sha1result[i+k] = HEX[sha1result[i+k] & 0x0F]; /* To hexa. */
		}
	}

	/* f47ac10b-58cc-4372-a567-0e02b2c3d479 */
	memcpy(&(*result)[0], &sha1result[0], 8);
	(*result)[8] = '-';

	memcpy(&(*result)[9], &sha1result[8], 4);
	(*result)[13] = '-';

	memcpy(&(*result)[14], &sha1result[12], 4);
	(*result)[18] = '-';

	memcpy(&(*result)[19], &sha1result[16], 4);
	(*result)[23] = '-';

	memcpy(&(*result)[24], &sha1result[20], 12);
	(*result)[36] = '\0';

	return 0;
}
