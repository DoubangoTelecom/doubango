# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

# SDP Message parser
ragel.exe $OPTIONS -o ./src/content/tmedia_content_cpim.c ./ragel/tmedia_content_cpim.rl

# 'image-attr' (RFC 6236)
ragel.exe $OPTIONS -o ./src/tmedia_imageattr.c ./ragel/tmedia_imageattr.rl