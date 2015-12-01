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

/**@file tdav_consumer_msrp.h
 * @brief The Message Session Relay Protocol (MSRP) consumer.
 * Used for both Message (RFC 4975) and file transfer (RFC 5547).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#ifndef TINYDAV_CONSUMER_MSRP_H
#define TINYDAV_CONSUMER_MSRP_H

#include "tinydav_config.h"

TDAV_BEGIN_DECLS


#define TDAV_CONSUMER_MSRP(self)		((tdav_consumer_msrp_t*)(self))


typedef struct tdav_consumer_msrp_s
{
	TMEDIA_DECLARE_CONSUMER;
}
tdav_consumer_msrp_t;


TDAV_END_DECLS

#endif /* TINYDAV_CONSUMER_MSRP_H */
