# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

# msrp Message parser
ragel.exe $OPTIONS -o ./src/parsers/tmsrp_parser_message.c ./ragel/tmsrp_parser_message.rl
# msrp URI
ragel.exe $OPTIONS -o ./src/parsers/tmsrp_parser_uri.c ./ragel/tmsrp_parser_uri.rl


# ==Authentication-Info.h
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Authentication-Info.c ./ragel/tmsrp_parser_header_Authentication-Info.rl

# ==Authorization
# Ignored

# ==Byte-Range
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Byte-Range.c ./ragel/tmsrp_parser_header_Byte-Range.rl

# ==Content-Type
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Content-Type.c ./ragel/tmsrp_parser_header_Content-Type.rl

# ==Dummy
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Dummy.c ./ragel/tmsrp_parser_header_Dummy.rl

# ==Expires
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Expires.c ./ragel/tmsrp_parser_header_Expires.rl

# ==Failure-Report
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Failure-Report.c ./ragel/tmsrp_parser_header_Failure-Report.rl

# ==From-Path
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_From-Path.c ./ragel/tmsrp_parser_header_From-Path.rl

# ==Max-Expires
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Max-Expires.c ./ragel/tmsrp_parser_header_Max-Expires.rl

# ==Message-ID
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Message-ID.c ./ragel/tmsrp_parser_header_Message-ID.rl

# ==Min-Expires
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Min-Expires.c ./ragel/tmsrp_parser_header_Min-Expires.rl

# ==Status
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Status.c ./ragel/tmsrp_parser_header_Status.rl

# ==Success-Report
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Success-Report.c ./ragel/tmsrp_parser_header_Success-Report.rl

# ==To-Path
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_To-Path.c ./ragel/tmsrp_parser_header_To-Path.rl

# ==Use-Path
ragel.exe $OPTIONS -o ./src/headers/tmsrp_header_Use-Path.c ./ragel/tmsrp_parser_header_Use-Path.rl

# ==WWW-Authenticate
# Ignored

