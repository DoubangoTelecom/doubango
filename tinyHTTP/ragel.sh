# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

#export OPTIONS="-C -L -G2"
export OPTIONS="-C -L -T0"

# HTTP/HTTPS URL parser
ragel.exe $OPTIONS -o ./src/parsers/thttp_parser_url.c ./ragel/thttp_parser_url.rl

# HTTP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ./src/parsers/thttp_parser_message.c ./ragel/thttp_parser_message.rl

# HTTP headers parser
ragel.exe $OPTIONS -o ./src/parsers/thttp_parser_header.c ./ragel/thttp_parser_header.rl




# ==Authorization
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Authorization.c ./ragel/thttp_parser_header_Authorization.rl

# ==Content-Length
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Content_Length.c ./ragel/thttp_parser_header_Content_Length.rl

# ==Content-Type
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Content_Type.c ./ragel/thttp_parser_header_Content_Type.rl

# ==Dummy
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Dummy.c ./ragel/thttp_parser_header_Dummy.rl

# ==ETag
ragel.exe $OPTIONS -o ./src/headers/thttp_header_ETag.c ./ragel/thttp_parser_header_ETag.rl

# ==Sec_WebSocket_Accept
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Sec_WebSocket_Accept.c ./ragel/thttp_parser_header_Sec_WebSocket_Accept.rl

# ==Sec_WebSocket_Key
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Sec_WebSocket_Key.c ./ragel/thttp_parser_header_Sec_WebSocket_Key.rl

# ==Sec_WebSocket_Protocol
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Sec_WebSocket_Protocol.c ./ragel/thttp_parser_header_Sec_WebSocket_Protocol.rl

# ==Sec_WebSocket_Version
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Sec_WebSocket_Version.c ./ragel/thttp_parser_header_Sec_WebSocket_Version.rl

# == Transfer-Encoding
ragel.exe $OPTIONS -o ./src/headers/thttp_header_Transfer_Encoding.c ./ragel/thttp_parser_header_Transfer_Encoding.rl

# ==WWW-Authenticate
ragel.exe $OPTIONS -o ./src/headers/thttp_header_WWW_Authenticate.c ./ragel/thttp_parser_header_WWW_Authenticate.rl