#ifndef W_wDataTest_H
#define W_wDataTest_H

#include <wTransform.h>
#include <wVideo.h>
#include <wData.h>

class wSomeData : public wData {
	public:
		wSomeData()
			: wData( gst_caps_from_string("application/x-wSomeData") ) {
			}
			
};

class wDataTest : public wTransform<wY800Video,wSomeData> {
	public:
		wDataTest()
			: f( "f","test float", 0.5, -1, 1 )
			, s( "s","test string", "hello world!" )
			{
			AddProperty( f );
			AddProperty( s );
		}
		
		virtual int OutputSize() {
			return( 0 );
		}

		virtual int InputSize() {
			return( In.Width() * In.Height() );
		}
		
		virtual bool Process( wY800Video *in, wSomeData *out ) {
			unsigned char *i = in->Data();
			unsigned char *o = out->Data();
			
//			g_message("wDataTest::Process()");
			out->Append((const unsigned char *)"hello",5);
			out->Append((const unsigned char *)" world",7);
			out->Finish();
			
			return true;
		}

	
	protected:
		wFloatProperty f;
		wStringProperty s;
	
	private:
		typedef wTransform<wY800Video,wRGBAVideo> inherited;
};

#endif
