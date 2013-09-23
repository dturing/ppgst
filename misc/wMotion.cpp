#include "wMotion.h"

#include <stdlib.h>

wMotion::wMotion() 
	: wInplace<wYUVVideo>()
	, motionThreshold( "motion-threshold","motion threshold", 8, 0, 255 )
	, presenceThreshold( "presence-threshold","presence threshold", 8, 0, 255 )
	, motionWeight( "motion-weight","new frame's weight for motion calculation", 0.1, 0, 2 )
	, presenceWeight( "presence-weight","new frame's weight for presence calculation", 0.0001, 0, 2 )
	, reset( "reset","reset", false )
	{
	AddProperty( motionWeight );
	AddProperty( presenceWeight );
	AddProperty( motionThreshold );
	AddProperty( presenceThreshold );
	AddProperty( reset );
	
	buffersSize = 0;
	cur = NULL;
	bg = recent = NULL;
}

wMotion::~wMotion() {
	if( cur ) delete[] cur;
	if( bg ) delete[] bg;
	if( recent ) delete[] recent;
}

void quarterScale( unsigned char *to, unsigned char *from, int w, int h ) {
	for( int y=0; y<h-1; y+=2 ) {
		int yw = (y*w);
		for( int x=0; x<w-1; x+=2 ) {
			to[ (yw/4) + (x/2) ] =
				(
					from[ yw + x ]
					+from[ yw + x + 1 ]
					+from[ yw + w + x ]
					+from[ yw + w + x + 1 ]
				) / 4;
		}
	}
}

bool wMotion::Process( wYUVVideo *in ) {

	int sz = in->Width()*in->Height();
	int sz4 = ( (in->Width()/2)*(in->Height()/2));

	unsigned char *Y = in->Data();
	unsigned char *U = Y + sz;
	unsigned char *V = U + sz4;
	
	if( reset.Get() ) {
		buffersSize = -1;
		reset.Set(false);
	}

	if( buffersSize != sz4 ) {
		if( cur ) delete[] cur;
		if( recent ) delete[] recent;
		if( bg ) delete[] bg;

		buffersSize = sz4;
		cur = new unsigned char[sz4];
		recent = new float[sz4];
		bg = new float[sz4];

		quarterScale( cur, Y, in->Width(), in->Height() );

		for( int i=0; i<sz4; i++ ) recent[i]=bg[i]=cur[i];
		
	} else {
		quarterScale( cur, Y, in->Width(), in->Height() );
	}
	
	unsigned char mthresh = motionThreshold.Get();
	unsigned char pthresh = presenceThreshold.Get();

	unsigned char *P = U;
	unsigned char *M = V;
	float pw = presenceWeight.Get();
	float pwn = 1.-pw;
	float mw = motionWeight.Get();
	float mwn = 1.-mw;
	for( int i=0; i<sz4; i++ ) {
		// 0-255, threshold
		//M[i] = abs( late[i]-cur[i] )>mthresh?255:0;
		//P[i] = abs( bg[i]-cur[i] )>pthresh?255:0;

		// good looking
		M[i] = abs( recent[i]-cur[i] )>mthresh?64:128;
		P[i] = abs( bg[i]-cur[i] )>pthresh?64:128;

		// "real"
		//M[i] = 128+(late[i]-cur[i]);
		//P[i] = 128+(bg[i]-cur[i]);
		
		bg[i] *= pwn; 
		bg[i] += cur[i]*pw;
		recent[i] *= mwn;
		recent[i] += cur[i]*mw;
	}

	return true;
}

