#ifndef W_wProducer_H
#define W_wProducer_H

#include <wBaseSrc.h>

template <class Format>
class wProducer : public wBaseSrc {
	protected:
		Format Data;
		guint64 RunningTime;
		
		GstBufferPool *pool;
	
	public:
		wProducer()
			{
				RunningTime = 0;
				pool = NULL;
			}

		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) {
				wBaseSrc::class_init( k, o, info );
				GstElementClass *klass = (GstElementClass*)k;
				gst_element_class_add_pad_template( klass, Data.SrcTemplate() );

				GST_BASE_SRC_CLASS(klass)->create = create;
				GST_BASE_SRC_CLASS(klass)->set_caps = setcaps;
				GST_BASE_SRC_CLASS(klass)->negotiate = negotiate;
			}
			
		bool create_bufferpool( GstCaps *caps ) {
			GstQuery *query;
			GstBufferPool *p;
			guint size;
			GstStructure *config;
			/*
			query = gst_query_new_allocation( caps, TRUE );
			if( gst_pad_peer_query( GST_BASE_SRC_PAD(Element), query )) {
				gst_query_parse_allocation_params( query, &alignment, &prefix, &size, &pool );
			} else {
				alignment=16; prefix = 0;
				size = Data.BufferSize();
			}
			*/
			p=NULL;
			
			if( p == NULL ) {
				p = gst_buffer_pool_new();
			}
			
			config = gst_buffer_pool_get_config(p);
			// XXX at least set size?
			gst_buffer_pool_set_config( p, config );
			
			if( pool ) gst_object_unref( pool );
			pool = p;
			
			gst_buffer_pool_set_active( pool, TRUE );
			return TRUE;
		}
			
		static gboolean negotiate( GstBaseSrc *base ) {
				wProducer<Format> *self = (wProducer<Format>*)GetSelf(G_OBJECT(base));
				GstCaps *caps;
				GstCaps *temp;
				gboolean result = FALSE;
				
				/* get all possible caps on this link */
				caps = gst_pad_get_allowed_caps (GST_BASE_SRC_PAD (base));
				temp = gst_caps_normalize (caps);
				gst_caps_unref (caps);
				caps = temp;
				
				if (gst_caps_get_size (caps) > 0) {
					GstStructure *structure;
					/* pick the first one */
					gst_caps_truncate (caps);
					
					self->Data.Fixate( caps );
					result = gst_pad_set_caps (GST_BASE_SRC_PAD (base), caps);
					gst_caps_unref (caps);
				}
				
				return result;
			}
			
		static gboolean setcaps( GstBaseSrc *base, GstCaps *caps ) {
				wProducer<Format> *self = (wProducer<Format>*)GetSelf(G_OBJECT(base));
				self->Data.SetCaps( caps );
				g_message("SetCaps on producer: %ix%i:%.2f", self->Data.Width(), self->Data.Height(), self->Data.Framerate() );
				
				self->create_bufferpool( caps );
			}
			
		static GstFlowReturn create( GstBaseSrc *base, guint64 offset, guint length, GstBuffer **buf ) {
				wProducer<Format> *self = (wProducer<Format>*)GetSelf(G_OBJECT(base));
				
				printf("create frame. pool: %p, element %p\n", self->pool, self->Element );

				if( !self->pool ) self->create_bufferpool( gst_pad_get_current_caps( GST_BASE_SRC_PAD (base) ) );

				printf("create frame. pool: %p\n", self->pool );
				gst_buffer_pool_acquire_buffer( self->pool, buf, NULL );
				printf("got buffer, size %i\n", GST_BUFFER_SIZE(buf) );
				/*
				if( gst_pad_alloc_buffer (GST_BASE_SRC_PAD (base), GST_BUFFER_OFFSET_NONE,
						self->Data.BufferSize(), gst_pad_get_current_caps (GST_BASE_SRC_PAD (base)), buf) // XXX JUSTFORREFERENCE
					!= GST_FLOW_OK ) return GST_FLOW_ERROR;
				*/
				self->Data.UseBuffer(*buf,GST_BASE_SRC_PAD(base));

				GST_BUFFER_TIMESTAMP(*buf) = self->RunningTime;
                GST_BUFFER_OFFSET(*buf) = GST_BUFFER_OFFSET_NONE;

				self->Process( &self->Data );

				self->RunningTime += GST_BUFFER_DURATION(*buf) = (guint64)(GST_SECOND / (int)self->Data.Framerate());
            //    g_message("Framerate: %f time %lli\n", self->Data.Framerate(), self->RunningTime );
				return GST_FLOW_OK;
			}
			
		/**
		 * \brief process function - override
		 * 
		 * manipulate data, return true on success.
		 */
		virtual bool Process( Format *data ) = 0;
		
	private:
		typedef wBaseSrc inherited;
};

#endif
