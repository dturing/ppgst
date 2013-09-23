#include "wProducerTest.h"
#include <string.h>
#include <stdlib.h>

wProducerTest::wProducerTest()
	: inherited()
	, Y( "y","Y plane value", 0xff, 0, 0xff )
	{
	AddProperty(Y);
}


bool wProducerTest::Process( wRGBAVideo *Data ) {
/*
	unsigned char *data;
	unsigned int w, h;
	
	// Y plane
	Data->PlaneData(0,&data,&w,&h);
	if( data ) {
		for( int y=0; y<h; y++ ) {
			for( int x=0; x<w; x++ ) {
				data[ (y*w)+x ] = Y.Get(); //(x*y*0xff)/(w*h);
			}
		}
	}
	
	// V plane
	Data->PlaneData(1,&data,&w,&h);
	if( data ) {
		for( int y=0; y<h; y++ ) {
			for( int x=0; x<w; x++ ) {
				data[ (y*w)+x ] = (x*0xff)/(w);
			}
		}
	}

	// V plane
	Data->PlaneData(2,&data,&w,&h);
	if( data ) {
		for( int y=0; y<h; y++ ) {
			for( int x=0; x<w; x++ ) {
				data[ (y*w)+x ] = (y*0xff)/(h);
			}
		}
	}
*/
	g_message("producer::Process, %ix%i", Data->Width(), Data->Height() );

	static int j=0;
	int *data = (int*)Data->Data();
	for( int i=0; i<Data->Size()/4; i++ ) {
		data[i] = 0x128; //random()/(RAND_MAX/0xff);
		data[i] |= data[i] << 8;
		data[i] |= data[i] << 16;
	}

	return true;
}
