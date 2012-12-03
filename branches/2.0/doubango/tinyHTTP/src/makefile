APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS) -I$(DOUBANGO_HOME)/tinySAK/src -I$(DOUBANGO_HOME)/tinyNET/src -I$(DOUBANGO_HOME)/$(PROJECT)/include 
LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive -ltinySAK -ltinyNET

all: $(APP)

OBJS = \
	thttp.o\
	thttp_message.o\
	thttp_operation.o\
	thttp_transport.o\
	thttp_url.o
	###################
	## auth
	###################
OBJS +=	auth/thttp_auth.o
	###################
	## headers
	###################
OBJS +=	headers/thttp_header.o\
	headers/thttp_header_Authorization.o\
	headers/thttp_header_Content_Length.o\
	headers/thttp_header_Content_Type.o\
	headers/thttp_header_Dummy.o\
	headers/thttp_header_Proxy_Authenticate.o\
	headers/thttp_header_WWW_Authenticate.o
	###################
	## parsers
	###################
OBJS +=	parsers/thttp_parser_header.o\
	parsers/thttp_parser_message.o\
	parsers/thttp_parser_url.o
	
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