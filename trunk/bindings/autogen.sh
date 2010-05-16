# CSharp
echo "CSharp..."
swig -c++ -csharp -outdir csharp -o csharp/tinyWRAP_wrap.cxx _common/tinyWRAP.i

# Java
echo "Java..."
swig -c++ -java -package org.doubango.tinyWRAP -outdir java -o java/tinyWRAP_wrap.cxx _common/tinyWRAP.i

# Python
echo "Python..."
swig -c++ -python -outdir python -o python/tinyWRAP_wrap.cxx _common/tinyWRAP.i

# Perl
echo "Perl..."
swig -c++ -perl -outdir perl -o Perl/tinyWRAP_wrap.cxx _common/tinyWRAP.i

# Ruby
echo "Ruby..."
#swig -c++ -ruby -outdir Ruby -o Ruby/tinyWRAP_wrap.cxx _common/tinyWRAP.i