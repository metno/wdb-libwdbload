AC_DEFUN([WDB_CHECK],
[
AC_ARG_WITH([wdb], 
	[AS_HELP_STRING([--with-wdb=DIR], [Location of wdb installation])],
	[],
	[with_wdb=check])
	
AS_IF([test "$with_wdb" = check], 
	[AC_PATH_PROG([wdb_conf], [wdb-config], [no])],
	[wdb_conf="$with_wdb/bin/wdb-config"])

AS_IF([test "$wdb_conf" = no],
	[AC_MSG_ERROR("Unable to find wdb - cannot proceed")]
)

AC_SUBST([wdb_CONFIG], [`$wdb_conf --bindir`/wdbConfiguration])
AC_SUBST([wdb_CFLAGS], [`$wdb_conf --cflags`])
AC_SUBST([wdb_LIBS], ["-L`$wdb_conf --libdir` -lwdbConfig -lwdbProj -lwdbExcept -lwdbLog"])
AC_SUBST([wdb_PKGLIBS], ["-L`$wdb_conf --libdir` -lwdbConfig -lwdbProj -lwdbExcept -lwdbLog -L`$wdb_conf --libdir`/wdb -lwdbTest"])
])
