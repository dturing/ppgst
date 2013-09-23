#ifndef W_wInplaceTest_H
#define W_wInplaceTest_H

#include <wInplace.h>
#include <wEnum.h>
#include <wVideo.h>

#define FOO_EINS 1
#define FOO_ZWEI 2
#define FOO_DREI 3
extern wEnum *foonum;

class wInplaceTest : public wInplace<wRGBAVideo> {
	public:
		wInplaceTest()
			: inherited()
			, f( "f","test float", 0.5, -1, 1 )
			, i( "i","test int", 5, 0, 100 )
			, s( "s","test string", "hello world!" )
			, e( "e","test enum", foonum->Type(), FOO_ZWEI )
			, l( "l","test floatlist" )
			{
			AddProperty( f );
			AddProperty( i );
			AddProperty( s );
			AddProperty( e );
			AddProperty( l );
		}

		virtual bool Process( wRGBAVideo *Data ) {
			printf("inplace_test::process, sz %i, f %.2f, s '%s' - %i x %i\n", Data->Size(), f.Get(), s.Get(), Data->Width(), Data->Height() );
			unsigned char *data = Data->Data();
			for( int i=0; i<Data->Size(); i++ ) {
				data[i] = 0xff-data[i];
			}
			
			f.Set( f.Get()*0.99 );
			return true;
		}
	
	protected:
		wFloatProperty f;
		wIntegerProperty i;
		wStringProperty s;
		wEnumProperty e;
		wFloatArrayProperty l;
	
		typedef wInplace<wRGBAVideo> inherited;
};

#endif
