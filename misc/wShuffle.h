#ifndef W_wShuffle_H
#define W_wShuffle_H

#include <wOutofplace.h>
#include <wVideo.h>

class wShuffle : public wOutofplace<wYUVVideo> {
	public:
		wShuffle();

		virtual bool Process( wYUVVideo *in, wYUVVideo *out );
	
	protected:
		wIntegerProperty stripes;
		
		void shuffleStripes( unsigned char *o, unsigned char *i, int *src, int n, int width, int height );
	
	private:
		typedef wOutofplace<wYUVVideo> inherited;
};

#endif
