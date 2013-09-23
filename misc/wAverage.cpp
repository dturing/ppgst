#include "wAverage.h"
#include <string.h>
#include <math.h>


wAverage::wAverage()
	: inherited() 
	, weight("weight","Weight",0.05,0,1)
	{
	AddProperty( weight );
	bufSize = 0;
	buffer = NULL;
}

bool wAverage::Process( wYUVVideo *in, wYUVVideo *out ) {
	if( buffer==NULL || bufSize != in->Size() ) {
		if( buffer ) delete[] buffer;
		bufSize = in->Size();
		buffer = new float[ bufSize ];
		for( int i=0; i<bufSize; i++ ) {
			buffer[i] = in->Data()[i];
		}
	}

	float *buf = buffer;
	unsigned char *src = in->Data();
	unsigned char *dst = out->Data();
	float sv = weight.Get();
	float bv = 1.0 - weight.Get();
	
	for( int i=0; i<bufSize; i++ ) {
		*buf *= bv;
		*buf += *src * sv;
		*dst = *buf;
		
		buf++;
		src++;
		dst++;
	}
	
	return true;
}
