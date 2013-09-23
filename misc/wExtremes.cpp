#include "wExtremes.h"
#include <string.h>
#include <math.h>


wExtremes::wExtremes()
	: inherited() 
	, left("left","left extreme",0,0,1)
	, right("right","right extreme",0,0,1)
	, top("top","top extreme",0,0,1)
	, bottom("bottom","bottom extreme",0,0,1)
	{
		AddProperty(left);
		AddProperty(right);
		AddProperty(top);
		AddProperty(bottom);
}

bool wExtremes::Process( wY800Video *in ) {
	unsigned char *data = in->Data();
	int w = in->Width();
	int h = in->Height();
	
	char finish=0;
	unsigned char threshold = 128;
	
	for( int x=0; x<w && !finish; x++ ) {
		unsigned char *d = &data[ x ];
		for( int y=0; y<h-1 && !finish; y++ ) {
			d+=w;
			if( *d > threshold ) {
				finish=1;
				left.Set( (float)x/w );
			}
		}
	}

	finish=0;
	for( int x=w-1; x>=0 && !finish; x-- ) {
		unsigned char *d = &data[ ((h-1)*w)+x ];
		for( int y=0; y<h-1 && !finish; y++ ) {
			d-=w;
			if( *d > threshold ) {
				finish=1;
				right.Set( (float)x/w );
			}
		}
	}

	finish=0;
	for( int y=0; y<h-1 && !finish; y++ ) {
		unsigned char *d = &data[ (y*w) ];
		for( int x=0; x<w && !finish; x++ ) {
			d++;
			if( *d > threshold ) {
				finish=1;
				top.Set( (float)y/h );
			}
		}
	}

	finish=0;
	for( int y=h-2; y>=0 && !finish; y-- ) {
		unsigned char *d = &data[ (y*w) ];
		for( int x=0; x<w && !finish; x++ ) {
			d++;
			if( *d > threshold ) {
				finish=1;
				bottom.Set( (float)y/h );
			}
		}
	}
	
	return true;
}
