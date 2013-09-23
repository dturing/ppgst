#include "wOverall.h"
#include <string.h>
#include <math.h>


wOverall::wOverall()
	: inherited() 
	, value("value","overall average pixel value",0,0,1)
	, scale("scale","value scale factor",1,-100,100)
	, shift("shift","value offset",0,-5,5)
	{
		AddProperty(value);
		AddProperty(scale);
		AddProperty(shift);
}

bool wOverall::Process( wY800Video *in ) {
	float acc;
	unsigned char *data = in->Data();
	for( int i=0; i<in->Size(); i++ ) {
		acc += data[i];
	}
	acc /= in->Size();
	acc /= 0xff;
	float v = MAX(0,MIN(1,(acc*scale.Get())+shift.Get()));
	
	value.Set( v );

	GstStructure *msg = gst_structure_new("overall",
		"value", G_TYPE_FLOAT, v,
		NULL );
	gst_bus_post( gst_element_get_bus(GST_ELEMENT(Element)),
		gst_message_new_application( GST_OBJECT(Element), msg ) );

	return true;
}
