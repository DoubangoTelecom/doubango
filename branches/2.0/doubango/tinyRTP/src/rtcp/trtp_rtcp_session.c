/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
/**@file trtp_rtcp_session.c
 * @brief RTCP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinyrtp/rtcp/trtp_rtcp_session.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_header.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_rr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sr.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_sdes.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_bye.h"
#include "tinyrtp/rtcp/trtp_rtcp_report_fb.h"
#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "ice/tnet_ice_ctx.h"
#include "turn/tnet_turn_session.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_md5.h"
#include "tsk_list.h"
#include "tsk_time.h"
#include "tsk_timer.h"
#include "tsk_safeobj.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h> /* INT_MAX */

#ifdef _MSC_VER
static double drand48() { return (((double)rand()) / RAND_MAX); }
static void srand48(long sv) { srand((unsigned int) sv); }
#endif

#define RTCP_BW			(160 * 50) // FIXME: default bandwidth (octet/second)
#define CODEC_RATE		8000		// FIXME
#define RTP_SEQ_MOD		(1 << 16)
#define MAX_DROPOUT		3000
#define MAX_MISORDER	100
#define MIN_SEQUENTIAL	2

typedef double time_tp;
typedef void* packet_;

typedef enum event_
{
	EVENT_BYE,
	EVENT_REPORT,
	EVENT_RTP
}
event_;

typedef enum PacketType_
{
	PACKET_RTCP_REPORT,
	PACKET_BYE,
	PACKET_RTP,
}
PacketType_;

#define TypeOfEvent(e)	(e)

#define TRTP_RTCP_SOURCE(self)	((trtp_rtcp_source_t*)self)

typedef struct trtp_rtcp_source_s
{
	TSK_DECLARE_OBJECT;

	uint32_t ssrc;			 /* source's ssrc */
	uint16_t max_seq;        /* highest seq. number seen */
	uint32_t cycles;         /* shifted count of seq. number cycles */
	uint32_t base_seq;       /* base seq number */
	uint32_t bad_seq;        /* last 'bad' seq number + 1 */
	uint32_t probation;      /* sequ. packets till source is valid */
	uint32_t received;       /* packets received */
	uint32_t expected_prior; /* packet expected at last interval */
	uint32_t received_prior; /* packet received at last interval */
	uint32_t transit;        /* relative trans time for prev pkt */
	double jitter;         /* estimated jitter */
	
	uint32_t base_ts;	/* base timestamp */
	uint32_t max_ts;	/* highest timestamp number seen */
	uint32_t rate;		/* codec sampling rate */

	uint32_t ntp_msw;  /* last received NTP timestamp from RTCP sender */
	uint32_t ntp_lsw;  /* last received NTP timestamp from RTCP sender */
	uint64_t dlsr;    /* delay since last SR */
}
trtp_rtcp_source_t;
typedef tsk_list_t trtp_rtcp_sources_L_t; /**< List of @ref trtp_rtcp_header_t elements */

static tsk_object_t* trtp_rtcp_source_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_source_t *source = self;
	if(source){
	}
	return self;
}
static tsk_object_t* trtp_rtcp_source_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_source_t *source = self;
	if(source){
	}
	return self;
}
static const tsk_object_def_t trtp_rtcp_source_def_s = 
{
	sizeof(trtp_rtcp_source_t),
	trtp_rtcp_source_ctor, 
	trtp_rtcp_source_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_source_def_t = &trtp_rtcp_source_def_s;

static int _trtp_rtcp_source_init_seq(trtp_rtcp_source_t* self, uint16_t seq, uint32_t ts);
static tsk_bool_t _trtp_rtcp_source_update_seq(trtp_rtcp_source_t* self, uint16_t seq, uint32_t ts);

static int __pred_find_source_by_ssrc(const tsk_list_item_t *item, const void *pssrc)
{
	if(item && item->data){
		trtp_rtcp_source_t *source = item->data;
		return source->ssrc - *((uint32_t*)pssrc);
	}
	return -1;
}

static trtp_rtcp_source_t* _trtp_rtcp_source_create(uint32_t ssrc, uint16_t seq, uint32_t ts)
{
	trtp_rtcp_source_t* source;
	if(!(source = tsk_object_new(trtp_rtcp_source_def_t))){
		TSK_DEBUG_ERROR("Failed to create source object");
		return tsk_null;
	}
	
	_trtp_rtcp_source_init_seq(source, seq, ts);
	source->ssrc = ssrc;
	source->max_seq = seq - 1;
	source->probation = MIN_SEQUENTIAL;
	source->rate = CODEC_RATE;//FIXME

	return source;
}

static int _trtp_rtcp_source_init_seq(trtp_rtcp_source_t* self, uint16_t seq, uint32_t ts)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->base_seq = seq;
	self->max_seq = seq;
	self->bad_seq = RTP_SEQ_MOD + 1;   /* so seq == bad_seq is false */
	self->cycles = 0;
	self->received = 0;
	self->received_prior = 0;
	self->expected_prior = 0;
	self->base_ts = ts;
	self->max_ts = ts;
	return 0;
}

static tsk_bool_t _trtp_rtcp_source_update_seq(trtp_rtcp_source_t* self, uint16_t seq, uint32_t ts)
{
	uint16_t udelta;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}

	 udelta = seq - self->max_seq;

	/*
	* Source is not valid until MIN_SEQUENTIAL packets with
	* sequential sequence numbers have been received.
	*/
	if (self->probation) {
		/* packet is in sequence */
		if (seq == self->max_seq + 1) {
			self->probation--;
			self->max_seq = seq;
			self->max_ts = ts;
			if (self->probation == 0) {
				_trtp_rtcp_source_init_seq(self, seq, ts);
				self->received++;
				return tsk_true;
			}
		} else {
			self->probation = MIN_SEQUENTIAL - 1;
			self->max_seq = seq;
			self->max_ts = ts;
		}
		return tsk_false;
	} else if (udelta < MAX_DROPOUT) {
		/* in order, with permissible gap */
		if (seq < self->max_seq) {
			/*
			* Sequence number wrapped - count another 64K cycle.
			*/
			self->cycles += RTP_SEQ_MOD;
		}
		self->max_seq = seq;
		self->max_ts = ts;
	} else if (udelta <= RTP_SEQ_MOD - MAX_MISORDER) {
		/* the sequence number made a very large jump */
		if (seq == self->bad_seq) {
			/*
			* Two sequential packets -- assume that the other side
			* restarted without telling us so just re-sync
			* (i.e., pretend this was the first packet).
			*/
			_trtp_rtcp_source_init_seq(self, seq, ts);
		}
		else {
			self->bad_seq = (seq + 1) & (RTP_SEQ_MOD-1);
			return tsk_false;
		}
	} else {
		/* duplicate or reordered packet */
	}
	self->received++;
	return tsk_true;
}

static tsk_bool_t _trtp_rtcp_source_is_probed(const trtp_rtcp_source_t* self)
{
	return (self && self->probation == 0);
}







typedef time_tp (*tc_f)();
static time_tp _trtp_rtcp_session_tc() { return (time_tp)tsk_time_now(); }

typedef struct trtp_rtcp_session_s
{
	TSK_DECLARE_OBJECT;
	
	tsk_bool_t is_started;
	tnet_fd_t local_fd;
	const struct sockaddr * remote_addr;
	struct tnet_ice_ctx_s* ice_ctx;
	tsk_bool_t is_ice_turn_active;

	const void* callback_data;
	trtp_rtcp_cb_f callback;

	int32_t app_bw_max_upload; // application specific (kbps)
	int32_t app_bw_max_download; // application specific (kbps)

	struct{
		tsk_timer_manager_handle_t* handle_global;
		tsk_timer_id_t id_report;
		tsk_timer_id_t id_bye;
	} timer;

	trtp_rtcp_source_t* source_local; /**< local source */
	trtp_rtcp_report_sdes_t* sdes;
	uint64_t time_start; /**< Start time in millis (NOT in NTP unit yet) */
	
	// <RTCP-FB>
	uint8_t fir_seqnr;
	// </RTCP-FB>

	// <sender>
	char* cname;
	uint32_t packets_count;
	uint32_t octets_count;
	// </sender>

	// <others>
	time_tp tp; /**< the last time an RTCP packet was transmitted; */
	tc_f tc; /**< the current time */
	time_tp tn; /**< the next scheduled transmission time of an RTCP packet */
	int32_t pmembers; /**< the estimated number of session members at the time tn was last recomputed */
	int32_t members; /**< the most current estimate for the number of session members */
	int32_t senders; /**< the most current estimate for the number of senders in the session */
	double rtcp_bw; /**< The target RTCP bandwidth, i.e., the total bandwidth
      that will be used for RTCP packets by all members of this session,
      in octets per second.  This will be a specified fraction of the
      "session bandwidth" parameter supplied to the application at
      startup*/
	tsk_bool_t we_sent; /**< Flag that is true if the application has sent data since the 2nd previous RTCP report was transmitted */
	double avg_rtcp_size; /**< The average compound RTCP packet size, in octets,
      over all RTCP packets sent and received by this participant.  The
      size includes lower-layer transport and network protocol headers
      (e.g., UDP and IP) as explained in Section 6.2*/
	tsk_bool_t initial; /**< Flag that is true if the application has not yet sent an RTCP packet */
	// </others>

	trtp_rtcp_sources_L_t *sources;

	TSK_DECLARE_SAFEOBJ;

#if HAVE_SRTP
	struct{
		const srtp_t* session;
	} srtp;
#endif
}
trtp_rtcp_session_t;

static tsk_object_t* trtp_rtcp_session_ctor(tsk_object_t * self, va_list * app)
{
	trtp_rtcp_session_t *session = self;
	if(session){
		session->app_bw_max_upload = INT_MAX; // INT_MAX or <=0 means undefined
		session->app_bw_max_download = INT_MAX; // INT_MAX or <=0 means undefined
		session->sources = tsk_list_create();
		session->timer.id_report = TSK_INVALID_TIMER_ID;
		session->timer.id_bye = TSK_INVALID_TIMER_ID;
		session->tc = _trtp_rtcp_session_tc;
		// get a handle for the global timer manager
		session->timer.handle_global = tsk_timer_mgr_global_ref();
		tsk_safeobj_init(session);
	}
	return self;
}
static tsk_object_t* trtp_rtcp_session_dtor(tsk_object_t * self)
{ 
	trtp_rtcp_session_t *session = self;
	if(session){
		trtp_rtcp_session_stop(session);

		TSK_OBJECT_SAFE_FREE(session->sources);
		TSK_OBJECT_SAFE_FREE(session->source_local);
		TSK_OBJECT_SAFE_FREE(session->sdes);
		TSK_OBJECT_SAFE_FREE(session->ice_ctx);
		TSK_FREE(session->cname);
		// release the handle for the global timer manager
		tsk_timer_mgr_global_unref(&session->timer.handle_global);

		tsk_safeobj_deinit(session);
	}
	return self;
}
static const tsk_object_def_t trtp_rtcp_session_def_s = 
{
	sizeof(trtp_rtcp_session_t),
	trtp_rtcp_session_ctor, 
	trtp_rtcp_session_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_rtcp_session_def_t = &trtp_rtcp_session_def_s;


static tsk_bool_t _trtp_rtcp_session_have_source(trtp_rtcp_session_t* self, uint32_t ssrc);
static trtp_rtcp_source_t* _trtp_rtcp_session_find_source(trtp_rtcp_session_t* self, uint32_t ssrc);
static trtp_rtcp_source_t* _trtp_rtcp_session_find_or_add_source(trtp_rtcp_session_t* self, uint32_t ssrc, uint16_t seq_if_add, uint32_t ts_id_add);
static int _trtp_rtcp_session_add_source(trtp_rtcp_session_t* self, trtp_rtcp_source_t* source);
static int _trtp_rtcp_session_add_source_2(trtp_rtcp_session_t* self, uint32_t ssrc, uint16_t seq, uint32_t ts, tsk_bool_t *added);
static int _trtp_rtcp_session_remove_source(trtp_rtcp_session_t* self, uint32_t ssrc, tsk_bool_t *removed);
static tsk_size_t _trtp_rtcp_session_send_pkt(trtp_rtcp_session_t* self, trtp_rtcp_packet_t* pkt);
static tsk_size_t _trtp_rtcp_session_send_raw(trtp_rtcp_session_t* self, const void* data, tsk_size_t size);
static int _trtp_rtcp_session_timer_callback(const void* arg, tsk_timer_id_t timer_id);

static void Schedule(trtp_rtcp_session_t* session, double tn, event_ e);
static void OnReceive(trtp_rtcp_session_t* session, const packet_ p, event_ e, tsk_size_t ReceivedPacketSize);
static void OnExpire(trtp_rtcp_session_t* session, event_ e);
static void SendBYEPacket(trtp_rtcp_session_t* session, event_ e);

trtp_rtcp_session_t* trtp_rtcp_session_create(uint32_t ssrc, const char* cname)
{
	trtp_rtcp_session_t* session;

	if(!(session = tsk_object_new(trtp_rtcp_session_def_t))){
		TSK_DEBUG_ERROR("Failed to create new session object");
		return tsk_null;
	}

	// RFC 3550 - 6.3.2 Initialization
	if(!(session->source_local = _trtp_rtcp_source_create(ssrc, 0, 0))){
		TSK_DEBUG_ERROR("Failed to create new local source");
		TSK_OBJECT_SAFE_FREE(session);
		goto bail;
	}
	_trtp_rtcp_session_add_source(session, session->source_local);
	session->initial = tsk_true;
	session->we_sent = tsk_false;
	session->senders = 1;
	session->members = 1;
	session->rtcp_bw = RTCP_BW;//FIXME: as parameter from the code, Also added possiblities to update this value
	session->cname = tsk_strdup(cname);
	
bail:
	return session;
}

struct trtp_rtcp_session_s* trtp_rtcp_session_create_2(struct tnet_ice_ctx_s* ice_ctx, uint32_t ssrc, const char* cname)
{
	struct trtp_rtcp_session_s* session = trtp_rtcp_session_create(ssrc, cname);
	if (session) {
		if ((session->ice_ctx = tsk_object_ref(ice_ctx))) {
			session->is_ice_turn_active = tnet_ice_ctx_is_turn_rtcp_active(session->ice_ctx);
		}
	}
	return session;
}

int trtp_rtcp_session_set_callback(trtp_rtcp_session_t* self, trtp_rtcp_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_safeobj_lock(self);
	self->callback = callback;
	self->callback_data = callback_data;
	tsk_safeobj_unlock(self);
	return 0;
}

#if HAVE_SRTP
int trtp_rtcp_session_set_srtp_sess(trtp_rtcp_session_t* self, const srtp_t* session)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);
	self->srtp.session = session;
	tsk_safeobj_unlock(self);

	return 0;
}
#endif

int trtp_rtcp_session_set_app_bandwidth_max(trtp_rtcp_session_t* self, int32_t bw_upload_kbps, int32_t bw_download_kbps)
{
	trtp_rtcp_report_rr_t* rr;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);

	self->app_bw_max_upload = bw_upload_kbps;
	self->app_bw_max_download = bw_download_kbps;

	if(self->is_started && self->source_local && self->app_bw_max_download > 0 && self->app_bw_max_download != INT_MAX){ // INT_MAX or <=0 means undefined
		tsk_list_item_t* item;
		uint32_t media_ssrc_list[256] = {0};
		uint32_t media_ssrc_list_count = 0;
		// retrieve sources as array
		tsk_list_foreach(item, self->sources){
			if(!item->data){ 
				continue;
			}
			if((media_ssrc_list_count + 1) < sizeof(media_ssrc_list)/sizeof(media_ssrc_list[0])){
				media_ssrc_list[media_ssrc_list_count++] = TRTP_RTCP_SOURCE(item->data)->ssrc;
			}
		}
		// create RTCP-RR packet and send it over the network
		if(media_ssrc_list_count > 0 && (rr = trtp_rtcp_report_rr_create_2(self->source_local->ssrc))){
			// app_bw_max_download unit is kbps while create_afb_remb() expect bps
			trtp_rtcp_report_psfb_t* psfb_afb_remb = trtp_rtcp_report_psfb_create_afb_remb(self->source_local->ssrc/*sender SSRC*/, media_ssrc_list, media_ssrc_list_count, (self->app_bw_max_download * 1024));
			if(psfb_afb_remb){
				TSK_DEBUG_INFO("Packing RTCP-AFB-REMB (bw_dwn=%d kbps) for outgoing RTCP-RR", self->app_bw_max_download);
				if(trtp_rtcp_packet_add_packet((trtp_rtcp_packet_t*)rr, (trtp_rtcp_packet_t*)psfb_afb_remb, tsk_false) == 0){
					_trtp_rtcp_session_send_pkt(self, (trtp_rtcp_packet_t*)rr);
				}
				TSK_OBJECT_SAFE_FREE(psfb_afb_remb);
			}
			TSK_OBJECT_SAFE_FREE(rr);
		}
	}

	tsk_safeobj_unlock(self);
	
	return 0;
}

int trtp_rtcp_session_start(trtp_rtcp_session_t* self, tnet_fd_t local_fd, const struct sockaddr * remote_addr)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->is_started){
		TSK_DEBUG_WARN("Already started");
		return 0;
	}
	
	// start global timer manager
	if((ret = tsk_timer_manager_start(self->timer.handle_global))){
		TSK_DEBUG_ERROR("Failed to start timer");
		return ret;
	}

	self->local_fd = local_fd;
	self->remote_addr = remote_addr;

	// Send Initial RR (mandatory)
	Schedule(self, 0., EVENT_REPORT);

	// set start time
	self->time_start = tsk_time_now();

	self->is_started = tsk_true;

	return ret;
}

int trtp_rtcp_session_stop(trtp_rtcp_session_t* self)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->is_started){
		// send BYE synchronous way
		SendBYEPacket(self, EVENT_REPORT);
		
		// this is a global timer shared by many components -> stopping it won't remove
		// all scheduled items as it could continue running if still used
		tsk_safeobj_lock(self); // must
		if(TSK_TIMER_ID_IS_VALID(self->timer.id_bye)){
			tsk_timer_manager_cancel(self->timer.handle_global, self->timer.id_bye);
			self->timer.id_bye = TSK_INVALID_TIMER_ID;
		}
		if(TSK_TIMER_ID_IS_VALID(self->timer.id_report)){
			tsk_timer_manager_cancel(self->timer.handle_global, self->timer.id_report);
			self->timer.id_report = TSK_INVALID_TIMER_ID;
		}
		tsk_safeobj_unlock(self);
		self->is_started = tsk_false;
	}

	return ret;
}

int trtp_rtcp_session_process_rtp_out(trtp_rtcp_session_t* self, const trtp_rtp_packet_t* packet_rtp, tsk_size_t size)
{
	int ret = 0;

	if(!self || !packet_rtp || !packet_rtp->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->is_started){
		TSK_DEBUG_ERROR("Not started");
		return -2;
	}

	tsk_safeobj_lock(self);

	// create local source if not already done
	// first destroy it if the ssrc don't match
	if(self->source_local && self->source_local->ssrc != packet_rtp->header->ssrc){
		tsk_bool_t removed = tsk_false;
		// local ssrc has changed: sould never happen ...but who know?
		// remove the source
		TSK_DEBUG_WARN("Not expected to be called");
		_trtp_rtcp_session_remove_source(self, self->source_local->ssrc, &removed);
		TSK_OBJECT_SAFE_FREE(self->source_local);
		TSK_OBJECT_SAFE_FREE(self->sdes);
		self->packets_count = 0;
		self->octets_count = 0;
		if(removed){
			--self->senders;
			--self->members;
		}
	}
	if(!self->source_local){
		if(!(self->source_local = _trtp_rtcp_source_create(packet_rtp->header->ssrc, packet_rtp->header->seq_num, packet_rtp->header->timestamp))){
			TSK_DEBUG_ERROR("Failed to create new local source");
		}
		// add the source (refresh the number of senders, ...)
		_trtp_rtcp_session_add_source(self, self->source_local);
		// 'members' and 'senders' were already initialized in the constructor
	}
	
	if(!self->we_sent){
		self->we_sent = tsk_true;
	}

	++self->packets_count;
	self->octets_count += size;

	tsk_safeobj_unlock(self);

	return ret;
}

int trtp_rtcp_session_process_rtp_in(trtp_rtcp_session_t* self, const trtp_rtp_packet_t* packet_rtp, tsk_size_t size)
{
	int ret = 0;
	trtp_rtcp_source_t* source;

	if(!self || !packet_rtp || !packet_rtp->header){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->is_started){
		TSK_DEBUG_INFO("RTCP session not started");
		return -2;
	}

	tsk_safeobj_lock(self);
	OnReceive(self, (const packet_)packet_rtp, EVENT_RTP, size);
	if((source = _trtp_rtcp_session_find_source(self, packet_rtp->header->ssrc))){
		if(_trtp_rtcp_source_update_seq(source, packet_rtp->header->seq_num, packet_rtp->header->timestamp)){
			// RFC 3550 A.8 Estimating the Interarrival Jitter
			/* uint32_t expected = (source->cycles + source->max_seq) - source->base_seq + 1; */
			double arrival = (((double)(source->max_ts - source->base_ts) / (double)source->rate) * 1000);
			int32_t transit = (int32_t)arrival - packet_rtp->header->timestamp;
			int32_t d = (transit - source->transit);
			if(d < 0) d = -d;
			source->transit = transit;
			source->jitter += (1./16.) * ((double)d - source->jitter);
		}
		TSK_OBJECT_SAFE_FREE(source);
	}

	tsk_safeobj_unlock(self);

	return ret;
}

int trtp_rtcp_session_process_rtcp_in(trtp_rtcp_session_t* self, const void* buffer, tsk_size_t size)
{
	int ret = 0;
	trtp_rtcp_packet_t* packet_rtcp = tsk_null;

	if(!self || !buffer || size < TRTP_RTCP_HEADER_SIZE){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->is_started){
		TSK_DEBUG_ERROR("Not started");
		return -2;
	}
	
	// derialize the RTCP packet for processing
	packet_rtcp = trtp_rtcp_packet_deserialize(buffer, size);
	if(packet_rtcp){
		tsk_safeobj_lock(self);
		OnReceive(self, 
			(const packet_)packet_rtcp, 
			(packet_rtcp->header->type == trtp_rtcp_packet_type_bye) ? EVENT_BYE : EVENT_REPORT, 
			size);
		if(packet_rtcp->header->type == trtp_rtcp_packet_type_sr){
			trtp_rtcp_source_t* source;
			const trtp_rtcp_report_sr_t* sr = (const trtp_rtcp_report_sr_t*)packet_rtcp;
			if((source = _trtp_rtcp_session_find_source(self, sr->ssrc))){
				source->ntp_lsw = sr->sender_info.ntp_lsw;
				source->ntp_msw = sr->sender_info.ntp_msw;
				source->dlsr = tsk_time_now();
				TSK_OBJECT_SAFE_FREE(source);
			}
		}
		tsk_safeobj_unlock(self); // must be before callback()

		if(self->callback){
			ret = self->callback(self->callback_data, packet_rtcp);
		}
		TSK_OBJECT_SAFE_FREE(packet_rtcp);
	}
		

	return ret;
}

int trtp_rtcp_session_signal_pkt_loss(trtp_rtcp_session_t* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count)
{
	trtp_rtcp_report_rr_t* rr;
	if(!self || !self->source_local || !seq_nums || !count){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->is_started){
		TSK_DEBUG_ERROR("Not started");
		return -1;
	}

	tsk_safeobj_lock(self);

	if((rr = trtp_rtcp_report_rr_create_2(self->source_local->ssrc))){
		trtp_rtcp_report_rtpfb_t* rtpfb;
		if((rtpfb = trtp_rtcp_report_rtpfb_create_nack(self->source_local->ssrc, ssrc_media, seq_nums, count))){
			trtp_rtcp_packet_add_packet((trtp_rtcp_packet_t*)rr,  (trtp_rtcp_packet_t*)rtpfb, tsk_false);
			_trtp_rtcp_session_send_pkt(self, (trtp_rtcp_packet_t*)rr);
			TSK_OBJECT_SAFE_FREE(rtpfb);
		}
		TSK_OBJECT_SAFE_FREE(rr);
	}

	tsk_safeobj_unlock(self);

	return 0;
}

// Frame corrupted means the prediction chain is broken -> Send FIR
int trtp_rtcp_session_signal_frame_corrupted(trtp_rtcp_session_t* self, uint32_t ssrc_media)
{
	trtp_rtcp_report_rr_t* rr;
	if(!self || !self->source_local){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!self->is_started){
		TSK_DEBUG_ERROR("Not started");
		return -1;
	}

	tsk_safeobj_lock(self);
	
	if((rr = trtp_rtcp_report_rr_create_2(self->source_local->ssrc))){
		trtp_rtcp_report_psfb_t* psfb_fir = trtp_rtcp_report_psfb_create_fir(self->fir_seqnr++, self->source_local->ssrc, ssrc_media);
		if(psfb_fir){
			trtp_rtcp_packet_add_packet((trtp_rtcp_packet_t*)rr, (trtp_rtcp_packet_t*)psfb_fir, tsk_false);
			_trtp_rtcp_session_send_pkt(self, (trtp_rtcp_packet_t*)rr);
			TSK_OBJECT_SAFE_FREE(psfb_fir);
		}
		TSK_OBJECT_SAFE_FREE(rr);
	}

	tsk_safeobj_unlock(self);
	return 0;
}

// for now send just a FIR
int trtp_rtcp_session_signal_jb_error(struct trtp_rtcp_session_s* self, uint32_t ssrc_media)
{
	return trtp_rtcp_session_signal_frame_corrupted(self, ssrc_media);
}

static tsk_bool_t _trtp_rtcp_session_have_source(trtp_rtcp_session_t* self, uint32_t ssrc)
{
	tsk_list_item_t* item;
	tsk_list_foreach(item, self->sources){
		if(TRTP_RTCP_SOURCE(item->data)->ssrc == ssrc){
			return tsk_true;
		}
	}
	return tsk_false;
}

// find source by ssrc
// the caller must release the returned object
static trtp_rtcp_source_t* _trtp_rtcp_session_find_source(trtp_rtcp_session_t* self, uint32_t ssrc)
{
	tsk_list_item_t* item;
	tsk_list_foreach(item, self->sources){
		if(TRTP_RTCP_SOURCE(item->data)->ssrc == ssrc){
			return tsk_object_ref(item->data);
		}
	}
	return tsk_null;
}

// find or add source by ssrc
// the caller must release the returned object
static trtp_rtcp_source_t* _trtp_rtcp_session_find_or_add_source(trtp_rtcp_session_t* self, uint32_t ssrc, uint16_t seq_if_add, uint32_t ts_id_add)
{
	trtp_rtcp_source_t* source;
	if((source = _trtp_rtcp_session_find_source(self, ssrc))){
		return source;
	}

	if((source = _trtp_rtcp_source_create(ssrc, seq_if_add, ts_id_add))){
		if((_trtp_rtcp_session_add_source(self, source)) != 0){
			TSK_DEBUG_ERROR("Failed to add source");
			TSK_OBJECT_SAFE_FREE(source);
			return tsk_null;
		}
		return tsk_object_ref(source);
	}
	return tsk_null;
}

int _trtp_rtcp_session_add_source(trtp_rtcp_session_t* self, trtp_rtcp_source_t* source)
{
	if(!self || !source){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_list_lock(self->sources);
	source = tsk_object_ref(source);
	tsk_list_push_back_data(self->sources, (void**)&source);
	tsk_list_unlock(self->sources);

	return 0;
}

// adds a source if doesn't exist
static int _trtp_rtcp_session_add_source_2(trtp_rtcp_session_t* self, uint32_t ssrc, uint16_t seq, uint32_t ts, tsk_bool_t *added)
{
	int ret = 0;
	tsk_list_item_t* item;
	trtp_rtcp_source_t* source;

	tsk_list_lock(self->sources);
	tsk_list_foreach(item, self->sources){
		if(TRTP_RTCP_SOURCE(item->data)->ssrc == ssrc){
			tsk_list_unlock(self->sources);
			*added = tsk_false;
			return 0;
		}
	}

	tsk_list_unlock(self->sources);

	if((source = _trtp_rtcp_source_create(ssrc, seq, ts))){
		ret = _trtp_rtcp_session_add_source(self, source);
	}

	TSK_OBJECT_SAFE_FREE(source);

	*added = tsk_true;
	return ret;
}

int _trtp_rtcp_session_remove_source(trtp_rtcp_session_t* self, uint32_t ssrc, tsk_bool_t *removed)
{
	*removed = tsk_false;
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_list_lock(self->sources);
	if((*removed = tsk_list_remove_item_by_pred(self->sources, __pred_find_source_by_ssrc, &ssrc)) == tsk_true){
		// ...
	}
	tsk_list_unlock(self->sources);
	return 0;
}

static tsk_size_t _trtp_rtcp_session_send_pkt(trtp_rtcp_session_t* self, trtp_rtcp_packet_t* pkt)
{
	tsk_size_t ret = 0;
	tsk_size_t __num_bytes_pad = 0;
	tsk_buffer_t* buffer;

	if(!self->remote_addr || self->local_fd <= 0){
		TSK_DEBUG_ERROR("Invalid network settings");
		return 0;
	}

#if HAVE_SRTP
	if(self->srtp.session) __num_bytes_pad = (SRTP_MAX_TRAILER_LEN + 0x4);
#endif

	// SDES
	if(!self->sdes && (self->sdes = trtp_rtcp_report_sdes_create_null())){
		trtp_rtcp_sdes_chunck_t* chunck = trtp_rtcp_sdes_chunck_create(self->source_local->ssrc);
		if(chunck){
			static const char* _name = "test@doubango.org";
			trtp_rtcp_sdes_chunck_add_item(chunck, trtp_rtcp_sdes_item_type_cname, self->cname, tsk_strlen(self->cname));
			trtp_rtcp_sdes_chunck_add_item(chunck, trtp_rtcp_sdes_item_type_name, _name, tsk_strlen(_name));
			trtp_rtcp_report_sdes_add_chunck(self->sdes, chunck);
			TSK_OBJECT_SAFE_FREE(chunck);
		}
	}
	if(self->sdes){
		trtp_rtcp_packet_add_packet(pkt, (trtp_rtcp_packet_t*)self->sdes, tsk_true);
	}

	if((buffer = trtp_rtcp_packet_serialize(pkt, __num_bytes_pad))){
		void* data = buffer->data;
		int size = buffer->size;
#if HAVE_SRTP
		if(self->srtp.session){
			if(srtp_protect_rtcp(((srtp_t)*self->srtp.session), data, &size) != err_status_ok){
				TSK_DEBUG_ERROR("srtp_protect_rtcp() failed");
			}
		}
#endif
		ret = _trtp_rtcp_session_send_raw(self, data, size);
		TSK_OBJECT_SAFE_FREE(buffer);
	}

	return ret;
}

static tsk_size_t _trtp_rtcp_session_send_raw(trtp_rtcp_session_t* self, const void* data, tsk_size_t size)
{
	tsk_size_t ret = 0;
	if (!self || !data || !size) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	if (self->is_ice_turn_active) {
		ret = (tnet_ice_ctx_send_turn_rtcp(self->ice_ctx, data, size) == 0) ? size : 0; // returns #0 if ok
	}
	else {
		if (tnet_sockfd_sendto(self->local_fd, self->remote_addr, data, size) == size){ // returns number of sent bytes
			ret = size;
		}
	}
	return ret;
}

static int _trtp_rtcp_session_timer_callback(const void* arg, tsk_timer_id_t timer_id)
{
	trtp_rtcp_session_t* session = (trtp_rtcp_session_t*)arg;
	tsk_safeobj_lock(session); // must
	if(session->timer.id_bye == timer_id){
		session->timer.id_bye = TSK_INVALID_TIMER_ID;
		OnExpire(session, EVENT_BYE);
	}
	else if(session->timer.id_report == timer_id){
		session->timer.id_report = TSK_INVALID_TIMER_ID;
		OnExpire(session, EVENT_REPORT);
	}
	tsk_safeobj_unlock(session);
	return 0;
}

static tsk_bool_t IsRtpPacket(const packet_ p)
{
	return (TSK_OBJECT_HEADER(p)->__def__ == trtp_rtp_packet_def_t);
}

static PacketType_ PacketType(const packet_ p)
{
	if(IsRtpPacket(p)){
		return PACKET_RTP;
	}
	else{
		switch(((const trtp_rtcp_packet_t*)p)->header->type){
			case trtp_rtcp_packet_type_bye: return PACKET_BYE;
			default: return PACKET_RTCP_REPORT;
		}
	}
}

// Returns true if the packet is from a member or not
// also checks all csrc
static tsk_bool_t NewMember(trtp_rtcp_session_t* session, const packet_ p)
{
	uint32_t ssrc = 0;
	if(IsRtpPacket(p)){
		const trtp_rtp_packet_t* packet_rtp = (const trtp_rtp_packet_t*)p;
		tsk_size_t i;
		for(i = 0; i < packet_rtp->header->csrc_count && i < sizeof(packet_rtp->header->csrc)/sizeof(packet_rtp->header->csrc[0]); ++i){
			if(!(_trtp_rtcp_session_have_source(session, packet_rtp->header->csrc[i]))){
				return tsk_false;
			}
		}
		ssrc = packet_rtp->header->ssrc;
	}
	else{
		switch(((const trtp_rtcp_packet_t*)p)->header->type){
			case trtp_rtcp_packet_type_rr: ssrc = ((const trtp_rtcp_report_rr_t*)p)->ssrc; break;
			case trtp_rtcp_packet_type_sr: ssrc = ((const trtp_rtcp_report_sr_t*)p)->ssrc; break;
			case trtp_rtcp_packet_type_bye:
				{
					tsk_size_t i;
					const trtp_rtcp_report_bye_t* bye = (const trtp_rtcp_report_bye_t*)p;
					for(i = 0; i < TRTP_RTCP_PACKET(bye)->header->rc; ++i){
						if(!_trtp_rtcp_session_have_source(session, bye->ssrc_list[i])){
							return tsk_false;
						}
					}
					return tsk_true;
				}
			default: return tsk_false;
		}
	}

	return !_trtp_rtcp_session_have_source(session, ssrc);
}

#define NewSender(session, p) NewMember((session), (p))

static tsk_size_t AddMemberUsingRTCPPacket(trtp_rtcp_session_t* session, const trtp_rtcp_packet_t* p, tsk_bool_t sender)
{
	trtp_rtcp_packets_L_t* packets = tsk_null;
	trtp_rtcp_rblocks_L_t* blocks = tsk_null;
	tsk_bool_t added = tsk_false;
	tsk_size_t count = 0;

	switch(p->header->type){
		case trtp_rtcp_packet_type_rr:
			{
				const trtp_rtcp_report_rr_t* rr = (const trtp_rtcp_report_rr_t*)p;
				_trtp_rtcp_session_add_source_2(session, ((const trtp_rtcp_report_rr_t*)p)->ssrc, 0, 0, &added);
				if(added) ++count;
				
				packets = rr->packets;
				blocks = rr->blocks;
				break;
			}
		case trtp_rtcp_packet_type_sr:
			{
				const trtp_rtcp_report_sr_t* sr = (const trtp_rtcp_report_sr_t*)p;
				_trtp_rtcp_session_add_source_2(session, ((const trtp_rtcp_report_sr_t*)p)->ssrc, 0, 0, &added);
				if(added) ++count;
				packets = sr->packets;
				blocks = sr->blocks;
				break;
			}
        default:
            {
                break;
            }
	}

	if(!sender){
		if(packets){
			const tsk_list_item_t *item;
			tsk_list_foreach(item, packets){
				AddMemberUsingRTCPPacket(session, (const trtp_rtcp_packet_t*)item->data, sender);
			}
		}
		if(blocks){
			const tsk_list_item_t *item;
			tsk_list_foreach(item, blocks){
				_trtp_rtcp_session_add_source_2(session, TRTP_RTCP_RBLOCK(item->data)->ssrc, 0, 0, &added);
				if(added) ++count;
			}
		}
	}

	return count;
}

static tsk_size_t AddMember_(trtp_rtcp_session_t* session, const packet_ p, tsk_bool_t sender)
{
	tsk_size_t count = 0;
	if(IsRtpPacket(p)){
		const trtp_rtp_packet_t* packet_rtp = (const trtp_rtp_packet_t*)p;
		tsk_size_t i;
		tsk_bool_t added = tsk_false;
		_trtp_rtcp_session_add_source_2(session, packet_rtp->header->ssrc, packet_rtp->header->seq_num, packet_rtp->header->timestamp, &added);
		if(added) ++count;
		for(i = 0; i < packet_rtp->header->csrc_count && i < sizeof(packet_rtp->header->csrc)/sizeof(packet_rtp->header->csrc[0]); ++i){
			_trtp_rtcp_session_add_source_2(session, packet_rtp->header->csrc[i], 0, 0, &added);
			if(added) ++count;
		}
	}
	else{
		count += AddMemberUsingRTCPPacket(session, (const trtp_rtcp_packet_t*) p, sender);
	}
	return count;
}

#define AddMember(session, p)	AddMember_((session), (p), tsk_false)
#define AddSender(session, p)	AddMember_((session), (p), tsk_true)


static tsk_size_t RemoveMemberUsingRTCPPacket(trtp_rtcp_session_t* session, const trtp_rtcp_packet_t* p)
{
	trtp_rtcp_packets_L_t* packets = tsk_null;
	trtp_rtcp_rblocks_L_t* blocks = tsk_null;
	tsk_bool_t removed = tsk_false;
	tsk_size_t count = 0;

	switch(p->header->type){
		case trtp_rtcp_packet_type_rr:
			{
				const trtp_rtcp_report_rr_t* rr = (const trtp_rtcp_report_rr_t*)p;
				_trtp_rtcp_session_remove_source(session, ((const trtp_rtcp_report_rr_t*)p)->ssrc, &removed);
				if(removed) ++count;
				
				packets = rr->packets;
				blocks = rr->blocks;
				break;
			}
		case trtp_rtcp_packet_type_sr:
			{
				const trtp_rtcp_report_sr_t* sr = (const trtp_rtcp_report_sr_t*)p;
				_trtp_rtcp_session_remove_source(session, ((const trtp_rtcp_report_sr_t*)p)->ssrc, &removed);
				if(removed) ++count;
				packets = sr->packets;
				blocks = sr->blocks;
				break;
			}
        default:
            {
                break;
            }
	}
	
	if(packets){
		const tsk_list_item_t *item;
		tsk_list_foreach(item, packets){
			RemoveMemberUsingRTCPPacket(session, (const trtp_rtcp_packet_t*)item->data);
		}
	}
	if(blocks){
		const tsk_list_item_t *item;
		tsk_list_foreach(item, blocks){
			_trtp_rtcp_session_remove_source(session, TRTP_RTCP_RBLOCK(item->data)->ssrc, &removed);
			if(removed) ++count;
		}
	}
	

	return count;
}

static tsk_size_t RemoveMember(trtp_rtcp_session_t* session, const packet_ p)
{
	tsk_size_t count = 0;
	if(IsRtpPacket(p)){
		const trtp_rtp_packet_t* packet_rtp = (const trtp_rtp_packet_t*)p;
		tsk_size_t i;
		tsk_bool_t removed = tsk_false;
		_trtp_rtcp_session_remove_source(session, packet_rtp->header->ssrc, &removed);
		if(removed) ++count;
		for(i = 0; i < packet_rtp->header->csrc_count && i < sizeof(packet_rtp->header->csrc)/sizeof(packet_rtp->header->csrc[0]); ++i){
			_trtp_rtcp_session_remove_source(session, packet_rtp->header->csrc[i], &removed);
			if(removed) ++count;
		}
	}
	else{
		count += RemoveMemberUsingRTCPPacket(session, (const trtp_rtcp_packet_t*) p);
	}
	return count;
}

#define RemoveSender(session, p) RemoveMember((session), (p))

// Sends BYE in synchronous mode
static void SendBYEPacket(trtp_rtcp_session_t* session, event_ e)
{
	trtp_rtcp_report_bye_t* bye;
	tsk_size_t __num_bytes_pad = 0;

	if(!session->remote_addr || session->local_fd <= 0){
		TSK_DEBUG_ERROR("Invalid network settings");
		return;
	}

	tsk_safeobj_lock(session);

#if HAVE_SRTP
	if(session->srtp.session) __num_bytes_pad = (SRTP_MAX_TRAILER_LEN + 0x4);
#endif

	if(session->source_local && (bye = trtp_rtcp_report_bye_create_2(session->source_local->ssrc))){
		tsk_buffer_t* buffer;
		// serialize and send the packet
		if((buffer = trtp_rtcp_packet_serialize((const trtp_rtcp_packet_t*)bye, __num_bytes_pad))){
			void* data = buffer->data;
			int size = buffer->size;
#if HAVE_SRTP
			if(session->srtp.session){
				if(srtp_protect_rtcp(((srtp_t)*session->srtp.session), data, &size) != err_status_ok){
					TSK_DEBUG_ERROR("srtp_protect_rtcp() failed");
				}
			}
#endif
			_trtp_rtcp_session_send_raw(session, data, size);
			TSK_OBJECT_SAFE_FREE(buffer);
		}
		TSK_OBJECT_SAFE_FREE(bye);
	}

	tsk_safeobj_unlock(session);
}

// returns sent packet size
static tsk_size_t SendRTCPReport(trtp_rtcp_session_t* session, event_ e)
{
	tsk_size_t ret = 0;

	tsk_safeobj_lock(session);

	if(session->initial){
		// Send Receiver report (manadatory to be the first on)
		trtp_rtcp_report_rr_t* rr = trtp_rtcp_report_rr_create_2(session->source_local->ssrc);
		if(rr){
			// serialize and send the packet
			ret = _trtp_rtcp_session_send_pkt(session, (trtp_rtcp_packet_t*)rr);
			TSK_OBJECT_SAFE_FREE(rr);
		}
	}
	else{
		trtp_rtcp_report_sr_t* sr = trtp_rtcp_report_sr_create_null();
		uint32_t media_ssrc_list[16] = {0};
		uint32_t media_ssrc_list_count = 0;
		if(sr){
			uint64_t ntp_now = tsk_time_ntp();
			uint64_t time_now = tsk_time_now();
			trtp_rtcp_rblock_t* rblock;
			trtp_rtcp_source_t* source;
			tsk_list_item_t *item;
			tsk_bool_t packet_lost = tsk_false;
			
			// sender info
			sr->ssrc = session->source_local->ssrc;
			sr->sender_info.ntp_msw = (ntp_now >> 32);
			sr->sender_info.ntp_lsw = (ntp_now & 0xFFFFFFFF);
			sr->sender_info.sender_pcount = session->packets_count;
			sr->sender_info.sender_ocount = session->octets_count;
			{	/* rtp_timestamp */
				struct timeval tv;
				uint64_t rtp_timestamp = (time_now - session->time_start) * (session->source_local->rate / 1000);
				tv.tv_sec = (long)(rtp_timestamp / 1000);
				tv.tv_usec = (long)(rtp_timestamp - ((rtp_timestamp / 1000) * 1000)) * 1000;
#if 1
				sr->sender_info.rtp_timestamp = (uint32_t)tsk_time_get_ms(&tv);
#else
				sr->sender_info.rtp_timestamp = (uint32_t)tsk_time_get_ntp_ms(&tv);
#endif
			}

			// report blocks
			tsk_list_foreach(item, session->sources){
				if(!(source = (trtp_rtcp_source_t*)item->data) || !_trtp_rtcp_source_is_probed(source)){
					continue;
				}
				if((rblock = trtp_rtcp_rblock_create_null())){
					uint32_t expected, expected_interval, received_interval, lost_interval;
					
					rblock->ssrc = source->ssrc;
					// RFC 3550 - A.3 Determining Number of Packets Expected and Lost
					expected = (source->cycles + source->max_seq) - source->base_seq + 1;
					expected_interval = expected - source->expected_prior;
					source->expected_prior = expected;
					received_interval = source->received - source->received_prior;
					source->received_prior = source->received;
					lost_interval = expected_interval - received_interval;
					if (expected_interval == 0 || lost_interval <= 0) rblock->fraction = 0;
					else rblock->fraction = (lost_interval << 8) / expected_interval;
					rblock->cumulative_no_lost = ((expected - source->received));
					if(!packet_lost && rblock->fraction) packet_lost = tsk_true;
					
					rblock->last_seq = ((source->cycles & 0xFFFF) << 16) | source->max_seq;
					rblock->jitter = (uint32_t)source->jitter;
					rblock->lsr = ((source->ntp_msw & 0xFFFF) << 16) | ((source->ntp_lsw & 0xFFFF0000) >> 16);
					if(source->dlsr){
						rblock->dlsr = (uint32_t)(((time_now - source->dlsr) * 65536) / 1000); // in units of 1/65536 seconds
					}
					
					trtp_rtcp_report_sr_add_block(sr, rblock);					
					TSK_OBJECT_SAFE_FREE(rblock);
				}

				if((media_ssrc_list_count + 1) < sizeof(media_ssrc_list)/sizeof(media_ssrc_list[0])){
					media_ssrc_list[media_ssrc_list_count++] = source->ssrc;
				}		
			}

			if(media_ssrc_list_count > 0){
				// draft-alvestrand-rmcat-remb-02
				if(session->app_bw_max_download > 0 && session->app_bw_max_download != INT_MAX){ // INT_MAX or <=0 means undefined
					// app_bw_max_download unit is kbps while create_afb_remb() expect bps
					trtp_rtcp_report_psfb_t* psfb_afb_remb = trtp_rtcp_report_psfb_create_afb_remb(session->source_local->ssrc/*sender SSRC*/, media_ssrc_list, media_ssrc_list_count, (session->app_bw_max_download * 1024));
					if(psfb_afb_remb){
						TSK_DEBUG_INFO("Packing RTCP-AFB-REMB (bw_dwn=%d kbps) for outgoing RTCP-SR", session->app_bw_max_download);
						trtp_rtcp_packet_add_packet((trtp_rtcp_packet_t*)sr, (trtp_rtcp_packet_t*)psfb_afb_remb, tsk_false);
						TSK_OBJECT_SAFE_FREE(psfb_afb_remb);
					}
				}
			}

			// serialize and send the packet
			ret = _trtp_rtcp_session_send_pkt(session, (trtp_rtcp_packet_t*)sr);
			TSK_OBJECT_SAFE_FREE(sr);
		}
	}

	tsk_safeobj_unlock(session);
	return ret;
}

static void Schedule(trtp_rtcp_session_t* session, double tn, event_ e)
{
	tsk_safeobj_lock(session); // must
	switch(e){
		case EVENT_BYE: 
			if(!TSK_TIMER_ID_IS_VALID(session->timer.id_bye)){
				session->timer.id_bye = tsk_timer_manager_schedule(session->timer.handle_global, (uint64_t)tn, _trtp_rtcp_session_timer_callback, session);
			}
			break;
		case EVENT_REPORT:
			if(!TSK_TIMER_ID_IS_VALID(session->timer.id_report)){
				session->timer.id_report = tsk_timer_manager_schedule(session->timer.handle_global, (uint64_t)tn, _trtp_rtcp_session_timer_callback, session);
			}
			break;
		default: TSK_DEBUG_ERROR("Unexpected code called"); break;
	}
	tsk_safeobj_unlock(session);
}

#define Reschedule(session, tn, e) Schedule((session), (tn), (e))

static double rtcp_interval(int32_t members,
					 int32_t senders,
					 double rtcp_bw,
					 int32_t we_sent,
					 double avg_rtcp_size,
					 tsk_bool_t initial)
{
	/*
	* Minimum average time between RTCP packets from this site (in
	* seconds).  This time prevents the reports from `clumping' when
	* sessions are small and the law of large numbers isn't helping
	* to smooth out the traffic.  It also keeps the report interval
	* from becoming ridiculously small during transient outages like
	* a network partition.
	*/
	#define RTCP_MIN_TIME 5.
	/*
	* Fraction of the RTCP bandwidth to be shared among active
	* senders.  (This fraction was chosen so that in a typical
	* session with one or two active senders, the computed report
	* time would be roughly equal to the minimum report time so that
	* we don't unnecessarily slow down receiver reports.)  The
	* receiver fraction must be 1 - the sender fraction.
	*/
	#define RTCP_SENDER_BW_FRACTION 0.25
	#define RTCP_RCVR_BW_FRACTION (1 - RTCP_SENDER_BW_FRACTION)
	/*
	* To compensate for "timer reconsideration" converging to a
	* value below the intended average.
	*/
	#define COMPENSATION  (2.71828 - 1.5)

	double t;                   /* interval */
	double rtcp_min_time = RTCP_MIN_TIME;
	int n;                      /* no. of members for computation */

	/*
	* Very first call at application start-up uses half the min
	* delay for quicker notification while still allowing some time
	* before reporting for randomization and to learn about other
	* sources so the report interval will converge to the correct
	* interval more quickly.
	*/
	if (initial) {
		rtcp_min_time /= 2;
	}
	/*
	* Dedicate a fraction of the RTCP bandwidth to senders unless
	* the number of senders is large enough that their share is
	* more than that fraction.
	*/
	n = members;
	if (senders <= members * RTCP_SENDER_BW_FRACTION) {
		if (we_sent) {
			rtcp_bw *= RTCP_SENDER_BW_FRACTION;
			n = senders;
		} else {
			rtcp_bw *= RTCP_RCVR_BW_FRACTION;
			n -= senders;
		}
	}

	/*
	* The effective number of sites times the average packet size is
	* the total number of octets sent when each site sends a report.
	* Dividing this by the effective bandwidth gives the time
	* interval over which those packets must be sent in order to
	* meet the bandwidth target, with a minimum enforced.  In that
	* time interval we send one report so this time is also our
	* average time between reports.
	*/
	t = avg_rtcp_size * n / rtcp_bw;
	if (t < rtcp_min_time) t = rtcp_min_time;

	/*
	* To avoid traffic bursts from unintended synchronization with
	* other sites, we then pick our actual next report interval as a
	* random number uniformly distributed between 0.5*t and 1.5*t.
	*/
	t = t * (drand48() + 0.5);
	t = t / COMPENSATION;

	return (t * 1000);
}


static void OnExpire(trtp_rtcp_session_t* session, event_ e)
{
	/* This function is responsible for deciding whether to send an
	* RTCP report or BYE packet now, or to reschedule transmission.
	* It is also responsible for updating the pmembers, initial, tp,
	* and avg_rtcp_size state variables.  This function should be
	* called upon expiration of the event timer used by Schedule().
	*/

	double t;     /* Interval */
	double tn;    /* Next transmit time */
	double tc;

	/* In the case of a BYE, we use "timer reconsideration" to
	* reschedule the transmission of the BYE if necessary */
	
	if (TypeOfEvent(e) == EVENT_BYE) {
		t = rtcp_interval(session->members,
			session->senders,
			session->rtcp_bw,
			session->we_sent,
			session->avg_rtcp_size,
			session->initial);
		tn = session->tp + t;
		if (tn <= session->tc()) {
			SendBYEPacket(session, e);
#if 0
			exit(1);
#endif
		} else {
#if 0
			Schedule(session, tn, e);
#else
			Schedule(session, 0, e);
#endif
		}

	} else if (TypeOfEvent(e) == EVENT_REPORT) {
		t = rtcp_interval(session->members,
			session->senders,
			session->rtcp_bw,
			session->we_sent,
			session->avg_rtcp_size,
			session->initial);
		tn = session->tp + t;
		if (tn <= (tc = session->tc())) {
			tsk_size_t SentPacketSize = SendRTCPReport(session, e);
			session->avg_rtcp_size = (1./16.)*SentPacketSize + (15./16.)*(session->avg_rtcp_size);
			session->tp = tc;

			/* We must redraw the interval.  Don't reuse the
			one computed above, since its not actually
			distributed the same, as we are conditioned
			on it being small enough to cause a packet to
			be sent */

			t = rtcp_interval(session->members,
				session->senders,
				session->rtcp_bw,
				session->we_sent,
				session->avg_rtcp_size,
				session->initial);
#if 0
			Schedule(session, t+tc, e);
#else
			Schedule(session, t, e);
#endif
			session->initial = tsk_false;
		} else {
#if 0
			Schedule(session, tn, e);
#else
			Schedule(session, 0, e);
#endif
		}
		session->pmembers = session->members;
	}
}

static void OnReceive(trtp_rtcp_session_t* session, const packet_ p, event_ e, tsk_size_t ReceivedPacketSize)
{
	/* What we do depends on whether we have left the group, and are
	* waiting to send a BYE (TypeOfEvent(e) == EVENT_BYE) or an RTCP
	* report.  p represents the packet that was just received.  */

	if (PacketType(p) == PACKET_RTCP_REPORT) {
		if (NewMember(session, p) && (TypeOfEvent(e) == EVENT_REPORT)) {
			session->members += AddMember(session, p);
		}
		session->avg_rtcp_size = (1./16.)*ReceivedPacketSize + (15./16.)*(session->avg_rtcp_size);
	} else if (PacketType(p) == PACKET_RTP) {
#if 0
		if (NewMember(session, p) && (TypeOfEvent(e) == EVENT_REPORT)) {
			session->members += AddMember(session, p);
		}
		if (NewSender(session, p) && (TypeOfEvent(e) == EVENT_REPORT)) {
			tsk_size_t count = AddSender(session, p);
			session->senders += count;
			session->members += count;
		}
#else
		if (NewSender(session, p)) {
			tsk_size_t count = AddSender(session, p);
			session->senders += count;
			session->members += count;
		}
#endif
	} else if (PacketType(p) == PACKET_BYE) {
		session->avg_rtcp_size = (1./16.)*ReceivedPacketSize + (15./16.)*(session->avg_rtcp_size);

		if (TypeOfEvent(e) == EVENT_REPORT) {
			double tc = session->tc();
			tsk_size_t count = RemoveMember(session, p);
			session->senders -= count;
			session->members -= count;
#if 0
			if (NewSender(session, p) == tsk_false) {
				RemoveSender(p);
				session->senders -= 1;
			}
			if (NewMember(session, p) == tsk_false) {
				RemoveMember(p);
				session->members -= 1;
			}
#endif

			if (session->members < session->pmembers && session->pmembers) {
				session->tn = (time_tp)(tc +
					(((double) session->members)/(session->pmembers))*(session->tn - tc));
				session->tp = (time_tp)(tc -
					(((double) session->members)/(session->pmembers))*(tc - session->tp));

				/* Reschedule the next report for time tn */

				Reschedule(session, session->tn, e);
				session->pmembers = session->members;
			}

		} else if (TypeOfEvent(e) == EVENT_BYE) {
			session->members += 1;
		}
	}
}
