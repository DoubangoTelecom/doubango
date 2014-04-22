/* Copyright (C) 2014 Mamadou DIOP.
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
#ifndef TBFCP_TYPES_H
#define TBFCP_TYPES_H

#include "tinybfcp_config.h"

TBFCP_BEGIN_DECLS

#if !defined(kBfcpFieldMYes)
#	define kBfcpFieldMYes 1
#endif /* kBfcpFieldMYes */
#if !defined(kBfcpFieldMNo)
#	define kBfcpFieldMNo 0
#endif /* kBfcpFieldMNo */

// RFC4582 - 5.1.  COMMON-HEADER Format
typedef enum tbfcp_primitive_e {
    tbfcp_primitive_FloorRequest		= 1,  // | P -> S           |
    tbfcp_primitive_FloorRelease		= 2,  // | P -> S           |
    tbfcp_primitive_FloorRequestQuery	= 3,  // | P -> S ; Ch -> S |
    tbfcp_primitive_FloorRequestStatus	= 4,  // | P <- S ; Ch <- S |
    tbfcp_primitive_UserQuery			= 5,  // | P -> S ; Ch -> S |
    tbfcp_primitive_UserStatus			= 6,  // | P <- S ; Ch <- S |
    tbfcp_primitive_FloorQuery			= 7,  // | P -> S ; Ch -> S |
    tbfcp_primitive_FloorStatus			= 8,  // | P <- S ; Ch <- S |
    tbfcp_primitive_ChairAction			= 9,  // | Ch -> S          |
    tbfcp_primitive_ChairActionAck		= 10, // | Ch <- S          |
    tbfcp_primitive_Hello				= 11, // | P -> S ; Ch -> S |
    tbfcp_primitive_HelloAck			= 12, // | P <- S ; Ch <- S |
    tbfcp_primitive_Error				= 13, // | P <- S ; Ch <- S
    //!\ Update tbfcp_pkt_create_HelloAck_2() if an entry is added or remove
} tbfcp_primitive_t;

// RFC4582 - 5.2.  Attribute Format
typedef enum tbfcp_attribute_type_e {
    tbfcp_attribute_type_BENEFICIARY_ID = 1,					// | Unsigned16    |
    tbfcp_attribute_type_FLOOR_ID = 2,						// | Unsigned16    |
    tbfcp_attribute_type_FLOOR_REQUEST_ID = 3,				// | Unsigned16    |
    tbfcp_attribute_type_PRIORITY = 4,						// | OctetString16 |
    tbfcp_attribute_type_REQUEST_STATUS = 5,					// | OctetString16 |
    tbfcp_attribute_type_ERROR_CODE = 6,						// | OctetString   |
    tbfcp_attribute_type_ERROR_INFO = 7,						// | OctetString   |
    tbfcp_attribute_type_PARTICIPANT_PROVIDED_INFO = 8,		// | OctetString   |
    tbfcp_attribute_type_STATUS_INFO = 9,						// | OctetString   |
    tbfcp_attribute_type_SUPPORTED_ATTRIBUTES = 10,			// | OctetString   |
    tbfcp_attribute_type_SUPPORTED_PRIMITIVES = 11,			// | OctetString   |
    tbfcp_attribute_type_USER_DISPLAY_NAME = 12,				// | OctetString   |
    tbfcp_attribute_type_USER_URI = 13,						// | OctetString   |
    tbfcp_attribute_type_BENEFICIARY_INFORMATION = 14,		// | Grouped       |
    tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION = 15,		// | Grouped       |
    tbfcp_attribute_type_REQUESTED_BY_INFORMATION  = 16,		// | Grouped       |
    tbfcp_attribute_type_FLOOR_REQUEST_STATUS = 17,			// | Grouped       |
    tbfcp_attribute_type_OVERALL_REQUEST_STATUS = 18,			// | Grouped
    //!\ Update tbfcp_pkt_create_HelloAck_2() if an entry is added or remove
} tbfcp_attribute_type_t;

// RFC4582 - 5.2.  Attribute Format
typedef enum tbfcp_attribute_format_e {
    tbfcp_attribute_format_Unknown,
    tbfcp_attribute_format_Unsigned16,
    tbfcp_attribute_format_OctetString16,
    tbfcp_attribute_format_OctetString,
    tbfcp_attribute_format_Grouped,
} tbfcp_attribute_format_t;

// RFC4582 - 5.2.4.  PRIORITY
typedef enum tbfcp_prio_e {
    tbfcp_prio_Lowest	= 0,
    tbfcp_prio_Low		= 1,
    tbfcp_prio_Normal	= 2,
    tbfcp_prio_High		= 3,
    tbfcp_prio_Highest	= 4
} tbfcp_prio_t;

// RFC4582 - 5.2.5.  REQUEST-STATUS
typedef enum tbfcp_reqstatus_e {
    tbfcp_reqstatus_Pending		= 1,
    tbfcp_reqstatus_Accepte		= 2,
    tbfcp_reqstatus_Granted		= 3,
    tbfcp_reqstatus_Denied		= 4,
    tbfcp_reqstatus_Cancelled	= 5,
    tbfcp_reqstatus_Released	= 6,
    tbfcp_reqstatus_Revoked		= 7
} tbfcp_reqstatus_t;

// RFC4582 - 5.2.6.  ERROR-CODE
typedef enum tbfcp_err_code_e {
    tbfcp_err_code_Conference_does_not_Exist = 1,
    tbfcp_err_code_User_does_not_Exist = 2,
    tbfcp_err_code_Unknown_Primitive = 3,
    tbfcp_err_code_Unknown_Mandatory_Attribute = 4,
    tbfcp_err_code_Unauthorized_Operation = 5,
    tbfcp_err_code_Invalid_Floor_ID = 6,
    tbfcp_err_code_Floor_Request_ID_Does_Not_Exist = 7,
    tbfcp_err_code_You_have_Already_Reached_the_Maximum_Number_of_OngoingFloor_Requests_for_this_Floor = 9,
    tbfcp_err_code_Use_TLS = 10,
} tbfcp_err_code_t;

TBFCP_END_DECLS

#endif /* TBFCP_TYPES_H */

