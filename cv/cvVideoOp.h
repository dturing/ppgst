#ifndef W_CVVIDEOOP_H
#define W_CVVIDEOOP_H

#include <wOutofplace.h>
#include <cvImageData.h>
#include "wSpatialProperties.h"

class cvVideoOp : public wOutofplace<cvImageData> {
	public:
		cvVideoOp()
			: wOutofplace<cvImageData>()
			, roi("roi","region of interest (no roi if all 4 values are equal); negative values are mirrored)")
//			, coi("coi","channel of interest (0=all)",0,0,4)
			{
			AddProperty(roi);
//			AddProperty(coi);
		}

		virtual bool Process( cvImageData *in, cvImageData *out ) {
			float x1,y1,x2,y2;
			x1 = roi.GetX();
			y1 = roi.GetY();
			x2 = roi.GetWidth();
			y2 = roi.GetHeight();

			CvRect ROI;
			ROI.x = 0; ROI.y=0; ROI.width=in->Width(); ROI.height=in->Height();
			cvSetImageROI( in->Image(), ROI );
			cvSetImageROI( out->Image(), ROI );

			if( x1 != x2 || x1 != y1 || x1 != y2 ) {
				cvCopy( in->Image(), out->Image() );

				if( x1 < 0 ) x1 += in->Width()+1;
				if( y1 < 0 ) y1 += in->Height()+1;
				if( x2 < 0 ) x2 += in->Width()+1;
				if( y2 < 0 ) y2 += in->Height()+1;
				ROI.x = (int)x1;
				ROI.y = (int)y1;
				ROI.width = (int)(x2-x1);
				ROI.height = (int)(y2-y1);
				cvSetImageROI( in->Image(), ROI );
				cvSetImageROI( out->Image(), ROI );
			}
			return Process( in->Image(), out->Image() );
		}
	
		virtual bool Process( IplImage *in, IplImage *out ) = 0;
	
	protected:
		wRectProperty roi;
	
	private:
		typedef wOutofplace<cvImageData> inherited;
};

#endif
