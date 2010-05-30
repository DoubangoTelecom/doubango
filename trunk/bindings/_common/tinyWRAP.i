/* File : tinyWRAP.i */
%module(directors="1") tinyWRAP

%newobject getSipHeaderValue;
%newobject getSipHeaderParamValue;
// don't seem to work
%newobject takeSessionOwnership;


/* Sip Stack */
%include SipStack.i

/* Xcap Stack */
%include XcapStack.i