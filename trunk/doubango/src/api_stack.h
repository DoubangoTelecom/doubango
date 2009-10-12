#ifndef __DOUBANGO_STACK_H__
#define  __DOUBANGO_STACK_H__

#include "pref.h"
#include "api_errors.h"
#include "sip_dialog.h"
#include <sofia-sip/nua.h>

#include <list>
#include <algorithm>

PREF_NAMESPACE_START

/* generate setter and getter methods*/
#define DEFINE_GET_SET(type, member)\
	inline type &get_##member(){ return this->##member; } \
	inline void set_##member(type val){ this->##member = val; }

/* predicate: find dialog by handle */
struct pred_dialog_find_by_handle: public std::binary_function< sip_dialog*, nua_handle_t*, bool > {
	bool operator () ( const sip_dialog* dlg, const nua_handle_t* handle ) const {
		return dlg->get_handle() == handle;
	}
};
/* predicate: find dialog by sipmethod */
struct pred_dialog_find_by_sipmethod: public std::binary_function< sip_dialog*, const char*, bool > {
	bool operator () ( const sip_dialog* dlg, const char* sipmethod ) const {
		return !stricmp(dlg->get_sipmethod(), sipmethod);
	}
};
/* predicate: find all dialogs in terminated state */
//==> IN C++ bool isDlgTerminated(const sip_dialog* dlg){ return dlg->get_terminated(); }
#define GET_DIALOG_TERMINATED(dlg)\
	sip_dialog* dlg = NULL; \
	std::list<sip_dialog*>::iterator iter = std::find_if( this->dialogs.begin(), this->dialogs.end(), isDlgTerminated ); \
	if(iter != this->dialogs.end()) dlg = *iter;

/* find dialog by sipmethod*/
#define GET_DIALOG_BY_SIPMETHOD(dlg, sipmethod) \
	sip_dialog* dlg = NULL; \
	std::list<sip_dialog*>::iterator iter = std::find_if( this->dialogs.begin(), this->dialogs.end(), std::bind2nd( pred_dialog_find_by_sipmethod(), sipmethod ) ); \
	if(iter != this->dialogs.end()) dlg = *iter;

/* find dialog by handle */
#define GET_DIALOG_BY_HANDLE(dlg, handle) \
	sip_dialog* dlg = NULL; \
	std::list<sip_dialog*>::iterator iter = std::find_if( this->dialogs.begin(), this->dialogs.end(), std::bind2nd( pred_dialog_find_by_handle(), handle ) ); \
	if(iter != this->dialogs.end()) dlg = *iter;

class DOUBANGO_API stack
{
public:
	stack(int id, nua_callback_f callback);
	~stack();
	
	inline ERR run();
	inline ERR shutdown();

	inline int get_id() const { return this->id; }
	inline bool get_running() const { return this->running; }
	inline bool get_initialized() const { return this->initialized; }
	inline su_home_t* get_home() { return this->home; }
	inline nua_t* get_nua() { return this->nua; }
	
	//
	//	SIP
	//
	ERR sip_register();
	ERR sip_unregister();

	ERR sip_message(const char* dest_address, const char* content_type, const char* content);


	//
	//	Authentication
	//
	DEFINE_GET_SET(char*, displayname);
	DEFINE_GET_SET(char*, public_id);
	DEFINE_GET_SET(char*, private_id);
	DEFINE_GET_SET(char*, realm);
	DEFINE_GET_SET(char*, password);
	DEFINE_GET_SET(char*, privacy);
	DEFINE_GET_SET(char*, pcscf_ip);
	DEFINE_GET_SET(int, pcscf_port);

	//
	//	Features
	//
	DEFINE_GET_SET(bool, smsip);
	DEFINE_GET_SET(bool, oma_large_msg);
	DEFINE_GET_SET(bool, oma_sip_im);
	DEFINE_GET_SET(bool, gsma_is);
	DEFINE_GET_SET(bool, gsma_vs);

	void callback_handle(nua_event_t event,
			       int status, char const *phrase,
			       nua_t *nua, nua_magic_t *magic,
			       nua_handle_t *nh, nua_hmagic_t *hmagic,
			       sip_t const *sip,
			       tagi_t tags[]);
private:
#ifdef WIN32
	static DWORD WINAPI ThreadRunner( void* magic );
#endif
	void loop();

private:
	int id;
	bool initialized;
	bool running;

	nua_callback_f callback;
	void* thread;

	std::list<sip_dialog*>dialogs;

	su_home_t      home[1];  /* memory home */
	su_root_t      *root;    /* root object */
	nua_t          *nua;     /* NUA stack object */

	//
	//	Authentication
	//
	char* displayname;
	char* public_id;
	char* private_id;
	char* realm;
	char* password;
	char* privacy;
	char* pcscf_ip;
	int pcscf_port;

	//
	// Features
	//
	bool smsip;
	bool oma_large_msg;
	bool oma_sip_im;
	bool gsma_is;
	bool gsma_vs;
};

#undef DEFINE_GET_SET

PREF_NAMESPACE_END

#endif /* __DOUBANGO_STACK_H__ */