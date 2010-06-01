
%{
#include "SipUri.h"
#include "SipMessage.h"
#include "SipEvent.h"
#include "SipSession.h"

#include "SipCallback.h"
#include "SafeObject.h"
#include "SipStack.h"
%}

/* turn on director wrapping Callback */
%feature("director") SipCallback;


%nodefaultctor;
%include "SipUri.h"
%include "SipMessage.h"
%include "SipEvent.h"
%include "SipSession.h"

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