#ifndef W_wOutofplace_H
#define W_wOutofplace_H

#include <wBaseTransform.h>

 
template <class Format>
class wOutofplace : public wBaseTransform {
	protected:
		Format In, Out;
		wBooleanProperty Bypass;

	public:
		wOutofplace()
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
				GST_BASE_TRANSFORM_CLASS(klass)->transform_size = transform_size;
			}
			
		static GstFlowReturn transform( GstBaseTransform *base, GstBuffer *inbuf, GstBuffer *outbuf ) {
				wOutofplace<Format> *self = (wOutofplace<Format>*)GetSelf(G_OBJECT(base));
				self->In.UseBuffer(inbuf,GST_BASE_TRANSFORM_SINK_PAD(base));
				self->Out.UseBuffer(outbuf,GST_BASE_TRANSFORM_SRC_PAD(base));
				// TODO: regard bypass
				if( !self->Process( &self->In, &self->Out ) ) {
					return GST_BASE_TRANSFORM_FLOW_DROPPED;
				}
				return GST_FLOW_OK;
			}
			
		static gboolean transform_size( GstBaseTransform *base, GstPadDirection direction, GstCaps *caps, gsize size, GstCaps *othercaps, gsize *othersize ) {
				*othersize = size;
				return TRUE;
			}
			
		/**
		 * \brief process function - override
		 * 
		 * manipulate data, return true on success.
		 */
		virtual bool Process( Format *in, Format *out ) = 0;
		
	private:
		typedef wBaseTransform inherited;
};

#endif
