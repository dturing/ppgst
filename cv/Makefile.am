# $ Id: $

# location where plug-in will be installed
plugindir= $(libdir)/i386-linux-gnu/gstreamer-@GST_MAJORMINOR@

NULL = 
MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	$(NULL)

plugin_LTLIBRARIES = libppcv.la

libppcv_la_SOURCES = \
	plugin.cpp \
	cvImageData.cpp \
	cvCamShift.cpp \
	$(NULL)
	
noinst_HEADERS = \
	cvImageData.h \
	cvVideoOp.h \
	cvVideoFilter.h \
	wSpatialProperties.h \
	\
	cvBlur.h \
	cvCamShift.h \
	cvDilate.h \
	cvErode.h \
	cvFlip.h \
	cvMultiplyS.h \
	cvThreshold.h \
	uvBlur.h \
	$(NULL)

libppcv_la_LIBADD = $(PPGST_LIBS) $(OPENCV_LIBS)
libppcv_la_CFLAGS = $(PPGST_CFLAGS) $(OPENCV_CFLAGS)
libppcv_la_CXXFLAGS = $(libppcv_la_CFLAGS)
