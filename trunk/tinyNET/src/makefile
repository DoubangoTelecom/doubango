APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS) -I$(DOUBANGO_HOME)/tinySAK/src -I$(DOUBANGO_HOME)/$(PROJECT)/src
LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive -ltinySAK

all: $(APP)

OBJS = \
	tnet.o\
	tnet_auth.o\
	tnet_nat.o\
	tnet_poll.o\
	tnet_socket.o\
	tnet_transport.o\
	tnet_transport_poll.o\
	tnet_utils.o
	###################
	## DHCP
	###################
OBJS +=	dhcp/tnet_dhcp.o\
	dhcp/tnet_dhcp_message.o\
	dhcp/tnet_dhcp_option.o\
	dhcp/tnet_dhcp_option_sip.o
	###################
	## DHCPv6
	###################
OBJS +=	dhcp6/tnet_dhcp6.o\
	dhcp6/tnet_dhcp6_duid.o\
	dhcp6/tnet_dhcp6_message.o\
	dhcp6/tnet_dhcp6_option.o
	###################
	## DDNS
	###################
OBJS +=	dns/tnet_dns.o\
	dns/tnet_dns_a.o\
	dns/tnet_dns_aaaa.o\
	dns/tnet_dns_cname.o\
	dns/tnet_dns_message.o\
	dns/tnet_dns_mx.o\
	dns/tnet_dns_naptr.o\
	dns/tnet_dns_ns.o\
	dns/tnet_dns_opt.o\
	dns/tnet_dns_ptr.o\
	dns/tnet_dns_rr.o\
	dns/tnet_dns_soa.o\
	dns/tnet_dns_srv.o\
	dns/tnet_dns_txt.o
	###################
	## ICE
	###################
OBJS +=	ice/tnet_ice.o
	
	###################
	## STUN
	###################
OBJS +=	stun/tnet_stun.o\
	stun/tnet_stun_attribute.o\
	stun/tnet_stun_message.o
	###################
	## TLS
	###################
OBJS +=	tls/tnet_tls.o
	###################
	## TURN
	###################
OBJS +=	turn/tnet_turn.o\
	turn/tnet_turn_attribute.o\
	turn/tnet_turn_message.o

$(APP): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@
	
install: $(APP)
	$(ANDROID_SDK_ROOT)/tools/adb remount
	$(ANDROID_SDK_ROOT)/tools/adb push $(APP) $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)
	
clean:
	@rm -f $(OBJS) $(APP)