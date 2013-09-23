#ifndef W_wData_H
#define W_wData_H

#include <wFormat.h>
#include <glib.h>

class wData : public wFormat {
	public:
		wData( GstCaps *caps = GST_CAPS_ANY );
		~wData();
	
		void Append( const unsigned char *data, int size );
		virtual void Finish();
	
	protected:
		GByteArray *ByteArray;
};

#endif
