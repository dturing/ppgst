#ifndef W_cvCamShift_H
#define W_cvCamShift_H

#include <cvVideoFilter.h>
#include <cvImageData.h>
#include <wProperty.h>

class wcvCamShift : public cvVideoFilter<cvGrayscale> {
	public:
		wcvCamShift()
				: inherited()
				, x( "x", "CamShift analysis result (center x)", 0.5, 0, 1 )
				, y( "y", "CamShift analysis result (center y)", 0.5, 0, 1 )
				, width( "width", "CamShift analysis result (width)", 0.5, 0, 2 )
				, height( "height", "CamShift analysis result (height)", 0.5, 0, 2 )
				, angle( "angle", "CamShift analysis result (angle)", 0.5, -360, 360 )
				{
				AddProperty(x);
				AddProperty(y);
				AddProperty(width);
				AddProperty(height);
				AddProperty(angle);
			}

		virtual bool Process( IplImage *image );
			
	protected:
		wFloatProperty x;
		wFloatProperty y;
		wFloatProperty width, height, angle;

	private:
		typedef cvVideoFilter<cvGrayscale> inherited;
};

#endif
