#!/bin/sh

# A file with a '.C' (uppercase C) extension will be considered to be a C++ module instead of a C module
# As 'libg729a' will be linked to a 'C' library we must change the extension or use [export "C"] on all
# public function. I prefer the first solution.

for f in `ls *.C`
do
  echo "Rename file from ${f%.*}.C -to-> ${f%.*}.c"
  mv ${f%.*}.C ${f%.*}.c
done
