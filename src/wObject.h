#ifndef W_wObject_H
#define W_wObject_H

#include <glib-object.h>
#include <wProperty.h>
#include <wList.h>

class wObjectBase;
	
struct wObjectInfo {
	const char *name;
	const char *description;
	const char *classification;
	const char *author;
	
	virtual wObjectBase *Factor() = 0;
};

class wObjectBase;

template <class wGParent>
struct wGObject {
	wGParent parent;
	wObjectBase* object;
};

template <class wGParentClass>
struct wGObjectClass {
	wGParentClass parent_class;
	wObjectInfo *info;
};

class wObjectBase {
	public:
		virtual void class_init( void *k, wObjectBase *o, wObjectInfo *info ) = 0;
		virtual void instance_init( GTypeInstance *instance, wObjectBase *object, void *klass ) = 0;
	
		static void g_class_init( void *k, gpointer data ) {
				wObjectInfo *info = (wObjectInfo *)data;
				wObjectBase *o = info->Factor();
				o->class_init( k, o, info );
			}
};

template <class wGParent, class wGParentClass>
class wObject : public wObjectBase {
	public:
		/**
		 * \brief Add an Object Property
		 *
		 * should be called only from the constructor of a derived class.
		 */
		void AddProperty( wProperty &property ) {
				Properties.Append( &property );
			}
			
		/**
		 * \brief Called when a property is changed
		 *
		 * can be overridden to react on a change.
		 */
		virtual void PropertyChanged( wProperty *property ) {
			}

	protected:
		wList<wProperty> Properties;
		wGObject<wGParent> *gobject;
	
	public:
		static void set_property( GObject *o, guint id, const GValue *value, GParamSpec *pspec ) {
				wObject<wGParent,wGParentClass> *object = (wObject<wGParent,wGParentClass> *)((wGObject<wGParent>*)o)->object;
				wProperty *property = object->Properties[id];
				if( property ) {
				//	printf("setting property [%s][%s]\n", property->GetName(), g_strdup_value_contents( value ) );
					property->SetValue( value );
					object->PropertyChanged( property );
				} else {
					G_OBJECT_WARN_INVALID_PROPERTY_ID( o, id, pspec );
				}
			}

		static void get_property( GObject *o, guint id, GValue *value, GParamSpec *pspec ) {
				wObject<wGParent,wGParentClass> *object = (wObject<wGParent,wGParentClass> *)((wGObject<wGParent>*)o)->object;
				wProperty *property = object->Properties[id];
				if( property ) {
					property->GetValue( value );
				} else {
					G_OBJECT_WARN_INVALID_PROPERTY_ID( o, id, pspec );
				}
			}
			
		virtual void class_init( void *kl, wObjectBase *o, wObjectInfo *info ) {
				wGObjectClass<wGParentClass> *k = (wGObjectClass<wGParentClass>*)kl;
				GObjectClass *oklass = (GObjectClass*)&(k->parent_class);
				wObject<wGParent,wGParentClass> *object = (wObject<wGParent,wGParentClass>*)o;
			
				oklass->set_property = set_property;
				oklass->get_property = get_property;
			
				int id=1;
				wListIterator<wProperty> j(&object->Properties);
				while( wProperty *property = j.Next() ) {
					property->AddToClass( oklass, id++ );
				}
				
				k->info = info;
			}

		static void g_instance_init( GTypeInstance *instance, gpointer k ) {
				wGObjectClass<wGParentClass> *klass = (wGObjectClass<wGParentClass> *)k;
				wGObject<wGParent> *o = (wGObject<wGParent>*)instance;
				o->object = klass->info->Factor();
				wObjectBase *object = (wObjectBase*)o->object;
				object->instance_init( instance, object, klass );
			}
			
		virtual void instance_init( GTypeInstance *instance, wObjectBase *o, void *kl ) {
				wGObjectClass<wGParentClass> *klass = (wGObjectClass<wGParentClass>*)kl;
				wGObject<wGParent> *gobject = (wGObject<wGParent>*)instance;
				wObject<wGParent,wGParentClass> *object = (wObject<wGParent,wGParentClass>*)gobject->object;
				object->gobject = gobject;
				
				wListIterator<wProperty> j(&object->Properties);
				while( wProperty *property = j.Next() ) {
					property->AddToElement( G_OBJECT(gobject) );
				}
			}
			
		static size_t get_class_size() {
				return sizeof( wGObjectClass<wGParentClass> );
			}
		static size_t get_instance_size() {
				return sizeof( wGObject<wGParent> );
			}
		static GInstanceInitFunc get_instance_init() {
				return (GInstanceInitFunc)wObject<wGParent,wGParentClass>::g_instance_init;
			}
			
		static wObject<wGParent,wGParentClass>* GetSelf( GObject *obj ) {
				wGObject<wGParent>* gobj = (wGObject<wGParent>*)obj;
				return (wObject<wGParent,wGParentClass>*)gobj->object;
			}
};

#endif
