#ifndef W_wNeartime_H
#define W_wNeartime_H

#include <wInplace.h>
#include <wVideo.h>
#include <sys/time.h>
#include <stdint.h>

uint64_t systemTimestamp() {
	timeval ts;
	gettimeofday(&ts,0);
	return( (((uint64_t)ts.tv_sec) * 1000 + (ts.tv_usec / 1000)) );
}

class wNeartime : public wInplace<wSomeVideo> {
	public:
		wNeartime() 
			: wInplace<wSomeVideo>()
			{
			mod = (24*60*60*1000);
		}
		
		~wNeartime() {
		}

		virtual bool Process( wSomeVideo *in ) {
			in->SetTime( (systemTimestamp() % mod)*1000000 );
			return true;
		}
	
	protected:
		uint64_t mod;

	private:
		typedef wInplace<wSomeVideo> inherited;
};

#endif
