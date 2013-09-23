#ifndef W_wRate_H
#define W_wRate_H

#include <wInplace.h>
#include <wFormat.h>
#include <sys/time.h>

class wRate : public wInplace<wFormat> {
	public:
		wRate();

		virtual bool Process( wFormat *in );
	
	protected:
		wFloatProperty buffers, kB, update;
		wBooleanProperty dump, post;
	
		time_t offset;
		time_t last;
		time_t update_d;
		double b, d;

	private:
		typedef wInplace<wFormat> inherited;
};

#endif
