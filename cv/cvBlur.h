#ifndef W_cvBlur_H
#define W_cvBlur_H

#include <cvVideoOp.h>
#include <wEnum.h>

wEnum *cvSmoothType = new wEnum("blur_type",
		"gaussian", CV_GAUSSIAN,
		"median", CV_MEDIAN,
		NULL
	);

class cvBlur : public cvVideoOp {
	public:
		cvBlur()
				: cvVideoOp()
				, h( "h","horizontal blur area", 3, 1, 100 )
				, v( "v","vertical blur area (0=use horizontal)", 0, 0, 100 )
				, type( "type", "smooth type", cvSmoothType->Type(), CV_GAUSSIAN )
				{
				AddProperty( h );
				AddProperty( v );
				AddProperty( type );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				cvSmooth( in, out, type.Get(), h.Get()|1, (v.Get() ? (v.Get()|1) : h.Get()|1) );
				return TRUE;
			}
	
	protected:
		wIntegerProperty h, v;
		wEnumProperty type;
	
	private:
		typedef cvVideoOp inherited;
};

#endif
