#ifndef W_wSpatialProperties_H
#define W_wSpatialProperties_H

#include "wProperty.h"
#include <stdio.h>

class wRectProperty : public wStringProperty {
	protected:
		float x, y, w, h;

	public:
		wRectProperty( const char *name, const char *blurb,
						float _x = 0, float _y = 0, float _w = 0, float _h = 0,
						const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE )
			: wStringProperty( name, blurb, g_strdup_printf("%f,%f,%f,%f", _x, _y, _w, _h ), flags )
			{ x = _x; y = _y; w = _w; h = _h; }
				
		
		float GetX() { return x; }
		float GetY() { return y; }
		float GetWidth() { return w; }
		float GetHeight() { return h; }

		virtual void SetValue( const GValue* value ) {
			wStringProperty::SetValue( value );
			sscanf( Value, "%f,%f,%f,%f", &x, &y, &w, &h );
		}
		
		void Set( float _x, float _y, float _w, float _h ) {
			x = _x; y = _y; w = _w; h = _h;
			if( Value ) g_free( (gchar*)Value );
			wStringProperty::Set( g_strdup_printf("%.4f,%.4f,%.4f,%.4f",x,y,w,h) );
		}
};

#endif
