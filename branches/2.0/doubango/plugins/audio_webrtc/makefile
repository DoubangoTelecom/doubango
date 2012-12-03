include ../../android-projects/root.mk
PLATFORM:=android-9 # required because of OpenSL ES
	
ifeq ($(BT), shared)
all:
	($(MAKE) -f droid-makefile all; \
	$(STRIP) --strip-all --remove-section=.comment --remove-section=.note ./lib$(PROJECT)_$(MARCH).$(EXT); \
	cp -f ./lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR); \
	)
else
all:
	($(MAKE) -f droid-makefile all; cp -f lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR))
endif

clean:
	($(MAKE) -f droid-makefile clean)


gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(EXEC_DIR)/gdbserver :1234 $(EXEC_DIR)/test