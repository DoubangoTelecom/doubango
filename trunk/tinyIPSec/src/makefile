APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS) -I$(DOUBANGO_HOME)/tinySAK/src
LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive

all: $(APP)

OBJS = tipsec_common.o\
	tipsec_racoon.o\
	tipsec_vista.o\
	tipsec_xp.o\
	tipsec.o

$(APP): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@
	
install: $(APP)
	$(ANDROID_SDK_ROOT)/tools/adb remount
	$(ANDROID_SDK_ROOT)/tools/adb push $(APP) $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)

shell:
	$(ANDROID_SDK_ROOT)/tools/adb shell

run:
	$(ANDROID_SDK_ROOT)/tools/adb shell $(INSTALL_DIR)/$(APP)

#dbg:
#	$(MAKE) $(MAKEFILE) DEBUG="-g -DDEBUG"
#	$(MAKE) $(MAKEFILE) install
	
clean:
	@rm -f $(OBJS) $(APP)