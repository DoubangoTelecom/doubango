include ../root.mk

export PROJECT=tinySAK

all:
	(cd ../../tinySAK/src ; $(MAKE) all; $(MAKE) install; cp libtinySAK.so $(OUTPUT_DIR))
	(cd ../../tinySAK/test; $(MAKE) all; $(MAKE) install; cp test $(OUTPUT_DIR))
	
clean:
	(cd ../../tinySAK/src; $(MAKE) clean)
	(cd ../../tinySAK/test; $(MAKE) clean)