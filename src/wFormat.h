#ifndef W_wFormat_H
#define W_wFormat_H

#include <gst/gst.h>
#include <stdio.h>

class wFormat {
	public:
		wFormat( GstCaps *s_caps = GST_CAPS_ANY ) {
				Caps = gst_caps_ref(s_caps);
				structure = NULL;
				memory = NULL;
			}
	
		virtual void UseBuffer( GstBuffer *buf, GstPad *pad ) {
				Buffer = buf;
				
				if( memory ) {
					gst_memory_unmap( memory, &info );
					gst_memory_unref( memory );
				}
				
				memory = gst_buffer_get_memory( buf, 0 );
				gst_memory_map( memory, &info, GST_MAP_READ );
				
				SetCaps( gst_pad_get_current_caps(pad) );
			}
		virtual void SetCaps( GstCaps *caps ) {
				gst_caps_ref(caps);
				if( caps && gst_caps_get_size(caps)>0 ) 
					structure = gst_caps_get_structure( caps, 0 );
			}
		virtual GstCaps *GetCaps() {
				return Caps;
			}
			
		int Size() { return info.size; }
		unsigned char *Data() { return info.data; }
		
        guint64 Duration() { return GST_BUFFER_DURATION(Buffer); }
        guint64 Time() { return GST_BUFFER_TIMESTAMP(Buffer); }
        
        void SetDuration( guint64 d ) { GST_BUFFER_DURATION(Buffer) = d; }
        void SetTime( guint64 t ) { GST_BUFFER_TIMESTAMP(Buffer) = t; }
        
        void SetDiscont() {
			GST_BUFFER_FLAG_SET( Buffer, GST_BUFFER_FLAG_DISCONT );
        }
        
		int GetCapsInt( const char *name ) {
				if( !structure ) return 0;
				int value = -1;
				gst_structure_get_int( structure, name, &value );
				return value;
			}
		double GetCapsDouble( const char *name ) {
				if( !structure ) return 0;
				double value = -1;
				gst_structure_get_double( structure, name, &value );
				return value;
			}
		int GetCapsFractionNumerator( const char *name ) {
				if( !structure ) return 0;
				int num, denom;
				gst_structure_get_fraction( structure, name, &num, &denom );
				return num;
			}
			
		virtual void Fixate( GstCaps *caps ) { ; }
		virtual int BufferSize() { return 0; }
		
		GstCaps *GetOriginalCaps() {
				return gst_caps_ref(Caps);
			}

		GstPadTemplate* RequestSinkTemplate() {
				return gst_pad_template_new( "sink_%d", GST_PAD_SINK, GST_PAD_REQUEST,
						GetOriginalCaps() ); 
			}
		GstPadTemplate* SinkTemplate() {
				return gst_pad_template_new( "sink", GST_PAD_SINK, GST_PAD_ALWAYS,
						GetOriginalCaps() ); 
			}
		GstPadTemplate* SrcTemplate() { 
				return gst_pad_template_new( "src", GST_PAD_SRC, GST_PAD_ALWAYS,
						GetOriginalCaps() ); 
			}
			
		void ReplaceData( unsigned char *data, int size ) {
		/*
				if( GST_BUFFER_MALLOCDATA(Buffer) ) g_free( GST_BUFFER_MALLOCDATA(Buffer) );
				if( malloced ) GST_BUFFER_MALLOCDATA(Buffer) = data;
				gst_buffer_set_data( Buffer, data, size );
*/
//				if( memory ) // FREE MEM? XXX				
				GstMemory *mem = gst_memory_new_wrapped( (GstMemoryFlags)0, data, size, 0, size, NULL, NULL );
				gst_buffer_replace_all_memory( Buffer, mem );
				memory = mem; // XXX yea?
				// gst_memory_unmap() ?
			}
			
	protected:
		GstBuffer *Buffer;
		GstStructure *structure;
		GstCaps *Caps;
		GstMemory *memory;
		GstMapInfo info;
};


#endif
