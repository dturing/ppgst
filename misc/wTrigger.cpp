#include "wTrigger.h"

#include <stdlib.h>

wTrigger::wTrigger() 
	: wInplace<wYUVVideo>()
	, threshold( "threshold","trigger threshold", .25, 0, 1 )
	, motion("motion","motion value in ROI",0,0,1)
	, presence("presence","presence value in ROI",0,0,1)
	, x("x","ROI x",.25,0,1)
	, y("y","ROI y",.25,0,1)
	, w("w","ROI w",.5,0,1)
	, h("h","ROI h",.5,0,1)
	{
	AddProperty( threshold );
	AddProperty( motion );
	AddProperty( presence );
	AddProperty(x);
	AddProperty(y);
	AddProperty(w);
	AddProperty(h);
}

wTrigger::~wTrigger() {
}

bool wTrigger::Process( wYUVVideo *in ) {

	int sz = in->Width()*in->Height();
	int sz4 = ( (in->Width()/2)*(in->Height()/2));
	unsigned char *Y = in->Data();
	unsigned char *U = Y + sz;
	unsigned char *V = U + sz4;
	
	int x0 = (int)(x.Get() * in->Width()/2);
	int y0 = (int)(y.Get() * in->Height()/2);
	int x1 = (int)(w.Get() * in->Width()/2);
	int y1 = (int)(h.Get() * in->Height()/2);
	int s = x1*y1;
	int bpl = in->Width()/2;

// motion=v
	float vacc=0,uacc=0;
	for( int cy=y0; cy<y0+y1; cy++ ) {
		for( int cx=x0; cx<x0+x1; cx++ ) {
			int p = (cy*bpl)+cx;
			if( abs(V[p]-128) > 16 ) vacc++;
			if( abs(U[p]-128) > 16 ) uacc++;
		}
	}
	vacc /= s;
	uacc /= s;
	motion.Set( MAX(0,MIN(1,vacc)) );
	presence.Set( MAX(0,MIN(1,uacc)) );

	GstStructure *msg = gst_structure_new( "value",
		"type", G_TYPE_STRING, "trigger",
		"element", G_TYPE_STRING, gst_element_get_name( GST_ELEMENT(Element) ),
		"motion", G_TYPE_FLOAT, motion.Get(),
		"presence", G_TYPE_FLOAT, presence.Get(),
		"timestamp", G_TYPE_FLOAT, (float)(in->Time()/1000000),
		NULL );

	gst_bus_post( gst_element_get_bus(Element),
		gst_message_new_application( GST_OBJECT(Element), msg ) );
/*
	if( false ) { // draw
		unsigned char v = 128-(acc*0xff);
		if( value.Get()>threshold.Get() ) v = 128+(acc*0xff);;
		
//		memset( U, 128, sz4 );
//		memset( V, 128, sz4 );
		
		for( int cy=y0; cy<y0+y1; cy++ ) {
			for( int cx=x0; cx<x0+x1; cx++ ) {
				V[ (cy*bpl)+cx ] = v;
				U[ (cy*bpl)+cx ] = v;
			}
		}
		for( int cy=y0; cy<y0+y1; cy++ ) {
			data[(cy*bpl)+x0] = data[(cy*bpl)+x1] = 0xff;
		}
		for( int cx=x0; cx<x0+x1; cx++ ) {
			data[(y0*bpl)+cx] = data[(y1*bpl)+cx] = 0xff;
		}				

		int x0 = (int)(x.Get() * in->Width());
		int y0 = (int)(y.Get() * in->Height());
		int x1 = (int)(w.Get() * in->Width());
		int y1 = (int)(h.Get() * in->Height());
		int bpl = in->Width();
		unsigned char *data = Y;
		for( int cy=y0; cy<y0+y1; cy++ ) {
			data[(cy*bpl)+x0] = data[(cy*bpl)+x1+x0] = 0xff;
		}
		for( int cx=x0; cx<x0+x1; cx++ ) {
			data[(y0*bpl)+cx] = data[((y0+y1)*bpl)+cx] = 0xff;
		}				

	}
*/
	return true;
}

