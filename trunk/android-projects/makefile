include ./root.mk

all:
	(cd ../$(PROJECT) ; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp --force lib$(PROJECT).so $(OUTPUT_DIR))
	(cd ../$(PROJECT)/test; $(MAKE) -f droid-makefile all; $(MAKE) -f droid-makefile install; cp --force test $(OUTPUT_DIR))

clean:
	(cd ../$(PROJECT); $(MAKE) -f droid-makefile clean)
	(cd ../$(PROJECT)/test; $(MAKE) -f droid-makefile clean)