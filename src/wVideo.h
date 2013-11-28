#ifndef W_wVideo_H
#define W_wVideo_H

#include <wFormat.h>
#include <string.h>
#include <gst/video/video.h>

class wVideo : public wFormat {
	public:
		wVideo( GstCaps *s_caps ) 
			: wFormat( s_caps )
			{ }
			
		int Width() { return GetCapsInt("width"); }
		int Height() { return GetCapsInt("height"); }
		double Framerate() { return GetCapsFractionNumerator("framerate"); }

		virtual void SetCaps( GstCaps *caps ) {
				gst_video_info_from_caps( &info, caps );
				wFormat::SetCaps( caps );
			}
		
		virtual void Fixate( GstCaps *caps ) {
				if( gst_caps_get_size(caps)>0 ) {
					structure = gst_caps_get_structure (caps, 0);
				
					gst_structure_fixate_field_nearest_int (structure, "width", 320);
					gst_structure_fixate_field_nearest_int (structure, "height", 240);
					gst_structure_fixate_field_nearest_fraction (structure, "framerate", 30, 1 );
				}
			}
			
	protected:
		GstVideoInfo info;
};

class wRGBAVideo : public wVideo {
	public:
		wRGBAVideo() 
			: wVideo( 
				gst_caps_from_string( 
					"video/x-raw, " 
						"format=RGBA, "
						"width=(int)[1, MAX], " 
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
				)
			)
			{ ; }
			
		virtual int BufferSize() {
				return( Width() * Height() * 4 );
			}
};

typedef wRGBAVideo wVariableRGBA;

class wY800Video : public wVideo {
	public:
		wY800Video() 
			: wVideo( 
				gst_caps_from_string(
					"video/x-raw, " 
						"format=GRAY8, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=I420, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] "
				)
			)
			{ }
			
		virtual int BufferSize() {
				double factor = 1;
				switch( GST_VIDEO_FORMAT_INFO_FORMAT(info.finfo) ) {
					case GST_VIDEO_FORMAT_GRAY8:
						break;
					default:
						factor=1.5;
				}
				return( Width() * Height() * factor );
			}
};

class wYUVVideo : public wVideo {
	public:
		wYUVVideo() 
			: wVideo( 
				gst_caps_from_string( 
					"video/x-raw, " 
						"format=I420, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] "
				)
			)
			{ ; }
			
		virtual int BufferSize() {
				return( (int)(Width() * Height() * 1.5) );
			}
};

#define W_FORMAT_RGBA 1
#define W_FORMAT_RGB 2
#define W_FORMAT_Y800 3
#define W_FORMAT_YUV 4
class wSomeVideo : public wVideo {
	public:
		wSomeVideo() 
			: wVideo( 
				gst_caps_from_string(
					"video/x-raw, " 
						"format=Y800, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=I420, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=RGBA, "
						"width=(int)[1, MAX], " 
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=RGB, "
						"width=(int)[1, MAX], " 
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
				)
			)
			{ }
		virtual void SetCaps( GstCaps *caps ) {
				wVideo::SetCaps( caps );
				
				format = 0;
				switch( GST_VIDEO_FORMAT_INFO_FORMAT(info.finfo) ) {
					case GST_VIDEO_FORMAT_GRAY8:
						format = W_FORMAT_Y800;
						break;
					case GST_VIDEO_FORMAT_YV12:
					case GST_VIDEO_FORMAT_I420:
						format = W_FORMAT_YUV;
						break;
					case GST_VIDEO_FORMAT_RGBA:
						format = W_FORMAT_RGBA;
						break;
					case GST_VIDEO_FORMAT_RGB:
						format = W_FORMAT_RGB;
						break;
					default:
						g_warning("Unknown SomeVideo: %s", gst_caps_to_string(caps) );
				}
			}
			
		int GetFormat() { return format; }
		
		virtual int BufferSize() {
				switch( format ) {
					case W_FORMAT_Y800:
						return( Width() * Height() );
					case W_FORMAT_YUV:
						return( (int)(Width() * Height() * 1.5) );
					case W_FORMAT_RGBA:
						return( Width() * Height() * 4 );
					case W_FORMAT_RGB:
						return( Width() * Height() * 3 );
				}
				return 0;
			}

	protected:
		int format;
};

class wPackedVideo : public wVideo {
	public:
		wPackedVideo() 
			: wVideo( 
				gst_caps_from_string(
					"video/x-raw, " 
						"format=I420, " 
						"width=(int)[1, MAX], "
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=RGBA"
						"width=(int)[1, MAX], " 
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
					"; "
					"video/x-raw, " 
						"format=RGB"
						"width=(int)[1, MAX], " 
						"height=(int)[1, MAX], " 
						"framerate = (fraction) [ 0/1, MAX ] " 
				)
			)
			{ }
			
		virtual int BufferSize() {
				return( Width() * Height() * Channels() );
			}

		int Channels() { int c = GetCapsInt("bpp"); return c==-1?1:c/8; }

	protected:
};

#endif
