# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

# SDP Message parser
ragel.exe $OPTIONS -o ./src/parsers/tsdp_parser_message.c ./ragel/tsdp_parser_message.rl



# ==A
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_A.c ./ragel/tsdp_parser_header_A.rl

# ==B
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_B.c ./ragel/tsdp_parser_header_B.rl

# ==C
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_C.c ./ragel/tsdp_parser_header_C.rl

# ==Dummy
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_Dummy.c ./ragel/tsdp_parser_header_Dummy.rl

# ==E
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_E.c ./ragel/tsdp_parser_header_E.rl

# ==I
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_I.c ./ragel/tsdp_parser_header_I.rl

# ==K
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_K.c ./ragel/tsdp_parser_header_K.rl

# ==M
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_M.c ./ragel/tsdp_parser_header_M.rl

# ==O
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_O.c ./ragel/tsdp_parser_header_O.rl

# ==P
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_P.c ./ragel/tsdp_parser_header_P.rl

# ==R
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_R.c ./ragel/tsdp_parser_header_R.rl

# ==S
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_S.c ./ragel/tsdp_parser_header_S.rl

# ==T
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_T.c ./ragel/tsdp_parser_header_T.rl

# ==U
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_U.c ./ragel/tsdp_parser_header_U.rl

# ==V
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_V.c ./ragel/tsdp_parser_header_V.rl

# ==Z
ragel.exe $OPTIONS -o ./src/headers/tsdp_header_Z.c ./ragel/tsdp_parser_header_Z.rl

