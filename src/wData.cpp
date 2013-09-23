#include <wData.h>
#include <stdio.h>

wData::wData( GstCaps *caps )
	: wFormat( caps ) {
	ByteArray = g_byte_array_new();
}

wData::~wData() {
	if( ByteArray ) g_byte_array_free( ByteArray, TRUE );
}

void wData::Append( const unsigned char *data, int size ) {
	g_byte_array_append( ByteArray, data, size );
}

void wData::Finish() {
//	g_message("wData::Finish, len %i", ByteArray->len );
/* XXX
	if( GST_BUFFER_MALLOCDATA(Buffer) ) g_free( GST_BUFFER_MALLOCDATA(Buffer) );
	GST_BUFFER_MALLOCDATA(Buffer) = ByteArray->data;
	gst_buffer_set_data( Buffer, ByteArray->data, ByteArray->len );
	*/

	// new, empty ByteArray
	g_byte_array_free( ByteArray, FALSE );
	ByteArray = g_byte_array_new();

	fprintf(stderr,"wData might be sh*t. FIXME\n");	
	ReplaceData( ByteArray->data, ByteArray->len );
}

