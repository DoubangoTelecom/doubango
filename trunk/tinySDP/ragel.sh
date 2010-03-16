# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

# SDP Message parser
ragel.exe $OPTIONS -o ../src/parsers/tsdp_parser_message.c tsdp_parser_message.rl



# ==A
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_A.c tsdp_parser_header_A.rl

# ==B
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_B.c tsdp_parser_header_B.rl

# ==C
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_C.c tsdp_parser_header_C.rl

# ==Dummy
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_Dummy.c tsdp_parser_header_Dummy.rl

# ==E
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_E.c tsdp_parser_header_E.rl

# ==I
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_I.c tsdp_parser_header_I.rl

# ==K
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_K.c tsdp_parser_header_K.rl

# ==M
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_M.c tsdp_parser_header_M.rl

# ==O
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_O.c tsdp_parser_header_O.rl

# ==P
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_P.c tsdp_parser_header_P.rl

# ==R
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_R.c tsdp_parser_header_R.rl

# ==S
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_S.c tsdp_parser_header_S.rl

# ==T
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_T.c tsdp_parser_header_T.rl

# ==U
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_U.c tsdp_parser_header_U.rl

# ==V
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_V.c tsdp_parser_header_V.rl

# ==Z
ragel.exe $OPTIONS -o ../src/headers/tsdp_header_Z.c tsdp_parser_header_Z.rl

