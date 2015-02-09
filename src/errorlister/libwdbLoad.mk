noinst_PROGRAMS = errorlister

errorlister_SOURCES = src/errorlister/errorlister.cpp
errorlister_LDADD = $(top_builddir)/libwdbLoad.la $(BOOST_SYSTEM_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)

pkgdata_DATA = errors.txt

errors.txt:	errorlister
	./$< > $@  

CLEANFILES += errors.txt

