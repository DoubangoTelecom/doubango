/* File : tinyWRAP.i */
%module(directors="1") tinyWRAP

//%apply int *OUTPUT { int *len };

%newobject getSipHeaderValue;

/* Sip Stack */
%include SipStack.i

/* Xcap Stack */
%include XcapStack.i