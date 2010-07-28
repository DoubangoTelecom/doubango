/* File : tinyWRAP.i */
%module(directors="1") tinyWRAP
%include "typemaps.i"

%{
#include "DDebug.h"
%}

%nodefaultctor;
%include "DDebug.h"
%clearnodefaultctor;


%feature("director") DDebugCallback;


/* ========== Sip Stack ========== */
%newobject getSipHeaderValue;
%newobject getSipHeaderParamValue;
%newobject SipStack::dnsENUM;
%newobject SipStack::dnsNaptrSrv;
%newobject SipStack::dnsSrv;

%newobject MessagingEvent::takeSessionOwnership;
%newobject CallEvent::takeSessionOwnership;

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