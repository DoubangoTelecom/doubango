%module ipsec_lib
%module(directors="1") ipsec_lib

%include "enums.swg"
%include <stdint.i>

%define %cs_marshal_array(TYPE, CSTYPE)
%typemap(ctype) TYPE[] "void*"
%typemap(imtype,
inattributes="[MarshalAs(UnmanagedType.LPArray)]") TYPE[] "CSTYPE[]"
%typemap(cstype) TYPE[] "CSTYPE[]"
%typemap(in) TYPE[] %{ $1 = (TYPE*)$input; %}
%typemap(csin) TYPE[] "$csinput"
%enddef

// Mapping void* as IntPtr
%typemap(ctype)  void * "void *"
%typemap(imtype) void * "IntPtr"
%typemap(cstype) void * "IntPtr"
%typemap(csin)   void * "$csinput"
%typemap(in)     void * %{ $1 = $input; %}
%typemap(out)    void * %{ $result = $1; %}
%typemap(csout)  void * { return $imcall; }
%typemap(csdirectorin) void * "$iminput"

%{
#include "IPSecCtx.h"
%}

%nodefaultctor;
%include "IPSecCtx.h"
%clearnodefaultctor;


/**@def tipsec_lifetime_t
*/
/**@def tipsec_spi_t
*/
/**@def tipsec_port_t
*/
/**@def tipsec_key_t
*/
typedef uint64_t tipsec_lifetime_t;
typedef uint32_t tipsec_spi_t;
typedef uint16_t tipsec_port_t;
typedef void tipsec_key_t;

/**@ingroup tipsec_common_group
 * List of IPSec modes.
**/
typedef enum tipsec_mode_e {
    //! IPSec transport mode.
    tipsec_mode_trans,
    //! IPSec tunneling mode.
    tipsec_mode_tun
}
tipsec_mode_t;

/** List of supported IPSec protocols.
**/
typedef enum tipsec_proto_e {
    //! AH protocol ("ah").
    tipsec_proto_ah = (0x01 << 0),
    //! ESP protocol ("esp").
    tipsec_proto_esp = (0x01 << 1),
    //! Both AH and ESP protocols ("ah/esp").
    tipsec_proto_both = (tipsec_proto_ah | tipsec_proto_esp)
}
tipsec_proto_t;

/**List of supported Internet protocols for IPSec.
**/
typedef enum tipsec_ipproto_e {
    //! UDP.
    tipsec_ipproto_udp,
    //! TCP.
    tipsec_ipproto_tcp,
    //! ICMP.
    tipsec_ipproto_icmp,
    //! ALL.
    tipsec_ipproto_all
}
tipsec_ipproto_t;

/**List of IPSec IPSec algorithms.
**/
typedef enum tipsec_alg_e {
    //! "hmac-md5-96" algorithm.
    tipsec_alg_hmac_md5_96,
    //! "hmac-sha-1-96" algorithm.
    tipsec_alg_hmac_sha_1_96
}
tipsec_alg_t;

/**List of supported IPSec encryption algorithms.
**/
typedef enum tipsec_ealg_e {
    //! "des-ede3-cbc" encryption algorithm.
    tipsec_ealg_des_ede3_cbc,
    //! "aes" encryption algorithm.
    tipsec_ealg_aes,
    //! "null" encryption algorithm.
    tipsec_ealg_null
}
tipsec_ealg_t;

/** List of IPSec states.
**/
typedef enum tipsec_state_e {
    //! The default state. At this state no SA is created. It's the first and default state.
    tipsec_state_initial,
    //! Partial state. At this state only inbound SAs (with their SPIs) have been created.
    tipsec_state_inbound,
    //! Full state. At this state both inbound and outbound SAs have been create. It's the final state.
    tipsec_state_full,
    //! All SAs are in active mode.
    tipsec_state_active
}
tipsec_state_t;

/** List of supported IPSec errors
*/
typedef enum tipsec_error_e {
    tipsec_error_success = 0, /**< Success */
    tipsec_error_invalid_param, /**< Invalid parameter */
	tipsec_error_invalid_state, /**< Invalid state */
    tipsec_error_access_violation, /**< Access violation */
    tipsec_error_permission_denied, /**< Permission denied */
    tipsec_error_outofmemory, /**< Out of memory */
    tipsec_error_outofbound, /**< Out of bound */
    tipsec_error_notfound, /**< Not found */
    tipsec_error_notimplemented, /**< Not implemented */
	tipsec_error_sys, /**< System error */
}
tipsec_error_t;
