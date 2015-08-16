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

// Mapping void* as IntPtr
%typemap(ctype)  void * "void *"
%typemap(imtype) void * "IntPtr"
%typemap(cstype) void * "IntPtr"
%typemap(csin)   void * "$csinput"
%typemap(in)     void * %{ $1 = $input; %}
%typemap(out)    void * %{ $result = $1; %}
%typemap(csout)  void * { return $imcall; }
%typemap(csdirectorin) void * "$iminput"


//======== SipMessage ========//
%typemap(cscode) SipMessage %{
  public byte[] getSipContent() {
    uint clen = this.getSipContentLength();
    if(clen>0){        
        IntPtr ptr = Marshal.AllocHGlobal((int)clen);
        this.getSipContent(ptr, clen);
        byte[] bytes = new byte[clen];
        Marshal.Copy(ptr, bytes, 0, bytes.Length);
        Marshal.FreeHGlobal(ptr);
        return bytes;
    }
    return null;
  }
%}

//======== MessagingSession ========//
%typemap(cscode) MessagingSession %{
  public bool send(byte[] buffer) {
	IntPtr ptr = Marshal.AllocHGlobal(buffer.Length);
    Marshal.Copy(buffer, 0, ptr, buffer.Length);
    bool ret = this.send(ptr, (uint)buffer.Length);
    Marshal.FreeHGlobal(ptr);
    return ret;
  }
%}

//======== InfoSession ========//
%typemap(cscode) InfoSession %{
public bool send(byte[] buffer, ActionConfig config) {
	IntPtr ptr = Marshal.AllocHGlobal(buffer.Length);
    Marshal.Copy(buffer, 0, ptr, buffer.Length);
    bool ret = this.send(ptr, (uint)buffer.Length, config);
    Marshal.FreeHGlobal(ptr);
    return ret;
  }
%}

//======= MediaContent ========//
%typemap(cscode) MediaContent %{
  public byte[] getPayload() {
    uint clen = this.getPayloadLength();
    if(clen>0){
		IntPtr ptr = Marshal.AllocHGlobal((int)clen);
        this.getPayload(ptr, clen);
        byte[] bytes = new byte[clen];
        Marshal.Copy(ptr, bytes, 0, bytes.Length);
        Marshal.FreeHGlobal(ptr);
        return bytes;
    }
    return null;
  }
%}


//======= SMSData ========//
%typemap(cscode) SMSData %{
  public byte[] getPayload() {
    uint clen = this.getPayloadLength();
    if(clen>0){
		IntPtr ptr = Marshal.AllocHGlobal((int)clen);
        this.getPayload(ptr, clen);
        byte[] bytes = new byte[clen];
        Marshal.Copy(ptr, bytes, 0, bytes.Length);
        Marshal.FreeHGlobal(ptr);
        return bytes;
    }
    return null;
  }
%}

//======= RPMessage ========//
%typemap(cscode) RPMessage %{
  public byte[] getPayload() {
    uint clen = this.getPayloadLength();
    if(clen>0){
		IntPtr ptr = Marshal.AllocHGlobal((int)clen);
        this.getPayload(ptr, clen);
        byte[] bytes = new byte[clen];
        Marshal.Copy(ptr, bytes, 0, bytes.Length);
        Marshal.FreeHGlobal(ptr);
        return bytes;
    }
    return null;
  }
%}

//======== XcapStack ========//
%typemap(cscode) XcapStack %{
  public bool putElement(string url, byte[] payload) {
	IntPtr ptr = Marshal.AllocHGlobal(payload.Length);
    Marshal.Copy(payload, 0, ptr, payload.Length);
    bool ret = this.putElement(url, ptr, (uint)payload.Length);
    Marshal.FreeHGlobal(ptr);
    return ret;
  }
  
  public bool putAttribute(string url, byte[] payload) {
	IntPtr ptr = Marshal.AllocHGlobal(payload.Length);
    Marshal.Copy(payload, 0, ptr, payload.Length);
    bool ret = this.putAttribute(url, ptr, (uint)payload.Length);
    Marshal.FreeHGlobal(ptr);
    return ret;
  }
  
  public bool putDocument(string url, byte[] payload, string contentType) {
	IntPtr ptr = Marshal.AllocHGlobal(payload.Length);
    Marshal.Copy(payload, 0, ptr, payload.Length);
    bool ret = this.putDocument(url, ptr, (uint)payload.Length, contentType);
    Marshal.FreeHGlobal(ptr);
    return ret;
  }
%}


//======== Deprecated ========//
// @deprecated: to be replaced by "tmedia_codec_id_t" in Doubango 3.0
%typemap(csattributes) tdav_codec_id_e "[System.Obsolete(\"Deprecated and replaced by 'tmedia_codec_id_t'. Will be removed in Doubango 3.0.\", false)]"



%include ../_common/tinyWRAP.i
