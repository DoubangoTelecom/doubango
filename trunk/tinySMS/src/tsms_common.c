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

/**@file tsms_common.c
 * @brief SMS Commons (3GPP TS 23.038 and al.).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySMS/tsms_common.h"

#include "tinySMS/tsms_packing.h"

#include "tsk_object.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h> /* strlen() */

extern tsms_tpdu_message_t* _tsms_tpdu_submit_deserialize(const void* data, size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_deliver_deserialize(const void* data, size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_command_deserialize(const void* data, size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_report_deserialize(const void* data, size_t size);
extern tsms_tpdu_message_t* _tsms_tpdu_status_report_deserialize(const void* data, size_t size);

extern int _tsms_tpdu_submit_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_deliver_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_command_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_report_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);
extern int _tsms_tpdu_status_report_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output);

int tsms_tpdu_message_init(tsms_tpdu_message_t* self, tsms_tpdu_mti_t mti)
{
	if(self){
		self->mti = mti;
		self->pid = TSMS_TPDU_DEFAULT_PID;
		self->dcs = TSMS_TPDU_DEFAULT_DCS;
		return 0;
	}
	return -1;
}

/**
*/
int tsms_tpdu_message_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output, tsk_bool_t MobOrig)
{
	if(!self){
		return -1;
	}

	/* 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI)
	bit1	bit0	Message type
		0	0	SMS-DELIVER (in the direction SC to MS)	
		0	0	SMS-DELIVER-REPORT (in the direction MS to SC)	
		1	0	SMS-STATUS-REPORT (in the direction SC to MS)		
		1	0	SMS-COMMAND (in the direction MS to SC)	
		0	1	SMS-SUBMIT (in the direction MS to SC)	
		0	1	SMS-SUBMIT-REPORT (in the direction SC to MS)	
		1	1	Reserved
	*/
	if(MobOrig){ /* MO */
		switch(self->mti){
			case tsms_tpdu_mti_deliver_report_mo: return _tsms_tpdu_report_serialize(self, output); /* SMS-DELIVER-REPORT */
			case tsms_tpdu_mti_command_mo: return _tsms_tpdu_command_serialize(self, output); /* SMS-COMMAND */
			case tsms_tpdu_mti_submit_mo: return _tsms_tpdu_submit_serialize(self, output); /* SMS-SUBMIT */
			default: return -2;
		}
	}
	else{ /* MT */
		switch(self->mti){
			case tsms_tpdu_mti_deliver_mt: return _tsms_tpdu_deliver_serialize(self, output); /* SMS-DELIVER */
			case tsms_tpdu_mti_status_report_mt: return _tsms_tpdu_status_report_serialize(self, output); /* SMS-STATUS-REPORT */
			case tsms_tpdu_mti_submit_report_mt: return _tsms_tpdu_report_serialize(self, output); /* SMS-SUBMIT-REPORT */
			default: return -2;
		}
	}
}

tsms_tpdu_message_t* tsms_tpdu_message_deserialize(const void* data, size_t size,  tsk_bool_t MobOrig)
{
	tsms_tpdu_mti_t mti;
	uint8_t smsc_len = 0;
	
	if(!data || size<=1){
		TSK_DEBUG_ERROR("Invalid parameter.");
		return tsk_null;
	}

#if TSMS_TPDU_APPEND_SMSC
	smsc_len = *((uint8_t*)data) + 1/* len itself*/;
	if(smsc_len>=size){
		TSK_DEBUG_ERROR("Too short.");
		return tsk_null;
	}
#endif
	
	mti = (*(((uint8_t*)data)+smsc_len) & 0x03);

	/* 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI)
	bit1	bit0	Message type
		0	0	SMS-DELIVER (in the direction SC to MS)	
		0	0	SMS-DELIVER-REPORT (in the direction MS to SC)	
		1	0	SMS-STATUS-REPORT (in the direction SC to MS)		
		1	0	SMS-COMMAND (in the direction MS to SC)	
		0	1	SMS-SUBMIT (in the direction MS to SC)	
		0	1	SMS-SUBMIT-REPORT (in the direction SC to MS)	
		1	1	Reserved
	*/
	if(MobOrig){ /* MO */
		switch(mti){
			case tsms_tpdu_mti_deliver_report_mo: return _tsms_tpdu_report_deserialize(data, size); /* SMS-DELIVER-REPORT */
			case tsms_tpdu_mti_command_mo: return _tsms_tpdu_command_deserialize(data, size); /* SMS-COMMAND */
			case tsms_tpdu_mti_submit_mo: return _tsms_tpdu_submit_deserialize(data, size); /* SMS-SUBMIT */
			default: return tsk_null;
		}
	}
	else{ /* MT */
		switch(mti){
			case tsms_tpdu_mti_deliver_mt: return _tsms_tpdu_deliver_deserialize(data, size); /* SMS-DELIVER */
			case tsms_tpdu_mti_status_report_mt: return _tsms_tpdu_status_report_deserialize(data, size); /* SMS-STATUS-REPORT */
			case tsms_tpdu_mti_submit_report_mt: return _tsms_tpdu_report_deserialize(data, size); /* SMS-SUBMIT-REPORT */
			default: return tsk_null;
		}
	}
}

char* tsms_tpdu_message_tostring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig)
{
	char* ret = tsk_null;
	tsk_buffer_t* output = tsk_null;
	if(!self){
		goto bail;
	}

	if((output = TSK_BUFFER_CREATE_NULL())){
		if(!tsms_tpdu_message_serialize(self, output, MobOrig)){
			ret = tsk_strndup(output->data, output->size);
		}
		TSK_OBJECT_SAFE_FREE(output);
	}

bail:
	return ret;
}

char* tsms_tpdu_message_tohexastring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig)
{
	char* ret = tsk_null;
	tsk_buffer_t* output = tsk_null;
	if(!self){
		goto bail;
	}

	if((output = TSK_BUFFER_CREATE_NULL())){
		if(!tsms_tpdu_message_serialize(self, output, MobOrig)){
			size_t i;
			for(i=0;i<output->size;i++){
				tsk_strcat_2(&ret, "%.2X", *(TSK_BUFFER_TO_U8(output)+i));
			}
		}
		TSK_OBJECT_SAFE_FREE(output);
	}

bail:
	return ret;
}

int tsms_tpdu_message_set_userdata(tsms_tpdu_message_t* self, const tsk_buffer_t* udata, tsms_alphabet_t alpha)
{
	if(!self || !udata){
		return -1;
	}
	/* SMS alphabet values as per 3GPP TS 23.038 v911 section 4. 
	* Part of TP-DCS (SMS Data Coding Scheme).
	*/
	self->dcs = ((self->dcs & 0xF3) | (alpha << 2)); /* Bit3 and Bit2 */

	TSK_OBJECT_SAFE_FREE(self->ud);

	/* 3GPP TS 23.040 ==> 9.2.3.16 TP-User-Data-Length (TP-UDL)
	* (alpha = SMS_ALPHA_7bit) ==> number of septets.
	* ((alpha == SMS_ALPHA_8bit) || (alpha == SMS_ALPHA_UCS2)) ==> number of octes.
	*/

	/* 3GPP TS 23.040 ==> 9.2.3.24 TP-User Data (TP-UD) */
	switch(alpha){
		case tsms_alpha_7bit: {
				self->udl = (udata->size) + (udata->size/7);
				self->ud = TSK_BUFFER_CREATE(udata->data, udata->size);
			}
			break;
		case tsms_alpha_8bit:
		case tsms_alpha_ucs2: {
				self->udl = udata->size;
				self->ud = TSK_BUFFER_CREATE(udata->data, udata->size);
			}
			break;

	default:
		{
			TSK_DEBUG_ERROR("Invalid Alphabet.");
			return -2;
		}
	}
	return 0;
}

int tsms_tpdu_message_deinit(tsms_tpdu_message_t* self)
{
	if(self){
		TSK_OBJECT_SAFE_FREE(self->ud);
		return 0;
	}
	return -1;
}
