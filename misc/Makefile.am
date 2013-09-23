# $ Id: $

# location where plug-in will be installed
plugindir= $(libdir)/i386-linux-gnu/gstreamer-@GST_MAJORMINOR@

NULL = 
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	$(NULL)

plugin_LTLIBRARIES = libppmisc.la

libppmisc_la_SOURCES = \
	wRate.cpp \
	wOverall.cpp \
	wShuffle.cpp \
	wInvert.cpp \
	wAverage.cpp \
	wAccumulate.cpp \
	wExtremes.cpp \
	wMotion.cpp \
	wStaticBackgroundMotion.cpp \
	wTrigger.cpp \
	plugin.cpp \
	$(NULL)

noinst_HEADERS = \
	wRate.h \
	wOverall.h \
	wShuffle.h \
	wInvert.h \
	wAverage.h \
	wAccumulate.h \
	wExtremes.h \
	wMotion.h \
	wStaticBackgroundMotion.h \
	wTrigger.h \
	wGate.h \
	wNeartime.h \
	$(NULL)

libppmisc_la_CXXFLAGS = $(PPGST_CFLAGS)
libppmisc_la_LIBADD = $(PPGST_LIBS)
