# Ragel generator

#export OPTIONS="-C -L -T0 -F1 -G2"
export OPTIONS="-C -L"

# SIP/SIPS/TEL URI parser
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_uri.c tsip_parser_uri.rl

# SIP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_message.c tsip_parser_message.rl

# SIP headers parser
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_header.c tsip_parser_header.rl


# ==Allow
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Allow.c tsip_parser_header_Allow.rl

# ==Call-ID
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Call_ID.c tsip_parser_header_Call_ID.rl

# ==CSeq
ragel.exe $OPTIONS -o ../source/headers/tsip_header_CSeq.c tsip_parser_header_CSeq.rl

# ==From
ragel.exe $OPTIONS -o ../source/headers/tsip_header_From.c tsip_parser_header_From.rl

# ==Max_Forwards
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Max_Forwards.c tsip_parser_header_Max_Forwards.rl

# ==Require
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Require.c tsip_parser_header_Require.rl

# ==Supported
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Supported.c tsip_parser_header_Supported.rl

# ==To
ragel.exe $OPTIONS -o ../source/headers/tsip_header_To.c tsip_parser_header_To.rl

# ==Via
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Via.c tsip_parser_header_Via.rl