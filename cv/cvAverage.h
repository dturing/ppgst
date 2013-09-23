#ifndef W_CVAVERAGE_H
#define W_CVAVERAGE_H

#include <cvVideoOp.h>

class cvAverage : public cvVideoOp {
	public:
		cvAverage()
				: cvVideoOp()
				, weight( "weight","new frame's weight", 0.01, 0, 2 )
				{
				AddProperty( weight );
				tmpImage = NULL;
			}

		virtual void Teardown() {
				if( tmpImage ) cvReleaseImage(&tmpImage);
				tmpImage = NULL;
				tmpSize.width = tmpSize.height = 0;
			}
			
		virtual bool Process( IplImage *in, IplImage *out ) {
			if( tmpImage && (tmpSize.width != out->width || tmpSize.height != out->height ) ) {
				Teardown();
			}
			if( !tmpImage ) {
				tmpSize.width = out->width;
				tmpSize.height = out->height;
				tmpImage = cvCreateImage( tmpSize, IPL_DEPTH_32F, out->nChannels );
				cvConvertScale( in, tmpImage, 1, 0 );
			}
			cvRunningAvg( in, tmpImage, weight.Get(), FALSE );
			cvConvertScale( tmpImage, out );
			return true;
		}
	
	protected:
		wFloatProperty weight;
	
		IplImage *tmpImage;
		CvSize tmpSize;
};

#endif
