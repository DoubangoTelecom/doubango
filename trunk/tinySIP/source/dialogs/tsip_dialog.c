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

/**@file tsip_dialog.c
 * @brief SIP dialog base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_nict.h"

int tsip_dialog_request_send(const tsip_dialog_t *self, const tsip_request_t* request)
{
	if(self && self->stack)
	{	
		tsip_transac_layer_t *layer = tsip_stack_get_transac_layer(self->stack);
		if(layer)
		{
			const tsip_transac_t *transac = tsip_transac_layer_new(layer, request);
			if(transac)
			{
				switch(transac->type)
				{
				case tsip_ict:
					{
						tsip_transac_nict_start(TSIP_TRANSAC_NICT(transac), request);
						break;
					}

				case tsip_nict:
					{
						break;
					}

				default:
					{
						return -2;
					}
				}

				return 0;
			}
		}
	}
	return -1;
}

int tsip_dialog_response_send(const tsip_dialog_t *self, const tsip_response_t* response)
{
	if(self && self->stack)
	{
		return 0;
	}
	return -1;
}

int tsip_dialog_init(tsip_dialog_t *self, const tsip_stack_handle_t * stack)
{
	if(self && self->stack)
	{
		self->stack = stack;
		return 0;
	}
	return -1;
}

int tsip_dialog_deinit(tsip_dialog_t *self)
{
	if(self && self->stack)
	{
		TSK_FREE(self->tag_local);
		TSK_FREE(self->tag_remote);
		
		TSK_FREE(self->cseq_method);
		TSK_FREE(self->callid);
		
		return 0;
	}
	return -1;
}