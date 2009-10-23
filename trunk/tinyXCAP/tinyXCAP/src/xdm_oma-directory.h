/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/

#ifndef _TINYXCAP_XDM_OMA_DIRECTORY_H_
#define _TINYXCAP_XDM_OMA_DIRECTORY_H_

#include "tinyxcap_config.h"
#include "xdm.h"
#include "xdm_rlist.h"

/* urn:oma:xml:xdm:xcap-directory */

#define XDM_OMADIR_FOLDER_CREATE(folder)		XDM_XXX_CREATE(folder, omadir_folder)
#define XDM_OMADIR_FOLDER_SAFE_FREE(folder)		XDM_XXX_SAFE_FREE(folder, omadir_folder)

/* folder */
typedef struct xdm_omadir_folder_s
{
	char* auid;
}
xdm_omadir_folder_t;
typedef xdm_list_t xdm_omadir_folder_L_t; /* contains xdm_omadir_folder_t elements */

/* oma xcap-directory */
typedef struct xdm_omadir_s
{
	xmlDocPtr docPtr;
}
xdm_omadir_t;

void xdm_omadir_folder_init(xdm_omadir_folder_t *folder);
void xdm_omadir_folder_free(void **folder);

xdm_omadir_folder_t* xdm_omadir_folder_from_xml(xmlNodePtr node);

TINYXCAP_API xdm_omadir_t* xdm_omadir_create(const char* buffer, size_t size);
TINYXCAP_API xdm_omadir_folder_L_t* xdm_omadir_get_all_folders(const xdm_omadir_t* omadir);
TINYXCAP_API xdm_rlist_entry_L_t* xdm_omadir_get_entries_by_folder(const xdm_omadir_t* omadir, const char* fo_auid);
TINYXCAP_API void xdm_omadir_free(xdm_omadir_t **omadir);

#endif /* _TINYXCAP_XDM_OMA_DIRECTORY_H_ */