# CSharp
echo "CSharp..."
swig -c++ -csharp -outdir csharp -o csharp/tinyWRAP_wrap.cxx _common/tinyWRAP.i

# Java
echo "Java..."
swig -c++ -java -package org.doubango.tinyWRAP -outdir java -o java/tinyWRAP_wrap.cxx _common/tinyWRAP.i