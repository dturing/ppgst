#include "cvCamShift.h"
#include <math.h>

bool wcvCamShift::Process( IplImage *image ) {
	
	static float lastangle = -10000;
	
	CvRect ROI;
	GetROI( &ROI, image->width, image->height );
	CvRect window;
	window.x = window.y = 0;
	window.width = ROI.width; window.height = ROI.height;

	CvBox2D box;
	cvCamShift( image, window, cvTermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 10, 1.0 ),
			NULL, &box );
	
	CvPoint2D32f ptf[4];
	box.angle *= -1;
	cvBoxPoints( box, ptf );
	CvPoint pt[4];
	for( int i=0; i<4; i++ ) {
		pt[i].x = (int)ptf[i].x;
		pt[i].y = (int)ptf[i].y;
	}

	float sh=90;
	if( lastangle != -10000 ) {
		while( box.angle < lastangle-sh ) box.angle+=sh*2;
		while( box.angle > lastangle+sh ) box.angle-=sh*2;
	}
	while( box.angle < 0 ) box.angle += 360;
	box.angle = fmod( box.angle, 360 );
		
//	while( box.angle<M_PI*2 ) box.angle += M_PI*2;
//	while( box.angle>M_PI*2 ) box.angle -= M_PI*2;
	lastangle = box.angle;

//	g_message("%.2f", box.angle ); //(box.angle/(M_PI*2))*360 );
	
	float max = sqrt((image->width*image->width)+(image->height*image->height));

//	g_message("box: %.2f/%.2f", box.center.x, box.center.y );
	
	x.Set( (box.center.x + ROI.x)/image->width );
	y.Set( (box.center.y + ROI.y)/image->height );
	width.Set( (float)box.size.width/max );
	height.Set( (float)box.size.height/max );
	angle.Set( box.angle );

/*	CvPoint center;
	center.x = (int)box.center.x;
	center.y = (int)box.center.y;
	CvSize size;
	size.width = box.size.width/2;
	size.height = box.size.height/2;
	cvEllipse( data->Image(), center, size, -90+((-180/M_PI)*box.angle), 0, 360, 0xff, 0 );
	g_message("%i/%i, %ix%i, %.2f", center.x, center.y, size.width, size.height, box.angle );
*/	
	return true;
}
