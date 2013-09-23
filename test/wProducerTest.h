#ifndef W_wProducerTest_H
#define W_wProducerTest_H

#include <wProducer.h>
#include <wVideo.h>

class wProducerTest : public wProducer<wRGBAVideo> {
	public:
		wProducerTest();
		
		virtual bool Process( wRGBAVideo *data );
	
	protected:
		wIntegerProperty Y;
	
	private:
		typedef wProducer<wRGBAVideo> inherited;
};

#endif
