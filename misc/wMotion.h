#ifndef W_wMotion_H
#define W_wMotion_H

#include <wInplace.h>
#include <wVideo.h>

class wMotion : public wInplace<wYUVVideo> {
	public:
		wMotion();
		~wMotion();

		virtual bool Process( wYUVVideo *in );
	
	protected:
		int buffersSize;
		unsigned char *cur;
		float *recent;
		float *bg;
		
		wIntegerProperty motionThreshold, presenceThreshold;
		wFloatProperty motionWeight, presenceWeight;
		wBooleanProperty reset;

	private:
		typedef wInplace<wYUVVideo> inherited;
};

#endif
