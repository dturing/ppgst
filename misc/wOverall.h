#ifndef W_wOverall_H
#define W_wOverall_H

#include <wInplace.h>
#include <wVideo.h>

class wOverall : public wInplace<wY800Video> {
	public:
		wOverall();

		virtual bool Process( wY800Video*in );
	
	protected:
		wFloatProperty value, scale, shift;
		
	private:
		typedef wInplace<wY800Video> inherited;
};

#endif
