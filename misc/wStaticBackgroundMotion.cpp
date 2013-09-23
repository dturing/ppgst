#include "wStaticBackgroundMotion.h"

#include <stdlib.h>

wStaticBackgroundMotion::wStaticBackgroundMotion() 
	: wInplace<wYUVVideo>()
	, motionThreshold( "motion-threshold","motion threshold", 8, 0, 255 )
	, presenceThreshold( "presence-threshold","presence threshold", 8, 0, 255 )
	, motionWeight( "motion-weight","new frame's weight for motion calculation", 0.5, 0, 2 )
	, presenceWeight( "presence-weight","new frame's weight for presence calculation", 0.0001, 0, 2 )
	, reset( "reset","reset", false )
	, bgFile( "bgfile","filename of raw background file (size must fit!)", "" )
	{
	AddProperty( motionWeight );
	AddProperty( presenceWeight );
	AddProperty( motionThreshold );
	AddProperty( presenceThreshold );
	AddProperty( reset );
	AddProperty( bgFile );
	
	buffersSize = 0;
	cur = NULL;
	bg = recent = NULL;
}

wStaticBackgroundMotion::~wStaticBackgroundMotion() {
	if( cur ) delete[] cur;
	if( bg ) delete[] bg;
	if( recent ) delete[] recent;
}

void quarterScale( unsigned char *to, unsigned char *from, int w, int h ); // HACKY: in wMotion.cpp

#define FATAL_ERROR printf
void* readWholeFile( const char *filename, int size ) {
	FILE *file = fopen( filename, "rb" );
	if( !file ) {
		FATAL_ERROR( "Could not read '%s'.\n",  filename );
		return NULL;
	}

	fseek( file, 0, SEEK_END );
	int fileSize = ftell( file );
	rewind( file );
	
	if( fileSize != size ) {
		FATAL_ERROR( "'%s' is != (required) %i bytes.\n", filename, size );
		abort();
	}			
	
	void* tmp = (void*)malloc( size );
	if( fileSize != fread( (void*)tmp, 1, fileSize, file ) ) {
		FATAL_ERROR( "Could not read '%s' completely.\n",  filename );
		abort();
	}
	fclose( file );	
	
	return tmp;
}

bool wStaticBackgroundMotion::Process( wYUVVideo *in ) {

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
		
		if( bgFile.Get() ) {
			printf("SHOULD load bg from %s\n",bgFile.Get() );
			unsigned char *bgt = (unsigned char*)readWholeFile( bgFile.Get(), sz4*3 );
			if( bgt ) {
/*				if( bg ) delete[] bg;
				int w = in->Width();
				memset( Y, 0, w*in->Height());
				for( int y=0; y<in->Height(); y++ ) {
					for( int x=0; x<w; x++ ) {
						Y[ (y*w)+x ] = bg[ (((y/2)*(w/2))+(x/2)) ]; 
					}
				}	
*/
				for( int i=0; i<sz4; i++ ) bg[i]=bgt[i];
				delete[] bgt;
			}
		}
		
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

		// good looking
		//M[i] = abs( recent[i]-cur[i] )>mthresh?64:128;
		//P[i] = abs( bg[i]-cur[i] )>pthresh?64:128;

		M[i] = abs( recent[i]-cur[i] )>mthresh?64:128;
		P[i] = abs( bg[i]-cur[i] )>pthresh?64:128;
//		if( P[i] > pthresh ) P[i] = 64;
//		else P[i] = 128-(P[i]*2);

		// "real"
		/*
		M[i] = 128-(abs(recent[i]-cur[i])*3);
		P[i] = 128-(abs(bg[i]-cur[i])*3);
		*/

		bg[i] *= pwn; 
		bg[i] += cur[i]*pw;
		recent[i] *= mwn;
		recent[i] += cur[i]*mw;
	}
	/* show just bg image *
	int w = in->Width();
//	memset( Y, 0, w*in->Height());
	for( int y=0; y<in->Height(); y++ ) {
		for( int x=0; x<w; x+=2 ) {
			int i4 = (((y/2)*(w/2))+(x/2));
			Y[ (y*w)+x ] = bg[ i4 ];
	//		U[i4]=V[i4]=128;
		}
	}	
*/
	return true;
}

