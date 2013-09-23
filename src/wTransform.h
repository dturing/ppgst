#ifndef W_wTransform_H
#define W_wTransform_H

#include <wBaseTransform.h>

 
template <class InFormat, class OutFormat>
class wTransform : public wBaseTransform {
	protected:
		InFormat In;
		OutFormat Out;
		wBooleanProperty Bypass;

	public:
		wTransform()
			: Bypass("bypass","bypass this filter",FALSE)
			{
				AddProperty(Bypass);
			}

		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) {
				wBaseTransform::class_init( k, o, info );
				GstElementClass *klass = (GstElementClass*)k;
				gst_element_class_add_pad_template( klass, In.SinkTemplate() );
				gst_element_class_add_pad_template( klass, Out.SrcTemplate() );

				GST_BASE_TRANSFORM_CLASS(klass)->transform = transform;
				GST_BASE_TRANSFORM_CLASS(klass)->transform_caps = transform_caps;
				GST_BASE_TRANSFORM_CLASS(klass)->transform_size = transform_size;
			//	GST_BASE_TRANSFORM_CLASS(klass)->get_unit_size = get_unit_size;
			//	GST_BASE_TRANSFORM_CLASS(klass)->set_caps = set_caps;
			}
			
		static gboolean set_caps( GstBaseTransform *base, GstCaps *incaps, GstCaps *outcaps ) {
			wTransform<InFormat,OutFormat> *self = (wTransform<InFormat,OutFormat>*)GetSelf(G_OBJECT(base));
			self->In.SetCaps( incaps );
			self->Out.SetCaps( outcaps );
			return true;
		}

		static gboolean get_unit_size( GstBaseTransform *base, GstCaps *caps, guint *size ) {
			wTransform<InFormat,OutFormat> *self = (wTransform<InFormat,OutFormat>*)GetSelf(G_OBJECT(base));
			/*
			GstStructure *s = gst_caps_get_structure(caps,0);
			int w; int h;
			gst_structure_get_int( s, "width", &w );
			gst_structure_get_int( s, "height", &h );
			guint32 format;
			gst_structure_get_fourcc( s, "format", &format );

			const char *name = gst_structure_get_name( s );
			if( !strcmp(name,"video/x-raw-yuv") ) {
				if( format==808596553 ) // i420
					*size = w*h*1.5;
				else
					*size = w*h;
			} else if( !strcmp(name,"video/x-raw-rgb") ) {
				*size = w*h*4;
			}
			
			g_message("-------------------- format: %s -> %s\n", *size, gst_caps_to_string(caps) );

			*/
			g_message( "get_unit_size currently not implemented. who needs this? XXX\n");
			return true;
		}
		
		static GstFlowReturn transform( GstBaseTransform *base, GstBuffer *inbuf, GstBuffer *outbuf ) {
				wTransform<InFormat,OutFormat> *self = (wTransform<InFormat,OutFormat>*)GetSelf(G_OBJECT(base));
			
				self->In.UseBuffer(inbuf,GST_BASE_TRANSFORM_SINK_PAD(base));
				self->Out.UseBuffer(outbuf,GST_BASE_TRANSFORM_SRC_PAD(base));
            
				GST_BUFFER_TIMESTAMP(outbuf) = GST_BUFFER_TIMESTAMP(inbuf);
				GST_BUFFER_DURATION(outbuf) = GST_BUFFER_DURATION(inbuf);
			
			//g_message("IN size %i, OUT size %i", GST_BUFFER_SIZE(inbuf), GST_BUFFER_SIZE(outbuf) );
				self->Process( &self->In, &self->Out );

            //    g_message("transform time: %lli, duration %lli caps %s, size %i", GST_BUFFER_TIMESTAMP(outbuf), GST_BUFFER_DURATION(outbuf), gst_caps_to_string(GST_BUFFER_CAPS(outbuf)), GST_BUFFER_SIZE(outbuf));
				return GST_FLOW_OK;
			}
			
		virtual void AdaptCapsDownstream( GstCaps *in, GstCaps *out ) {
			}

		virtual void AdaptCapsUpstream( GstCaps *in, GstCaps *out ) {
			}
			
		static GstCaps *transform_caps( GstBaseTransform *base, GstPadDirection direction, GstCaps *caps, GstCaps *filter ) {
				wTransform<InFormat,OutFormat> *self = (wTransform<InFormat,OutFormat>*)GetSelf(G_OBJECT(base));
				printf("--------foo\n");
				if( direction == GST_PAD_SINK ) {
					GstCaps *c = self->Out.GetOriginalCaps();
					GstCaps *outcaps = gst_caps_copy(c);
				//	gst_caps_unref(c);
                 //   g_message("COPY caps for adaptcapsdownstream: %p",gst_caps_get_structure(outcaps,0) );
					self->AdaptCapsDownstream( caps, outcaps );
					return outcaps;
				} else {
					GstCaps *c = self->In.GetOriginalCaps();
					GstCaps *outcaps = gst_caps_copy(c);
				//	gst_caps_unref(c);
					self->AdaptCapsUpstream( caps, outcaps );
					return outcaps;
				}
			}
			
		virtual int OutputSize() {
			return( Out.BufferSize() );
		}

		virtual int InputSize() {
			return( In.BufferSize() );
		}
			
		static gboolean transform_size( GstBaseTransform *base, GstPadDirection direction, GstCaps *caps, guint size, GstCaps *othercaps, guint *othersize ) {
				wTransform<InFormat,OutFormat> *self = (wTransform<InFormat,OutFormat>*)GetSelf(G_OBJECT(base));
				if( direction == GST_PAD_SINK ) {
					self->Out.SetCaps( othercaps );
					*othersize = self->OutputSize();
				//	g_message("%s SINK calc other size: %i",gst_element_get_name(base), *othersize);
					if( *othersize == -1 ) *othersize = size;
				} else {
					self->In.SetCaps( othercaps );
					*othersize = self->InputSize();
//					g_message("%s SRC calc other size: %i",gst_element_get_name(base), *othersize);
					if( *othersize == -1 ) *othersize = size;
				}
				return TRUE;
			}
			
		/**
		 * \brief process function - override
		 * 
		 * manipulate data, return true on success.
		 */
		virtual bool Process( InFormat *in, OutFormat *out ) = 0;
		
	private:
		typedef wBaseTransform inherited;
};

#endif
