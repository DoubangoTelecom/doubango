/* File : java.i 
* http://www.swig.org/Doc1.3/Java.html
*/

// http://www.swig.org/Doc1.3/Java.html#enumerations
%include "enums.swg"
%javaconst(1);


//================== Mapping void* (Java to C) as ByteBuffer
%typemap(jni) void * "jbyteArray"
%typemap(jtype) void * "java.nio.ByteBuffer"
%typemap(jstype) void * "java.nio.ByteBuffer"
%typemap(javain) void * "$javainput"
%typemap(javaout) void * { return $jnicall; }

// (From Java to C)
%typemap(in) void * %{ 
	$1 = jenv->GetDirectBufferAddress($input); 
%}

// (From C to Java)
//%typemap(out) void * %{
//	$result = $1; 
//%}
%typemap(javadirectorin) void * "$jniinput"
//==================


%typemap(javacode) SipMessage %{
  public byte[] getSipContent() {
    final int clen = (int)this.getSipContentLength();
    if(clen>0){
		final java.nio.ByteBuffer buffer = java.nio.ByteBuffer.allocateDirect(clen);
        final int read = (int)this.getSipContent(buffer, clen);
        final byte[] bytes = new byte[read];
        buffer.get(bytes, 0, read);
        return bytes;
    }
    return null;
  }
%}

%typemap(javacode) SipSession %{
  protected java.nio.ByteBuffer getByteBuffer(byte[] bytes) {
    if(bytes != null){
		final java.nio.ByteBuffer byteBuffer = java.nio.ByteBuffer.allocateDirect(bytes.length);
        byteBuffer.put(bytes);
        return byteBuffer;
    }
    return null;
  }
%}

%typemap(javacode) PublicationSession %{
  public boolean Publish(byte[] bytes) {
    if(bytes != null){
		final java.nio.ByteBuffer byteBuffer = this.getByteBuffer(bytes);
        return this.publish(byteBuffer, bytes.length);
    }
    return false;
  }
%}

%typemap(javacode) XcapMessage %{
  public byte[] getXcapContent() {
    final int clen = (int)this.getXcapContentLength();
    if(clen>0){
		final java.nio.ByteBuffer buffer = java.nio.ByteBuffer.allocateDirect(clen);
        final int read = (int)this.getXcapContent(buffer, clen);
        final byte[] bytes = new byte[read];
        buffer.get(bytes, 0, read);
        return bytes;
    }
    return null;
  }
%}

%typemap(javacode) T140CallbackData %{
  public byte[] getData() {
    final int size = (int)this.getSize();
    if(size > 0){
		final java.nio.ByteBuffer buffer = java.nio.ByteBuffer.allocateDirect(size);
        final int read = (int)this.getData(buffer, size);
        final byte[] bytes = new byte[read];
        buffer.get(bytes, 0, read);
        return bytes;
    }
    return null;
  }
%}

%include ../_common/tinyWRAP.i