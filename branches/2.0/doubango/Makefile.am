AUTOMAKE_OPTIONS = foreign dist-tarZ
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = tinySAK \
	tinyNET \
	tinyHTTP \
	tinyXCAP \
	tinyIPSec \
	tinySMS \
	tinySIGCOMP \
	tinySDP \
	tinyMSRP \
	tinyMEDIA \
	tinyRTP \
	tinyDAV \
	tinySIP

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = doubango.pc

install-data-hook:
	$(MKDIR_P) $(DESTDIR)/etc/ld.so.conf.d
	echo "$(libdir)" > $(DESTDIR)/etc/ld.so.conf.d/doubango.conf
	ldconfig

uninstall-hook:
	rm -f $(DESTDIR)/etc/ld.so.conf.d/doubango.conf