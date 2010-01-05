# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

#export OPTIONS="-C -L -T0 -F1 -G2"
export OPTIONS="-C -L"

# SIP/SIPS/TEL URI parser
#ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_uri.c tsip_parser_uri.rl

# SIP message (both requests an responses) parser.
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_message.c tsip_parser_message.rl

# SIP headers parser
ragel.exe $OPTIONS -o ../source/parsers/tsip_parser_header.c tsip_parser_header.rl


# ==Allow
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Allow.c tsip_parser_header_Allow.rl

# ==Allow-Events
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Allow_Events.c tsip_parser_header_Allow_Events.rl

# ==Authorization
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Authorization.c tsip_parser_header_Authorization.rl

# ==Call-ID
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Call_ID.c tsip_parser_header_Call_ID.rl

# ==Content-Length
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Content_Length.c tsip_parser_header_Content_Length.rl

# ==Contact
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Contact.c tsip_parser_header_Contact.rl

# ==CSeq
ragel.exe $OPTIONS -o ../source/headers/tsip_header_CSeq.c tsip_parser_header_CSeq.rl

# ==Expires
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Expires.c tsip_parser_header_Expires.rl

# ==From
ragel.exe $OPTIONS -o ../source/headers/tsip_header_From.c tsip_parser_header_From.rl

# ==Max-Forwards
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Max_Forwards.c tsip_parser_header_Max_Forwards.rl

# ==Min-Expires
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Min_Expires.c tsip_parser_header_Min_Expires.rl

# ==P-Access-Network-Info
ragel.exe $OPTIONS -o ../source/headers/tsip_header_P_Access_Network_Info.c tsip_parser_header_P_Access_Network_Info.rl

# ==P-Preferred-Identity
ragel.exe $OPTIONS -o ../source/headers/tsip_header_P_Preferred_Identity.c tsip_parser_header_P_Preferred_Identity.rl

# ==Privacy
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Privacy.c tsip_parser_header_Privacy.rl

# ==Proxy-Authenticate
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Proxy_Authenticate.c tsip_parser_header_Proxy_Authenticate.rl

# ==Proxy-Authorization
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Proxy_Authorization.c tsip_parser_header_Proxy_Authorization.rl

# ==Require
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Require.c tsip_parser_header_Require.rl

# ==Supported
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Supported.c tsip_parser_header_Supported.rl

# ==To
ragel.exe $OPTIONS -o ../source/headers/tsip_header_To.c tsip_parser_header_To.rl

# ==User-Agent
ragel.exe $OPTIONS -o ../source/headers/tsip_header_User_Agent.c tsip_parser_header_User_Agent.rl

# ==Via
ragel.exe $OPTIONS -o ../source/headers/tsip_header_Via.c tsip_parser_header_Via.rl

# ==WWW-Authenticate
ragel.exe $OPTIONS -o ../source/headers/tsip_header_WWW_Authenticate.c tsip_parser_header_WWW_Authenticate.rl
