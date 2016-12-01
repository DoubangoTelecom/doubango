/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tmsrp_header_Failure_Report.h
 * @brief MSRP 'Failure-Report' header.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef _TMSRP_HEADER_FAILURE_REPORT_H_
#define _TMSRP_HEADER_FAILURE_REPORT_H_

#include "tinymsrp_config.h"
#include "tinymsrp/headers/tmsrp_header.h"

TMSRP_BEGIN_DECLS

#define TMSRP_HEADER_FAILURE_REPORT_VA_ARGS(freport_type)		tmsrp_header_Failure_Report_def_t, (tmsrp_freport_type_t)freport_type

/** Failure report type;
*/
typedef enum tmsrp_freport_type_e {
    freport_yes,
    freport_no,
    freport_partial
}
tmsrp_freport_type_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct
///
/// @brief	MSRP 'Failure-Report' header.
///
/// @par ABNF :  "Failure-Report:" ( "yes" / "no" / "partial" )
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tmsrp_header_Failure_Report_s {
    TMSRP_DECLARE_HEADER;
    tmsrp_freport_type_t type;
}
tmsrp_header_Failure_Report_t;

typedef tsk_list_t tmsrp_headers_Failure_Report_L_t;


TINYMSRP_API tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create(tmsrp_freport_type_t freport_type);
TINYMSRP_API tmsrp_header_Failure_Report_t* tmsrp_header_Failure_Report_create_null();

TINYMSRP_API tmsrp_header_Failure_Report_t *tmsrp_header_Failure_Report_parse(const char *data, tsk_size_t size);

TINYMSRP_GEXTERN const tsk_object_def_t *tmsrp_header_Failure_Report_def_t;

TMSRP_END_DECLS

#endif /* _TMSRP_HEADER_FAILURE_REPORT_H_ */

