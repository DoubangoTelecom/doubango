///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
//
///**@file tnet_stun_message.c
// * @brief STUN2 (RFC 5389) message parser.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_stun_message.h"
//
//#include "tnet_stun.h"
//
//#include "../tnet_types.h"
//#include "../tnet_endianness.h"
//#include "../turn/tnet_turn_attribute.h"
//#include "stun/tnet_stun_types.h"
//
//#include "tsk_memory.h"
//#include "tsk_hmac.h"
//#include "tsk_string.h"
//#include "tsk_ppfcs32.h"
//
//#include <string.h>
//
//static int __pred_find_attribute_by_type(const tsk_list_item_t *item, const void *att_type)
//{
//    if(item && item->data) {
//        tnet_stun_attr_t *att = item->data;
//        tnet_stun_attr_type_t type = *((tnet_stun_attr_type_t*)att_type);
//        return (att->type - type);
//    }
//    return -1;
//}
//
///**@ingroup tnet_stun_group
//* Creates new STUN message.
//* @retval @ref tnet_stun_pkt_t object.
//* @sa tnet_stun_message_create_null.
//*/
//
//tnet_stun_pkt_t* tnet_stun_message_create(const char* username, const char* password)
//{
//    return tsk_object_new(tnet_stun_message_def_t, username, password);
//}
//
///**@ingroup tnet_stun_group
//* Creates new STUN message.
//* @retval @ref tnet_stun_pkt_t object.
//* @sa tnet_stun_message_create.
//*/
//tnet_stun_pkt_t* tnet_stun_message_create_null()
//{
//    return tnet_stun_message_create(tsk_null, tsk_null);
//}
//
//#define SERIALIZE_N_ADD_ATTRIBUTE(att_name, payload, payload_size) \
//		attribute = (tnet_stun_attr_t *)tnet_stun_attribute_##att_name##_create(payload, payload_size); \
//		tnet_stun_attribute_serialize(attribute, output); \
//		tnet_stun_attribute_pad(attribute, output); \
//		TSK_OBJECT_SAFE_FREE(attribute);
//
///**@ingroup tnet_stun_group
// * Serializes a STUN message as binary data.
// * @param [in,out]	self	The STUN message to serialize.
// * @retval	A buffer holding the binary data (result) if serialization succeed and zero otherwise.
//**/
//tsk_buffer_t* tnet_stun_pkt_serialize(const tnet_stun_pkt_t *self)
//{
//    tsk_buffer_t *output = 0;
//    tnet_stun_attr_t *attribute;
//    unsigned compute_integrity = self->integrity;
//
//    if(!self) {
//        goto bail;
//    }
//
//    output = tsk_buffer_create_null();
//
//    /*	RFC 5389 - 6.  STUN Message Structure
//       0                   1                   2                   3
//       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//      |0 0|     STUN Message Type     |         Message Length        |
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//      |                         Magic Cookie                          |
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//      |                                                               |
//      |                     Transaction ID (96 bits)                  |
//      |                                                               |
//      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    */
//
//    /* STUN Message Type
//    */
//    {
//        uint16_t type = tnet_htons(self->type);
//        tsk_buffer_append(output, &(type), 2);
//    }
//
//    /* Message Length ==> Will be updated after attributes have been added. */
//    {
//        static const uint16_t length = 0;
//        tsk_buffer_append(output, &(length), 2);
//    }
//
//    /* Magic Cookie
//    */
//    {
//        uint32_t cookie = tnet_htonl(self->cookie);
//        tsk_buffer_append(output, &(cookie), 4);
//    }
//
//
//    /* Transaction ID (96 bits==>16bytes)
//    */
//    tsk_buffer_append(output, self->transac_id, TNET_STUN_TRANSACID_SIZE);
//
//    /* DONT-FRAGMENT
//    */
//    if(self->dontfrag) {
//        /*attribute = (tnet_stun_attr_t *)tnet_turn_attribute_dontfrag_create();
//        tnet_stun_attribute_serialize(attribute, output);
//        TSK_OBJECT_SAFE_FREE(attribute);*/
//    }
//
//    /* AUTHENTICATION */
//    if(self->realm && self->nonce) { // long term
//        SERIALIZE_N_ADD_ATTRIBUTE(realm, self->realm, tsk_strlen(self->realm));
//        SERIALIZE_N_ADD_ATTRIBUTE(nonce, self->nonce, tsk_strlen(self->nonce));
//
//        compute_integrity = !self->nointegrity;
//    }
//    else if(self->password) { // short term
//        compute_integrity = !self->nointegrity;
//    }
//
//    if(compute_integrity && self->username) {
//        SERIALIZE_N_ADD_ATTRIBUTE(username, self->username, tsk_strlen(self->username));
//    }
//
//    /*=== Attributes === */
//    {
//        tsk_list_item_t *item;
//        tsk_list_foreach(item, self->attributes) {
//            attribute = item->data;
//            tnet_stun_attribute_serialize(attribute, output);
//            tnet_stun_attribute_pad(attribute, output);
//        }
//    }
//
//    /* Message Length: The message length MUST contain the size, in bytes, of the message
//    					not including the 20-byte STUN header.
//    */
//    {
//        // compute length for 'MESSAGE-INTEGRITY'
//        // will be computed again to store the correct value
//        uint16_t length = (output->size) - kStunAttrHdrSizeInOctets;
//#if 0
//        if(self->fingerprint) {
//            length += (2/* Type */ + 2 /* Length */+ 4 /* FINGERPRINT VALUE*/);
//        }
//#endif
//
//        if(compute_integrity) {
//            length += (2/* Type */ + 2 /* Length */+ TSK_SHA1_DIGEST_SIZE /* INTEGRITY VALUE*/);
//        }
//
//        *(((uint16_t*)output->data)+1)  = tnet_htons(length);
//    }
//
//    /* MESSAGE-INTEGRITY */
//    if(compute_integrity) {
//        /* RFC 5389 - 15.4.  MESSAGE-INTEGRITY
//           The MESSAGE-INTEGRITY attribute contains an HMAC-SHA1 [RFC2104] of the STUN message.
//
//           For long-term credentials ==> key = MD5(username ":" realm ":" SASLprep(password))
//           For short-term credentials ==> key = SASLprep(password)
//        */
//
//        tsk_sha1digest_t hmac;
//
//        if(self->username && self->realm && self->password) { // long term
//            char* keystr = tsk_null;
//            tsk_md5digest_t md5;
//            tsk_sprintf(&keystr, "%s:%s:%s", self->username, self->realm, self->password);
//            TSK_MD5_DIGEST_CALC(keystr, tsk_strlen(keystr), md5);
//            hmac_sha1digest_compute(output->data, output->size, (const char*)md5, TSK_MD5_DIGEST_SIZE, hmac);
//
//            TSK_FREE(keystr);
//        }
//        else { // short term
//            hmac_sha1digest_compute(output->data, output->size, self->password, tsk_strlen(self->password), hmac);
//        }
//
//        SERIALIZE_N_ADD_ATTRIBUTE(integrity, hmac, TSK_SHA1_DIGEST_SIZE);
//    }
//
//    // LENGTH
//    *(((uint16_t*)output->data) + 1)  = tnet_htons((output->size - kStunAttrHdrSizeInOctets + (self->fingerprint ? 8 : 0)));
//
//    /* FINGERPRINT */
//    if(self->fingerprint) { //JINGLE_ICE
//        /*	RFC 5389 - 15.5.  FINGERPRINT
//        	The FINGERPRINT attribute MAY be present in all STUN messages.  The
//        	value of the attribute is computed as the CRC-32 of the STUN message
//        	up to (but excluding) the FINGERPRINT attribute itself, XOR'ed with
//        	the 32-bit value 0x5354554e
//        */
//        uint32_t fingerprint = tsk_pppfcs32(TSK_PPPINITFCS32, output->data, output->size);
//        fingerprint ^= 0x5354554e;
//        fingerprint = tnet_htonl(fingerprint);
//
//        attribute = (tnet_stun_attr_t *)tnet_stun_attribute_fingerprint_create(fingerprint);
//        tnet_stun_attribute_serialize(attribute, output);
//        TSK_OBJECT_SAFE_FREE(attribute);
//    }
//
//bail:
//    return output;
//}
//
//
///**@ingroup tnet_stun_group
// *
// * Deserializes a STUN message from binary data.
// *
// * @param [in,out]	data	A pointer to the binary data.
// * @param	size			The size of the binary data.
// *
// * @retval	A STUN message if deserialization succeed or NULL otherwise.
//**/
//tnet_stun_pkt_t* tnet_stun_message_deserialize(const uint8_t *data, tsk_size_t size)
//{
//    tnet_stun_pkt_t *message = 0;
//    uint8_t* dataPtr, *dataEnd;
//
//
//    if(!data || (size  < kStunAttrHdrSizeInOctets) || !TNET_STUN_BUFF_IS_STUN2(data, size)) {
//        goto bail;
//    }
//
//    dataPtr = (uint8_t*)data;
//    dataEnd = (dataPtr + size);
//
//    message = tnet_stun_message_create_null();
//
//    /* Message Type
//    */
//    message->type = (tnet_stun_pkt_type_t)tnet_ntohs_2(dataPtr);
//    dataPtr += 2;
//
//    /* Message Length
//    */
//    message->length = tnet_ntohs_2(dataPtr);
//    dataPtr += 2;
//
//    /* Check message validity
//    */
//    if((message->length + kStunAttrHdrSizeInOctets) != size) {
//        TSK_OBJECT_SAFE_FREE(message);
//        goto bail;
//    }
//
//    /* Magic Cookie
//    	==> already set by the constructor and checked by @ref TNET_IS_STUN2
//    */
//    dataPtr += 4;
//
//    /* Transaction ID
//    */
//    memcpy(message->transac_id, dataPtr, TNET_STUN_TRANSACID_SIZE);
//    dataPtr += TNET_STUN_TRANSACID_SIZE;
//
//    /*	== Parse attributes
//    */
//    while(dataPtr < dataEnd) {
//        tnet_stun_attr_t *attribute = tnet_stun_attribute_deserialize(dataPtr, (dataEnd - dataPtr));
//        if(attribute) {
//            tsk_size_t att_size = (attribute->length + 2 /* Type*/ + 2/* Length */);
//            att_size += (att_size & 0x03) ? 4-(att_size & 0x03) : 0; // Skip zero bytes used to pad the attribute.
//
//            dataPtr += att_size;
//            tsk_list_push_back_data(message->attributes, (void**)&attribute);
//
//            continue;
//        }
//        else {
//            continue;
//        }
//    }
//
//bail:
//    return message;
//}
//
///**@ingroup tnet_stun_group
//*/
//tsk_bool_t tnet_stun_message_has_attribute(const tnet_stun_pkt_t *self, tnet_stun_attr_type_t type)
//{
//    return (tnet_stun_message_get_attribute(self, type) != tsk_null);
//}
//
///**@ingroup tnet_stun_group
//* Adds an attribute to a STUN message.
//* @param self The STUN message into which to add the attribute.
//* @param attribute The attribute to add.
//* @retval Zero if succeed and non-zero error code otherwise.
//*/
//int tnet_stun_message_add_attribute(tnet_stun_pkt_t *self, tnet_stun_attr_t** attribute)
//{
//    if(self && attribute && *attribute) {
//        tsk_list_push_back_data(self->attributes, (void**)attribute);
//        return 0;
//    }
//    return -1;
//}
//
///**@ingroup tnet_stun_group
//*/
//int tnet_stun_message_remove_attribute(tnet_stun_pkt_t *self, tnet_stun_attr_type_t type)
//{
//    if(self && self->attributes) {
//        tsk_list_remove_item_by_pred(self->attributes, __pred_find_attribute_by_type, &type);
//    }
//    return 0;
//}
//
//
///**@ingroup tnet_stun_group
//* Gets a STUN attribute from a message.
//* @param self The message from which to get the attribute.
//* @param type The type of the attribute to retrieve.
//* @retval @ref tnet_stun_attr_t object if found and NULL otherwise.
//*/
//const tnet_stun_attr_t* tnet_stun_message_get_attribute(const tnet_stun_pkt_t *self, tnet_stun_attr_type_t type)
//{
//    tnet_stun_attr_t* attribute;
//
//    if(self && !TSK_LIST_IS_EMPTY(self->attributes)) {
//        tsk_list_item_t *item;
//        tsk_list_foreach(item, self->attributes) {
//            if((attribute = item->data) && attribute->type == type) {
//                return attribute;
//            }
//        }
//    }
//    return 0;
//}
//
///**@ingroup tnet_stun_group
//* Gets the STUN error-code attribute value from the message.
//* @param self The STUN message from which to get the error code.
//* @retval The error code if the message contain such attribute or -1 otherwise.
//*/
//short tnet_stun_message_get_errorcode(const tnet_stun_pkt_t *self)
//{
//    const tnet_stun_attribute_errorcode_t* error = (const tnet_stun_attribute_errorcode_t*)tnet_stun_message_get_attribute(self, stun_error_code);
//    if(error) {
//        return  ((error->_class*100) + error->number);
//    }
//    return -1;
//}
//
///**@ingroup tnet_stun_group
//* Gets the STUN @b realm attribute value from the message.
//* @param self The STUN message from which to get the @b realm.
//* @retval The @b realm as a string pointer code if the message contain such attribute or NULL otherwise.
//*/
//const char* tnet_stun_message_get_realm(const tnet_stun_pkt_t *self)
//{
//    const tnet_stun_attribute_realm_t* realm = (const tnet_stun_attribute_realm_t*)tnet_stun_message_get_attribute(self, stun_realm);
//    if(realm) {
//        return realm->value;
//    }
//    return 0;
//}
//
///**@ingroup tnet_stun_group
//* Gets the STUN @b nonce attribute value from the message.
//* @param self The STUN message from which to get the @b nonce.
//* @retval The @b nonce as a string pointer code if the message contain such attribute or NULL otherwise.
//*/
//const char* tnet_stun_message_get_nonce(const tnet_stun_pkt_t *self)
//{
//    const tnet_stun_attribute_nonce_t* nonce = (const tnet_stun_attribute_nonce_t*)tnet_stun_message_get_attribute(self, stun_nonce);
//    if(nonce) {
//        return nonce->value;
//    }
//    return 0;
//}
//
///**@ingroup tnet_stun_group
//* Gets the STUN @b lifetime attribute value from the message.
//* @param self The STUN message from which to get the @b lifetime.
//* @retval The @b lifetime (any positive value) if the message contain such attribute or -1 otherwise.
//*/
//int32_t tnet_stun_message_get_lifetime(const tnet_stun_pkt_t *self)
//{
//    /*const tnet_turn_attribute_lifetime_t* lifetime = (const tnet_turn_attribute_lifetime_t*)tnet_stun_message_get_attribute(self, stun_lifetime);
//    if(lifetime) {
//        return lifetime->value;
//    }*/
//    return -1;
//}
//
///**@ingroup tnet_stun_group
//*/
//tsk_bool_t tnet_stun_utils_transac_id_equals(const tnet_stun_transac_id_t id1, const tnet_stun_transac_id_t id2)
//{
//    tsk_size_t i;
//    static const tsk_size_t size = sizeof(tnet_stun_transac_id_t);
//    for(i = 0; i < size; i++) {
//        if(id1[i] != id2[i]) {
//            return tsk_false;
//        }
//    }
//    return tsk_true;
//}
//
//
//
//
//
//
////=================================================================================================
////	STUN2 MESSAGE object definition
////
//static tsk_object_t* tnet_stun_message_ctor(tsk_object_t * self, va_list * app)
//{
//    tnet_stun_pkt_t *message = self;
//    if(message) {
//        message->username = tsk_strdup(va_arg(*app, const char*));
//        message->password = tsk_strdup(va_arg(*app, const char*));
//
//        message->cookie = kStunMagicCookieLong;
//        message->attributes = tsk_list_create();
//
//        message->fingerprint = 1;
//        message->integrity = 0;
//
//        {
//            // Create random transaction id
//            int i;
//            for(i = 0; i < sizeof(message->transac_id)/sizeof(message->transac_id[0]); ++i) {
//                message->transac_id[i] = rand();
//            }
//        }
//    }
//    return self;
//}
//
//static tsk_object_t* tnet_stun_message_dtor(tsk_object_t * self)
//{
//    tnet_stun_pkt_t *message = self;
//    if(message) {
//        TSK_FREE(message->username);
//        TSK_FREE(message->password);
//        TSK_FREE(message->realm);
//        TSK_FREE(message->nonce);
//
//        TSK_OBJECT_SAFE_FREE(message->attributes);
//    }
//
//    return self;
//}
//
//static const tsk_object_def_t tnet_stun_message_def_s = {
//    sizeof(tnet_stun_pkt_t),
//    tnet_stun_message_ctor,
//    tnet_stun_message_dtor,
//    tsk_null,
//};
//const tsk_object_def_t *tnet_stun_message_def_t = &tnet_stun_message_def_s;
//
