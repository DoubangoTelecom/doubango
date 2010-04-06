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

/**@file txc_macros.h
 * @brief Macro helpers
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYXCAP_MACROS_H_
#define _TINYXCAP_MACROS_H_

/**@def TXC_XXX_CREATE
* @brief Macro helper to create and initialize an object.
* @param heap The memory on which to create the object
* @param xxx object to create
* @param prefix the prefix of the object to create
*/
#define TXC_XXX_CREATE(heap, xxx, prefix)\
	xxx = (txc_##prefix##_t*)tsk_calloc(heap, 1, sizeof(txc_##prefix##_t)); \
	txc_##prefix##_init(xxx);

/**@def TXC_XXX_SAFE_FREE
* @brief Macro helper to safely free an object.
* @param heap The memory from which to free the object
* @param xxx object to free
* @param prefix the prefix of the object to free
*/
#define TXC_XXX_SAFE_FREE(heap, xxx, prefix)\
	if(xxx) { txc_##prefix##_free(heap, &(xxx)); xxx = 0; }

/**@def TXC_XXX_CREATE2
* @brief Macro helper to create and initialize an object.
* @param xxx object to create
* @param prefix the prefix of the object to create
*/
#define TXC_XXX_CREATE2(xxx, prefix)\
	xxx = (txc_##prefix##_t*)tsk_calloc(1, sizeof(txc_##prefix##_t)); \
	txc_##prefix##_init(xxx);

/**@def TXC_XXX_SAFE_FREE2
* @brief Macro helper to safely free an object.
* @param xxx object to free
* @param prefix the prefix of the object to free
*/
#define TXC_XXX_SAFE_FREE2(xxx, prefix)\
	if(xxx) { txc_##prefix##_free(&(xxx)); xxx = 0; }

#endif /* _TINYXCAP_MACROS_H_ */