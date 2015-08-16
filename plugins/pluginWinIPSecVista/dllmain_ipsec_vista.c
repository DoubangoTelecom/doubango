/* Copyright (C) 2013-2014 Mamadou DIOP
* Copyright (C) 2013-2014 Doubango Telecom <http://www.doubango.org>
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
*/
#include "plugin_win_ipsec_vista_config.h"

#include "tipsec.h"
#include "tsk_plugin.h"
#include "tsk_debug.h"

#include <windows.h>

extern const tipsec_plugin_def_t *plugin_win_ipsec_vista_plugin_def_t;

PLUGIN_WIN_IPSEC_VISTA_BEGIN_DECLS /* BEGIN */
PLUGIN_WIN_IPSEC_VISTA_API int __plugin_get_def_count();
PLUGIN_WIN_IPSEC_VISTA_API tsk_plugin_def_type_t __plugin_get_def_type_at(int index);
PLUGIN_WIN_IPSEC_VISTA_API tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index);
PLUGIN_WIN_IPSEC_VISTA_API tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index);
PLUGIN_WIN_IPSEC_VISTA_END_DECLS /* END */

typedef enum PLUGIN_INDEX_E {
    PLUGIN_INDEX_WFP, // Windows Filtering platform

    PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


int __plugin_get_def_count()
{
    return PLUGIN_INDEX_COUNT;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
    switch(index) {
    case PLUGIN_INDEX_WFP: {
        return tsk_plugin_def_type_ipsec;
    }
    }
    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_plugin_def_type_none;
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
    switch(index) {
    case PLUGIN_INDEX_WFP: {
        return tsk_plugin_def_media_type_all;
    }
    }

    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_plugin_def_media_type_none;
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
    switch(index) {
    case PLUGIN_INDEX_WFP: {
        return plugin_win_ipsec_vista_plugin_def_t;
    }
    }

    TSK_DEBUG_ERROR("No plugin at index %d", index);
    return tsk_null;
}
