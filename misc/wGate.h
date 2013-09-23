#ifndef W_wGate_H
#define W_wGate_H

#include <wInplace.h>
#include <wFormat.h>

class wGate : public wInplace<wFormat> {
	public:
		wGate() 
			: wInplace<wFormat>()
			, open( "open", "pass on buffers (gate open)", true )
			, openat( "openat", "timestamp for next open", -1, -1, 100000000 )
			, closeat( "closeat", "timestamp for next close", -1, -1, 100000000 )
			, frames( "frames", "number of frames passed thru", 0, 0, 0x0fffffff )
			{
			AddProperty(open);			
			AddProperty(openat);
			AddProperty(closeat);
			AddProperty(frames);
		}
		
		~wGate() {
		}
		/*
		virtual void Post( const char *value, float t, float late=-1, float in=-1 ) {
			GstStructure *msg = gst_structure_new( value,
				"type", G_TYPE_STRING, "gate",
				"element", G_TYPE_STRING, gst_element_get_name( GST_ELEMENT(Element) ),
				"open", G_TYPE_FLOAT, open.Get(),
				"late", G_TYPE_FLOAT, late,
				"in", G_TYPE_FLOAT, in,
				"t", G_TYPE_FLOAT, t,
				NULL );

			gst_bus_post( gst_element_get_bus(Element),
				gst_message_new_application( GST_OBJECT(Element), msg ) );
		}
*/
		virtual bool Process( wFormat *in ) {
			float t = ((double)in->Time())/GST_SECOND;
			
			static int frame=0;
			if( frame++ % 10 == 0 ) {		
				printf("GATE %s t %.3f", (open.Get()?"open":"closed"), t );
				printf(" open %.2f", openat.Get() );
				printf(" close %.2f\n", closeat.Get() );
				printf("\n");
				fflush(stdout);
			}
			if( closeat.Get() != -1 ) {
				if( t >= closeat.Get() ) {
					float late = (closeat.Get()==0.?0.:t-(closeat.Get()));
					printf("GATE_CLOSE %f late\n", late );

					open.Set(0);
					closeat.Set(-1);

					gst_pad_push_event( 
						GST_BASE_TRANSFORM_SRC_PAD( gobject ),
						gst_event_new_eos() );
	/*					
					GstBin *parent = GST_BIN(gst_element_get_parent( GST_ELEMENT(Element) ));
					GstElement *slave = gst_bin_get_by_name_recurse_up( parent, "stopsource" );
					printf("slave: %s\n", gst_element_get_name( stopsource ) );
*/
//				} else {
//					printf("GATE t %f, close in %f\n", t, closeat.Get()-t );
				}
			} else if( openat.Get() != -1 ) {
		//	g_message("t %f, will open in %f", t, openat.Get()-t );
				if( t >= openat.Get() ) { 
					float late = (openat.Get()==0.?0.:t-(openat.Get()));
					printf("GATE_OPEN %f late\n", late );
					frames.Set(0);
					open.Set(1);
					openat.Set(-1);
//				} else {
//					printf("GATE t %f, open in %f\n", t, closeat.Get()-t );
				}
			}

			if( open.Get() ) {
				frames.Set( frames.Get()+1 );
				return true;
			}
			return false;
		}
	
	protected:
		wBooleanProperty open;
		wIntegerProperty frames;
		wFloatProperty openat,closeat;

	private:
		typedef wInplace<wFormat> inherited;
};

#endif
