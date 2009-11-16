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

/**@file txc_client.c
 * @brief Hight level XDMC API helper.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "txc_client.h"

/**@defgroup txc_client_group Hight level XDMC API helper 
*/

#define DEBUG_STATE_MACHINE	1

/**@ingroup txc_client_group
* Internal function to initialize an XMDC context.
* You MUST use @ref TXC_XDMC_CREATE to create and initialize an XDMC context.
* @param xdmc The XDMC context to initialize.
* @sa @ref TXC_REQUEST_CREATE
*/
void txc_xdmc_init(txc_xdmc_t *xdmc)
{
	xdmc->firsttime = 1;

	txc_xdmcContext_Init(&(xdmc->_fsm), xdmc);

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(xdmc->_fsm), DEBUG_STATE_MACHINE);
#endif

	/* To perform Entry state action */
	txc_xdmcContext_EnterStartState(&(xdmc->_fsm));
}

/**@ingroup txc_client_group
* Internal function to free an XMDC context previously created using @ref TXC_XDMC_CREATE.
* You MUST use @ref TXC_XDMC_SAFE_FREE to free an XMDC context.
* @param xdmc The XMDC context to free.
* @sa @ref TXC_XDMC_SAFE_FREE
**/
void txc_xdmc_free(txc_xdmc_t** xdmc)
{
	if(*xdmc)
	{
		//TSK_FREE((*xdmc)->data);
		//TSK_FREE((*xdmc)->type);
		
		free(*xdmc);
		(*xdmc) = 0;
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_connect(txc_xdmc_t *xdmc)
{
	if(!xdmc->connected)
	{
		txc_xdmcContext_Connect(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_disconnect(txc_xdmc_t *xdmc)
{
	if(xdmc->connected)
	{
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_state_set(txc_xdmc_t* xdmc, txc_xdmc_state_t state)
{
	xdmc->state = state;
}

/**@ingroup txc_client_group
*/
void txc_xdmc_connection_set(txc_xdmc_t* xdmc, int connected)
{
	xdmc->connected = connected;
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_caps(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_caps);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_omadir(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_omadir);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_rlist(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_rls);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_rls(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_rls);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_omapres(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_omapres);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_ietfpres(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_ietfpres);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

/**@ingroup txc_client_group
*/
void txc_xdmc_get_omaprescont(txc_xdmc_t* xdmc)
{
	txc_xdmc_state_set(xdmc, txc_getting_omaprescont);

	if(1)
	{
		txc_xdmcContext_Success(&(xdmc->_fsm));
	}
	else
	{
		txc_xdmcContext_Failure(&(xdmc->_fsm));
	}
}

