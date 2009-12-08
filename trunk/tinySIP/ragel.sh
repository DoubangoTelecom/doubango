# Ragel generator

#export OPTIONS="-C -L -T0 -F1 -G2"
export OPTIONS="-C -L"

# SIP/SIPS/TEL URI parser
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_uri.c tsip_parser_uri.rl

# SIP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_message.c tsip_parser_message.rl

# SIP headers parser
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_header.c tsip_parser_header.rl




# ==From
ragel.exe $OPTIONS -o ../source/headers/tsip_header_From.c tsip_parser_header_From.rl

# ==Via
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Via.c tsip_parser_header_Via.rl