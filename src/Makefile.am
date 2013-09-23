# $ Id: $

# location where plug-in will be installed
plugindir= $(libdir)/gstreamer-@GST_MAJORMINOR@

NULL = 
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	$(NULL)

lib_LTLIBRARIES = libppgst.la

libppgst_la_SOURCES = \
	wEnum.cpp \
	wProperty.cpp \
	wData.cpp \
	\
	$(NULL)

libppgst_la_CFLAGS = $(GST_CFLAGS)
libppgst_la_CXXFLAGS = $(GST_CFLAGS)
libppgst_la_LDFLAGS = $(GST_LIBS)

noinst_HEADERS = \
	$(NULL)
	
pkginclude_HEADERS = \
	wList.h \
	wEnum.h \
	wProperty.h \
	wRegister.h \
	wObject.h \
	wElement.h \
	\
	wBaseTransform.h \
	wBaseSrc.h \
	wBaseMixer.h \
	\
	wProducer.h \
	wInplace.h \
	wOutofplace.h \
	wTransform.h \
	\
	wFormat.h \
	wVideo.h \
	wAudio.h \
	wData.h \
	wFloatData.h \
	$(NULL)
