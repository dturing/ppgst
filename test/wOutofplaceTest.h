#ifndef W_wOutofplaceTest_H
#define W_wOutofplaceTest_H

#include <wOutofplace.h>
#include <wVideo.h>

class wOutofplaceTest : public wOutofplace<wRGBAVideo> {
	public:
		wOutofplaceTest()
			: f( "f","test float", 0.5, -1, 1 )
			, s( "s","test string", "hello world!" )
			{
			AddProperty( f );
			AddProperty( s );
		}

		virtual bool Process( wRGBAVideo *in, wRGBAVideo *out ) {
			unsigned char *i = in->Data();
			unsigned char *o = out->Data();
			for( int n=0; n<MIN(in->Size(),out->Size()); n++ ) {
				o[n] = 0xff-i[n];
			}
			
			f.Set( f.Get()*0.99 );
			
			printf("outofplace_test::process, sz %i/%i, f %.2f, s '%s'\n", in->Size(), out->Size(), f.Get(), s.Get() );
			
			return true;
		}

	
	protected:
		wFloatProperty f;
		wStringProperty s;
	
	private:
		typedef wOutofplace<wRGBAVideo> inherited;
};

#endif
