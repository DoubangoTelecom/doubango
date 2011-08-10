include ../root.mk

export PROJECT=tinySAK

all:
	(cd ../../$(PROJECT)/src ; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp lib$(PROJECT).so $(OUTPUT_DIR))
	(cd ../../$(PROJECT)/test; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp test $(OUTPUT_DIR))
	
clean:
	(cd ../../$(PROJECT)/src; $(MAKE) -f droid-makefile clean)
	(cd ../../$(PROJECT)/test; $(MAKE) -f droid-makefile clean)