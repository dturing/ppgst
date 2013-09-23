#include "wShuffle.h"
#include <string.h>
#include <math.h>


wShuffle::wShuffle()
	: inherited() 
	, stripes("stripes","number of stripes to shuffle",4,1,10)
	{
	AddProperty( stripes );
}

bool wShuffle::Process( wYUVVideo *in, wYUVVideo *out ) {

	int n = stripes.Get();
	
	int src[n];
	for( int i=0; i<n; i++ ) {
		src[i] = (n-1)-i;
	}

	unsigned char *o = out->Data();
	unsigned char *i = in->Data();
	shuffleStripes( o, i, src, n, in->Width(), in->Height() );
	
	o += in->Width()*in->Height();
	i += in->Width()*in->Height();
	shuffleStripes( o, i, src, n, in->Width()/2, in->Height()/2 );

	o += (in->Width()/2)*(in->Height()/2);
	i += (in->Width()/2)*(in->Height()/2);
	shuffleStripes( o, i, src, n, in->Width()/2, in->Height()/2 );
	
	return true;
}

void wShuffle::shuffleStripes( unsigned char *o, unsigned char *i, int *src, int n, int width, int height ) {
	int w = width / n;
	for( int y=0; y<height; y++ ) {
		for( int stripe=0; stripe<n; stripe++ ) {
			int d = stripe*w;
			int s = src[stripe]*w;
			memcpy( &o[d], &i[s], w );
		}
		o += width;
		i += width;
	}
}
