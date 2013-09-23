# $ Id: $

# location where plug-in will be installed
plugindir= $(libdir)/i386-linux-gnu/gstreamer-@GST_MAJORMINOR@

NULL = 
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	$(NULL)

plugin_LTLIBRARIES = libppgsttest.la

libppgsttest_la_SOURCES = \
	plugin.cpp \
	wProducerTest.cpp \
	$(NULL)

libppgsttest_la_CXXFLAGS = $(PPGST_CFLAGS)
libppgsttest_la_LIBADD = $(PPGST_LIBS)

noinst_HEADERS = \
	wInplaceTest.h \
	wOutofplaceTest.h \
	wTransformTest.h \
	wProducerTest.h \
	wDataTest.h \
	$(NULL)
