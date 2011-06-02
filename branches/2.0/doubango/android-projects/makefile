include ./root.mk






ifeq ($(PROJECT), tinyWRAP) ##### tinyWRAP
all:
	(cd ../bindings/java/android; $(MAKE) -f droid-makefile all; cp -f lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR))
clean:
	(cd ../bindings/java/android; $(MAKE) -f droid-makefile clean)



else ifeq ($(PROJECT), tinyDEMO) ##### tinyDEMO
all:
	(cd ../tinyDEMO $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp -f demo $(OUTPUT_DIR))
clean:
	(cd ../tinyDEMO; $(MAKE) -f droid-makefile clean)



else ##### All other libraries (tiny*)
all:
	(cd ../$(PROJECT) ; $(MAKE) -f droid-makefile all; cp -f lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR))
install:
	(cd ../$(PROJECT) ; $(MAKE) -f droid-makefile install)

clean:
	(cd ../$(PROJECT); $(MAKE) -f droid-makefile clean)
endif


gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(EXEC_DIR)/gdbserver :1234 $(EXEC_DIR)/test