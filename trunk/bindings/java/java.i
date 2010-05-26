/* File : java.i 
* http://www.swig.org/Doc1.3/Java.html
*/

// http://www.swig.org/Doc1.3/Java.html#enumerations
%include "enums.swg"
%javaconst(1);


// Mapping void* as byte[]
%typemap(jni) void * "void *"
%typemap(jtype) void * "byte[]"
%typemap(jstype) void * "byte[]"
%typemap(javain) void * "$javainput"
%typemap(javaout) void * { return $jnicall; }
%typemap(in) void * %{ $1 = $input; %}
%typemap(out) void * %{ $result = $1; %}
%typemap(javadirectorin) void * "$jniinput"


%include ../_common/tinyWRAP.i
