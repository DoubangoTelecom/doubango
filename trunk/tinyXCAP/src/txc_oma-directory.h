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

#ifndef _TINYXCAP_TXC_OMA_DIRECTORY_H_
#define _TINYXCAP_TXC_OMA_DIRECTORY_H_

#include "tinyxcap_config.h"
#include "txc.h"
#include "txc_rlist.h"

/* urn:oma:xml:xdm:xcap-directory */

#define TXC_OMADIR_FOLDER_CREATE(folder)		TXC_XXX_CREATE2(folder, omadir_folder)
#define TXC_OMADIR_FOLDER_SAFE_FREE(folder)		TXC_XXX_SAFE_FREE2(folder, omadir_folder)

/* folder */
typedef struct txc_omadir_folder_s
{
	char* auid;
}
txc_omadir_folder_t;
typedef tsk_list_t txc_omadir_folder_L_t; /* contains txc_omadir_folder_t elements */

/* oma xcap-directory */
typedef struct txc_omadir_s
{
	xmlDocPtr docPtr;
}
txc_omadir_t;

void txc_omadir_folder_init(txc_omadir_folder_t *folder);
void txc_omadir_folder_free(void **folder);

txc_omadir_folder_t* txc_omadir_folder_from_xml(xmlNodePtr node);

TINYXCAP_API txc_omadir_t* txc_omadir_create(const char* buffer, size_t size);
TINYXCAP_API txc_omadir_folder_L_t* txc_omadir_get_all_folders(const txc_omadir_t* omadir);
TINYXCAP_API txc_rlist_entry_L_t* txc_omadir_get_entries_by_folder(const txc_omadir_t* omadir, const char* fo_auid);
TINYXCAP_API void txc_omadir_free(txc_omadir_t **omadir);

#endif /* _TINYXCAP_TXC_OMA_DIRECTORY_H_ */