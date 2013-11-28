#include <gst/gst.h>
#include <wRegister.h>

#include "cvBlur.h"
#include "cvFlip.h"
#include "cvMultiplyS.h"
#include "cvAverage.h"
#include "cvThreshold.h"
#include "cvDilate.h"
#include "cvErode.h"
#include "cvCamShift.h"
#include "uvBlur.h"
#include "cvMotion.h"
#include "cvMultiplyS.h"

extern "C" {

#define AUTHORS "Daniel Fischer <dan@subsignal.org>\nOpenCV developers"
#define SORT "Filter"
	
#define SORT_BASIC SORT"/Video"
#define SORT_VISION SORT"/Analyzer/Video"
#define SORT_STRUCTURAL SORT"/Operator/Video"

int wCvError( CVStatus status, const char *func, const char *context, const char *file, int line ) {
	if( status <= CV_StsError ) {
		g_log("OpenCV", G_LOG_LEVEL_WARNING, "%s\n\tin function [%s:%i]:%s", cvErrorStr(status), file, line, func );
	}
}
	
static gboolean
plugin_init( GstPlugin *plugin ) {

	// basic image processing algorithms
	wRegister<cvBlur>("blur","smooth image",SORT_BASIC,AUTHORS,plugin);
	wRegister<wcvFlip>("flip","flip image",SORT_BASIC,AUTHORS,plugin);
	wRegister<cvMultiplyS>("scale","multiply with scalar",SORT_BASIC,AUTHORS,plugin);
	wRegister<cvAverage>("average","running average",SORT_BASIC,AUTHORS,plugin);
	wRegister<wcvThreshold>("threshold","binary threshold",SORT_BASIC,AUTHORS,plugin);
	wRegister<wcvDilate>("dilate","dilate image",SORT_BASIC,AUTHORS,plugin);
	wRegister<wcvErode>("erode","erode image",SORT_BASIC,AUTHORS,plugin);
	wRegister<wcvCamShift>("camshift","CamShift algorithm",SORT_VISION,AUTHORS,plugin);
	wRegister<uvBlur>("uvblur","smooth U and/or V channel of YUV image",SORT_BASIC,AUTHORS,plugin);
	wRegister<cvMotion>("motion","absolute difference to running average",SORT_BASIC,AUTHORS,plugin);
	wRegister<cvMultiplyS>("multiplys","multiply with scalar",SORT_BASIC,AUTHORS,plugin);
}
	
GST_PLUGIN_DEFINE (
	GST_VERSION_MAJOR,
	GST_VERSION_MINOR,
	ppcv,
	"ppgst Elements utilizing the Intel Open Computer Vision Library",
	plugin_init,
	PACKAGE_VERSION,
	"LGPL",
	PACKAGE_NAME,
	"http://subsignal.org/"
);

}
