APP := test

CFLAGS := $(CFLAGS) -I$(DOUBANGO_HOME)/$(PROJECT)/src
LDFLAGS := $(LDFLAGS) -Bsymbolic -l$(PROJECT)

all: $(APP)

OBJS += $(APP).o

$(APP): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@
	
install: $(APP)
	$(ANDROID_SDK_ROOT)/tools/adb push $(APP) $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)

run:
	$(ANDROID_SDK_ROOT)/tools/adb shell $(INSTALL_DIR)/$(APP)

#dbg:
#	$(MAKE) $(MAKEFILE) DEBUG="-g -DDEBUG"
#	$(MAKE) $(MAKEFILE) install
	
clean:
	@rm -f $(OBJS) $(APP)