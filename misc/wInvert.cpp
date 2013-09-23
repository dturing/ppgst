#include "wInvert.h"
#include <string.h>
#include <math.h>


wInvert::wInvert()
	: inherited() 
	{
}

bool wInvert::Process( wYUVVideo *in, wYUVVideo *out ) {
	unsigned char *dst = out->Data();
	unsigned char *src = in->Data();
	
	int s = in->Size(); //in->Width()*in->Height();
	for( int i=0; i<s; i++ ) {
		dst[i] = 0xff^src[i];
	}
	
	return true;
}

