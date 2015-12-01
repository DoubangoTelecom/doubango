swig -c++ -java -package org.doubango.utils -outdir ./java -o ./utils_wrap.cxx ./api.i

sed -i 's/dynamic_cast/static_cast/g' ./utils_wrap.cxx
sed -i 's/AttachCurrentThread((void \*\*)/AttachCurrentThread((JNIEnv \*\*)/g' ./utils_wrap.cxx
sed -i 's/AttachCurrentThreadAsDaemon((void \*\*)/AttachCurrentThreadAsDaemon((JNIEnv \*\*)/g' ./utils_wrap.cxx