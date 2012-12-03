%module utils
%module(directors="1") utils

%include "enums.swg"
%include <stdint.i>
%javaconst(1);

%{
#include "api.h"
%}

%nodefaultctor;
%include "api.h"
%clearnodefaultctor;
