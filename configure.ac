#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Pre-Conf
#-----------------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT(libwdbLoad, 1.0.1, [wdb@met.no])
# Config
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL

#-----------------------------------------------------------------------------
# Source & Build Directories
#-----------------------------------------------------------------------------



#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S


#-----------------------------------------------------------------------------
# Checks
#-----------------------------------------------------------------------------

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h sys/time.h unistd.h wordexp.h ctype.h])
AC_CHECK_HEADERS([string vector iterator iosfwd iostream sstream fstream])
AC_CHECK_HEADERS([cmath algorithm memory new stdexcept cstdlib exception])


# string exception sstream 

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MEMCMP

# Check for libraries.
AC_CXX_REQUIRE_STL
VL_LIB_READLINE

#-----------------------------------------------------------------------------
# Module Checks
#-----------------------------------------------------------------------------

# wdb
WDB_CHECK

# BOOST
WDB_BOOST_CHECK(1.33.1)

# LOG4CPP
WDB_LOG4CPP_CHECK(1.0)

WDB_PQXX_CHECK(2.6.8)

#-----------------------------------------------------------------------------
# documentation
#-----------------------------------------------------------------------------

WDB_DOCBOOK_CHECK

#-----------------------------------------------------------------------------
# Automake
#-----------------------------------------------------------------------------

AM_INIT_AUTOMAKE(-Wall -Werror subdir-objects)
AM_PROG_CC_C_O


#-----------------------------------------------------------------------------
# Configure and Exit
#-----------------------------------------------------------------------------

# Configuration Files
AC_CONFIG_FILES([Makefile libwdbLoad.pc])
AC_OUTPUT

# Print Out Summary of Configuration
AC_MSG_RESULT([ ])
AC_MSG_RESULT([$PACKAGE_NAME $PACKAGE_VERSION])
AC_MSG_RESULT([Configuration:])
AC_MSG_RESULT([-------------------------------------------------------------------------])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([ Database Name:	  $DATABASE_NAME])
AC_MSG_RESULT([ Source Code:      $srcdir])
AC_MSG_RESULT([ Host System:      $host])
AC_MSG_RESULT([  ])
AC_MSG_RESULT([ Prefix:           $prefix])
AC_MSG_RESULT([ Binaries:         $exec_prefix])
AC_MSG_RESULT([ Manuals:          $mandir])
AC_MSG_RESULT([ Data:             $datadir])
AC_MSG_RESULT([ System Config:    $sysconfdir])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([ CPPFLAGS:         $CPPFLAGS])
AC_MSG_RESULT([ LDFLAGS:          $LDFLAGS])
AC_MSG_RESULT([ LIBS:             $LIBS])
AC_MSG_RESULT([  ])
AC_MSG_RESULT([-------------------------------------------------------------------------])
AC_MSG_RESULT([ ])
