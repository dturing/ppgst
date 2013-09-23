#include <gst/gst.h>
#include <wRegister.h>
#include "wInplaceTest.h"
#include "wOutofplaceTest.h"
#include "wProducerTest.h"
#include "wTransformTest.h"
#include "wDataTest.h"

wEnum *foonum;

extern "C" {

#define AUTHORS "Daniel Fischer <dan@subsignal.org>"
#define SORT "ppgst/test"
	
static gboolean
plugin_init( GstPlugin *plugin ) {
	foonum = new wEnum( "foonum", "eins", FOO_EINS, "zwei", FOO_ZWEI, "drei", FOO_DREI, NULL );
	wRegister<wInplaceTest>("inplace_test","wInplace test",SORT,AUTHORS,plugin);
	wRegister<wOutofplaceTest>("outofplace_test","wOutofplace test",SORT,AUTHORS,plugin);
	wRegister<wProducerTest>("producer_test","wProducer test",SORT,AUTHORS,plugin);
	wRegister<wTransformTest>("transform_test","wTransform test",SORT,AUTHORS,plugin);
	wRegister<wDataTest>("data_test","wData test",SORT,AUTHORS,plugin);
	
	return TRUE;
}
	
GST_PLUGIN_DEFINE (
	GST_VERSION_MAJOR,
	GST_VERSION_MINOR,
	pptest,
	"ppgst filter tests",
	plugin_init,
	PACKAGE_VERSION,
	"LGPL",
	PACKAGE_NAME,
	"http://subsignal.org/"
);

}
