#ifndef WCV_IMAGE_DATA_H
#define WCV_IMAGE_DATA_H

#include <wVideo.h>
#include <cv.h>

class cvImageData : public wVideo {
	public:
		cvImageData( GstCaps *caps = NULL );
	
		virtual void SetCaps( GstCaps *caps );
		virtual void UseBuffer( GstBuffer *buffer, GstPad *pad );
		virtual int BufferSize();

		IplImage *Image() { return img; }
		CvSize ImageSize() { return imgsize; }
		int Channels() { return channels; }
		int Depth() { return depth; }
	
	protected:
		CvSize imgsize;
		IplImage *img;
		int channels;
		int depth;
		
	private:
		typedef wVideo inherited;
};

template <int n>
class cvNChannelImageData : public cvImageData {
	public:
		cvNChannelImageData() 
			: cvImageData( gst_caps_from_string(
				(n==1?
					"video/x-raw, "
						"format=GRAY8, "
						"framerate = (fraction) [ 0/1, MAX ] "
				: (n==3 ? 
					"video/x-raw, "
						"format=RGB, "
						"bpp=(int)24, "
						"depth=(int)24, "
						"endianness=(int)4321, "
						"red_mask=(int)65280, "
						"green_mask=(int)16711680, "
						"blue_mask=(int)-16777216, "
						"framerate = (fraction) [ 0/1, MAX ] "
				: 
					"video/x-raw, "
						"format=RGBA, "
						"bpp=(int)32, "
						"depth=(int)24, "
						"endianness=(int)4321, "
						"red_mask=(int)65280, "
						"green_mask=(int)16711680, "
						"blue_mask=(int)-16777216, "
						"framerate = (fraction) [ 0/1, MAX ] "
				) ) )
			) { ; }
};

typedef cvNChannelImageData<1> cvGrayscale;
typedef cvNChannelImageData<3> cvRGB24;
typedef cvNChannelImageData<4> cvRGBA32;

class cvFloatImageData : public cvImageData {
	public:
		cvFloatImageData() 
			: cvImageData( gst_caps_from_string(
					"video/x-raw, "
					"format=FLOAT, "
					"depth=(int)3 "
				) )
			{ ; }
};

class cv1ChannelFloatImageData : public cvImageData {
	public:
		cv1ChannelFloatImageData() 
			: cvImageData( gst_caps_from_string(
					"video/x-raw, "
					"format=FLOAT, "
					"channels=(int)1,"
					"depth=(int)3 "
				) )
			{ ; }
};

typedef cv1ChannelFloatImageData cvContourData;

/*****************************************************
  YUV
  ******************************************************/
 
class cvYUVImageData : public wVideo {
	public:
		cvYUVImageData( GstCaps *caps = NULL );
	
		virtual void SetCaps( GstCaps *caps );
		virtual void UseBuffer( GstBuffer *buffer, GstPad *pad );
		virtual int BufferSize();

		IplImage *GetY() { return Y; }
		IplImage *GetU() { return U; }
		IplImage *GetV() { return V; }
		CvSize YSize() { return ysize; }
		CvSize UVSize() { return uvsize; }
	
	protected:
		CvSize ysize, uvsize;
		IplImage *Y, *U, *V;
		
	private:
		typedef wVideo inherited;
};


#endif
