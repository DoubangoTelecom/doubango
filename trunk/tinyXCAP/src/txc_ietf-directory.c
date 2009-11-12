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
/**@file txc_ietf-directory.c
 * @brief <a href="http://tools.ietf.org/html/draft-garcia-simple-xcap-directory-00">draft-garcia-simple-xcap-directory</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "txc_ietf-directory.h"

/**@defgroup txc_ietf_directory_group IETF XCAP Directory
*/

/**@page txc_ietf_directory_page IETF XCAP Directory Tutorial (directory)
* @par Application Unique ID (AUID)
* - '<span style="text-decoration:underline;">directory</span>' as per draft-garcia-simple-xcap-directory-00 subclause 9.1
* @par Default Document Namespace
* - '<span style="text-decoration:underline;">urn:ietf:params:xml:ns:xcap-directory</span>' as per draft-garcia-simple-xcap-directory-00 subclause 6
* @par MIME Type
* - '<span style="text-decoration:underline;">application/directory+xml</span>' as per draft-garcia-simple-xcap-directory-00 subclause 9.2
* @par Default document name
* - '<span style="text-decoration:underline;">directory.xml</span>' as per draft-garcia-simple-xcap-directory-00 subclause 9.6
*/