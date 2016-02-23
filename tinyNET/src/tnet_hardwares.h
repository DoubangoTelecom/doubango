/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file tnet_hardwares.h
 * @brief List of Hardware types as assigned by the IANA.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef TNET_HARDWARES_H
#define TNET_HARDWARES_H

#include "tinynet_config.h"

TNET_BEGIN_DECLS

/**
*	List of Hardware types as assigned by the IANA.
*	See RFC 1340, 826 and... for more information.
*/
typedef enum tnet_hardware_type_e {
    tnet_htype_Ethernet_10Mb = 1, /**<    Ethernet (10Mb) */
    tnet_htype_Ethernet_3Mb = 2, /**<     Experimental Ethernet (3Mb) */
    tnet_htype_AX_25 = 3, /**<     Amateur Radio AX.25 */
    tnet_htype_Token_Ring = 4, /**<     Proteon ProNET Token Ring */
    tnet_htype_Chaos = 5, /**<     Chaos */
    tnet_htype_IEEE_802_Networks = 6, /**<     IEEE 802 Networks */
    tnet_htype_ARCNET = 7, /**<     ARCNET */
    tnet_htype_Hyperchannel = 8, /**<     Hyperchannel */
    tnet_htype_Lanstar = 9, /**<     Lanstar	 */
    tnet_htype_Autonet_Short_Address  = 10, /**<     Autonet Short Address */
    tnet_htype_ALocalTalk = 11, /**<     LocalTalk	 */
    tnet_htype_LocalNet= 12, /**<     LocalNet (IBM PCNet or SYTEK LocalNET)	 */
    tnet_htype_Ultra_link =  13, /**<     Ultra link */
    tnet_htype_SMDS = 14, /**<     SMDS	 */
    tnet_htype_Frame_Relay = 15, /**<     Frame Relay	 */
    tnet_htype_ATM = 16, /**<     Asynchronous Transmission Mode (ATM) */
}
tnet_hardware_type_t;

TNET_END_DECLS

#endif /* TNET_HARDWARES_H */
