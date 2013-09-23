#ifndef W_wElement_H
#define W_wElement_H

#include <gst/gst.h>
#include <wObject.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Basic Element
 *
 */

template <class wGParent, class wGParentClass>
class wElement : public wObject<wGParent,wGParentClass> {
	public:
		wElement() {
				Element = NULL;
			}
			
		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) {
				wObject<wGParent,wGParentClass>::class_init( k, o, info );
				GstElementClass *klass = (GstElementClass*)k;
				
				gst_element_class_set_metadata( klass,
					(gchar *)info->name, (gchar *)info->classification,
					(gchar *)info->description, (gchar *)info->author );
			}

	protected:
		GstElement *Element;
		
		void error( const char *format, ... ) {
				va_list ap; va_start(ap, format);
				fprintf( stderr, "Element %s Error", GST_ELEMENT_NAME(Element) );
				vfprintf( stderr, format, ap );
				fprintf( stderr, "\n" );
				va_end( ap );
				abort();
			}
		
	public:
};

#endif
