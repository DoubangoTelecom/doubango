include ./root.mk

all:
	($(MAKE) -f $(PROJECT)-makefile all; cp -f lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR))
install:
	(cd ../$(PROJECT) ; $(MAKE) -f droid-makefile install)

clean:
	($(MAKE) -f $(PROJECT)-makefile clean)


gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(EXEC_DIR)/gdbserver :1234 $(EXEC_DIR)/test