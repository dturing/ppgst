#include "wRate.h"
#include <string.h>
#include <unistd.h>

#define SECOND 1000000

wRate::wRate()
	: inherited()
	, buffers("buffers","buffer per second",0,0,100000)
	, kB("kB","kilobytes per second",0,0,1000000)
	, update("update","measurements per second",1,0,100)
	, dump("dump","dump to stdout",true)
	, post("post-message","post message on bus",true)
	{
	AddProperty(buffers);
	AddProperty(kB);
	AddProperty(update);
	AddProperty(dump);
	AddProperty(post);

	offset = 0;
	b = d = 0;
}

bool wRate::Process( wFormat *in ) {
	timeval tv;
	int now;
	
	if( !offset ) {
		gettimeofday( &tv, NULL );
		offset = tv.tv_sec;
		last = tv.tv_usec;
		return true;
	}

	gettimeofday( &tv, NULL );
	now = ((tv.tv_sec - offset)*SECOND)+tv.tv_usec;
	
	int delta = now - last;
	b++;
	d+=in->Size();
	
	if( delta > SECOND / update.Get() ) {
		b *= (double)SECOND/delta;
		
		float s = (d/b)/1024;
		d *= (double)SECOND/delta;
		d /= 1024;
		
		if( dump.Get() ) g_message("%.1f Buffers, %.0fkB per second, avg %.2fkB/buffer", b, d, s );

		if( post.Get() ) {
			GstStructure *msg = gst_structure_new("rate",
				"element", G_TYPE_STRING, gst_element_get_name( GST_ELEMENT(Element) ),
				"Bps", G_TYPE_INT, (int)d*1024,
				"fps", G_TYPE_FLOAT, (float)b,
				NULL );
			gst_bus_post( gst_element_get_bus( Element ),
				gst_message_new_application( GST_OBJECT(Element), msg ) );
		}
		
		buffers.Set( b );
		kB.Set( d );
		
		b = d = 0;
		last = now;
	}
	
	return true;
}
