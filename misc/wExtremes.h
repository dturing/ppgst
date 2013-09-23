#ifndef W_wExtremes_H
#define W_wExtremes_H

#include <wInplace.h>
#include <wVideo.h>

class wExtremes : public wInplace<wY800Video> {
	public:
		wExtremes();

		virtual bool Process( wY800Video*in );
	
	protected:
		wFloatProperty left,right,top,bottom;
		
	private:
		typedef wInplace<wY800Video> inherited;
};

#endif
