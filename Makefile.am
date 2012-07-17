#-----------------------------------------------------------------------------
# Project Makefile for wdb's probabilityForecast
#
# This file is processed by automake to generate the non-recursive
# project Makefile
#-----------------------------------------------------------------------------


# Scanned by Autoreconf; must be static (no expansion)
ACLOCAL_AMFLAGS = 		-I config/m4

# Share directory organization
pkgincludedir = 		$(includedir)/wdb
pkgconfigdir =			$(libdir)/pkgconfig

# This is to be replaced by a "find wdb" macro:
AM_CXXFLAGS = -DSYSCONFDIR=\"$(pkgsysconfdir)\" -I@top_srcdir@/src/ $(wdb_CFLAGS) -fPIC 
AM_LDFLAGS = -fPIC

# Setting Targets
lib_LTLIBRARIES = libwdbLoad.la

libwdbLoad_la_SOURCES =

pkginclude_HEADERS =

libwdbLoad_la_CPPFLAGS =	$(wdb_CFLAGS)
libwdbLoad_la_LIBADD = 		$(wdb_LIBS)
LIBTOOL_VERSION_CURRENT = 	2
LIBTOOL_VERSION_REVISION = 	0 
LIBTOOL_VERSION_AGE = 		0
libwdbLoad_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(LIBTOOL_VERSION_CURRENT):$(LIBTOOL_VERSION_REVISION):$(LIBTOOL_VERSION_AGE)

pkgconfig_DATA = libwdbLoad.pc

WDB_VERSION = "$(VERSION)"

CLEANFILES =

EXTRA_DIST =

include src/libwdbLoad.mk

#-----------------------------------------------------------------------------
# Debian Makefile
#-----------------------------------------------------------------------------

include config/mk/debian.mk
	
debian: common-debian