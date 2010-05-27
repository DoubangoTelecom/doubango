/* File : csharp.i 
* http://www.swig.org/Doc1.3/CSharp.html
*/


%define %cs_marshal_array(TYPE, CSTYPE)
%typemap(ctype) TYPE[] "void*"
%typemap(imtype,
inattributes="[MarshalAs(UnmanagedType.LPArray)]") TYPE[] "CSTYPE[]"
%typemap(cstype) TYPE[] "CSTYPE[]"
%typemap(in) TYPE[] %{ $1 = (TYPE*)$input; %}
%typemap(csin) TYPE[] "$csinput"
%enddef

// Mapping void* as byte[]
%typemap(ctype) void * "void *"
%typemap(imtype) void * "byte[]"
%typemap(cstype) void * "byte[]"
%typemap(csin) void * "$csinput"
%typemap(csout) void * { return $imcall; }
%typemap(in) void * %{ $1 = $input; %}
%typemap(out) void * %{ $result = $1; %}
%typemap(csdirectorin) void * "$iminput"



%typemap(cscode) SipMessage %{
  public byte[] getSipContent() {
    uint clen = this.getSipContentLength();
    if(clen>0){
        byte[] bytes = new byte[clen];
        this.getSipContent(bytes, clen);
        return bytes;
    }
    return null;
  }
%}

%include ../_common/tinyWRAP.i
