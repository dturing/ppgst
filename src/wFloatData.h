#ifndef W_wFloatData_H
#define W_wFloatData_H

#include <wData.h>

/* 
	application/x-float-data:
	int n; n*[ int l; l*float ]
*/

class wFloatData : public wData {
	public:
		wFloatData() 
			: wData( gst_caps_from_string(
				"application/x-float-data"
			) ) { 
				current = NULL;
				current_l = NULL;
				left = 0;
			}
			
		virtual void UseBuffer( GstBuffer *buf, GstPad *pad ) {
				wData::UseBuffer( buf, pad );
				if( !Data() ) {
					g_message("wFloatData got empty buffer");
				//	left = 0; 
					return;
				}
				left = *((int*)Data());
				current_l = (int*)(&Data()[ sizeof(int) ]);
				current = (float*)(&current_l[1]);
			
			}
			
		bool Next( float **d, int *len ) {
				if( left<=0 ) return false;
					
				*len = *current_l;
				*d = current;
				int l=*current_l;
				current_l = (int*)(&current[l]);
				current = (float*)(&current_l[1]);

				left--;
				
				return( true );
			}
			
		int Height() {
				return( left );
			}
			
		void Open( int n ) {
				wData::Append( (unsigned char *)&n, sizeof(int) );
			}
			
		void Append( float *data, int len ) {
				wData::Append( (unsigned char *)&len, sizeof(int) );
				wData::Append( (unsigned char *)data, sizeof(float)*len );
			}
			
		virtual int BufferSize() {
				return sizeof(int);
			}
			
	protected:
		float *current;
		int *current_l;
		int left;
};

#endif
