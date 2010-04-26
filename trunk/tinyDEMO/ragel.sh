#export OPTIONS="-C -L -G2"
export OPTIONS="-C -L -T0"

ragel.exe $OPTIONS -o ./cmd_parser.c ./cmd_parser.rl