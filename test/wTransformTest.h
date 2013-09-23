#ifndef W_wTransformTest_H
#define W_wTransformTest_H

#include <wTransform.h>
#include <wVideo.h>

class wTransformTest : public wTransform<wY800Video,wRGBAVideo> {
	public:
		wTransformTest()
			: f( "f","test float", 0.5, -1, 1 )
			, s( "s","test string", "hello world!" )
			{
			AddProperty( f );
			AddProperty( s );
		}

		virtual void AdaptCapsDownstream( GstCaps *in, GstCaps *out ) {
			GstStructure *i = gst_caps_get_structure( in, 0 );
			GstStructure *o = gst_caps_get_structure( out, 0 );
			int width=320, height=240;
			int framerate_num=30;
			int framerate_denom=1;
			if( i != NULL
				&& gst_structure_get_int( i, "width", &width )
				&& gst_structure_get_int( i, "height", &height )
				&& gst_structure_get_fraction( i, "framerate", &framerate_num, &framerate_denom ) ) {
				gst_structure_set( o, "width", G_TYPE_INT, width, "height", G_TYPE_INT, height, "framerate", GST_TYPE_FRACTION, framerate_num, framerate_denom, NULL );
			}
//			g_message("wTransformTest::AdaptCaps\n\t%s\n\t%s", gst_caps_to_string(in), gst_caps_to_string(out) );
		}
		
		virtual void AdaptCapsUpstream( GstCaps *in, GstCaps *out ) {
			// its just the same logic.
			AdaptCapsDownstream( in, out );
		}
		
		virtual bool Process( wY800Video *in, wRGBAVideo *out ) {
			unsigned char *i = in->Data();
			unsigned char *o = out->Data();
			
			int m = MIN( in->Width()*in->Height(), out->Width()*out->Height() );

			printf("transform_test::process, sz %i/%i (%i), %i/%i, %i/%i f %.2f, s '%s'\n", in->Size(), out->Size(), m, in->Width(), in->Height(), out->Width(), out->Height(), f.Get(), s.Get() );
			
			for( int n=0; n<m; n++ ) {
				*o++ = *i;
				*o++ = *i;
				*o++ = *i++;
				*o++ = 0xff;
			}
			
			f.Set( f.Get()*0.99 );
			
			return true;
		}

	
	protected:
		wFloatProperty f;
		wStringProperty s;
	
	private:
		typedef wTransform<wY800Video,wRGBAVideo> inherited;
};

#endif
