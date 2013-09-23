# $Id$

AC_INIT( libppgst, 0.3, mail@danielturing.com )
AM_INIT_AUTOMAKE( [0.3 foreign] )

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

dnl Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)

dnl Give error and exit if we don't have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
  AC_MSG_ERROR(you need to have pkgconfig installed !)
fi

# Checks for libraries.

# check for gstreamer
GST_REQUIRED=1.0
GST_MAJORMINOR=1.0
PKG_CHECK_MODULES(GST, \
  gstreamer-$GST_MAJORMINOR >= $GST_REQUIRED gstreamer-base-$GST_MAJORMINOR >= $GST_REQUIRED gstreamer-video-$GST_MAJORMINOR >= $GST_REQUIRED,
  HAVE_GST=yes,HAVE_GST=no)
if test "x$HAVE_GST" = "xno"; then
  AC_MSG_ERROR(you need gstreamer development packages installed !)
fi
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)
AC_SUBST(GST_MAJORMINOR)

# local libppgst
PPGST_CFLAGS="-I\$(top_srcdir)/src $GST_CFLAGS"
PPGST_LIBS="-L\$(top_srcdir)/src -lppgst $GST_LIBS"
AC_SUBST(PPGST_CFLAGS)
AC_SUBST(PPGST_LIBS)


# some checking helpers
# (actually useless currently?)

AC_DEFUN([CHECK_CONFIG_PROG],[
	AC_PATH_PROG([$1]_CONFIG, [$2], no)
	if test x$[$1]_CONFIG = xno; then
		[$1]_LIBS=
		[$1]_CFLAGS=
		HAVE_[$1]=no
	else
		[$1]_LIBS=`[$2] --libs [$3]`
		[$1]_CFLAGS=`[$2] --cflags [$3]`
		HAVE_[$1]=yes
	fi
	AM_CONDITIONAL(USE_[$1], test x$HAVE_[$1] = xyes)
])

AC_DEFUN([CHECK_PKG],[
	PKG_CHECK_MODULES($1,$2,HAVE_$1=yes,HAVE_$1=no)
	AM_CONDITIONAL(USE_[$1], test x$HAVE_[$1] = xyes)
])


# check optional libraries

AC_OUTPUT([
	Makefile
	src/Makefile
	test/Makefile
	misc/Makefile
	libppgst.pc
	])

