#ifndef W_wAverage_H
#define W_wAverage_H

#include <wOutofplace.h>
#include <wVideo.h>

class wAverage : public wOutofplace<wYUVVideo> {
	public:
		wAverage();

		virtual bool Process( wYUVVideo *in, wYUVVideo *out );
	
	protected:
		wFloatProperty weight;
	
		float *buffer;
		int bufSize;
	
	private:
		typedef wOutofplace<wYUVVideo> inherited;
};

#endif
