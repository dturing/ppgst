#include "wAccumulate.h"
#include <string.h>
#include <math.h>


wAccumulate::wAccumulate()
	: inherited() 
	, accelleration("acc","Accelleration",.2,0,2)
	, friction("friction","Friction",.8,0,2)
	{
	AddProperty( accelleration );
	AddProperty( friction );
	bufSize = 0;
	buffer = NULL;
}

bool wAccumulate::Process( wY800Video *in, wY800Video *out ) {
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
	float sv = accelleration.Get();
	float bv = friction.Get();
	
	for( int i=0; i<bufSize; i++ ) {
		*buf *= bv;
		*buf += *src * sv;
		if( *buf>0xff ) *buf=0xff;
		*dst = *buf;
		
		buf++;
		src++;
		dst++;
	}
	
	return true;
}
