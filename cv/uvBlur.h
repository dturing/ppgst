#ifndef W_uvBlur_H
#define W_uvBlur_H

#include <wInplace.h>
#include <cvImageData.h>
#include <wEnum.h>

#ifndef W_cvBlur_H
wEnum *cvSmoothType; // initialized in plugin.cpp
#endif

class uvBlur : public wInplace<cvYUVImageData> {
	public:
		uvBlur()
				: wInplace<cvYUVImageData>()
				, uh( "uh","U channel horizontal blur (0=none)", 3, 1, 100 )
				, uv( "uv","U channel vertical blur (0=use horizontal)", 0, 0, 100 )
				, vh( "vh","V channel horizontal blur (0=none)", 3, 1, 100 )
				, vv( "vv","V channel vertical blur (0=use horizontal)", 0, 0, 100 )
				, type( "type", "smooth type", cvSmoothType->Type(), CV_GAUSSIAN )
				{
				AddProperty( uh );
				AddProperty( uv );
				AddProperty( vh );
				AddProperty( vv );
				AddProperty( type );
			}

		virtual bool Process( cvYUVImageData *img ) {
				if( uh.Get() ) cvSmooth( img->GetU(), img->GetU(), type.Get(), uh.Get(), (uv.Get() ? uv.Get() : uh.Get()) );
				if( vh.Get() ) cvSmooth( img->GetV(), img->GetV(), type.Get(), vh.Get(), (vv.Get() ? vv.Get() : vh.Get()) );
				return TRUE;
			}
	
	protected:
		wIntegerProperty uh, uv, vh, vv;
		wEnumProperty type;
	
	private:
		typedef wInplace<cvYUVImageData> inherited;
};

#endif
