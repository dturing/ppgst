#include "wInserterTest.h"
#include <string.h>

wInserterTest::wInserterTest() {
}


bool wInserterTest::Process( wPad *data1, wPad *data2 ) {

	memcpy( data1->Data(), data2->Data(), MIN(data1->Size(),data2->Size())/2 );
	
	return true;
}
