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
	tinyBFCP \
	tinyMSRP \
	tinyMEDIA \
	tinyRTP \
	tinyDAV \
	tinySIP \
	\
	bindings

if TARGET_OS_IS_ANDROID
SUBDIRS += plugins/audio_opensles
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = doubango.pc


if CROSS_COMPILING
install-data-hook:
	echo "Cross-compilation is active. Do not run ldconfig";
else
install-data-hook:
	$(MKDIR_P) $(DESTDIR)/etc/ld.so.conf.d
	echo "$(libdir)" > $(DESTDIR)/etc/ld.so.conf.d/doubango.conf
	ldconfig
endif

if CROSS_COMPILING
	echo "Cross-compilation is active. Do not remove libs";
else
uninstall-hook:
	rm -f $(DESTDIR)/etc/ld.so.conf.d/doubango.conf
endif