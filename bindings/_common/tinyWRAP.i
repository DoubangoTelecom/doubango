/* File : tinyWRAP.i */
%module(directors="1") tinyWRAP
%include "typemaps.i"
%include <stdint.i>

%{
#include "tinyWRAP_config.h"
#include "DDebug.h"
#include "AudioResampler.h"
%}

%feature("director") DDebugCallback;

%nodefaultctor;
%include "tinyWRAP_config.h"
%include "DDebug.h"
%include "AudioResampler.h"
%include "Common.h"
%clearnodefaultctor;





/* ========== Sip/Sdp Stack ========== */
%newobject getSipHeaderValue;
%newobject getSdpHeaderValue;
%newobject getSdpHeaderAValue;
%newobject getSipHeaderParamValue;
%newobject SipStack::dnsENUM;
%newobject SipStack::dnsNaptrSrv;
%newobject SipStack::dnsSrv;
%newobject SipStack::getPreferredIdentity;
%newobject SipStack::getLocalIPnPort;
%newobject MediaSessionMgr::producerGetCodec;

%newobject MessagingEvent::takeSessionOwnership;
%newobject InviteEvent::takeCallSessionOwnership;
%newobject InviteEvent::takeMsrpSessionOwnership;
%newobject RegistrationEvent::takeSessionOwnership;

%newobject ProxyPluginMgr::createInstance;

%newobject MediaContent::parse;

%include SipStack.i


/* ========== Xcap Stack ========== */
%newobject XcapSelector::getString;

%newobject getXcapHeaderValue;
%newobject getXcapHeaderParamValue;

%include Xcap.i


/* ========== SMS ========== */
%newobject SMSEncoder::encodeSubmit;
%newobject SMSEncoder::encodeDeliver;
%newobject SMSEncoder::encodeACK;
%newobject SMSEncoder::encodeError;
%newobject SMSEncoder::decode;

%include SMS.i


/* ========== MSRP ========== */
%newobject getMsrpHeaderValue;
%newobject getMsrpHeaderParamValue;

%include Msrp.i
