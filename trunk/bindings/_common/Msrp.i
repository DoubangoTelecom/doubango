%{
#include "Msrp.h"
%}

/* Callbacks */
%feature("director") MsrpCallback;


%nodefaultctor;
%include "Msrp.h"
%clearnodefaultctor;
