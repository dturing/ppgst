#include "cvImageData.h"

cvImageData::cvImageData( GstCaps *caps ) 
	: inherited( caps?caps:gst_caps_from_string(
			"video/x-raw, "
				"format=RGBA, "
				"bpp=(int)32, "
				"depth=(int)24, "
				"endianness=(int)4321, "
				"red_mask=(int)65280, "
				"green_mask=(int)16711680, "
				"blue_mask=(int)-16777216, "
				"framerate = (fraction) [ 0/1, MAX ] "
			";"
			"video/x-raw, "
				"format=RGB, "
				"bpp=(int)24, "
				"depth=(int)24, "
				"endianness=(int)4321, "
				"red_mask=(int)0xff, "
				"green_mask=(int)0xff00, "
				"blue_mask=(int)0xff0000, "
				"framerate = (fraction) [ 0/1, MAX ] "
			";"
			"video/x-raw, "
				"format=GRAY8, "
				"framerate = (fraction) [ 0/1, MAX ] "
		) ) {
		
	img = NULL;
	imgsize.width = imgsize.height = 0;
	channels = depth = 0;
}

void cvImageData::SetCaps( GstCaps *caps ) {
	inherited::SetCaps( caps );
	// Y800 or RGB/A
	int bpp=8;
	gst_structure_get_int(structure,"bpp",&bpp);
	channels=bpp/8;
	depth = IPL_DEPTH_8U;
	imgsize.width = Width();
	imgsize.height = Height();

	if( img ) cvReleaseImageHeader( &img );
	img = cvCreateImageHeader( imgsize, depth, channels );
}

void cvImageData::UseBuffer( GstBuffer *buf, GstPad *pad ) {
	inherited::UseBuffer( buf, pad );
	if( !channels || !depth || !Width() ) {
		g_warning("incomplete image format: %ix%i, %i Channels %s", Width(), Height(), channels, depth==IPL_DEPTH_8U?"8bit":"other depth (likely float)" ); 
		return;
	}
	if( Data() ) {
		cvSetData( img, Data(), channels*Width()*(abs(depth)/8) );
	}
}

int cvImageData::BufferSize() {
	int pixelSize = abs(depth)/8;
	return( Width()*Height()*channels*pixelSize );
}



cvYUVImageData::cvYUVImageData( GstCaps *caps ) 
	: inherited( caps?caps:gst_caps_from_string(
			"video/x-raw, "
				"format=I420, "
				"framerate = (fraction) [ 0/1, MAX ] "
			";"
		) ) {
	Y = U = V = NULL;
	ysize.width = ysize.height = 0;
	uvsize.width = uvsize.height = 0;
}

void cvYUVImageData::SetCaps( GstCaps *caps ) {
	inherited::SetCaps( caps );

	ysize.width = Width();
	ysize.height = Height();
	uvsize.width = Width()/2;
	uvsize.height = Height()/2;

	if( Y ) cvReleaseImageHeader( &Y );
	Y = cvCreateImageHeader( ysize, IPL_DEPTH_8U, 1 );
	if( U ) cvReleaseImageHeader( &U );
	U = cvCreateImageHeader( uvsize, IPL_DEPTH_8U, 1 );
	if( V ) cvReleaseImageHeader( &V );
	V = cvCreateImageHeader( uvsize, IPL_DEPTH_8U, 1 );
}

void cvYUVImageData::UseBuffer( GstBuffer *buf, GstPad *pad ) {
	inherited::UseBuffer( buf, pad );
	if( !Width() ) {
		g_warning("incomplete image format: %ix%i", Width(), Height() ); 
		return;
	}
	unsigned char *data = Data();
	if( data ) {
		cvSetData( Y, data, ysize.width );
		cvSetData( U, data+(ysize.width*ysize.height), uvsize.width );
		cvSetData( V, data+(ysize.width*ysize.height)+(uvsize.width*uvsize.height), uvsize.width );
	}
}

int cvYUVImageData::BufferSize() {
	return( Width()*Height()*1.5 );
}

