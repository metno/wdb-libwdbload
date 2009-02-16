#-----------------------------------------------------------------------------
# Project Makefile for wdb's probabilityForecast
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------


# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I config

# Share directory organization
pkgincludedir = $(includedir)/wdb
pkgconfigdir = $(libdir)/pkgconfig

# This is to be replaced by a "find wdb" macro:
AM_CXXFLAGS = -I@top_srcdir@/src/
AM_LDFLAGS = 

# Setting Targets
lib_LTLIBRARIES = libwdbLoaderBase.la

libwdbLoaderBase_la_SOURCES =
pkginclude_HEADERS =

libwdbLoaderBase_la_CPPFLAGS =$(wdb_CFLAGS)
libwdbLoaderBase_la_LIBADD = $(wdb_LIBS)

pkgconfig_DATA = wdbLoaderBase.pc

WDB_VERSION = "$(VERSION)"

CLEANFILES =

EXTRA_DIST =

include src/wdb.mk


###
# Debian
###

PKG_DIR = $(PACKAGE)-$(VERSION)
DEBIAN_DIR = $(PKG_DIR)/debian
DEBIAN_PACKAGE = $(PACKAGE)_$(VERSION)
DEBIAN_VERSION = 6
ARCH = `dpkg-architecture -qDEB_HOST_ARCH_CPU`
DEBIAN_PACKAGE_NAME = $(DEBIAN_PACKAGE)-$(DEBIAN_VERSION)_$(ARCH).deb

debian:	dist clean-debian 
	tar xvzf $(PKG_DIR).tar.gz
	mkdir -p $(DEBIAN_DIR)
	rm -rf $(DEBIAN_DIR)/*
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cp $(PKG_DIR).tar.gz $(DEBIAN_PACKAGE).orig.tar.gz
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc
	lintian $(DEBIAN_PACKAGE_NAME)
	linda $(DEBIAN_PACKAGE_NAME)

clean-debian:
	rm -rf $(PKG_DIR) $(DEBIAN_PACKAGE)*
