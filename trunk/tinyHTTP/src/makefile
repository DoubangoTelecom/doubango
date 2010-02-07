APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS) -I$(DOUBANGO_HOME)/tinySAK/src -I$(DOUBANGO_HOME)/$(PROJECT)/include
LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive -ltinySAK

all: $(APP)

OBJS = \
	thttp.o\
	thttp_message.o\
	thttp_utl.o
	###################
	## auth
	###################
OBJS +=	auth/thttp_auth.o
	###################
	## headers
	###################

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