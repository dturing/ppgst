#ifndef W_cvFlip_H
#define W_cvFlip_H

#include <cvVideoOp.h>

class wcvFlip : public cvVideoOp {
	public:
		wcvFlip()
				: cvVideoOp()
				, h( "h","flip horizontally", FALSE )
				, v( "v","flip vertically", FALSE )
				{
				AddProperty( h );
				AddProperty( v );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				bool H = h.Get();
				bool V = v.Get();
				if( !(H||V) ) {
					cvCopy( in, out  );
				} else {
					int mode;
					if( H && V ) mode=-1;
					else if( V ) mode=0;
					else mode=1;
						
					cvFlip( in, out, mode );
				}
				return TRUE;
			}
	
	protected:
		wBooleanProperty h, v;
};

#endif
