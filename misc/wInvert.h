#ifndef W_wInvert_H
#define W_wInvert_H

#include <wOutofplace.h>
#include <wVideo.h>

class wInvert : public wOutofplace<wYUVVideo> {
	public:
		wInvert();

		virtual bool Process( wYUVVideo *in, wYUVVideo *out );
	
	protected:

	private:
		typedef wOutofplace<wYUVVideo> inherited;
};

#endif
