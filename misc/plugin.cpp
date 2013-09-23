#include <gst/gst.h>
#include <wRegister.h>
#include "wRate.h"
#include "wOverall.h"
#include "wShuffle.h"
#include "wInvert.h"
#include "wAverage.h"
#include "wAccumulate.h"
#include "wExtremes.h"
#include "wMotion.h"
#include "wStaticBackgroundMotion.h"
#include "wGate.h"
#include "wNeartime.h"
#include "wTrigger.h"

extern "C" {

#define AUTHORS "Daniel Turing <mail@danielturing.com>"
static gboolean plugin_init( GstPlugin *plugin );

GST_PLUGIN_DEFINE (
	GST_VERSION_MAJOR,
	GST_VERSION_MINOR,
	ppmisc,
	"miscellaneous ppgst-based plugins",
	plugin_init,
	PACKAGE_VERSION,
	"LGPL",
	PACKAGE_NAME,
	"http://danturing.com/"
);

} // extern "C"

static gboolean
plugin_init( GstPlugin *plugin ) {
		
	wRegister<wRate>("rate","measure and print rate","Filter/Video",AUTHORS,plugin);
	wRegister<wOverall>("overall","overall average value","Filter/Operator/Video",AUTHORS,plugin);
	wRegister<wShuffle>("shuffle","shuffle around horizontal stripes","Filter/Video",AUTHORS,plugin);
	wRegister<wInvert>("invert","invert video","Filter/Video",AUTHORS,plugin);
	wRegister<wAverage>("avg","average video","Filter/Video",AUTHORS,plugin);
	wRegister<wAccumulate>("acc","accumulate video","Filter/Video",AUTHORS,plugin);
	wRegister<wExtremes>("extremes","spatial extremes","Source/Video",AUTHORS,plugin);

	wRegister<wMotion>("wmotion","motion/presence detector (from Y to UV)","Source/Video",AUTHORS,plugin);
	wRegister<wStaticBackgroundMotion>("wstaticbgmotion","motion/presence detector (static background version)","Source/Video",AUTHORS,plugin);
	wRegister<wGate>("gate","Simple Gate (passes on buffers if open, swallows when closed)","Source/Video",AUTHORS,plugin);
	wRegister<wNeartime>("neartime","pseudo realtime timestamps","Source/Video",AUTHORS,plugin);
	wRegister<wTrigger>("uvtrigger","motion/presence trigger","Source/Video",AUTHORS,plugin);
}
	


