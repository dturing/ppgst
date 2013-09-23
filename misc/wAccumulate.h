#ifndef W_wAccumulate_H
#define W_wAccumulate_H

#include <wOutofplace.h>
#include <wVideo.h>

class wAccumulate : public wOutofplace<wY800Video> {
	public:
		wAccumulate();

		virtual bool Process( wY800Video *in, wY800Video *out );
	
	protected:
		wFloatProperty accelleration, friction;
	
		float *buffer;
		int bufSize;
	
	private:
		typedef wOutofplace<wY800Video> inherited;
};

#endif
