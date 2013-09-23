#ifndef W_wAudio_H
#define W_wAudio_H

#include <wFormat.h>

/** \brief Generic Audio format
 */
template <class T>
class wAudio : public wFormat {
	public:
		wAudio( GstCaps *caps ) 
				: wFormat( caps ) {
			}

	// interrogation
		int Channels() { return GetCapsInt("channels"); }
		int Rate() { return GetCapsInt("rate"); }
		int Depth() { return GetCapsInt("depth"); }

	// data access
		T* Samples() {
				return( (T*)Data() );
			}
		int Length() {
				return( Size() / sizeof(T) );
			}
};

/** \brief 1-channel, 16-bit audio format */
class wShortMonoAudio : public wAudio<short> {
	public:
        int preferredBufferSize;
    
		wShortMonoAudio() 
			:wAudio<short>( gst_caps_from_string(
				"audio/x-raw-int, "
					"rate=(int)[ 1, MAX ], "
					"channels=(int)1, "
					"endianness=(int)1234, "
					"width=(int)16, "
					"depth=(int)16, "
					"signed=(boolean)true "
			)) { 
                preferredBufferSize=1024; }
            
        void setPreferredBufferSize( int s ) {
            preferredBufferSize=s;
        }            
        
		virtual int BufferSize() {
				return( preferredBufferSize );
			}
};


#endif
