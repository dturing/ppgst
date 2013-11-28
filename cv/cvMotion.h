#ifndef W_cvMotion_H
#define W_cvMotion_H

#include <cvVideoOp.h>

class cvMotion : public cvVideoOp {
	public:
		cvMotion()
				: cvVideoOp()
				, weight( "weight","new frame's weight", 0.000001, 0, 2 )
				, scale( "scale","scale", 35, -100, 100 )
				, shift( "shift","shift", -500, -1000, 1000 )
				, reset( "reset","reset", false )
				, mode( "mode","0-abs 1-positive 2-negative", 0, 0, 2 )
				{
				AddProperty( weight );
				AddProperty( scale );
				AddProperty( shift );
				AddProperty( reset );
                AddProperty( mode );
				accImage = NULL;
				tmpImage = NULL;
				reset_ctr=0;
			}

		virtual void Teardown() {
				if( accImage ) cvReleaseImage(&accImage);
				accImage = NULL;
				tmpSize.width = tmpSize.height = 0;
			}
			
		virtual bool Process( IplImage *in, IplImage *out ) {
			if( reset.Get() ) {
				reset_ctr=5;
				Teardown();
				reset.Set(false);
			}
			if( reset_ctr ) {
				reset_ctr--;
				return true;
			}
			if( accImage && (tmpSize.width != out->width || tmpSize.height != out->height ) ) {
				Teardown();
			}
			if( !accImage ) {
				tmpSize.width = out->width;
				tmpSize.height = out->height;
				accImage = cvCreateImage( tmpSize, IPL_DEPTH_32F, out->nChannels );
				tmpImage = cvCreateImage( tmpSize, IPL_DEPTH_8U, out->nChannels );
				cvConvertScale( in, accImage, 1, 0 );
			}
			cvRunningAvg( in, accImage, weight.Get(), FALSE );
			cvConvertScale( accImage, tmpImage );
            
            switch( mode.Get() ) {
                case 0:
                    cvAbsDiff( in, tmpImage, out );
                    break;
                case 1:
                    cvSub( in, tmpImage, out );
                    break;
                case 2:
                    cvSub( tmpImage, in, out );
                    break;
            }
            
			if( scale.Get() != 1 || shift.Get() != 0 )
				cvConvertScale( out, out, scale.Get(), shift.Get() );
			return true;
		}
	
	protected:
		wFloatProperty weight, scale, shift;
		wBooleanProperty reset;
        wIntegerProperty mode;
		int reset_ctr;
	
		IplImage *accImage, *tmpImage;
		CvSize tmpSize;
};

#endif
