# Ragel generator
# For more information about Ragel: http://www.complang.org/ragel/

export OPTIONS="-C -L -T0"
#export OPTIONS="-C -L -G2"

ragel.exe $OPTIONS -o ./src/dns/tnet_dns_regexp.c ./ragel/tnet_dns_regexp.rl
ragel.exe $OPTIONS -o ./src/dns/tnet_dns_resolvconf.c ./ragel/tnet_dns_resolvconf.rl
