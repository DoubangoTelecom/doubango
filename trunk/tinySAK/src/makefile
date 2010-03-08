APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS)
LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive

all: $(APP)

OBJS = tsk.o\
	tsk_base64.o\
	tsk_binaryutils.o\
	tsk_buffer.o\
	tsk_condwait.o\
	tsk_debug.o\
	tsk_fsm.o\
	tsk_hmac.o\
	tsk_list.o\
	tsk_md5.o\
	tsk_memory.o\
	tsk_mutex.o\
	tsk_object.o\
	tsk_params.o\
	tsk_ppfcs16.o\
	tsk_ppfcs32.o\
	tsk_ragel_state.o\
	tsk_runnable.o\
	tsk_safeobj.o\
	tsk_semaphore.o\
	tsk_sha1.o\
	tsk_string.o\
	tsk_thread.o\
	tsk_time.o\
	tsk_timer.o\
	tsk_url.o\
	tsk_uuid.o\
	tsk_xml.o

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