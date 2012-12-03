#include "tnet_ice_pair.h"
#include "tnet_ice_utils.h"
#include "tnet_ice_candidate.h"

#include "stun/tnet_stun.h"
#include "stun/tnet_stun_message.h"

#include "tnet_endianness.h"
#include "tnet_utils.h"

#include "tsk_hmac.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <string.h>

static int __pred_find_by_pair(const tsk_list_item_t *item, const void *pair)
{
	if(item && item->data){
		const tnet_ice_pair_t *_pair = item->data;
		return (_pair - ((const tnet_ice_pair_t *)pair));
	}
	return -1;
}


static tsk_object_t* tnet_ice_pair_ctor(tsk_object_t * self, va_list * app)
{
	tnet_ice_pair_t *pair = self;
	if(pair){
		pair->state_offer = tnet_ice_pair_state_frozen;
		pair->state_answer = tnet_ice_pair_state_frozen;
	}
	return self;
}
static tsk_object_t* tnet_ice_pair_dtor(tsk_object_t * self)
{ 
	tnet_ice_pair_t *pair = self;
	if(pair){
		TSK_OBJECT_SAFE_FREE(pair->candidate_offer);
		TSK_OBJECT_SAFE_FREE(pair->candidate_answer);
		TSK_OBJECT_SAFE_FREE(pair->last_request);
	}
	return self;
}
static int tnet_ice_pair_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
{
	const tnet_ice_pair_t *p1 = _p1;
	const tnet_ice_pair_t *p2 = _p2;

	if(p1 && p2){
		return (int)(p1->priority - p2->priority);
	}
	else if(!p1 && !p2) return 0;
	else return -1;
}
static const tsk_object_def_t tnet_ice_pair_def_s = 
{
	sizeof(tnet_ice_pair_t),
	tnet_ice_pair_ctor, 
	tnet_ice_pair_dtor,
	tnet_ice_pair_cmp, 
};

tnet_ice_pair_t* tnet_ice_pair_create(const tnet_ice_candidate_t* candidate_offer, const tnet_ice_candidate_t* candidate_answer, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle)
{
	tnet_ice_pair_t *pair;
	if(!candidate_offer || !candidate_answer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	if((pair = tsk_object_new(&tnet_ice_pair_def_s))){
		pair->candidate_offer = tsk_object_ref((tsk_object_t*)candidate_offer);
		pair->candidate_answer = tsk_object_ref((tsk_object_t*)candidate_answer);
		pair->is_controlling = is_controlling;
		pair->tie_breaker = tie_breaker;
		pair->is_ice_jingle = is_ice_jingle;
		pair->priority = (((uint64_t)TSK_MIN(candidate_offer->priority, candidate_answer->priority)) << 32) +
			(TSK_MAX(candidate_offer->priority, candidate_answer->priority) << 1) +
			((candidate_offer->priority > candidate_answer->priority) ? 1 : 0);
	}

	return pair;
}

int tnet_ice_pair_send_conncheck(tnet_ice_pair_t *self)
{
	char* username = tsk_null;
	const char* password;
	uint32_t priority;
	struct sockaddr_storage remote_addr;
	tnet_stun_attribute_t *stun_att;
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}		
	
	if(ret = tnet_sockaddr_init(self->candidate_answer->connection_addr, self->candidate_answer->port, self->candidate_offer->socket->type, &remote_addr)){
		TNET_PRINT_LAST_ERROR("tnet_sockaddr_init(%s:%d) failed", self->candidate_answer->connection_addr, self->candidate_answer->port);
		goto bail;
	}

	// 7.1.2.3. Forming Credentials
	// username=  "RFRAG:LFRAG"
	// password= "RPASS"
	if(self->is_ice_jingle){
		tsk_sprintf(&username, "%s%s", tnet_ice_candidate_get_ufrag(self->candidate_answer), tnet_ice_candidate_get_ufrag(self->candidate_offer));
	}
	else{
		tsk_sprintf(&username, "%s:%s", tnet_ice_candidate_get_ufrag(self->candidate_answer), tnet_ice_candidate_get_ufrag(self->candidate_offer));
	}
	password = tnet_ice_candidate_get_pwd(self->candidate_answer);
	
	if(!self->last_request && (self->last_request = tnet_stun_message_create(username, password))){
		self->last_request->type = stun_binding_request;
		
		// 7.1.2.1. PRIORITY
		priority = tnet_ice_utils_get_priority(tnet_ice_cand_type_prflx, self->candidate_offer->local_pref, self->candidate_offer->is_rtp);
		if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_priority_create(priority))){
			tnet_stun_message_add_attribute(self->last_request, &stun_att);
		}
		// 7.1.2.1. USE-CANDIDATE
		if(self->is_controlling){ // aggressive mode
			if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_use_candidate_create())){
				tnet_stun_message_add_attribute(self->last_request, &stun_att);
			}
		}
		// 7.1.2.2. ICE-CONTROLLING / ICE-CONTROLLED
		if(self->is_controlling){
			if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_controlling_create(self->tie_breaker))){
				tnet_stun_message_add_attribute(self->last_request, &stun_att);
			}
		}
		else{
			if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_controlled_create(self->tie_breaker))){
				tnet_stun_message_add_attribute(self->last_request, &stun_att);
			}
		}

		// SOFTWARE
		if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_software_create(TNET_SOFTWARE, tsk_strlen(TNET_SOFTWARE)))){
			tnet_stun_message_add_attribute(self->last_request, &stun_att);
		}
	}
	else if(self->last_request){
		if(self->is_controlling){
			tnet_stun_message_remove_attribute(self->last_request, stun_ice_controlled);
			if(!tnet_stun_message_has_attribute(self->last_request, stun_ice_controlling)){
				if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_controlling_create(self->tie_breaker))){
					tnet_stun_message_add_attribute(self->last_request, &stun_att);
				}
			}
			if(!tnet_stun_message_has_attribute(self->last_request, stun_ice_use_candidate)){
				if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_use_candidate_create())){
					tnet_stun_message_add_attribute(self->last_request, &stun_att);
				}
			}
		}
		else{
			tnet_stun_message_remove_attribute(self->last_request, stun_ice_use_candidate);
			tnet_stun_message_remove_attribute(self->last_request, stun_ice_controlling);
			if(!tnet_stun_message_has_attribute(self->last_request, stun_ice_controlled)){
				if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_ice_controlled_create(self->tie_breaker))){
					tnet_stun_message_add_attribute(self->last_request, &stun_att);
				}
			}
		}
	}

	if(self->last_request){
		tsk_buffer_t *req_buffer;
		self->last_request->fingerprint = !self->is_ice_jingle;
		if((req_buffer = tnet_stun_message_serialize(self->last_request))){
			/*int sendBytes =*/ tnet_sockfd_sendto(self->candidate_offer->socket->fd, (const struct sockaddr*)&remote_addr, req_buffer->data, req_buffer->size);
			TSK_OBJECT_SAFE_FREE(req_buffer);
		}
	}

bail:
	TSK_FREE(username);

	if(ret == 0 && self->state_offer == tnet_ice_pair_state_frozen){
		self->state_offer = tnet_ice_pair_state_in_progress;
	}

	return ret;
}

int tnet_ice_pair_send_response(tnet_ice_pair_t *self, const tnet_stun_request_t* request, const short code, const char* phrase, const struct sockaddr_storage *remote_addr)
{
	tnet_stun_message_t* message;
	const char *password, *username;
	int ret = -1;
	tnet_stun_attribute_t *stun_att = tsk_null;
	struct sockaddr_storage dest_addr;
	tsk_bool_t is_error = ((code / 100) != 2);

	if(!self || !phrase || !request || !self->candidate_offer || !self->candidate_answer){
		TSK_DEBUG_ERROR("Invalid paramter");
		return -1;
	}

	if(ret = tnet_sockaddr_init(self->candidate_answer->connection_addr, self->candidate_answer->port, self->candidate_offer->socket->type, &dest_addr)){
		TNET_PRINT_LAST_ERROR("tnet_sockaddr_init(%s:%d) failed", self->candidate_answer->connection_addr, self->candidate_answer->port);
		return ret;
	}

	username = tsk_null;
	password = tnet_ice_candidate_get_pwd(self->candidate_offer);

	if((message = tnet_stun_message_create(username, password))){
		tsk_buffer_t *req_buffer;
		
		message->type = is_error ? stun_binding_error_response : stun_binding_success_response;
		memcpy(message->transaction_id, request->transaction_id, sizeof(request->transaction_id));
		message->nointegrity = self->is_ice_jingle;
		message->fingerprint = !self->is_ice_jingle;

		// ERROR
		if(is_error){
			tnet_stun_attribute_errorcode_t* stun_att_err;
			if((stun_att_err = tnet_stun_attribute_errorcode_create(tsk_null, 0))){
				stun_att_err->_class = ((code / 100) & 0x07);
				stun_att_err->number = (code - ((code / 100) * 100));
				stun_att_err->reason_phrase = tsk_strdup(phrase);
				TNET_STUN_ATTRIBUTE(stun_att_err)->length = 4 + tsk_strlen(phrase);
				tnet_stun_message_add_attribute(message, (tnet_stun_attribute_t**)&stun_att_err);
			}
		}
		else{
			tnet_stun_attribute_xmapped_addr_t* stun_att_xma;
			if(self->is_ice_jingle){
				const tnet_stun_attribute_username_t * stun_att_usr_name;
				tnet_stun_attribute_mapped_addr_t* stun_att_ma;
				
				// USERNAME
				if((stun_att_usr_name = (const tnet_stun_attribute_username_t *)tnet_stun_message_get_attribute(request, stun_username))){
					stun_att_usr_name = tsk_object_ref((tsk_object_t*)stun_att_usr_name);
					tnet_stun_message_add_attribute(message, (tnet_stun_attribute_t**)&stun_att_usr_name);
				}
				
				// MAPPED-ADDRESS
				if((stun_att_ma = tnet_stun_attribute_mapped_address_create(tsk_null, 0))){
					uint32_t addr;
					if(TNET_SOCKET_TYPE_IS_IPV6(self->candidate_offer->socket->type)){
						tsk_size_t i;
						const struct sockaddr_in6* sin6 = (const struct sockaddr_in6*)remote_addr;
						stun_att_ma->family = stun_ipv6;
						stun_att_ma->port = tnet_ntohs(sin6->sin6_port);
						for(i = 0; i < 16; i+=4){
							addr = tnet_ntohl_2(&sin6->sin6_addr.s6_addr[i]);
							memcpy(&stun_att_ma->address[i], &addr, 4);
						}
						TNET_STUN_ATTRIBUTE(stun_att_ma)->length = 4 + 16;
					}
					else{
						const struct sockaddr_in* sin = (const struct sockaddr_in*)remote_addr;
						stun_att_ma->family = stun_ipv4;
						stun_att_ma->port = tnet_ntohs(sin->sin_port);
						addr = tnet_ntohl(sin->sin_addr.s_addr);
						memcpy(&stun_att_ma->address[0], &addr, 4);
						TNET_STUN_ATTRIBUTE(stun_att_ma)->length = 4 + 4;
					}
					tnet_stun_message_add_attribute(message, (tnet_stun_attribute_t**)&stun_att_ma);
				}
			}
			
			// XOR-MAPPED-ADDRESS
			if((stun_att_xma = tnet_stun_attribute_xmapped_address_create(tsk_null, 0))){//JINGLE_ICE
				tsk_size_t addr_size;
				tsk_size_t i;
				uint32_t addr;
				if(TNET_SOCKET_TYPE_IS_IPV6(self->candidate_offer->socket->type)){
					addr_size = 16;
					stun_att_xma->family = stun_ipv6;
				}
				else{
					addr_size = 4;
					stun_att_xma->family = stun_ipv4;
				}
				
				if(stun_att_xma->family == stun_ipv4){
					const struct sockaddr_in* sin = (const struct sockaddr_in*)remote_addr;
					stun_att_xma->xport = tnet_ntohs(sin->sin_port) ^ 0x2112;
					addr = (tnet_ntohl(sin->sin_addr.s_addr) ^ TNET_STUN_MAGIC_COOKIE);
					memcpy(&stun_att_xma->xaddress[0], &addr, 4);
				}
				else{
					const struct sockaddr_in6* sin6 = (const struct sockaddr_in6*)remote_addr;
					stun_att_xma->xport = tnet_ntohs(sin6->sin6_port) ^ 0x2112;
					for(i = 0; i < addr_size; i+=4){
						addr = (tnet_ntohl_2(&sin6->sin6_addr.s6_addr[i]) ^ TNET_STUN_MAGIC_COOKIE);
						memcpy(&stun_att_xma->xaddress[i], &addr, 4);
					}
				}

				TNET_STUN_ATTRIBUTE(stun_att_xma)->length = 4 + addr_size;
				tnet_stun_message_add_attribute(message, (tnet_stun_attribute_t**)&stun_att_xma);				
			}

			// SOFTWARE
			if((stun_att = (tnet_stun_attribute_t*)tnet_stun_attribute_software_create(TNET_SOFTWARE, tsk_strlen(TNET_SOFTWARE)))){
				tnet_stun_message_add_attribute(message, &stun_att);
			}
		}

		if((req_buffer = tnet_stun_message_serialize(message))){
			int sendBytes = tnet_sockfd_sendto(self->candidate_offer->socket->fd, (const struct sockaddr*)&dest_addr, req_buffer->data, req_buffer->size);
			TSK_OBJECT_SAFE_FREE(req_buffer);
			ret = (sendBytes > 0) ? 0 : -2;
		}

		TSK_OBJECT_SAFE_FREE(message);
	}
	
	if(ret == 0 && !is_error){
		tsk_bool_t change_state = 
			self->is_ice_jingle || // ICE-JINGLE don't have ICE-CONTROLLING/ICE-CONTROLLED attributes
			(!self->is_controlling && tnet_stun_message_has_attribute(request, stun_ice_use_candidate)) || // Sender is controlling and uses "ICE-USE-CANDIDATE" attribute
			(self->is_controlling) // We always use agressive nomination and final check-list will have high-priority pairs on the top
			;
		if(change_state){
			self->state_answer = tnet_ice_pair_state_succeed;
		}
	}

	return ret;
}

int tnet_ice_pair_auth_conncheck(const tnet_ice_pair_t *self, const tnet_stun_request_t* request, const void* request_buff, tsk_size_t request_buff_size, short* resp_code, char** resp_phrase)
{
	const uint8_t* _request_buff = (const uint8_t*)request_buff;
	
	const tnet_stun_attribute_t* stun_att;
	const tnet_stun_attribute_username_t *stun_att_usr_name;
	const tnet_stun_attribute_fingerprint_t *stun_att_fingerprint;
	const tnet_stun_attribute_integrity_t *stun_att_integrity;
	
	const tsk_list_item_t *item;
	tsk_sha1digest_t hmac;
	const char* pwd;

	tsk_size_t msg_integrity_start = 0, length, i;

	if(!self || !request || !request_buff || !request_buff_size || !resp_code || !resp_phrase){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!TNET_IS_STUN2_MSG(_request_buff, request_buff_size)){
		TSK_DEBUG_ERROR("Not STUN buffer");
		return -1;
	}

	pwd = tnet_ice_candidate_get_pwd(self->candidate_offer);
	stun_att_usr_name = tsk_null;
	stun_att_fingerprint = tsk_null;
	stun_att_integrity = tsk_null;

	tsk_list_foreach(item, request->attributes){
		if((!(stun_att = item->data))){
			continue;
		}
		
		switch(stun_att->type){
			case stun_username:
				{
					stun_att_usr_name = (const tnet_stun_attribute_username_t *)stun_att;
					break;
				}
			case stun_fingerprint:
				{
					stun_att_fingerprint = (const tnet_stun_attribute_fingerprint_t *)stun_att;
					break;
				}
			case stun_message_integrity:
				{
					stun_att_integrity = (const tnet_stun_attribute_integrity_t *)stun_att;
					break;
				}
		}
		
		if(!stun_att_integrity){
			if((length = (4 + stun_att->length)) & 0x03){
				length += (4 - (length & 0x03));
			}
			msg_integrity_start += length;
		}
	}

	if(!stun_att_usr_name){
		TSK_DEBUG_ERROR("USERNAME is missing");
		*resp_code = 400;
		tsk_strupdate(resp_phrase, "USERNAME is missing");
		return -2;
	}

	if(!stun_att_integrity){
		if(self->is_ice_jingle){ // Bug introduced in Chrome 20.0.1120.0
			*resp_code = 200;
			tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY is missing by accepted");
			return 0;
		}
		else{
			TSK_DEBUG_ERROR("MESSAGE-INTEGRITY is missing");
			*resp_code = 400;
			tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY is missing");
			return -3;
		}
	}

	if((TNET_STUN_HEADER_SIZE + msg_integrity_start) >= request_buff_size){
		TSK_DEBUG_ERROR("Invalid length");
		*resp_code = 400;
		tsk_strupdate(resp_phrase, "Invalid length");
		return -20;
	}

	if(request->length != msg_integrity_start){
		tsk_size_t size = (TNET_STUN_HEADER_SIZE + msg_integrity_start);
		uint8_t* new_buffer = (uint8_t*)tsk_calloc(size, 1);
		if(!new_buffer){
			TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", msg_integrity_start);
			return -30;
		}
		memcpy(new_buffer, request_buff, size);
		length = msg_integrity_start + (2/* Type */ + 2 /* Length */+ TSK_SHA1_DIGEST_SIZE /* INTEGRITY VALUE*/);
		new_buffer[2] = (length >> 8) & 0xFF;
		new_buffer[3] = (length & 0xFF);
		hmac_sha1digest_compute(new_buffer, size, pwd, tsk_strlen(pwd), hmac);
		TSK_FREE(new_buffer);
	}
	else{
		// must never happen
		hmac_sha1digest_compute(request_buff, request_buff_size, pwd, tsk_strlen(pwd), hmac);
	}

	for(i = 0; i < TSK_SHA1_DIGEST_SIZE; ++i){
		if(hmac[i] != stun_att_integrity->sha1digest[i]){
			TSK_DEBUG_ERROR("MESSAGE-INTEGRITY MISMATCH");
			*resp_code = 401;
			tsk_strupdate(resp_phrase, "MESSAGE-INTEGRITY MISMATCH");
			return -40;
		}
	}
	
	*resp_code = 200;
	tsk_strupdate(resp_phrase, "Ok");

	return 0;
}

int tnet_ice_pair_recv_response(tnet_ice_pair_t *self, const tnet_stun_response_t* response)
{
	if(self && response){
		if(self->last_request && tnet_stun_message_transac_id_equals(self->last_request->transaction_id, response->transaction_id)){
			// ignore errors (e.g. STALE-CREDENTIALS) which could happen in some special cases before success
			if(TNET_STUN_RESPONSE_IS_SUCCESS(response)){
				self->state_offer = tnet_ice_pair_state_succeed;
			}
		}
	}
	return 0;
}

const tnet_ice_pair_t* tnet_ice_pairs_find_by_response(tnet_ice_pairs_L_t* pairs, const tnet_stun_message_t* response)
{
	if(pairs && response){
		const tsk_list_item_t *item;
		const tnet_ice_pair_t *pair;
		tsk_list_foreach(item, pairs){
			if(!(pair = item->data)){
				continue;
			}
			if(pair->last_request && tnet_stun_message_transac_id_equals(pair->last_request->transaction_id, response->transaction_id)){
				return pair;
			}
		}
	}
	return tsk_null;
}

const tnet_ice_pair_t* tnet_ice_pairs_find_by_fd_and_addr(tnet_ice_pairs_L_t* pairs, uint16_t local_fd, const struct sockaddr_storage *remote_addr)
{
	int ret;
	const tsk_list_item_t *item;
	const tnet_ice_pair_t *pair;
	tnet_ip_t remote_ip;
	tnet_port_t remote_port;
	
	if(!pairs || !remote_addr){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if((ret = tnet_get_sockip_n_port((const struct sockaddr*)remote_addr, &remote_ip, &remote_port))){
		TNET_PRINT_LAST_ERROR("tnet_get_sockip_n_port() failed");
		return tsk_null;
	}

	tsk_list_foreach(item, pairs){
		if(!(pair = item->data) || !pair->candidate_offer || !pair->candidate_offer->socket || pair->candidate_offer->socket->fd != local_fd){
			continue;
		}
		if(!tsk_striequals(pair->candidate_answer->connection_addr, remote_ip) || pair->candidate_answer->port != remote_port){
			continue;
		}

		return pair;
	}

	return tsk_null;
}

// both RTP and RTCP have succeeded
#define _tnet_ice_pairs_get_nominated_offer_at(pairs, index, comp_id, check_fullness, ret)	_tnet_ice_pairs_get_nominated_at((pairs), offer, answer, (index), (comp_id), (check_fullness), (ret))
#define _tnet_ice_pairs_get_nominated_answer_at(pairs, index, comp_id, check_fullness, ret)	_tnet_ice_pairs_get_nominated_at((pairs), answer, offer, (index), (comp_id), (check_fullness), (ret))
#define _tnet_ice_pairs_get_nominated_at(pairs, dir_1, dir_2, index, _comp_id, check_fullness, ret) \
{ \
	ret = tsk_null; \
	if(pairs){ \
		const tsk_list_item_t *item; \
		const tnet_ice_pair_t *pair; \
		tsk_size_t pos; \
		tsk_bool_t nominated; \
		tsk_list_foreach(item, pairs){ \
			if(!(pair = item->data)){ \
				continue; \
			} \
			if(pair->state_##dir_1 == tnet_ice_pair_state_succeed && pair->candidate_##dir_1->comp_id == _comp_id){ \
				pos = 0; \
				nominated = tsk_true; \
				if(check_fullness){ \
					/* find another pair with same foundation (e.g. 'host') but different comp-id (e.g. RTCP) */ \
					const tsk_list_item_t *item2; \
					const tnet_ice_pair_t *pair2; \
					tsk_list_foreach(item2, pairs){ \
						if(!(pair2 = item2->data)){ \
							continue; \
						} \
						if((tsk_striequals(pair2->candidate_##dir_2->foundation, pair->candidate_##dir_2->foundation)) \
							&& (pair2->candidate_##dir_2->comp_id != pair->candidate_##dir_2->comp_id)){ \
								nominated = (pair2->state_##dir_2 == tnet_ice_pair_state_succeed); \
								break; \
						} \
					} \
				} \
 \
				if(nominated && (pos++ >= index)){ \
					ret = pair; \
					break; \
				}\
			} \
		} \
	} \
} \

// true only if both RTP and RTCP are nominated
tsk_bool_t tnet_ice_pairs_have_nominated_offer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
	const tnet_ice_pair_t *pair_ = tsk_null;
	tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;
	_tnet_ice_pairs_get_nominated_offer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTP, check_rtcp, (pair_));
	if((is_nominated_rtp = (pair_ != tsk_null)) && check_rtcp){
		_tnet_ice_pairs_get_nominated_offer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTCP, check_rtcp, (pair_));
		is_nominated_rtcp =(pair_ != tsk_null);
	}
	return (is_nominated_rtp && is_nominated_rtcp);
}

// true only if both RTP and RTCP are nominated
tsk_bool_t tnet_ice_pairs_have_nominated_answer(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
	const tnet_ice_pair_t *pair_ = tsk_null;
	tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;
	_tnet_ice_pairs_get_nominated_answer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTP, check_rtcp, (pair_));
	if((is_nominated_rtp = (pair_ != tsk_null)) && check_rtcp){
		_tnet_ice_pairs_get_nominated_answer_at((pairs), 0, TNET_ICE_CANDIDATE_COMPID_RTCP, check_rtcp, (pair_));
		is_nominated_rtcp =(pair_ != tsk_null);
	}
	return (is_nominated_rtp && is_nominated_rtcp);
}

// true only if both RTP and RTCP are nominated in symetric way
tsk_bool_t tnet_ice_pairs_have_nominated_symetric(const tnet_ice_pairs_L_t* pairs, tsk_bool_t check_rtcp)
{
	const tnet_ice_candidate_t *candidate_offer, *candidate_answer_src, *candidate_answer_dest;
	tsk_bool_t is_nominated_rtp, is_nominated_rtcp = tsk_true;

	int ret = tnet_ice_pairs_get_nominated_symetric(pairs, TNET_ICE_CANDIDATE_COMPID_RTP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
	is_nominated_rtp = (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest);
	if(is_nominated_rtp && check_rtcp){
		ret = tnet_ice_pairs_get_nominated_symetric(pairs, TNET_ICE_CANDIDATE_COMPID_RTCP, &candidate_offer, &candidate_answer_src, &candidate_answer_dest);
		is_nominated_rtcp = (ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest);
	}
	return (is_nominated_rtp && is_nominated_rtcp);
}

// gets symetric nominated candidates with the highest priority
// will succeed only if both RTP and RTCP are ok
int tnet_ice_pairs_get_nominated_symetric(const tnet_ice_pairs_L_t* pairs, uint32_t comp_id,
										  const tnet_ice_candidate_t** candidate_offer, 
										  const tnet_ice_candidate_t** candidate_answer_src,
										  const tnet_ice_candidate_t** candidate_answer_dest)
{
	const tnet_ice_pair_t *pair_offer = tsk_null;
	const tnet_ice_pair_t *pair_answer = tsk_null;
	tsk_size_t i_offer, i_answer;
	static const tsk_bool_t __check_fullness = tsk_false; // pairs will be checked seperatly

	if(!pairs || !candidate_offer || !candidate_answer_src || !candidate_answer_dest){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	*candidate_offer = tsk_null;
	*candidate_answer_src = tsk_null;
	*candidate_answer_dest = tsk_null;

	i_offer = 0;
	while(1){
		_tnet_ice_pairs_get_nominated_offer_at((pairs), i_offer, comp_id, __check_fullness, (pair_offer)); // pair with socket SO as sender
		if(!pair_offer) return 0;
		++i_offer;
		if(pair_offer->candidate_offer->comp_id != comp_id) continue;
		// find another pair with socket SO as receiver

		i_answer = 0;
		while(1){
			_tnet_ice_pairs_get_nominated_answer_at((pairs), i_answer, comp_id, __check_fullness, (pair_answer));
			if(!pair_answer) break;
			++i_answer;
			if(pair_answer->candidate_offer->comp_id != comp_id) continue;
			if(pair_answer->candidate_offer == pair_offer->candidate_offer){
				*candidate_offer = pair_offer->candidate_offer;
				*candidate_answer_src = pair_answer->candidate_answer;
				*candidate_answer_dest = pair_offer->candidate_answer;
				return 0;
			}
		}
	}
	return 0;
	
}
