#ifndef W_wStaticBackgroundMotion_H
#define W_wStaticBackgroundMotion_H

#include <wInplace.h>
#include <wVideo.h>

class wStaticBackgroundMotion : public wInplace<wYUVVideo> {
	public:
		wStaticBackgroundMotion();
		~wStaticBackgroundMotion();

		virtual bool Process( wYUVVideo *in );
	
	protected:
		int buffersSize;
		unsigned char *cur;
		float *recent, *bg;
		
		wIntegerProperty motionThreshold, presenceThreshold;
		wFloatProperty motionWeight, presenceWeight;
		wBooleanProperty reset;
		wStringProperty bgFile;

	private:
		typedef wInplace<wYUVVideo> inherited;
};

#endif
