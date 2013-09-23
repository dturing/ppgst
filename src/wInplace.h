#ifndef W_wInplace_H
#define W_wInplace_H

#include <wBaseTransform.h>
#include <gst/gstpad.h>

template <class Format>
class wInplace : public wBaseTransform {
	protected:
		Format Data;
		wBooleanProperty Bypass;

	public:
		wInplace()
			: Bypass("bypass","bypass this filter",FALSE)
			{
				AddProperty(Bypass);
			}

		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) {
				wBaseTransform::class_init( k, o, info );
				GstElementClass *klass = (GstElementClass*)k;
				gst_element_class_add_pad_template( klass, Data.SinkTemplate() );
				gst_element_class_add_pad_template( klass, Data.SrcTemplate() );
				GST_BASE_TRANSFORM_CLASS(klass)->transform_ip = transform_ip;
			}
			
		static GstFlowReturn transform_ip( GstBaseTransform *base, GstBuffer *buf ) {
				wInplace<Format> *self = (wInplace<Format>*)GetSelf(G_OBJECT(base));
				if( !self->Element ) self->Element = GST_ELEMENT(base);
				self->Data.UseBuffer( buf, GST_BASE_TRANSFORM_SINK_PAD(base) );
				if( !self->Bypass.Get() ) {
					if( !self->Process( &self->Data ) ) {
						return GST_BASE_TRANSFORM_FLOW_DROPPED;
					}
				}
				return GST_FLOW_OK;
			}
			
		/**
		 * \brief process function - override
		 * 
		 * manipulate data, return true on success.
		 */
		virtual bool Process( Format *data ) = 0;
		
	private:
		typedef wBaseTransform inherited;
};

#endif
