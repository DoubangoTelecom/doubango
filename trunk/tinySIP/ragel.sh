# Ragel generator

# SIP message (both requests an responses) parser.
ragel.exe -C -L -F1 -G2 -o ../source/parsers/tsip_parser_message.c tsip_parser_message.rl

# SIP headers parser
ragel.exe -C -L -F1 -G2 -o ../source/parsers/tsip_parser_header.c tsip_parser_header.rl

# ==Via
ragel.exe -C -L -F1 -G2 -o ../source/headers/tsip_header_Via.c tsip_parser_header_Via.rl