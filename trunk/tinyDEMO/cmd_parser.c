
/* #line 1 "./cmd_parser.rl" */
/*
* Copyright (C) 2009 Mamadou Diop.
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
#include "cmd_parser.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_ragel_state.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/

/* #line 123 "./cmd_parser.rl" */


tsk_options_L_t *cmd_parser_parse(const char *line, cmd_type_t* cmd)
{
	int cs = 0;
	const char *p = line;
	const char *pe = p + tsk_strlen(p);
	const char *eof = pe;
	tsk_option_t* curr_opt = tsk_null;
	tsk_options_L_t *options = tsk_null;
	char* temp = tsk_null;
	
	const char *tag_start;
	
	/* global vars and initilization (Ragel) */
	
/* #line 55 "./cmd_parser.c" */
static const char _demo_machine_parser_cmd_actions[] = {
	0, 1, 0, 1, 1, 1, 5, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 1, 17, 1, 
	18, 1, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 24, 1, 25, 1, 
	26, 1, 27, 1, 28, 1, 29, 1, 
	30, 1, 31, 1, 32, 1, 33, 1, 
	34, 1, 35, 1, 36, 1, 37, 1, 
	38, 1, 39, 1, 40, 1, 41, 1, 
	42, 1, 43, 1, 44, 2, 0, 4, 
	2, 0, 5, 2, 3, 2, 2, 5, 
	1, 2, 6, 1, 2, 7, 1, 2, 
	8, 1, 2, 9, 1, 2, 10, 1, 
	2, 11, 1, 2, 12, 1, 2, 13, 
	1, 2, 14, 1, 2, 15, 1, 2, 
	16, 1, 2, 17, 1, 2, 18, 1, 
	2, 19, 1, 2, 20, 1, 2, 21, 
	1, 2, 22, 1, 2, 23, 0, 2, 
	24, 0, 2, 25, 0, 2, 26, 0, 
	2, 27, 0, 2, 28, 0, 2, 29, 
	0, 2, 30, 0, 2, 31, 0, 2, 
	32, 0, 2, 33, 0, 2, 34, 0, 
	2, 35, 0, 2, 36, 0, 2, 37, 
	0, 2, 38, 0, 2, 39, 0, 2, 
	40, 0, 2, 41, 0, 2, 42, 0, 
	2, 43, 0, 2, 44, 0, 3, 0, 
	3, 2, 3, 0, 5, 1, 3, 3, 
	2, 1, 4, 0, 3, 2, 1, 4, 
	0, 4, 3, 2, 4, 23, 0, 3, 
	2, 4, 24, 0, 3, 2, 4, 25, 
	0, 3, 2, 4, 26, 0, 3, 2, 
	4, 27, 0, 3, 2, 4, 28, 0, 
	3, 2, 4, 29, 0, 3, 2, 4, 
	30, 0, 3, 2, 4, 31, 0, 3, 
	2, 4, 32, 0, 3, 2, 4, 33, 
	0, 3, 2, 4, 34, 0, 3, 2, 
	4, 35, 0, 3, 2, 4, 36, 0, 
	3, 2, 4, 37, 0, 3, 2, 4, 
	38, 0, 3, 2, 4, 39, 0, 3, 
	2, 4, 40, 0, 3, 2, 4, 41, 
	0, 3, 2, 4, 42, 0, 3, 2, 
	4, 43, 0, 3, 2, 4, 44, 0, 
	3, 2, 5, 0, 4, 3, 2, 1, 
	5, 23, 0, 3, 2, 1, 5, 24, 
	0, 3, 2, 1, 5, 25, 0, 3, 
	2, 1, 5, 26, 0, 3, 2, 1, 
	5, 27, 0, 3, 2, 1, 5, 28, 
	0, 3, 2, 1, 5, 29, 0, 3, 
	2, 1, 5, 30, 0, 3, 2, 1, 
	5, 31, 0, 3, 2, 1, 5, 32, 
	0, 3, 2, 1, 5, 33, 0, 3, 
	2, 1, 5, 34, 0, 3, 2, 1, 
	5, 35, 0, 3, 2, 1, 5, 36, 
	0, 3, 2, 1, 5, 37, 0, 3, 
	2, 1, 5, 38, 0, 3, 2, 1, 
	5, 39, 0, 3, 2, 1, 5, 40, 
	0, 3, 2, 1, 5, 41, 0, 3, 
	2, 1, 5, 42, 0, 3, 2, 1, 
	5, 43, 0, 3, 2, 1, 5, 44, 
	0, 3, 2, 1
};

static const short _demo_machine_parser_cmd_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 5, 6, 
	7, 8, 10, 11, 12, 13, 15, 17, 
	18, 19, 20, 21, 22, 23, 24, 25, 
	26, 27, 28, 29, 30, 31, 32, 33, 
	35, 36, 38, 39, 40, 41, 42, 43, 
	44, 45, 47, 48, 49, 50, 51, 52, 
	53, 54, 56, 57, 58, 59, 60, 61, 
	62, 63, 64, 65, 66, 69, 70, 71, 
	72, 73, 74, 75, 76, 77, 78, 79, 
	80, 81, 82, 84, 85, 86, 87, 88, 
	89, 92, 93, 94, 95, 96, 97, 98, 
	99, 100, 101, 102, 103, 104, 105, 106, 
	107, 108, 109, 110, 111, 113, 114, 115, 
	116, 117, 118, 119, 120, 121, 122, 123, 
	124, 125, 126, 127, 128, 129, 130, 131, 
	132, 133, 134, 135, 136, 137, 139, 140, 
	141, 142, 144, 145, 146, 147, 148, 149, 
	150, 151, 152, 154, 155, 156, 157, 158, 
	159, 160, 161, 176, 180, 184, 201, 205, 
	210, 215, 220, 225, 230, 235, 240, 245, 
	250, 255, 260, 265, 270, 275, 280, 285, 
	290, 295, 300, 305, 310, 315, 320, 336, 
	351, 367, 383, 398, 413, 428, 443, 459, 
	474, 490, 506, 522, 537, 552, 568, 583, 
	598, 613, 628, 643, 658, 673, 688, 703, 
	718, 733
};

static const char _demo_machine_parser_cmd_trans_keys[] = {
	43, 43, 45, 109, 102, 97, 112, 115, 
	104, 110, 99, 112, 118, 52, 54, 97, 
	115, 109, 101, 45, 110, 97, 112, 116, 
	114, 114, 111, 109, 101, 97, 100, 101, 
	114, 109, 112, 112, 105, 117, 118, 54, 
	111, 99, 97, 108, 45, 105, 112, 112, 
	111, 114, 116, 112, 105, 100, 97, 99, 
	115, 115, 119, 111, 114, 100, 115, 99, 
	102, 45, 105, 112, 116, 112, 111, 114, 
	116, 114, 97, 110, 115, 101, 97, 108, 
	109, 105, 100, 103, 99, 111, 109, 112, 
	111, 102, 111, 115, 110, 103, 117, 112, 
	115, 115, 97, 103, 101, 117, 98, 105, 
	115, 104, 103, 115, 116, 101, 114, 109, 
	117, 115, 100, 101, 111, 98, 99, 114, 
	105, 98, 101, 110, 105, 116, 108, 112, 
	108, 101, 105, 116, 110, 102, 105, 103, 
	45, 102, 115, 105, 108, 101, 101, 116, 
	115, 115, 105, 111, 110, 97, 99, 107, 
	115, 116, 100, 105, 111, 105, 100, 101, 
	111, 32, 45, 97, 99, 101, 102, 104, 
	109, 112, 113, 114, 115, 118, 9, 13, 
	32, 45, 9, 13, 32, 45, 9, 13, 
	32, 45, 61, 97, 99, 100, 102, 104, 
	105, 108, 111, 112, 114, 115, 116, 9, 
	13, 32, 45, 9, 13, 32, 45, 61, 
	9, 13, 32, 45, 61, 9, 13, 32, 
	45, 61, 9, 13, 32, 45, 61, 9, 
	13, 32, 45, 61, 9, 13, 32, 45, 
	61, 9, 13, 32, 45, 61, 9, 13, 
	32, 45, 61, 9, 13, 32, 45, 61, 
	9, 13, 32, 45, 61, 9, 13, 32, 
	45, 61, 9, 13, 32, 45, 61, 9, 
	13, 32, 45, 61, 9, 13, 32, 45, 
	61, 9, 13, 32, 45, 61, 9, 13, 
	32, 45, 61, 9, 13, 32, 45, 61, 
	9, 13, 32, 45, 61, 9, 13, 32, 
	45, 61, 9, 13, 32, 45, 61, 9, 
	13, 32, 45, 61, 9, 13, 32, 45, 
	61, 9, 13, 32, 45, 61, 9, 13, 
	32, 45, 97, 99, 101, 102, 104, 109, 
	112, 113, 114, 115, 117, 118, 9, 13, 
	32, 45, 97, 99, 101, 102, 104, 109, 
	112, 113, 114, 115, 118, 9, 13, 32, 
	45, 97, 99, 101, 102, 104, 109, 112, 
	113, 114, 115, 118, 120, 9, 13, 32, 
	45, 97, 99, 101, 102, 104, 105, 109, 
	112, 113, 114, 115, 118, 9, 13, 32, 
	45, 97, 99, 101, 102, 104, 109, 112, 
	113, 114, 115, 118, 9, 13, 32, 45, 
	97, 99, 101, 102, 104, 109, 112, 113, 
	114, 115, 118, 9, 13, 32, 45, 97, 
	99, 101, 102, 104, 109, 112, 113, 114, 
	115, 118, 9, 13, 32, 45, 97, 99, 
	101, 102, 104, 109, 112, 113, 114, 115, 
	118, 9, 13, 32, 45, 97, 99, 101, 
	102, 104, 108, 109, 112, 113, 114, 115, 
	118, 9, 13, 32, 45, 97, 99, 101, 
	102, 104, 109, 112, 113, 114, 115, 118, 
	9, 13, 32, 45, 97, 99, 101, 102, 
	104, 109, 112, 113, 114, 115, 117, 118, 
	9, 13, 32, 45, 97, 99, 101, 102, 
	104, 109, 112, 113, 114, 115, 117, 118, 
	9, 13, 32, 45, 97, 99, 101, 102, 
	104, 105, 109, 112, 113, 114, 115, 118, 
	9, 13, 32, 45, 97, 99, 101, 102, 
	104, 109, 112, 113, 114, 115, 118, 9, 
	13, 32, 45, 97, 99, 101, 102, 104, 
	109, 112, 113, 114, 115, 118, 9, 13, 
	32, 45, 97, 99, 101, 102, 104, 105, 
	109, 112, 113, 114, 115, 118, 9, 13, 
	32, 45, 97, 99, 101, 102, 104, 109, 
	112, 113, 114, 115, 118, 9, 13, 32, 
	45, 97, 99, 101, 102, 104, 109, 112, 
	113, 114, 115, 118, 9, 13, 32, 45, 
	97, 99, 101, 102, 104, 109, 112, 113, 
	114, 115, 118, 9, 13, 32, 45, 97, 
	99, 101, 102, 104, 109, 112, 113, 114, 
	115, 118, 9, 13, 32, 45, 97, 99, 
	101, 102, 104, 109, 112, 113, 114, 115, 
	118, 9, 13, 32, 45, 97, 99, 101, 
	102, 104, 109, 112, 113, 114, 115, 118, 
	9, 13, 32, 45, 97, 99, 101, 102, 
	104, 109, 112, 113, 114, 115, 118, 9, 
	13, 32, 45, 97, 99, 101, 102, 104, 
	109, 112, 113, 114, 115, 118, 9, 13, 
	32, 45, 97, 99, 101, 102, 104, 109, 
	112, 113, 114, 115, 118, 9, 13, 32, 
	45, 97, 99, 101, 102, 104, 109, 112, 
	113, 114, 115, 118, 9, 13, 32, 45, 
	97, 99, 101, 102, 104, 109, 112, 113, 
	114, 115, 118, 9, 13, 32, 45, 97, 
	99, 101, 102, 104, 109, 112, 113, 114, 
	115, 118, 9, 13, 0
};

static const char _demo_machine_parser_cmd_single_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 2, 1, 1, 1, 2, 2, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 3, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 2, 1, 1, 1, 1, 1, 
	3, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 2, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 2, 1, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 1, 2, 1, 1, 1, 1, 1, 
	1, 1, 13, 2, 2, 15, 2, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 14, 13, 
	14, 14, 13, 13, 13, 13, 14, 13, 
	14, 14, 14, 13, 13, 14, 13, 13, 
	13, 13, 13, 13, 13, 13, 13, 13, 
	13, 13
};

static const char _demo_machine_parser_cmd_range_lengths[] = {
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
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1
};

static const short _demo_machine_parser_cmd_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 19, 21, 23, 25, 28, 31, 
	33, 35, 37, 39, 41, 43, 45, 47, 
	49, 51, 53, 55, 57, 59, 61, 63, 
	66, 68, 71, 73, 75, 77, 79, 81, 
	83, 85, 88, 90, 92, 94, 96, 98, 
	100, 102, 105, 107, 109, 111, 113, 115, 
	117, 119, 121, 123, 125, 129, 131, 133, 
	135, 137, 139, 141, 143, 145, 147, 149, 
	151, 153, 155, 158, 160, 162, 164, 166, 
	168, 172, 174, 176, 178, 180, 182, 184, 
	186, 188, 190, 192, 194, 196, 198, 200, 
	202, 204, 206, 208, 210, 213, 215, 217, 
	219, 221, 223, 225, 227, 229, 231, 233, 
	235, 237, 239, 241, 243, 245, 247, 249, 
	251, 253, 255, 257, 259, 261, 264, 266, 
	268, 270, 273, 275, 277, 279, 281, 283, 
	285, 287, 289, 292, 294, 296, 298, 300, 
	302, 304, 306, 321, 325, 329, 346, 350, 
	355, 360, 365, 370, 375, 380, 385, 390, 
	395, 400, 405, 410, 415, 420, 425, 430, 
	435, 440, 445, 450, 455, 460, 465, 481, 
	496, 512, 528, 543, 558, 573, 588, 604, 
	619, 635, 651, 667, 682, 697, 713, 728, 
	743, 758, 773, 788, 803, 818, 833, 848, 
	863, 878
};

static const unsigned char _demo_machine_parser_cmd_trans_targs[] = {
	2, 0, 146, 0, 149, 0, 5, 0, 
	152, 0, 7, 0, 8, 0, 153, 0, 
	10, 14, 0, 11, 0, 12, 0, 13, 
	0, 154, 155, 0, 15, 17, 0, 16, 
	0, 156, 0, 18, 0, 19, 0, 20, 
	0, 21, 0, 22, 0, 157, 0, 24, 
	0, 25, 0, 158, 0, 27, 0, 28, 
	0, 29, 0, 30, 0, 159, 0, 32, 
	34, 0, 33, 0, 160, 161, 0, 35, 
	0, 162, 0, 37, 0, 38, 0, 39, 
	0, 40, 0, 41, 0, 42, 43, 0, 
	163, 0, 44, 0, 45, 0, 164, 0, 
	47, 0, 48, 0, 165, 0, 50, 56, 
	0, 51, 0, 52, 0, 53, 0, 54, 
	0, 55, 0, 166, 0, 57, 0, 58, 
	0, 59, 0, 60, 0, 61, 62, 65, 
	0, 167, 0, 63, 0, 64, 0, 168, 
	0, 66, 0, 67, 0, 68, 0, 169, 
	0, 70, 0, 71, 0, 72, 0, 170, 
	0, 74, 0, 171, 75, 0, 76, 0, 
	77, 0, 78, 0, 172, 0, 173, 0, 
	175, 120, 138, 0, 82, 0, 83, 0, 
	84, 0, 179, 0, 86, 0, 87, 0, 
	88, 0, 89, 0, 181, 0, 91, 0, 
	182, 0, 93, 0, 94, 0, 183, 0, 
	186, 0, 97, 0, 98, 0, 99, 0, 
	187, 0, 101, 105, 0, 188, 0, 103, 
	0, 104, 0, 190, 0, 191, 0, 107, 
	0, 108, 0, 109, 0, 110, 0, 192, 
	0, 193, 0, 113, 0, 194, 0, 115, 
	0, 195, 0, 117, 0, 196, 0, 119, 
	0, 197, 0, 121, 0, 122, 0, 123, 
	0, 124, 0, 125, 0, 126, 129, 0, 
	127, 0, 128, 0, 175, 0, 130, 135, 
	0, 131, 0, 132, 0, 133, 0, 134, 
	0, 198, 0, 136, 0, 137, 0, 199, 
	0, 198, 199, 0, 140, 0, 141, 0, 
	200, 0, 143, 0, 144, 0, 145, 0, 
	201, 0, 148, 3, 174, 80, 176, 177, 
	178, 180, 90, 184, 185, 100, 189, 148, 
	147, 148, 3, 148, 147, 148, 3, 148, 
	0, 151, 3, 151, 4, 6, 9, 23, 
	26, 31, 36, 46, 49, 69, 73, 79, 
	151, 150, 148, 3, 148, 150, 151, 3, 
	151, 151, 150, 151, 3, 151, 151, 150, 
	151, 3, 151, 151, 150, 151, 3, 151, 
	151, 150, 151, 3, 151, 151, 150, 151, 
	3, 151, 151, 150, 151, 3, 151, 151, 
	150, 151, 3, 151, 151, 150, 151, 3, 
	151, 151, 150, 151, 3, 151, 151, 150, 
	151, 3, 151, 151, 150, 151, 3, 151, 
	151, 150, 151, 3, 151, 151, 150, 151, 
	3, 151, 151, 150, 151, 3, 151, 151, 
	150, 151, 3, 151, 151, 150, 151, 3, 
	151, 151, 150, 151, 3, 151, 151, 150, 
	151, 3, 151, 151, 150, 151, 3, 151, 
	151, 150, 151, 3, 151, 151, 150, 151, 
	3, 151, 151, 150, 151, 3, 151, 151, 
	150, 148, 3, 174, 80, 176, 177, 178, 
	180, 90, 184, 185, 100, 139, 201, 148, 
	0, 148, 3, 174, 80, 176, 177, 178, 
	180, 90, 184, 185, 100, 189, 148, 0, 
	148, 3, 174, 80, 176, 177, 178, 180, 
	90, 184, 185, 100, 189, 118, 148, 0, 
	148, 3, 174, 80, 176, 177, 178, 116, 
	180, 90, 184, 185, 100, 189, 148, 0, 
	148, 3, 81, 80, 114, 177, 178, 180, 
	179, 184, 185, 100, 189, 148, 0, 148, 
	3, 174, 80, 176, 177, 178, 180, 90, 
	184, 185, 100, 189, 148, 0, 148, 3, 
	174, 80, 85, 177, 178, 180, 90, 184, 
	185, 100, 189, 148, 0, 148, 3, 174, 
	80, 176, 177, 178, 180, 90, 184, 185, 
	100, 189, 148, 0, 148, 3, 174, 80, 
	176, 177, 178, 92, 180, 90, 184, 185, 
	100, 189, 148, 0, 148, 3, 174, 80, 
	176, 177, 178, 180, 90, 184, 185, 100, 
	189, 148, 0, 148, 3, 174, 80, 176, 
	177, 178, 180, 90, 184, 185, 100, 112, 
	189, 148, 0, 148, 3, 174, 80, 95, 
	177, 178, 180, 90, 184, 185, 100, 111, 
	189, 148, 0, 148, 3, 174, 80, 176, 
	177, 178, 96, 180, 90, 184, 185, 100, 
	189, 148, 0, 148, 3, 174, 80, 176, 
	177, 178, 180, 90, 184, 185, 100, 189, 
	148, 0, 148, 3, 174, 80, 176, 177, 
	178, 180, 90, 184, 185, 100, 189, 148, 
	0, 148, 3, 174, 80, 176, 177, 178, 
	102, 180, 90, 184, 185, 100, 189, 148, 
	0, 148, 3, 174, 80, 176, 177, 178, 
	180, 90, 184, 185, 100, 189, 148, 0, 
	148, 3, 174, 80, 176, 177, 178, 180, 
	90, 184, 185, 106, 189, 148, 0, 148, 
	3, 174, 80, 176, 177, 178, 180, 90, 
	184, 185, 100, 189, 148, 0, 148, 3, 
	174, 80, 176, 177, 178, 180, 90, 184, 
	185, 100, 189, 148, 0, 148, 3, 174, 
	80, 176, 177, 178, 180, 90, 184, 185, 
	100, 189, 148, 0, 148, 3, 174, 80, 
	176, 177, 178, 180, 90, 184, 185, 100, 
	189, 148, 0, 148, 3, 174, 80, 176, 
	177, 178, 180, 90, 184, 185, 100, 189, 
	148, 0, 148, 3, 174, 80, 176, 177, 
	178, 180, 90, 184, 185, 100, 189, 148, 
	0, 148, 3, 174, 80, 176, 177, 178, 
	180, 90, 184, 185, 100, 189, 148, 0, 
	148, 3, 174, 80, 176, 177, 178, 180, 
	90, 184, 185, 100, 189, 148, 0, 148, 
	3, 174, 80, 176, 177, 178, 180, 90, 
	184, 185, 100, 142, 148, 0, 148, 3, 
	174, 80, 176, 177, 178, 180, 90, 184, 
	185, 100, 189, 148, 0, 0
};

static const short _demo_machine_parser_cmd_trans_actions[] = {
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
	0, 0, 88, 218, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 88, 
	1, 5, 94, 5, 0, 0, 3, 0, 
	0, 85, 346, 85, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	85, 85, 91, 222, 91, 0, 0, 226, 
	0, 0, 1, 41, 352, 41, 41, 148, 
	43, 358, 43, 43, 151, 45, 364, 45, 
	45, 154, 47, 370, 47, 47, 157, 49, 
	376, 49, 49, 160, 51, 382, 51, 51, 
	163, 53, 388, 53, 53, 166, 55, 394, 
	55, 55, 169, 57, 400, 57, 57, 172, 
	59, 406, 59, 59, 175, 61, 412, 61, 
	61, 178, 63, 418, 63, 63, 181, 65, 
	424, 65, 65, 184, 67, 430, 67, 67, 
	187, 69, 436, 69, 69, 190, 71, 442, 
	71, 71, 193, 73, 448, 73, 73, 196, 
	75, 454, 75, 75, 199, 77, 460, 77, 
	77, 202, 79, 466, 79, 79, 205, 81, 
	472, 81, 81, 208, 83, 478, 83, 83, 
	211, 7, 97, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 0, 0, 7, 
	0, 11, 103, 11, 11, 11, 11, 11, 
	11, 11, 11, 11, 11, 11, 11, 0, 
	17, 112, 17, 17, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 0, 17, 0, 
	21, 118, 21, 21, 21, 21, 21, 0, 
	21, 21, 21, 21, 21, 21, 21, 0, 
	25, 124, 0, 25, 0, 25, 25, 25, 
	0, 25, 25, 25, 25, 25, 0, 23, 
	121, 23, 23, 23, 23, 23, 23, 23, 
	23, 23, 23, 23, 23, 0, 27, 127, 
	27, 27, 0, 27, 27, 27, 27, 27, 
	27, 27, 27, 27, 0, 27, 127, 27, 
	27, 27, 27, 27, 27, 27, 27, 27, 
	27, 27, 27, 0, 29, 130, 29, 29, 
	29, 29, 29, 0, 29, 29, 29, 29, 
	29, 29, 29, 0, 29, 130, 29, 29, 
	29, 29, 29, 29, 29, 29, 29, 29, 
	29, 29, 0, 19, 115, 19, 19, 19, 
	19, 19, 19, 19, 19, 19, 19, 0, 
	19, 19, 0, 33, 136, 33, 33, 0, 
	33, 33, 33, 33, 33, 33, 33, 0, 
	33, 33, 0, 31, 133, 31, 31, 31, 
	31, 31, 0, 31, 31, 31, 31, 31, 
	31, 31, 0, 31, 133, 31, 31, 31, 
	31, 31, 31, 31, 31, 31, 31, 31, 
	31, 0, 35, 139, 35, 35, 35, 35, 
	35, 35, 35, 35, 35, 35, 35, 35, 
	0, 39, 145, 39, 39, 39, 39, 39, 
	0, 39, 39, 39, 39, 39, 39, 39, 
	0, 39, 145, 39, 39, 39, 39, 39, 
	39, 39, 39, 39, 39, 39, 39, 0, 
	37, 142, 37, 37, 37, 37, 37, 37, 
	37, 37, 37, 0, 37, 37, 0, 37, 
	142, 37, 37, 37, 37, 37, 37, 37, 
	37, 37, 37, 37, 37, 0, 33, 136, 
	33, 33, 33, 33, 33, 33, 33, 33, 
	33, 33, 33, 33, 0, 19, 115, 19, 
	19, 19, 19, 19, 19, 19, 19, 19, 
	19, 19, 19, 0, 25, 124, 25, 25, 
	25, 25, 25, 25, 25, 25, 25, 25, 
	25, 25, 0, 21, 118, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 0, 17, 112, 17, 17, 17, 17, 
	17, 17, 17, 17, 17, 17, 17, 17, 
	0, 13, 106, 13, 13, 13, 13, 13, 
	13, 13, 13, 13, 13, 13, 13, 0, 
	15, 109, 15, 15, 15, 15, 15, 15, 
	15, 15, 15, 15, 15, 15, 0, 7, 
	97, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 0, 7, 0, 9, 100, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 0, 0
};

static const short _demo_machine_parser_cmd_eof_actions[] = {
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
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 88, 5, 0, 231, 91, 214, 
	236, 241, 246, 251, 256, 261, 266, 271, 
	276, 281, 286, 291, 296, 301, 306, 311, 
	316, 321, 326, 331, 336, 341, 7, 11, 
	17, 21, 25, 23, 27, 27, 29, 29, 
	19, 33, 31, 31, 35, 39, 39, 37, 
	37, 33, 19, 25, 21, 17, 13, 15, 
	7, 9
};

static const int demo_machine_parser_cmd_start = 1;
static const int demo_machine_parser_cmd_first_final = 146;
static const int demo_machine_parser_cmd_error = 0;

static const int demo_machine_parser_cmd_en_main = 1;


/* #line 139 "./cmd_parser.rl" */
	
/* #line 601 "./cmd_parser.c" */
	{
	cs = demo_machine_parser_cmd_start;
	}

/* #line 140 "./cmd_parser.rl" */
	
	/* default values */
	*cmd = cmd_none;
	
	/* exec */
	
/* #line 613 "./cmd_parser.c" */
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
	_keys = _demo_machine_parser_cmd_trans_keys + _demo_machine_parser_cmd_key_offsets[cs];
	_trans = _demo_machine_parser_cmd_index_offsets[cs];

	_klen = _demo_machine_parser_cmd_single_lengths[cs];
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

	_klen = _demo_machine_parser_cmd_range_lengths[cs];
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
	cs = _demo_machine_parser_cmd_trans_targs[_trans];

	if ( _demo_machine_parser_cmd_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _demo_machine_parser_cmd_actions + _demo_machine_parser_cmd_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 37 "./cmd_parser.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 41 "./cmd_parser.rl" */
	{
		TSK_OBJECT_SAFE_FREE(curr_opt);
		curr_opt = tsk_option_create_null();
	}
	break;
	case 2:
/* #line 46 "./cmd_parser.rl" */
	{
		tsk_options_add_option_2(&options, curr_opt);
		TSK_OBJECT_SAFE_FREE(curr_opt);
	}
	break;
	case 3:
/* #line 51 "./cmd_parser.rl" */
	{
		if(curr_opt){
			TSK_PARSER_SET_STRING(curr_opt->value);
		}
	}
	break;
	case 4:
/* #line 57 "./cmd_parser.rl" */
	{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid option.", temp);
	}
	break;
	case 5:
/* #line 62 "./cmd_parser.rl" */
	{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid command.", temp);
	}
	break;
	case 6:
/* #line 71 "./cmd_parser.rl" */
	{ *cmd = cmd_audio; }
	break;
	case 7:
/* #line 72 "./cmd_parser.rl" */
	{ *cmd = cmd_audiovideo; }
	break;
	case 8:
/* #line 73 "./cmd_parser.rl" */
	{ *cmd = cmd_config_file; }
	break;
	case 9:
/* #line 74 "./cmd_parser.rl" */
	{ *cmd = cmd_config_session; }
	break;
	case 10:
/* #line 75 "./cmd_parser.rl" */
	{ *cmd = cmd_config_stack; }
	break;
	case 11:
/* #line 76 "./cmd_parser.rl" */
	{ *cmd = cmd_exit; }
	break;
	case 12:
/* #line 77 "./cmd_parser.rl" */
	{ *cmd = cmd_quit; }
	break;
	case 13:
/* #line 78 "./cmd_parser.rl" */
	{ *cmd = cmd_file; }
	break;
	case 14:
/* #line 79 "./cmd_parser.rl" */
	{ *cmd = cmd_hangup; }
	break;
	case 15:
/* #line 80 "./cmd_parser.rl" */
	{ *cmd = cmd_help; }
	break;
	case 16:
/* #line 81 "./cmd_parser.rl" */
	{ *cmd = cmd_message; }
	break;
	case 17:
/* #line 82 "./cmd_parser.rl" */
	{ *cmd = cmd_publish; }
	break;
	case 18:
/* #line 83 "./cmd_parser.rl" */
	{ *cmd = cmd_register; }
	break;
	case 19:
/* #line 84 "./cmd_parser.rl" */
	{ *cmd = cmd_run; }
	break;
	case 20:
/* #line 85 "./cmd_parser.rl" */
	{ *cmd = cmd_sms; }
	break;
	case 21:
/* #line 86 "./cmd_parser.rl" */
	{ *cmd = cmd_subscribe; }
	break;
	case 22:
/* #line 87 "./cmd_parser.rl" */
	{ *cmd = cmd_video; }
	break;
	case 23:
/* #line 91 "./cmd_parser.rl" */
	{ curr_opt->id = opt_amf; }
	break;
	case 24:
/* #line 92 "./cmd_parser.rl" */
	{ curr_opt->id = opt_caps; }
	break;
	case 25:
/* #line 93 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dhcpv4; }
	break;
	case 26:
/* #line 94 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dhcpv6; }
	break;
	case 27:
/* #line 95 "./cmd_parser.rl" */
	{ curr_opt->id = opt_amf; }
	break;
	case 28:
/* #line 96 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dname; }
	break;
	case 29:
/* #line 97 "./cmd_parser.rl" */
	{ curr_opt->id = opt_from; }
	break;
	case 30:
/* #line 98 "./cmd_parser.rl" */
	{ curr_opt->id = opt_header; }
	break;
	case 31:
/* #line 99 "./cmd_parser.rl" */
	{ curr_opt->id = opt_impi; }
	break;
	case 32:
/* #line 100 "./cmd_parser.rl" */
	{ curr_opt->id = opt_impu; }
	break;
	case 33:
/* #line 101 "./cmd_parser.rl" */
	{ curr_opt->id = opt_ipv6; }
	break;
	case 34:
/* #line 102 "./cmd_parser.rl" */
	{ curr_opt->id = opt_local_ip; }
	break;
	case 35:
/* #line 103 "./cmd_parser.rl" */
	{ curr_opt->id = opt_local_port; }
	break;
	case 36:
/* #line 104 "./cmd_parser.rl" */
	{ curr_opt->id = opt_opid; }
	break;
	case 37:
/* #line 105 "./cmd_parser.rl" */
	{ curr_opt->id = opt_password; }
	break;
	case 38:
/* #line 106 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_ip; }
	break;
	case 39:
/* #line 107 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_port; }
	break;
	case 40:
/* #line 108 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_trans; }
	break;
	case 41:
/* #line 109 "./cmd_parser.rl" */
	{ curr_opt->id = opt_realm; }
	break;
	case 42:
/* #line 110 "./cmd_parser.rl" */
	{ curr_opt->id = opt_sid; }
	break;
	case 43:
/* #line 111 "./cmd_parser.rl" */
	{ curr_opt->id = opt_sigcomp; }
	break;
	case 44:
/* #line 112 "./cmd_parser.rl" */
	{ curr_opt->id = opt_to; }
	break;
/* #line 884 "./cmd_parser.c" */
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
	const char *__acts = _demo_machine_parser_cmd_actions + _demo_machine_parser_cmd_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
/* #line 37 "./cmd_parser.rl" */
	{
		tag_start = p;
	}
	break;
	case 2:
/* #line 46 "./cmd_parser.rl" */
	{
		tsk_options_add_option_2(&options, curr_opt);
		TSK_OBJECT_SAFE_FREE(curr_opt);
	}
	break;
	case 3:
/* #line 51 "./cmd_parser.rl" */
	{
		if(curr_opt){
			TSK_PARSER_SET_STRING(curr_opt->value);
		}
	}
	break;
	case 4:
/* #line 57 "./cmd_parser.rl" */
	{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid option.", temp);
	}
	break;
	case 5:
/* #line 62 "./cmd_parser.rl" */
	{
		TSK_PARSER_SET_STRING(temp);
		TSK_DEBUG_ERROR("%s not a valid command.", temp);
	}
	break;
	case 6:
/* #line 71 "./cmd_parser.rl" */
	{ *cmd = cmd_audio; }
	break;
	case 7:
/* #line 72 "./cmd_parser.rl" */
	{ *cmd = cmd_audiovideo; }
	break;
	case 8:
/* #line 73 "./cmd_parser.rl" */
	{ *cmd = cmd_config_file; }
	break;
	case 9:
/* #line 74 "./cmd_parser.rl" */
	{ *cmd = cmd_config_session; }
	break;
	case 10:
/* #line 75 "./cmd_parser.rl" */
	{ *cmd = cmd_config_stack; }
	break;
	case 11:
/* #line 76 "./cmd_parser.rl" */
	{ *cmd = cmd_exit; }
	break;
	case 12:
/* #line 77 "./cmd_parser.rl" */
	{ *cmd = cmd_quit; }
	break;
	case 13:
/* #line 78 "./cmd_parser.rl" */
	{ *cmd = cmd_file; }
	break;
	case 14:
/* #line 79 "./cmd_parser.rl" */
	{ *cmd = cmd_hangup; }
	break;
	case 15:
/* #line 80 "./cmd_parser.rl" */
	{ *cmd = cmd_help; }
	break;
	case 16:
/* #line 81 "./cmd_parser.rl" */
	{ *cmd = cmd_message; }
	break;
	case 17:
/* #line 82 "./cmd_parser.rl" */
	{ *cmd = cmd_publish; }
	break;
	case 18:
/* #line 83 "./cmd_parser.rl" */
	{ *cmd = cmd_register; }
	break;
	case 19:
/* #line 84 "./cmd_parser.rl" */
	{ *cmd = cmd_run; }
	break;
	case 20:
/* #line 85 "./cmd_parser.rl" */
	{ *cmd = cmd_sms; }
	break;
	case 21:
/* #line 86 "./cmd_parser.rl" */
	{ *cmd = cmd_subscribe; }
	break;
	case 22:
/* #line 87 "./cmd_parser.rl" */
	{ *cmd = cmd_video; }
	break;
	case 23:
/* #line 91 "./cmd_parser.rl" */
	{ curr_opt->id = opt_amf; }
	break;
	case 24:
/* #line 92 "./cmd_parser.rl" */
	{ curr_opt->id = opt_caps; }
	break;
	case 25:
/* #line 93 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dhcpv4; }
	break;
	case 26:
/* #line 94 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dhcpv6; }
	break;
	case 27:
/* #line 95 "./cmd_parser.rl" */
	{ curr_opt->id = opt_amf; }
	break;
	case 28:
/* #line 96 "./cmd_parser.rl" */
	{ curr_opt->id = opt_dname; }
	break;
	case 29:
/* #line 97 "./cmd_parser.rl" */
	{ curr_opt->id = opt_from; }
	break;
	case 30:
/* #line 98 "./cmd_parser.rl" */
	{ curr_opt->id = opt_header; }
	break;
	case 31:
/* #line 99 "./cmd_parser.rl" */
	{ curr_opt->id = opt_impi; }
	break;
	case 32:
/* #line 100 "./cmd_parser.rl" */
	{ curr_opt->id = opt_impu; }
	break;
	case 33:
/* #line 101 "./cmd_parser.rl" */
	{ curr_opt->id = opt_ipv6; }
	break;
	case 34:
/* #line 102 "./cmd_parser.rl" */
	{ curr_opt->id = opt_local_ip; }
	break;
	case 35:
/* #line 103 "./cmd_parser.rl" */
	{ curr_opt->id = opt_local_port; }
	break;
	case 36:
/* #line 104 "./cmd_parser.rl" */
	{ curr_opt->id = opt_opid; }
	break;
	case 37:
/* #line 105 "./cmd_parser.rl" */
	{ curr_opt->id = opt_password; }
	break;
	case 38:
/* #line 106 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_ip; }
	break;
	case 39:
/* #line 107 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_port; }
	break;
	case 40:
/* #line 108 "./cmd_parser.rl" */
	{ curr_opt->id = opt_pcscf_trans; }
	break;
	case 41:
/* #line 109 "./cmd_parser.rl" */
	{ curr_opt->id = opt_realm; }
	break;
	case 42:
/* #line 110 "./cmd_parser.rl" */
	{ curr_opt->id = opt_sid; }
	break;
	case 43:
/* #line 111 "./cmd_parser.rl" */
	{ curr_opt->id = opt_sigcomp; }
	break;
	case 44:
/* #line 112 "./cmd_parser.rl" */
	{ curr_opt->id = opt_to; }
	break;
/* #line 1091 "./cmd_parser.c" */
		}
	}
	}

	_out: {}
	}

/* #line 146 "./cmd_parser.rl" */
	
	if( cs < 
/* #line 1102 "./cmd_parser.c" */
146
/* #line 147 "./cmd_parser.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse [%s] command-Line.", line);
		TSK_OBJECT_SAFE_FREE(options);
	}
	TSK_OBJECT_SAFE_FREE(curr_opt);
	TSK_FREE(temp);
	
	return options;
}

