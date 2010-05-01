include ./root.mk

ifeq ($(PROJECT), tinyDEMO) ##### tinyDEMO
all:
	(cd ../tinyDEMO $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp --force demo $(OUTPUT_DIR))
clean:
	(cd ../tinyDEMO; $(MAKE) -f droid-makefile clean)

else ##### All other libraries (tiny*)
all:
	(cd ../$(PROJECT) ; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp --force lib$(PROJECT).so $(OUTPUT_DIR))
	(cd ../$(PROJECT)/test; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp --force test $(OUTPUT_DIR))

clean:
	(cd ../$(PROJECT); $(MAKE) -f droid-makefile clean)
	(cd ../$(PROJECT)/test; $(MAKE) -f droid-makefile clean)
endif


gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(EXEC_DIR)/gdbserver :1234 $(EXEC_DIR)/test