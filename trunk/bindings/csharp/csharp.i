/* File : csharp.i 
* http://www.swig.org/Doc1.3/CSharp.html
*/


// Mapping void* as byte[]
%typemap(ctype) void * "void *"
%typemap(imtype) void * "byte[]"
%typemap(cstype) void * "byte[]"
%typemap(csin) void * "$csinput"
%typemap(csout) void * { return $imcall; }
%typemap(in) void * %{ $1 = $input; %}
%typemap(out) void * %{ $result = $1; %}
%typemap(csdirectorin) void * "$iminput"

%include ../_common/tinyWRAP.i
