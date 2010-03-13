
/* #line 1 "tsdp_parser_message.rl" */
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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file tsdp_machine_message.rl
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/parsers/tsdp_parser_message.h"

#include "tinySDP/headers/tsdp_header_B.h"
#include "tinySDP/headers/tsdp_header_C.h"
#include "tinySDP/headers/tsdp_header_Dummy.h"
#include "tinySDP/headers/tsdp_header_E.h"
#include "tinySDP/headers/tsdp_header_I.h"
#include "tinySDP/headers/tsdp_header_K.h"
#include "tinySDP/headers/tsdp_header_O.h"
#include "tinySDP/headers/tsdp_header_P.h"
#include "tinySDP/headers/tsdp_header_R.h"
#include "tinySDP/headers/tsdp_header_S.h"
#include "tinySDP/headers/tsdp_header_T.h"
#include "tinySDP/headers/tsdp_header_U.h"
#include "tinySDP/headers/tsdp_header_V.h"
#include "tinySDP/headers/tsdp_header_Z.h"

#include "tsk_debug.h"


/* #line 225 "tsdp_parser_message.rl" */


/* Ragel data */

/* #line 57 "../src/parsers/tsdp_parser_message.c" */
static const char _tsdp_machine_message_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 2, 1, 0, 2, 2, 
	0, 2, 3, 0, 2, 4, 0, 2, 
	5, 0, 2, 6, 0, 2, 7, 0, 
	2, 8, 0, 2, 9, 0, 2, 10, 
	0, 2, 11, 0, 2, 12, 0, 2, 
	13, 0, 2, 14, 0, 2, 15, 0, 
	2, 16, 0
};

static const short _tsdp_machine_message_key_offsets[] = {
	0, 0, 2, 3, 4, 6, 7, 8, 
	10, 11, 12, 14, 15, 16, 18, 19, 
	20, 22, 23, 24, 26, 27, 28, 30, 
	31, 32, 34, 35, 36, 38, 39, 40, 
	42, 43, 44, 46, 47, 48, 50, 51, 
	52, 54, 55, 56, 58, 59, 60, 62, 
	63, 64, 98, 132, 166, 200, 234, 268, 
	302, 336, 370, 404, 438, 472, 506, 540, 
	574, 608
};

static const char _tsdp_machine_message_trans_keys[] = {
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	32, 61, 13, 10, 32, 61, 13, 10, 
	65, 66, 67, 69, 73, 75, 77, 79, 
	80, 82, 83, 84, 85, 86, 90, 97, 
	98, 99, 101, 105, 107, 109, 111, 112, 
	114, 115, 116, 117, 118, 122, 68, 89, 
	100, 121, 65, 66, 67, 69, 73, 75, 
	77, 79, 80, 82, 83, 84, 85, 86, 
	90, 97, 98, 99, 101, 105, 107, 109, 
	111, 112, 114, 115, 116, 117, 118, 122, 
	68, 89, 100, 121, 65, 66, 67, 69, 
	73, 75, 77, 79, 80, 82, 83, 84, 
	85, 86, 90, 97, 98, 99, 101, 105, 
	107, 109, 111, 112, 114, 115, 116, 117, 
	118, 122, 68, 89, 100, 121, 65, 66, 
	67, 69, 73, 75, 77, 79, 80, 82, 
	83, 84, 85, 86, 90, 97, 98, 99, 
	101, 105, 107, 109, 111, 112, 114, 115, 
	116, 117, 118, 122, 68, 89, 100, 121, 
	65, 66, 67, 69, 73, 75, 77, 79, 
	80, 82, 83, 84, 85, 86, 90, 97, 
	98, 99, 101, 105, 107, 109, 111, 112, 
	114, 115, 116, 117, 118, 122, 68, 89, 
	100, 121, 65, 66, 67, 69, 73, 75, 
	77, 79, 80, 82, 83, 84, 85, 86, 
	90, 97, 98, 99, 101, 105, 107, 109, 
	111, 112, 114, 115, 116, 117, 118, 122, 
	68, 89, 100, 121, 65, 66, 67, 69, 
	73, 75, 77, 79, 80, 82, 83, 84, 
	85, 86, 90, 97, 98, 99, 101, 105, 
	107, 109, 111, 112, 114, 115, 116, 117, 
	118, 122, 68, 89, 100, 121, 65, 66, 
	67, 69, 73, 75, 77, 79, 80, 82, 
	83, 84, 85, 86, 90, 97, 98, 99, 
	101, 105, 107, 109, 111, 112, 114, 115, 
	116, 117, 118, 122, 68, 89, 100, 121, 
	65, 66, 67, 69, 73, 75, 77, 79, 
	80, 82, 83, 84, 85, 86, 90, 97, 
	98, 99, 101, 105, 107, 109, 111, 112, 
	114, 115, 116, 117, 118, 122, 68, 89, 
	100, 121, 65, 66, 67, 69, 73, 75, 
	77, 79, 80, 82, 83, 84, 85, 86, 
	90, 97, 98, 99, 101, 105, 107, 109, 
	111, 112, 114, 115, 116, 117, 118, 122, 
	68, 89, 100, 121, 65, 66, 67, 69, 
	73, 75, 77, 79, 80, 82, 83, 84, 
	85, 86, 90, 97, 98, 99, 101, 105, 
	107, 109, 111, 112, 114, 115, 116, 117, 
	118, 122, 68, 89, 100, 121, 65, 66, 
	67, 69, 73, 75, 77, 79, 80, 82, 
	83, 84, 85, 86, 90, 97, 98, 99, 
	101, 105, 107, 109, 111, 112, 114, 115, 
	116, 117, 118, 122, 68, 89, 100, 121, 
	65, 66, 67, 69, 73, 75, 77, 79, 
	80, 82, 83, 84, 85, 86, 90, 97, 
	98, 99, 101, 105, 107, 109, 111, 112, 
	114, 115, 116, 117, 118, 122, 68, 89, 
	100, 121, 65, 66, 67, 69, 73, 75, 
	77, 79, 80, 82, 83, 84, 85, 86, 
	90, 97, 98, 99, 101, 105, 107, 109, 
	111, 112, 114, 115, 116, 117, 118, 122, 
	68, 89, 100, 121, 65, 66, 67, 69, 
	73, 75, 77, 79, 80, 82, 83, 84, 
	85, 86, 90, 97, 98, 99, 101, 105, 
	107, 109, 111, 112, 114, 115, 116, 117, 
	118, 122, 68, 89, 100, 121, 65, 66, 
	67, 69, 73, 75, 77, 79, 80, 82, 
	83, 84, 85, 86, 90, 97, 98, 99, 
	101, 105, 107, 109, 111, 112, 114, 115, 
	116, 117, 118, 122, 68, 89, 100, 121, 
	65, 66, 67, 69, 73, 75, 77, 79, 
	80, 82, 83, 84, 85, 86, 90, 97, 
	98, 99, 101, 105, 107, 109, 111, 112, 
	114, 115, 116, 117, 118, 122, 68, 89, 
	100, 121, 0
};

static const char _tsdp_machine_message_single_lengths[] = {
	0, 2, 1, 1, 2, 1, 1, 2, 
	1, 1, 2, 1, 1, 2, 1, 1, 
	2, 1, 1, 2, 1, 1, 2, 1, 
	1, 2, 1, 1, 2, 1, 1, 2, 
	1, 1, 2, 1, 1, 2, 1, 1, 
	2, 1, 1, 2, 1, 1, 2, 1, 
	1, 30, 30, 30, 30, 30, 30, 30, 
	30, 30, 30, 30, 30, 30, 30, 30, 
	30, 30
};

static const char _tsdp_machine_message_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2
};

static const short _tsdp_machine_message_index_offsets[] = {
	0, 0, 3, 5, 7, 10, 12, 14, 
	17, 19, 21, 24, 26, 28, 31, 33, 
	35, 38, 40, 42, 45, 47, 49, 52, 
	54, 56, 59, 61, 63, 66, 68, 70, 
	73, 75, 77, 80, 82, 84, 87, 89, 
	91, 94, 96, 98, 101, 103, 105, 108, 
	110, 112, 145, 178, 211, 244, 277, 310, 
	343, 376, 409, 442, 475, 508, 541, 574, 
	607, 640
};

static const char _tsdp_machine_message_trans_targs[] = {
	1, 2, 0, 3, 2, 50, 0, 4, 
	5, 0, 6, 5, 51, 0, 7, 8, 
	0, 9, 8, 52, 0, 10, 11, 0, 
	12, 11, 53, 0, 13, 14, 0, 15, 
	14, 54, 0, 16, 17, 0, 18, 17, 
	55, 0, 19, 20, 0, 21, 20, 56, 
	0, 22, 23, 0, 24, 23, 57, 0, 
	25, 26, 0, 27, 26, 58, 0, 28, 
	29, 0, 30, 29, 59, 0, 31, 32, 
	0, 33, 32, 60, 0, 34, 35, 0, 
	36, 35, 61, 0, 37, 38, 0, 39, 
	38, 62, 0, 40, 41, 0, 42, 41, 
	63, 0, 43, 44, 0, 45, 44, 64, 
	0, 46, 47, 0, 48, 47, 65, 0, 
	1, 4, 7, 13, 16, 19, 22, 25, 
	28, 31, 34, 37, 40, 43, 46, 1, 
	4, 7, 13, 16, 19, 22, 25, 28, 
	31, 34, 37, 40, 43, 46, 10, 10, 
	0, 1, 4, 7, 13, 16, 19, 22, 
	25, 28, 31, 34, 37, 40, 43, 46, 
	1, 4, 7, 13, 16, 19, 22, 25, 
	28, 31, 34, 37, 40, 43, 46, 10, 
	10, 0, 1, 4, 7, 13, 16, 19, 
	22, 25, 28, 31, 34, 37, 40, 43, 
	46, 1, 4, 7, 13, 16, 19, 22, 
	25, 28, 31, 34, 37, 40, 43, 46, 
	10, 10, 0, 1, 4, 7, 13, 16, 
	19, 22, 25, 28, 31, 34, 37, 40, 
	43, 46, 1, 4, 7, 13, 16, 19, 
	22, 25, 28, 31, 34, 37, 40, 43, 
	46, 10, 10, 0, 1, 4, 7, 13, 
	16, 19, 22, 25, 28, 31, 34, 37, 
	40, 43, 46, 1, 4, 7, 13, 16, 
	19, 22, 25, 28, 31, 34, 37, 40, 
	43, 46, 10, 10, 0, 1, 4, 7, 
	13, 16, 19, 22, 25, 28, 31, 34, 
	37, 40, 43, 46, 1, 4, 7, 13, 
	16, 19, 22, 25, 28, 31, 34, 37, 
	40, 43, 46, 10, 10, 0, 1, 4, 
	7, 13, 16, 19, 22, 25, 28, 31, 
	34, 37, 40, 43, 46, 1, 4, 7, 
	13, 16, 19, 22, 25, 28, 31, 34, 
	37, 40, 43, 46, 10, 10, 0, 1, 
	4, 7, 13, 16, 19, 22, 25, 28, 
	31, 34, 37, 40, 43, 46, 1, 4, 
	7, 13, 16, 19, 22, 25, 28, 31, 
	34, 37, 40, 43, 46, 10, 10, 0, 
	1, 4, 7, 13, 16, 19, 22, 25, 
	28, 31, 34, 37, 40, 43, 46, 1, 
	4, 7, 13, 16, 19, 22, 25, 28, 
	31, 34, 37, 40, 43, 46, 10, 10, 
	0, 1, 4, 7, 13, 16, 19, 22, 
	25, 28, 31, 34, 37, 40, 43, 46, 
	1, 4, 7, 13, 16, 19, 22, 25, 
	28, 31, 34, 37, 40, 43, 46, 10, 
	10, 0, 1, 4, 7, 13, 16, 19, 
	22, 25, 28, 31, 34, 37, 40, 43, 
	46, 1, 4, 7, 13, 16, 19, 22, 
	25, 28, 31, 34, 37, 40, 43, 46, 
	10, 10, 0, 1, 4, 7, 13, 16, 
	19, 22, 25, 28, 31, 34, 37, 40, 
	43, 46, 1, 4, 7, 13, 16, 19, 
	22, 25, 28, 31, 34, 37, 40, 43, 
	46, 10, 10, 0, 1, 4, 7, 13, 
	16, 19, 22, 25, 28, 31, 34, 37, 
	40, 43, 46, 1, 4, 7, 13, 16, 
	19, 22, 25, 28, 31, 34, 37, 40, 
	43, 46, 10, 10, 0, 1, 4, 7, 
	13, 16, 19, 22, 25, 28, 31, 34, 
	37, 40, 43, 46, 1, 4, 7, 13, 
	16, 19, 22, 25, 28, 31, 34, 37, 
	40, 43, 46, 10, 10, 0, 1, 4, 
	7, 13, 16, 19, 22, 25, 28, 31, 
	34, 37, 40, 43, 46, 1, 4, 7, 
	13, 16, 19, 22, 25, 28, 31, 34, 
	37, 40, 43, 46, 10, 10, 0, 1, 
	4, 7, 13, 16, 19, 22, 25, 28, 
	31, 34, 37, 40, 43, 46, 1, 4, 
	7, 13, 16, 19, 22, 25, 28, 31, 
	34, 37, 40, 43, 46, 10, 10, 0, 
	1, 4, 7, 13, 16, 19, 22, 25, 
	28, 31, 34, 37, 40, 43, 46, 1, 
	4, 7, 13, 16, 19, 22, 25, 28, 
	31, 34, 37, 40, 43, 46, 10, 10, 
	0, 0
};

static const char _tsdp_machine_message_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0, 35, 35, 35, 35, 35, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	35, 0, 38, 38, 38, 38, 38, 38, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	38, 38, 38, 38, 38, 38, 38, 38, 
	38, 38, 0, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 41, 41, 41, 41, 41, 
	41, 41, 41, 0, 44, 44, 44, 44, 
	44, 44, 44, 44, 44, 44, 44, 44, 
	44, 44, 44, 44, 44, 44, 44, 44, 
	44, 44, 44, 44, 44, 44, 44, 44, 
	44, 44, 44, 44, 0, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 47, 47, 47, 
	47, 47, 47, 47, 47, 0, 50, 50, 
	50, 50, 50, 50, 50, 50, 50, 50, 
	50, 50, 50, 50, 50, 50, 50, 50, 
	50, 50, 50, 50, 50, 50, 50, 50, 
	50, 50, 50, 50, 50, 50, 0, 53, 
	53, 53, 53, 53, 53, 53, 53, 53, 
	53, 53, 53, 53, 53, 53, 53, 53, 
	53, 53, 53, 53, 53, 53, 53, 53, 
	53, 53, 53, 53, 53, 53, 53, 0, 
	56, 56, 56, 56, 56, 56, 56, 56, 
	56, 56, 56, 56, 56, 56, 56, 56, 
	56, 56, 56, 56, 56, 56, 56, 56, 
	56, 56, 56, 56, 56, 56, 56, 56, 
	0, 59, 59, 59, 59, 59, 59, 59, 
	59, 59, 59, 59, 59, 59, 59, 59, 
	59, 59, 59, 59, 59, 59, 59, 59, 
	59, 59, 59, 59, 59, 59, 59, 59, 
	59, 0, 62, 62, 62, 62, 62, 62, 
	62, 62, 62, 62, 62, 62, 62, 62, 
	62, 62, 62, 62, 62, 62, 62, 62, 
	62, 62, 62, 62, 62, 62, 62, 62, 
	62, 62, 0, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 65, 65, 65, 65, 65, 
	65, 65, 65, 0, 68, 68, 68, 68, 
	68, 68, 68, 68, 68, 68, 68, 68, 
	68, 68, 68, 68, 68, 68, 68, 68, 
	68, 68, 68, 68, 68, 68, 68, 68, 
	68, 68, 68, 68, 0, 71, 71, 71, 
	71, 71, 71, 71, 71, 71, 71, 71, 
	71, 71, 71, 71, 71, 71, 71, 71, 
	71, 71, 71, 71, 71, 71, 71, 71, 
	71, 71, 71, 71, 71, 0, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 74, 74, 
	74, 74, 74, 74, 74, 74, 0, 77, 
	77, 77, 77, 77, 77, 77, 77, 77, 
	77, 77, 77, 77, 77, 77, 77, 77, 
	77, 77, 77, 77, 77, 77, 77, 77, 
	77, 77, 77, 77, 77, 77, 77, 0, 
	80, 80, 80, 80, 80, 80, 80, 80, 
	80, 80, 80, 80, 80, 80, 80, 80, 
	80, 80, 80, 80, 80, 80, 80, 80, 
	80, 80, 80, 80, 80, 80, 80, 80, 
	0, 0
};

static const char _tsdp_machine_message_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 5, 7, 9, 11, 13, 
	15, 17, 19, 21, 23, 25, 27, 29, 
	31, 33
};

static const int tsdp_machine_message_start = 49;
static const int tsdp_machine_message_first_final = 49;
static const int tsdp_machine_message_error = 0;

static const int tsdp_machine_message_en_main = 49;


/* #line 229 "tsdp_parser_message.rl" */

tsdp_message_t* tsdp_message_parse(const void *input, size_t size)
{
	tsdp_message_t* sdp_msg = TSDP_NULL;
	const char* tag_start = TSDP_NULL;
	tsdp_header_t *header = TSDP_NULL;
	tsdp_header_T_t *hdr_T = TSDP_NULL;

	/* Ragel variables */
	int cs = 0;
	const char* p = input;
	const char* pe = p + size;
	const char* eof = TSDP_NULL;

	if(!input || !size){
		TSK_DEBUG_ERROR("Null or empty buffer.");
		goto bail;
	}

	if(!(sdp_msg = TSDP_MESSAGE_CREATE())){
		goto bail;
	}

	/* Ragel init */
	
/* #line 425 "../src/parsers/tsdp_parser_message.c" */
	{
	cs = tsdp_machine_message_start;
	}

/* #line 254 "tsdp_parser_message.rl" */

	/* Ragel execute */
	
/* #line 434 "../src/parsers/tsdp_parser_message.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _tsdp_machine_message_trans_keys + _tsdp_machine_message_key_offsets[cs];
	_trans = _tsdp_machine_message_index_offsets[cs];

	_klen = _tsdp_machine_message_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _tsdp_machine_message_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _tsdp_machine_message_trans_targs[_trans];

	if ( _tsdp_machine_message_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tsdp_machine_message_actions + _tsdp_machine_message_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 56 "tsdp_parser_message.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 63 "tsdp_parser_message.rl" */
	{
		TSK_DEBUG_INFO("Header A");
	}
	break;
	case 2:
/* #line 67 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header B");
	}
	break;
	case 3:
/* #line 75 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_C_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header C");
	}
	break;
	case 4:
/* #line 83 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_Dummy_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Dummy");
	}
	break;
	case 5:
/* #line 91 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_E_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header E");
	}
	break;
	case 6:
/* #line 99 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_I_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header I");
	}
	break;
	case 7:
/* #line 107 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_K_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header K");
	}
	break;
	case 8:
/* #line 115 "tsdp_parser_message.rl" */
	{
		TSK_DEBUG_INFO("Header M");
	}
	break;
	case 9:
/* #line 119 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_O_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header O");
	}
	break;
	case 10:
/* #line 127 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_P_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header P");
	}
	break;
	case 11:
/* #line 135 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_R_parse(tag_start, (p - tag_start)))){
			if(hdr_T){
				if(!hdr_T->repeat_fields){
					hdr_T->repeat_fields = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(hdr_T->repeat_fields, (void**)&header);
			}
			else{
				tsdp_message_add_header(sdp_msg, header);
				tsk_object_unref(header);
			}
		}
		TSK_DEBUG_INFO("Header R");
	}
	break;
	case 12:
/* #line 151 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_S_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header S");
	}
	break;
	case 13:
/* #line 159 "tsdp_parser_message.rl" */
	{
		if((hdr_T = tsdp_header_T_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_T));
			hdr_T = tsk_object_unref(hdr_T);
		}
		TSK_DEBUG_INFO("Header T");
	}
	break;
	case 14:
/* #line 167 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_U_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header U");
	}
	break;
	case 15:
/* #line 175 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_V_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header V");
	}
	break;
	case 16:
/* #line 183 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_Z_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Z");
	}
	break;
/* #line 673 "../src/parsers/tsdp_parser_message.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _tsdp_machine_message_actions + _tsdp_machine_message_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
/* #line 63 "tsdp_parser_message.rl" */
	{
		TSK_DEBUG_INFO("Header A");
	}
	break;
	case 2:
/* #line 67 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header B");
	}
	break;
	case 3:
/* #line 75 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_C_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header C");
	}
	break;
	case 4:
/* #line 83 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_Dummy_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Dummy");
	}
	break;
	case 5:
/* #line 91 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_E_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header E");
	}
	break;
	case 6:
/* #line 99 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_I_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header I");
	}
	break;
	case 7:
/* #line 107 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_K_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header K");
	}
	break;
	case 8:
/* #line 115 "tsdp_parser_message.rl" */
	{
		TSK_DEBUG_INFO("Header M");
	}
	break;
	case 9:
/* #line 119 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_O_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header O");
	}
	break;
	case 10:
/* #line 127 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_P_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header P");
	}
	break;
	case 11:
/* #line 135 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_R_parse(tag_start, (p - tag_start)))){
			if(hdr_T){
				if(!hdr_T->repeat_fields){
					hdr_T->repeat_fields = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(hdr_T->repeat_fields, (void**)&header);
			}
			else{
				tsdp_message_add_header(sdp_msg, header);
				tsk_object_unref(header);
			}
		}
		TSK_DEBUG_INFO("Header R");
	}
	break;
	case 12:
/* #line 151 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_S_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header S");
	}
	break;
	case 13:
/* #line 159 "tsdp_parser_message.rl" */
	{
		if((hdr_T = tsdp_header_T_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_T));
			hdr_T = tsk_object_unref(hdr_T);
		}
		TSK_DEBUG_INFO("Header T");
	}
	break;
	case 14:
/* #line 167 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_U_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header U");
	}
	break;
	case 15:
/* #line 175 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_V_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header V");
	}
	break;
	case 16:
/* #line 183 "tsdp_parser_message.rl" */
	{
		if((header = (tsdp_header_t*)tsdp_header_Z_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Z");
	}
	break;
/* #line 849 "../src/parsers/tsdp_parser_message.c" */
		}
	}
	}

	_out: {}
	}

/* #line 257 "tsdp_parser_message.rl" */

	/* Check result */
	if( cs < 
/* #line 861 "../src/parsers/tsdp_parser_message.c" */
49
/* #line 259 "tsdp_parser_message.rl" */
 )
	{
		TSK_DEBUG_ERROR("Failed to parse SDP message.");
		TSK_OBJECT_SAFE_FREE(sdp_msg);
		goto bail;
	}
	
bail:
	return sdp_msg;
}