/*
 * Copyright (C) 2010-2011 Mamadou Diop.
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

#ifndef TDAV_APPLE_H
#define TDAV_APPLE_H

#include "tinydav_config.h"

#if TDAV_UNDER_APPLE

TDAV_BEGIN_DECLS

int tdav_apple_init();
int tdav_apple_enable_audio();
int tdav_apple_deinit();

TDAV_END_DECLS

#endif /* TDAV_UNDER_APPLE */

#endif /* TDAV_APPLE_H */