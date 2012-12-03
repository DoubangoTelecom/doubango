%{
#include "Msrp.h"
%}

/* Callbacks */
%feature("director") MsrpCallback;


%nodefaultctor;
%include "Msrp.h"
%clearnodefaultctor;

/* From tinyMSRP/tmsrp_message.h */
typedef enum tmsrp_request_type_e
{
	tmsrp_NONE = 0,

	tmsrp_SEND,
	tmsrp_REPORT,
	tmsrp_AUTH
	//... 
}
tmsrp_request_type_t;


/* From tinyMSRP/tmsrp_event.h */
typedef enum tmsrp_event_type_e
{
	tmsrp_event_type_none,
	tmsrp_event_type_connected,
	tmsrp_event_type_disconnected,
	tmsrp_event_type_message,
}
tmsrp_event_type_t;