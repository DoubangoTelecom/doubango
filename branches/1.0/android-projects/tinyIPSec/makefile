include ../root.mk

export PROJECT=tinyIPSec

all:
	(cd $(DOUBANGO_HOME)/$(PROJECT)/src ; $(MAKE) all; $(MAKE) install; cp lib$(PROJECT).so $(OUTPUT_DIR))
	(cd $(DOUBANGO_HOME)/$(PROJECT)/test; $(MAKE) all; $(MAKE) install; cp test $(OUTPUT_DIR))
	
clean:
	(cd $(DOUBANGO_HOME)/$(PROJECT)/src; $(MAKE) clean)
	(cd $(DOUBANGO_HOME)/$(PROJECT)/test; $(MAKE) clean)