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

/**@file txc_client.h
 * @brief Hight level XDMC API helper.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_CLIENT_H_
#define _TINYXCAP_TXC_CLIENT_H_

#include "tinyxcap_config.h"
#include "txc.h"

#include "txc_client_sm.h"

/**@def TXC_XDMC_CREATE
* Helper function to easly create and initialize an XDMC context.
* You MUST use @ref TXC_XDMC_SAFE_FREE to free an XDMC context.
* @sa @ref TXC_XDMC_SAFE_FREE
*/
/**@def TXC_XDMC_SAFE_FREE
* Helper function to safely free an XDDMC context previously created using @ref TXC_XDMC_CREATE.
* @sa @ref TXC_XDMC_CREATE.
*/
#define TXC_XDMC_CREATE(this)		TXC_XXX_CREATE2(this, xdmc)
#define TXC_XDMC_SAFE_FREE(this)	TXC_XXX_SAFE_FREE2(this, xdmc)


#define TXC_XDMC_FIRSTTIME(xdmc)	(xdmc->firsttime)
#define TXC_XDMC_CONNECTED(xdmc)	(xdmc->connected)

/**@def txc_xdmc_state_t
*  
*/
typedef enum txc_xdmc_state_e
{
	txs_none,
	txs_initialized,
	txc_getting_caps,
	txc_getting_omadir,
	txc_getting_rls,
	txc_getting_omapres,
	txc_getting_ietfpres,
	txc_getting_omaprescont,

}
txc_xdmc_state_t;

/**@def txc_xdmc_t
* XMDC (aka XDM Client) context.
*/
typedef struct txc_xdmc
{
	struct txc_xdmcContext _fsm; /**< The state machine context. */
	
	txc_xdmc_state_t state; /**< The current state */

	unsigned connected:1; /**< TODO */
	unsigned firsttime:1; /**< TODO */
}
txc_xdmc_t;

static txc_xdmc_statechanged_callback(txc_xdmc_state_t state);

TINYXCAP_API void txc_xdmc_init(txc_xdmc_t *xdmc);
TINYXCAP_API void txc_xdmc_connect(txc_xdmc_t *xdmc);
TINYXCAP_API void txc_xdmc_disconnect(txc_xdmc_t *xdmc);
TINYXCAP_API void txc_xdmc_free(txc_xdmc_t** xdmc);

void txc_xdmc_state_set(txc_xdmc_t* xdmc, txc_xdmc_state_t state);
void txc_xdmc_connection_set(txc_xdmc_t* xdmc, int connected);

void txc_xdmc_get_caps(txc_xdmc_t* xdmc);
void txc_xdmc_get_omadir(txc_xdmc_t* xdmc);
void txc_xdmc_get_rlist(txc_xdmc_t* xdmc);
void txc_xdmc_get_rls(txc_xdmc_t* xdmc);
void txc_xdmc_get_omapres(txc_xdmc_t* xdmc);
void txc_xdmc_get_ietfpres(txc_xdmc_t* xdmc);
void txc_xdmc_get_omaprescont(txc_xdmc_t* xdmc);

#endif /* _TINYXCAP_TXC_CLIENT_H_ */