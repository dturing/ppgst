#ifndef W_REGISTER_H
#define W_REGISTER_H

#include <wObject.h>
#include <new>
#include <gst/base/gstbasetransform.h>


template <class T>
struct wTypedElementInfo : public wObjectInfo {
	virtual wObjectBase *Factor() {
			T *n = new T;
			return (wObjectBase*)n;
		}
};

template<class T>
void wRegister( const char *name, const char *description, 
	const char *classification, const char *author, 
	GstPlugin *plugin ) {

	wObjectInfo *meta = new wTypedElementInfo<T>;
	meta->name = name;
	meta->description = description;
	meta->classification = classification;
	meta->author = author;

	// create and register the GType
	GTypeInfo *info = new GTypeInfo;
	info->class_size = T::get_class_size(); //sizeof( wGObjectClass<wGParentClass> );
	info->base_init = NULL;
	info->base_finalize = NULL;
	info->class_init = (GClassInitFunc)wObjectBase::g_class_init;
	info->class_finalize = NULL;
	info->class_data = meta;
	info->instance_size = T::get_instance_size(); //sizeof( wGObject<wGParent> );
	info->n_preallocs = 0;
	info->instance_init = T::get_instance_init(); //(GInstanceInitFunc)wObject<wGParent,wGParentClass>::g_instance_init;
	info->value_table = NULL;
	GType type = g_type_register_static( T::get_parent_type(), name, info, (GTypeFlags)0 );

	gst_element_register( plugin, name, GST_RANK_NONE, type );
}

#endif
