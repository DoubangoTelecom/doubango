/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
/**@file txcap_selector.h
 * @brief XCAP selector.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYXCAP_TXCAP_SELECTOR_H
#define TINYXCAP_TXCAP_SELECTOR_H

#include "tinyxcap_config.h"

#include "txcap.h"

TXCAP_BEGIN_DECLS

TINYXCAP_API char* txcap_selector_get_url(const txcap_stack_handle_t* stack, const char* auid_id, ...);
TINYXCAP_API char* txcap_selector_get_url_2(const char* xcap_root, const char* auid_id, const char* xui, const char* doc_name, ...);

TXCAP_END_DECLS

#endif /* TINYXCAP_TXCAP_SELECTOR_H */
