lib_LTLIBRARIES         = libtinyNET.la
libtinyNET_la_LIBADD = ../tinySAK/libtinySAK.la
libtinyNET_la_CPPFLAGS = -I../tinySAK/src -I./src

if USE_SSL
libtinyNET_la_LIBADD += ${LIBSSL_LIBADD}
libtinyNET_la_CPPFLAGS += -DHAVE_OPENSSL=1
endif

if USE_RESOLV
libtinyNET_la_LIBADD += ${LIBRESOLV_LIBADD}
endif

libtinyNET_la_SOURCES  = \
	src/tnet.c\
	src/tnet_auth.c\
	src/tnet_endianness.c\
	src/tnet_nat.c\
	src/tnet_proxy_node_socks_plugin.c\
	src/tnet_proxy_plugin.c\
	src/tnet_proxydetect.c\
	src/tnet_poll.c\
	src/tnet_socket.c\
	src/tnet_transport.c\
	src/tnet_transport_poll.c\
	src/tnet_utils.c
	
libtinyNET_la_SOURCES +=	src/dhcp/tnet_dhcp.c\
	src/dhcp/tnet_dhcp_message.c\
	src/dhcp/tnet_dhcp_option.c\
	src/dhcp/tnet_dhcp_option_sip.c

libtinyNET_la_SOURCES +=	src/dhcp6/tnet_dhcp6.c\
	src/dhcp6/tnet_dhcp6_duid.c\
	src/dhcp6/tnet_dhcp6_message.c\
	src/dhcp6/tnet_dhcp6_option.c

libtinyNET_la_SOURCES +=	src/dns/tnet_dns.c\
	src/dns/tnet_dns_a.c\
	src/dns/tnet_dns_aaaa.c\
	src/dns/tnet_dns_cname.c\
	src/dns/tnet_dns_message.c\
	src/dns/tnet_dns_mx.c\
	src/dns/tnet_dns_naptr.c\
	src/dns/tnet_dns_ns.c\
	src/dns/tnet_dns_opt.c\
	src/dns/tnet_dns_ptr.c\
	src/dns/tnet_dns_regexp.c\
	src/dns/tnet_dns_resolvconf.c\
	src/dns/tnet_dns_rr.c\
	src/dns/tnet_dns_soa.c\
	src/dns/tnet_dns_srv.c\
	src/dns/tnet_dns_txt.c
	
libtinyNET_la_SOURCES +=	src/ice/tnet_ice_candidate.c\
	src/ice/tnet_ice_ctx.c\
	src/ice/tnet_ice_event.c\
	src/ice/tnet_ice_pair.c\
	src/ice/tnet_ice_utils.c
	
libtinyNET_la_SOURCES +=	src/stun/tnet_stun.c\
	src/stun/tnet_stun_attr.c\
	src/stun/tnet_stun_binding.c\
	src/stun/tnet_stun_pkt.c\
	src/stun/tnet_stun_utils.c\
	\
	src/stun/tnet_stun_attribute.c\
	src/stun/tnet_stun_message.c
	
libtinyNET_la_SOURCES +=	src/tls/tnet_tls.c\
	src/tls/tnet_dtls.c
	
libtinyNET_la_SOURCES +=	src/turn/tnet_turn.c\
	src/turn/tnet_turn_session.c\
	\
	src/turn/tnet_turn_attribute.c\
	src/turn/tnet_turn_message.c
	

libtinyNET_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyNET_la_LDFLAGS += -static
endif

src_includedir = $(includedir)/tinynet
src_include_HEADERS = src/*.h
dhcp_includedir = $(includedir)/tinynet/dhcp
dhcp_include_HEADERS = src/dhcp/*.h
dhcp6_includedir = $(includedir)/tinynet/dhcp6
dhcp6_include_HEADERS = src/dhcp6/*.h
dns_includedir = $(includedir)/tinynet/dns
dns_include_HEADERS = src/dns/*.h
ice_includedir = $(includedir)/tinynet/ice
ice_include_HEADERS = src/ice/*.h
stun_includedir = $(includedir)/tinynet/stun
stun_include_HEADERS = src/stun/*.h
tls_includedir = $(includedir)/tinynet/tls
tls_include_HEADERS = src/tls/*.h
turn_includedir = $(includedir)/tinynet/turn
turn_include_HEADERS = src/turn/*.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyNET.pc
