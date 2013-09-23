#ifndef W_PROPERTY_H
#define W_PROPERTY_H

#include <glib-object.h>
#include <string.h>

/**
 * \brief Basic Property
 *
 * The wProperty class interfaces GObject Properties to the Warsaw doctrine
 * of doing it in C++. Children of wProperty usually appear as member variables
 * of \ref wElement s, and are added to the element class in its constructor
 * with \ref wElement::AddProperty().
 *
 * You wouldn't use wProperty directly in a \ref wElement, instead, 
 * use a concrete class like \ref wFloatProperty, \ref wIntegerProperty,
 * \ref wStringProperty or another subclass.
 */
class wProperty {
	public:
		wProperty( const char *name, GParamSpec *spec, GType type );
	
		/** a subclass would have to override this
		* to set its internal value <b>from</b> a GValue. */
		virtual void SetValue( const GValue *value ) = 0;

		/** a subclass would have to override this
		* to convert its internal value <b>to</b> a GValue. */
		virtual void GetValue( GValue *value ) = 0;
	
		const char *GetName() { return Name; }
	
	protected:
		const char *Name;
		GType Type;
		GParamSpec *Spec;

		GObject *Object;
	
	public:
		void AddToClass( GObjectClass *k, int id );
		void AddToElement( GObject *element );
};


/**
 * \brief Templated Property
 */
template <class T>
class wTypedProperty : public wProperty {
	public:
		wTypedProperty( const char *name, GParamSpec *spec, GType type, T default_value,
			T (*getFunc)(const GValue*), void (*setFunc)(GValue*,T) );

		// GProperty interface functions
		virtual void SetValue( const GValue *value ) { if( GetFunc ) Value = GetFunc(value); }
		virtual void GetValue( GValue *value ) { if( SetFunc ) SetFunc(value,Value); }

		T Get() { return( Value ); }
		void Set( T v ) { if( Object ) g_object_set( Object, Name, v, NULL ); else Value = v; }
	
		operator T() { return Value; }
		T operator =( T c ) { return Value=c; }
//		void SetReference( GObject *o, const char *name ) { Object=o; Name=name; }
//		bool Is( void *p ) { return Value == p; }
//		void PropagateUpdate();
	
	protected:
		T Value;
		T Default;
	
		T (*GetFunc)(const GValue*);
		void (*SetFunc)(GValue*,T);
};

/**
 * \brief Floating-point Property
 */
class wFloatProperty : public wTypedProperty<float> {
	public:
		wFloatProperty(
			const char *name, const char *blurb,
			float default_value, float minimum, float maximum,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

	protected:
};

/**
 * \brief Double-precision Floating-point Property
 */
class wDoubleProperty : public wTypedProperty<double> {
	public:
		wDoubleProperty(
			const char *name, const char *blurb,
			double default_value, double minimum, double maximum,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

	protected:
};

/**
 * \brief Integer Property
 */
class wIntegerProperty : public wTypedProperty<int> {
	public:
		wIntegerProperty(
			const char *name, const char *blurb,
			int default_value, int minimum, int maximum,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

	protected:
};

/**
 * \brief Boolean Property
 */
class wBooleanProperty : public wTypedProperty<gboolean> {
	public:
		wBooleanProperty(
			const char *name, const char *blurb,
			gboolean default_value,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

	protected:
};

/**
 * \brief String Property
 */
class wStringProperty : public wTypedProperty<const char*> {
	public:
		wStringProperty(
			const char *name, const char *blurb,
			const char *default_value,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

		virtual void SetValue( const GValue *value ) {
				if( Value ) { g_free((char*)Value); Value=NULL; }
				if( GetFunc ) Value = g_strdup(GetFunc(value));
			}

	protected:
			// T Value, derived from wTypedProperty, is really a "char*" here, not const.
};

/**
 * \brief Enumeration Property
 */
class wEnumProperty : public wTypedProperty<int> {
	public:
		wEnumProperty(
			const char *name, const char *blurb,
			GType enum_type, int default_value,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			);

	protected:
};

/**
 * \brief Array (of GValues) Property
 */
class wArrayProperty : public wProperty {
	public:
		wArrayProperty(
			const char *name, const char *blurb,
			GParamSpec *value_spec = NULL,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			) 
			: wProperty( name, g_param_spec_value_array(name,name,blurb,value_spec,flags), G_TYPE_ARRAY )
			{
				value = g_array_new(false, true, 32);
			}
			
		~wArrayProperty() {
			g_array_unref( value );
		}

		virtual void SetValue( const GValue *v ) {
				g_array_unref( value );
				value = g_array_ref((GArray*)g_value_get_boxed( v ));
			}
		virtual void GetValue( GValue *v ) {
				g_value_set_boxed( v, value );
			}

		unsigned int Length() { return( value?value->len:0 ); }
		void Clear() { g_array_unref( value ); value = g_array_new(false,true,32); }
		void Set() { if( Object ) g_object_set( Object, Name, value, NULL ); }

		GValue *GetEntry( unsigned int n ) { 
			if( n<Length() ) return NULL;
			return( &g_array_index( value, GValue, n ) );
			}
		void AppendEntry( const GValue *v ) { g_array_append_val( value, v ); }
		
	protected:
		GArray *value;
};

/**
 * \brief Array (of float-GValues) Property
 */
class wFloatArrayProperty : public wArrayProperty {
	public:
		wFloatArrayProperty(
			const char *name, const char *blurb,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			)
			: wArrayProperty( name, blurb, NULL, flags )
			{ ; }
		
		float Get( unsigned int i ) { 
				GValue *entry = GetEntry(i);
				if( entry )
					return g_value_get_float(entry); 
				return -1;
			}
		void Append( float v ) {
				GValue *val = (GValue*)g_malloc( sizeof(GValue) );
				memset(val,0,sizeof(GValue));
				g_value_init( val, G_TYPE_FLOAT );
				g_value_set_float( val, v );
				AppendEntry( val );
				g_free( val );
			}
};

/**
 * \brief Pointer Property
 */
template <class T>
class wPointerProperty : public wProperty {
	public:
		wPointerProperty(
			const char *name, const char *blurb,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			) 
			: wProperty( name, g_param_spec_pointer(name,name,blurb,flags), G_TYPE_POINTER )
			{
				value = NULL;
			}
			
		~wPointerProperty() {
		}

		virtual void SetValue( const GValue *v ) {
				value = (T*)g_value_get_pointer(v);
			}
		virtual void GetValue( GValue *v ) {
				g_value_set_pointer( v, (gpointer)value );
			}

		T* Get() { return( value ); }
		void Set( T* v ) { if( Object ) g_object_set( Object, Name, v, NULL ); else value = v; }

	protected:
		T *value;
};

/**
 * \brief GObject Property
 *
 * wraps a GObject (glib/gtk) into a property, transfers it binarily
 * very experimental.
 */
class wGObjectProperty : public wProperty {
	public:
		wGObjectProperty(
			const char *name, const char *blurb,
			GType type,
			const GParamFlags flags = (GParamFlags)G_PARAM_READWRITE
			) 
			: wProperty( name, g_param_spec_object(name,name,blurb,type,flags), G_TYPE_OBJECT )
			{
				value = NULL;
			}
			
		~wGObjectProperty() {
			if( value ) g_object_unref( value );
		}

		virtual void SetValue( const GValue *v ) {
			//	if( value ) g_object_unref( value );
				value = G_OBJECT(g_value_get_object(v));
			//	if( value ) g_object_ref( value );
			}
		virtual void GetValue( GValue *v ) {
				if( value ) 
					g_value_set_object( v, (gpointer)value );
			}

		GObject *Get() { return value; }
		void Set( GObject *obj ) {
				if( obj ) g_object_set( Object, Name, obj, NULL ); else value = obj;
			}

	protected:
		GType type;
		GObject *value;
};

#endif
