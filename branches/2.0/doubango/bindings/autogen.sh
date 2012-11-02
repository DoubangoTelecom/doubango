##### CSharp
echo "--->CSharp...<---"
swig -c++ -csharp -namespace org.doubango.tinyWRAP -outdir csharp -o csharp/tinyWRAP_wrap.cxx csharp/csharp.i

##### Objective-C
#echo "--->Objective-C...<---"
#swig -c++ -objc -outdir objc -o -objc/tinyWRAP_wrap.cxx -objc/-objc.i


##### Java
echo "--->Java...<---"
swig -c++ -java -package org.doubango.tinyWRAP -outdir java -o java/tinyWRAP_wrap.cxx java/java.i
echo "Java(Google Dalvik)..."
echo "Google Android special tasks"
swig -c++ -java -package org.doubango.tinyWRAP -outdir java/android -o java/android/tinyWRAP_wrap.cxx java/java.i
sed -i 's/dynamic_cast/static_cast/g' java/android/tinyWRAP_wrap.cxx
#sed -i 's/AttachCurrentThread((void \*\*)/AttachCurrentThread((JNIEnv \*\*)/g' java/android/tinyWRAP_wrap.cxx
#sed -i 's/AttachCurrentThreadAsDaemon((void \*\*)/AttachCurrentThreadAsDaemon((JNIEnv \*\*)/g' java/android/tinyWRAP_wrap.cxx
#for f in SipCallback DDebugCallback ProxyPluginMgrCallback ProxyAudioConsumerCallback ProxyAudioProducerCallback ProxyVideoProducerCallback ProxyVideoConsumerCallback XcapCallback  MsrpCallback T140Callback
#do
#	sed -i 's/_director_connect(this, swigCPtr, swigCMemOwn, true)/_director_connect(this, swigCPtr, swigCMemOwn, false)/g' java/android/$f.java
#done

##### Python
echo "--->Python...<---"
swig -c++ -python -outdir python -o python/tinyWRAP_wrap.cxx python/python.i

##### Perl
echo "--->Perl...<---"
swig -c++ -perl -outdir perl -o Perl/tinyWRAP_wrap.cxx perl/perl.i

##### Ruby
echo "--->Ruby...<---"
#swig -c++ -ruby -outdir Ruby -o Ruby/tinyWRAP_wrap.cxx ruby/ruby.i