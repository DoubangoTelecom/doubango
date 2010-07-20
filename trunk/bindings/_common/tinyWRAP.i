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

%newobject MessagingEvent::takeSessionOwnership;
%newobject CallEvent::takeSessionOwnership;

%include SipStack.i


/* ========== Xcap Stack ========== */
%newobject XcapSelector::getString;

%newobject getXcapHeaderValue;
%newobject getXcapHeaderParamValue;

%include Xcap.i