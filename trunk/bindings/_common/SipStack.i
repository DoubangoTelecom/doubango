
%{
#include "SipUri.h"
#include "SipMessage.h"
#include "SipEvent.h"
#include "SipSession.h"

#include "ProxyConsumer.h"
#include "ProxyProducer.h"

#include "SipCallback.h"
#include "SafeObject.h"
#include "SipStack.h"
%}

/* Callbacks */
%feature("director") SipCallback;
%feature("director") ProxyAudioConsumer;
%feature("director") ProxyAudioProducer;


%nodefaultctor;
%include "SipUri.h"
%include "SipMessage.h"
%include "SipEvent.h"
%include "SipSession.h"

%include "ProxyConsumer.h"
%include "ProxyProducer.h"

%include "SipCallback.h"
%include "SafeObject.h"
%include "SipStack.h"
%clearnodefaultctor;





/* ====== From "tinySIP\include\tinysip\tsip_event.h"  ====== */
typedef enum tsip_event_type_e
{
	tsip_event_invite,
	tsip_event_message,
	tsip_event_options,
	tsip_event_publish,
	tsip_event_register,
	tsip_event_subscribe,
	
	tsip_event_dialog
}
tsip_event_type_t;

// 7xx ==> errors
#define tsip_event_code_dialog_transport_error		702
#define tsip_event_code_dialog_global_error			703
#define tsip_event_code_dialog_message_error		704

// 8xx ==> success
#define tsip_event_code_dialog_request_incoming		800
#define tsip_event_code_dialog_request_cancelled	801
#define tsip_event_code_dialog_request_sent			802

// 9xx ==> Informational
#define tsip_event_code_dialog_connecting			900
#define tsip_event_code_dialog_connected			901
#define tsip_event_code_dialog_terminating			902
#define tsip_event_code_dialog_terminated			903
#define tsip_event_code_stack_started				950
#define tsip_event_code_stack_stopped				951
#define tsip_event_code_stack_failed_to_start		952
#define tsip_event_code_stack_failed_to_stop		953

/* ====== From "tinySIP\include\tinysip\tsip_api_register.h"  ====== */
typedef enum tsip_register_event_type_e
{
	tsip_i_register,
	tsip_ao_register,

	tsip_i_unregister,
	tsip_ao_unregister,
}
tsip_register_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_subscribe.h"  ====== */
typedef enum tsip_subscribe_event_type_e
{
	tsip_i_subscribe,
	tsip_ao_subscribe,
	
	tsip_i_unsubscribe,
	tsip_ao_unsubscribe,

	tsip_i_notify,
	tsip_ao_notify
}
tsip_subscribe_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_publish.h"  ====== */
typedef enum tsip_publish_event_type_e
{
	tsip_i_publish,
	tsip_ao_publish,
	
	tsip_i_unpublish,
	tsip_ao_unpublish
}
tsip_publish_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_message.h"  ====== */
typedef enum tsip_message_event_type_e
{
	tsip_i_message,
	tsip_ao_message,
}
tsip_message_event_type_t;

/* ====== From "tinySIP\include\tinysip\tsip_api_options.h"  ====== */
typedef enum tsip_options_event_type_e
{
	tsip_i_options,
	tsip_ao_options,
}
tsip_options_event_type_t;


/* ====== From "tinySIP\include\tinysip\tsip_api_invite.h"  ====== */
typedef enum tsip_invite_event_type_e
{
	// ============================
	//	Sip Events
	//
	tsip_i_newcall,
	
	//! in-dialog requests/reponses
	tsip_i_request,
	tsip_ao_request,
	
	/* Explicit Call Transfer (ECT) */
	tsip_o_ect_ok,
	tsip_o_ect_nok,
	tsip_i_ect,
	
	// ============================
	//	Media Events
	//

	/* 3GPP TS 24.610: Communication Hold */
	tsip_m_local_hold_ok,
	tsip_m_local_hold_nok,
	tsip_m_local_resume_ok,
	tsip_m_local_resume_nok,
	tsip_m_remote_hold,
	tsip_m_remote_resume,
}
tsip_invite_event_type_t;