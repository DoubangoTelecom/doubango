# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

# HTTP/HTTPS URL parser
ragel.exe $OPTIONS -o ../src/parsers/thttp_parser_url.c thttp_parser_url.rl

# HTTP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ../src/parsers/thttp_parser_message.c thttp_parser_message.rl

# HTTP headers parser
ragel.exe $OPTIONS -o ../src/parsers/thttp_parser_header.c thttp_parser_header.rl




# ==Authorization
ragel.exe $OPTIONS -o ../src/headers/thttp_header_Authorization.c thttp_parser_header_Authorization.rl

# ==Content-Length
ragel.exe $OPTIONS -o ../src/headers/thttp_header_Content_Length.c thttp_parser_header_Content_Length.rl

# ==Dummy
ragel.exe $OPTIONS -o ../src/headers/thttp_header_Dummy.c thttp_parser_header_Dummy.rl

# ==Content-Type
ragel.exe $OPTIONS -o ../src/headers/thttp_header_Content_Type.c thttp_parser_header_Content_Type.rl

# ==WWW-Authenticate
ragel.exe $OPTIONS -o ../src/headers/thttp_header_WWW_Authenticate.c thttp_parser_header_WWW_Authenticate.rl