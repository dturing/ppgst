#ifndef W_wTrigger_H
#define W_wTrigger_H

#include <wInplace.h>
#include <wVideo.h>

class wTrigger : public wInplace<wYUVVideo> {
	public:
		wTrigger();
		~wTrigger();

		virtual bool Process( wYUVVideo *in );
	
	protected:
		wFloatProperty threshold;
		wFloatProperty motion,presence;
		wFloatProperty x,y,w,h;

	private:
		typedef wInplace<wYUVVideo> inherited;
};

#endif
