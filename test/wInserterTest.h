#ifndef W_INSERTERTEST_H
#define W_INSERTERTEST_H

#include <wInserter.h>

class wInserterTest : public wInserter<wPad,wPad> {
	public:
		wInserterTest();

		virtual bool Process( wPad *data1, wPad *data2 );
	
	protected:
	
	private:
		typedef wInserter<wPad,wPad> inherited;
};

#endif
