#ifndef W_cvVideoFilter_H
#define W_cvVideoFilter_H

#include <wInplace.h>
#include <cvImageData.h>
#include "wSpatialProperties.h"

template <class T>
class cvVideoFilter : public wInplace<T> {
	public:
		cvVideoFilter()
			: wInplace<T>()
			, roi("roi","region of interest (no roi if all 4 values are 0); negative values are mirrored)")
			{
			this->AddProperty(roi);
		}

		bool GetROI( CvRect * ROI, int width, int height ) {
			float x1,y1,x2,y2;
			x1 = roi.GetX()*width;
			y1 = roi.GetY()*height;
			x2 = roi.GetWidth()*width;
			y2 = roi.GetHeight()*height;

//			g_message("ROI: %f,%f - %f,%f", x1, y1, x2, y2 );
			
			if( x1 < x2 && y1 < y2 ) {
				if( x1 < 0 ) x1 += width+1;
				if( y1 < 0 ) y1 += height+1;
				if( x2 < 0 ) x2 += width+1;
				if( y2 < 0 ) y2 += height+1;
				ROI->x = (int)x1;
				ROI->y = (int)y1;
				ROI->width = (int)(x2-x1);
				ROI->height = (int)(y2-y1);
				return true;
			} else {
				ROI->x = 0; ROI->y=0; ROI->width=width; ROI->height=height;
				return false;
			}
		}

		virtual bool Process( T *in ) {
			
			CvRect ROI;
			GetROI( &ROI, in->Width(), in->Height() );
			cvSetImageROI( in->Image(), ROI );
			
			return Process( in->Image() );
		}
	
		virtual bool Process( IplImage *image ) = 0;
	
	protected:
		wRectProperty roi;
	
	private:
		typedef wInplace<T> inherited;
};

#endif
