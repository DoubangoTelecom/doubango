include ./root.mk

ifeq ($(PROJECT), tinyWRAP)
PROJECT_ROOT:=../bindings/java/android
else
PROJECT_ROOT:=../$(PROJECT)
endif

ifeq ($(STRIP_LIB), )
	STRIP_LIB:=no
	ifeq ($(DEBUG), no)
		ifeq ($(BT), shared)
			STRIP_LIB:=yes
		endif
	endif
endif


ifeq ($(STRIP_LIB), yes)
all:
	(cd $(PROJECT_ROOT); \
	$(MAKE) -f droid-makefile all; \
	$(STRIP) --strip-all --remove-section=.comment --remove-section=.note ./lib$(PROJECT)_$(MARCH).$(EXT); \
	cp -f ./lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR);\
	)
else
all:
	(cd $(PROJECT_ROOT); $(MAKE) -f droid-makefile all; cp -f lib$(PROJECT)_$(MARCH).$(EXT) $(OUTPUT_DIR))
endif
	
install:
	(cd $(PROJECT_ROOT) ; $(MAKE) -f droid-makefile install)

clean:
	(cd $(PROJECT_ROOT); $(MAKE) -f droid-makefile clean)

gdbserver:
	$(ANDROID_SDK_ROOT)/tools/adb forward tcp:1234: tcp:1234
	$(ANDROID_SDK_ROOT)/tools/adb shell $(EXEC_DIR)/gdbserver :1234 $(EXEC_DIR)/test