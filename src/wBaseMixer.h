#ifndef W_wBaseMixer_H
#define W_wBaseMixer_H

#include <gst/gstelement.h>
#include <gst/base/gstcollectpads.h>
#include <wElement.h>

template <class Format>
class wBaseMixer : public wElement<GstElement,GstElementClass> {
	public:
		wBaseMixer() {
				collect = gst_collect_pads_new();
				gst_collect_pads_set_function( collect, collected, this );
				src = NULL;
			}
			
		~wBaseMixer() {
				g_mutex_free( lock );
			}

		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) {
				wElement<GstElement,GstElementClass>::class_init( k, o, info );
				GstElementClass *klass = (GstElementClass*)k;

				gst_element_class_add_pad_template( klass, Out.RequestSinkTemplate() );
				gst_element_class_add_pad_template( klass, Out.SrcTemplate() );
			
				klass->change_state = change_state;
				klass->request_new_pad = request_new_pad;
			}

		virtual void instance_init( GTypeInstance *instance, wObjectBase *object, void *k ) {
				wElement<GstElement,GstElementClass>::instance_init( instance, object, k );
				GstElementClass *klass = (GstElementClass*)k;
				GstElement *element = (GstElement*)instance;
			
				src = gst_pad_new_from_template( Out.SrcTemplate(), "src" );
				gst_element_add_pad( element, src );
			}
			
		virtual GstFlowReturn Collected() = 0;
			/*	// sample behaviour (feed all buffers to src):
					GstFlowReturn ret;
					GstBuffer *outbuf;
				
					GSList *walk = collect->data;
					while( walk ) {
						GstCollectData *data = (GstCollectData*)walk->data;
						GstBuffer *buf;
						
						buf = gst_collect_pads_pop( collect, data );
						if( buf ) {
							ret = gst_pad_push( src, buf );
							if( ret != GST_FLOW_OK ) return ret;
						}
						
						walk = g_slist_next( walk );
					}
			
				// to create a buffer, do sth like:
					ret = gst_pad_alloc_buffer( src, GST_BUFFER_OFFSET_NONE, 1, GST_PAD_CAPS( src ), &outbuf );
					if( ret != GST_FLOW_OK ) return ret;
						
					ret = gst_pad_push( src, outbuf );
			*/
			
		static GstFlowReturn collected( GstCollectPads *pad, void *mixer ) {
				wBaseMixer *self = (wBaseMixer*)mixer;
				return self->Collected();
			}
			
		static GstStateChangeReturn change_state( GstElement *element, GstStateChange transition ) {
				wBaseMixer *self = (wBaseMixer*)GetSelf(G_OBJECT(element));
				GstStateChangeReturn ret;
				
				switch (transition) {
					case GST_STATE_CHANGE_READY_TO_PAUSED:
						GST_LOG ("starting collect_pads");
						gst_collect_pads_start( self->collect );
						break;
					case GST_STATE_CHANGE_PAUSED_TO_READY:
						GST_LOG ("stopping collect_pads");
						gst_collect_pads_stop( self->collect );
						break;
					default:
						break;
				}
				
				// FIXME BAAD! unref!
				ret = GST_ELEMENT_CLASS(g_type_class_ref(GST_TYPE_ELEMENT))->change_state (element, transition);
				
				return ret;
			}
			
		static GstPad *request_new_pad( GstElement *element, GstPadTemplate *templ, const gchar *name ) {
				wBaseMixer *self = (wBaseMixer*)GetSelf(G_OBJECT(element));
				GstPad *pad;
				GstCollectData *col;
			
				g_return_val_if_fail (templ != NULL, NULL);
				
				if (templ->direction != GST_PAD_SINK) {
					g_warning ("wBaseMixer: request pad that is not a SINK pad\n");
					return NULL;
				}
				if (templ == gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS(element), "sink_%d")) {
					pad = gst_pad_new_from_template( templ, name );
					col = gst_collect_pads_add_pad( self->collect, pad, sizeof(GstCollectData) );
				} else {
					g_warning ("wBaseMixer: this is not our template!\n");
					return NULL;
				}
				
				gst_element_add_pad (element, GST_PAD (pad));
				
				return GST_PAD (pad);
			}
		
	protected:
		GstCollectPads *collect;
		GstPad *src;
		Format Out;
		GMutex *lock;
	
	public:
		static GType get_parent_type() {
				return GST_TYPE_ELEMENT;
			}
};

#endif
