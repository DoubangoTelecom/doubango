/* File : tinyWRAP.i */
%module(directors="1") tinyWRAP

%newobject getSipHeaderValue;
%newobject getSipHeaderParamValue;

/* Sip Stack */
%include SipStack.i

/* Xcap Stack */
%include XcapStack.i