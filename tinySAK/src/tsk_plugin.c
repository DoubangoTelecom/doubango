/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#include "tsk_plugin.h"
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

typedef int (*symbol_get_def_count)(void);
typedef tsk_plugin_def_type_t (*symbol_get_def_type_at)(int index);
typedef tsk_plugin_def_media_type_t (*symbol_get_def_media_type_at)(int index);
typedef tsk_plugin_def_ptr_const_t (*symbol_get_def_at)(int index);

#define TSK_PLUGIN_FUNC_NAME_DEF_COUNT				"__plugin_get_def_count"
#define TSK_PLUGIN_FUNC_NAME_DEF_TYPE_AT			"__plugin_get_def_type_at"
#define TSK_PLUGIN_FUNC_NAME_DEF_MEDIA_TYPE_AT		"__plugin_get_def_media_type_at"
#define TSK_PLUGIN_FUNC_NAME_DEF_AT					"__plugin_get_def_at"

#if TSK_UNDER_WINDOWS
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

#if !TSK_UNDER_WINDOWS_CE
#include <sys/stat.h> /* stat() */
#endif

#if (TSK_UNDER_WINDOWS_RT || TSK_UNDER_WINDOWS_CE)
static const wchar_t* szFormat = L"%hs";
#endif

static int _tsk_plugin_handle_destroy(tsk_plugin_handle_t** self);
static tsk_plugin_symbol_t* _tsk_plugin_handle_get_symbol(tsk_plugin_handle_t* handle, const char* symbol_name);

typedef struct tsk_plugin_s {
    TSK_DECLARE_OBJECT;

    tsk_plugin_handle_t* handle;
    int def_count;
    char* path;
}
tsk_plugin_t;

static tsk_object_t* tsk_plugin_ctor(tsk_object_t * self, va_list * app)
{
    tsk_plugin_t *plugin = (tsk_plugin_t*)self;
    if(plugin) {

    }
    return self;
}
static tsk_object_t* tsk_plugin_dtor(tsk_object_t * self)
{
    tsk_plugin_t *plugin = (tsk_plugin_t*)self;
    if(plugin) {
        TSK_FREE(plugin->path);
        _tsk_plugin_handle_destroy(&plugin->handle);
    }

    return self;
}
static const tsk_object_def_t tsk_plugin_def_s = {
    sizeof(tsk_plugin_t),
    tsk_plugin_ctor,
    tsk_plugin_dtor,
    tsk_null,
};

tsk_plugin_t* tsk_plugin_create(const char* path)
{
    tsk_plugin_t* plugin;
    symbol_get_def_count funcptr_get_def_count;
    tsk_plugin_handle_t* handle;

#if TSK_UNDER_WINDOWS
#	if TSK_UNDER_WINDOWS_RT
    wchar_t* szPath = (wchar_t*)tsk_calloc(tsk_strlen(path) + 1, sizeof(wchar_t));
    swprintf(szPath, tsk_strlen(path) * sizeof(wchar_t), szFormat, path);
    handle = LoadPackagedLibrary(szPath, 0x00000000);
    TSK_FREE(szPath);
#	else /* Windows desktop/CE */
#if TSK_UNDER_WINDOWS_CE
    wchar_t* szPath = (wchar_t*)tsk_calloc(tsk_strlen(path) + 1, sizeof(wchar_t));
    swprintf_s(szPath, tsk_strlen(path) * sizeof(wchar_t), szFormat, path);
    handle = LoadLibrary(szPath);
    TSK_FREE(szPath);
#else
    UINT currErrMode = SetErrorMode(SEM_FAILCRITICALERRORS); // save current ErrorMode. GetErrorMode() not supported on XP.
    SetErrorMode(currErrMode | SEM_FAILCRITICALERRORS);
    handle = LoadLibraryA(path);
    SetErrorMode(currErrMode); // restore ErrorMode
#endif /* !TSK_UNDER_WINDOWS_CE */
#	endif /*end-of-else-TSK_UNDER_WINDOWS_RT*/
#else
    handle = dlopen(path, RTLD_NOW);
#endif

    if(!handle) {
        TSK_DEBUG_ERROR("Failed to load library with path=%s", path);
        return tsk_null;
    }

    if(!(funcptr_get_def_count = (symbol_get_def_count)_tsk_plugin_handle_get_symbol(handle, TSK_PLUGIN_FUNC_NAME_DEF_COUNT))) {
        TSK_DEBUG_ERROR("Cannot find function with name=%s", TSK_PLUGIN_FUNC_NAME_DEF_COUNT);
        _tsk_plugin_handle_destroy(&handle);
        return tsk_null;
    }

    if(!(plugin = (tsk_plugin_t*)tsk_object_new(&tsk_plugin_def_s))) {
        TSK_DEBUG_ERROR("Failed to create plugin object");
        _tsk_plugin_handle_destroy(&handle);
        return tsk_null;
    }

    plugin->handle = handle;
    plugin->def_count = funcptr_get_def_count();
    plugin->path = tsk_strdup(path);

    TSK_DEBUG_INFO("Plugin with path=[%s] created with [%d] defs", plugin->path, plugin->def_count);

    return plugin;
}

tsk_plugin_def_ptr_const_t tsk_plugin_get_def_2(struct tsk_plugin_s* self, tsk_plugin_def_type_t type, tsk_plugin_def_media_type_t media_type, tsk_size_t index)
{
    tsk_plugin_def_ptr_const_t def_ptr_const;
    symbol_get_def_type_at funcptr_get_def_type_at;
    symbol_get_def_media_type_at funcptr_get_def_media_type_at;
    symbol_get_def_at funcptr_get_def_at;
    int i;
    tsk_size_t _index = 0;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    if(!(funcptr_get_def_type_at = (symbol_get_def_type_at)tsk_plugin_get_symbol(self, TSK_PLUGIN_FUNC_NAME_DEF_TYPE_AT))) {
        TSK_DEBUG_ERROR("[%s] function not implemented in plugin with path=[%s]", TSK_PLUGIN_FUNC_NAME_DEF_TYPE_AT, self->path);
        return tsk_null;
    }
    if(!(funcptr_get_def_media_type_at = (symbol_get_def_media_type_at)tsk_plugin_get_symbol(self, TSK_PLUGIN_FUNC_NAME_DEF_MEDIA_TYPE_AT))) {
        TSK_DEBUG_ERROR("[%s] function not implemented in plugin with path=[%s]", TSK_PLUGIN_FUNC_NAME_DEF_MEDIA_TYPE_AT, self->path);
        return tsk_null;
    }
    if(!(funcptr_get_def_at = (symbol_get_def_at)tsk_plugin_get_symbol(self, TSK_PLUGIN_FUNC_NAME_DEF_AT))) {
        TSK_DEBUG_ERROR("[%s] function not implemented in plugin with path=[%s]", TSK_PLUGIN_FUNC_NAME_DEF_AT, self->path);
        return tsk_null;
    }

    for(i = 0; i < self->def_count; ++i) {
        if((funcptr_get_def_type_at(i) & type) && (funcptr_get_def_media_type_at(i) & media_type)) {
            if((def_ptr_const = funcptr_get_def_at(i))) {
                if(_index++ == index) {
                    return def_ptr_const;
                }
            }
        }
    }
    return tsk_null;
}

tsk_plugin_def_ptr_const_t tsk_plugin_get_def(tsk_plugin_t* self, tsk_plugin_def_type_t type, tsk_plugin_def_media_type_t media_type)
{
    return tsk_plugin_get_def_2(self, type, media_type, 0);
}

tsk_plugin_symbol_t* tsk_plugin_get_symbol(tsk_plugin_t* self, const char* symbol_name)
{
    if(!self || !self->handle || !symbol_name) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    return _tsk_plugin_handle_get_symbol(self->handle, symbol_name);
}

tsk_bool_t tsk_plugin_file_exist(const char* path)
{
    if (path) {
#if TSK_UNDER_WINDOWS_CE
        wchar_t* szPath = (wchar_t*)tsk_calloc(tsk_strlen(path) + 1, sizeof(wchar_t));
        DWORD dwAttrib;
        swprintf_s(szPath, tsk_strlen(path) * sizeof(wchar_t), szFormat, path);
        dwAttrib = GetFileAttributes(szPath);
        TSK_FREE(szPath);
        return ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
        struct stat _stat;
        return (stat(path, &_stat) == 0 && _stat.st_size > 0);
#endif /* TSK_UNDER_WINDOWS_CE */
    }
    return tsk_false;
}

static tsk_plugin_symbol_t* _tsk_plugin_handle_get_symbol(tsk_plugin_handle_t* handle, const char* symbol_name)
{
    if(!handle || !symbol_name) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
#if TSK_UNDER_WINDOWS
#	if TSK_UNDER_WINDOWS_CE
    return (tsk_plugin_symbol_t*)GetProcAddressA((HMODULE)handle, symbol_name);
#	else
    return (tsk_plugin_symbol_t*)GetProcAddress((HMODULE)handle, symbol_name);
#	endif
#else
    return (tsk_plugin_symbol_t*)dlsym(handle, symbol_name);
#endif
}

static int _tsk_plugin_handle_destroy(tsk_plugin_handle_t** handle)
{
    if(handle && *handle) {
#if TSK_UNDER_WINDOWS
        FreeLibrary((HMODULE)*handle);
#else
        dlclose(*handle);
#endif
        *handle = tsk_null;
    }
    return 0;
}
