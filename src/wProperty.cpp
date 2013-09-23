#include "wProperty.h"
#include <stdio.h>
#include <gst/gst.h>
#include <string.h>

wProperty::wProperty( const char *name, GParamSpec *spec, GType type ) {
	Name = name;
	Type = type;
	Spec = spec;
}

void wProperty::AddToClass( GObjectClass *k, int id ) {
	g_object_class_install_property( k, id, Spec );
}

void wProperty::AddToElement( GObject *element ) {
	Object = element;
}

template <class T>
wTypedProperty<T>::wTypedProperty( const char *name, GParamSpec *spec, GType type, T default_value,
		T (*getFunc)(const GValue*), void (*setFunc)(GValue*,T) )
		: wProperty( name, spec, type )
{

	Value = Default = default_value;
	
	GetFunc = getFunc;
	SetFunc = setFunc;
}


wFloatProperty::wFloatProperty( 
	const char *name, const char *blurb,
	float default_value, float minimum, float maximum,
	const GParamFlags flags
	)
	: wTypedProperty<float>( name,
		g_param_spec_float( name, name, blurb, minimum, maximum, default_value, flags ),
		G_TYPE_FLOAT, default_value,
		g_value_get_float, g_value_set_float
		)
{
}

wDoubleProperty::wDoubleProperty( 
	const char *name, const char *blurb,
	double default_value, double minimum, double maximum,
	const GParamFlags flags
	)
	: wTypedProperty<double>( name,
		g_param_spec_double( name, name, blurb, minimum, maximum, default_value, flags ),
		G_TYPE_DOUBLE, default_value,
		g_value_get_double, g_value_set_double
		)
{
}

wIntegerProperty::wIntegerProperty( 
	const char *name, const char *blurb,
	int default_value, int minimum, int maximum,
	const GParamFlags flags
	)
	: wTypedProperty<int>( name,
		g_param_spec_int( name, name, blurb, minimum, maximum, default_value, flags ),
		G_TYPE_INT, default_value,
		g_value_get_int, g_value_set_int
		)
{
}

wBooleanProperty::wBooleanProperty( 
	const char *name, const char *blurb,
	gboolean default_value,
	const GParamFlags flags
	)
	: wTypedProperty<gboolean>( name,
		g_param_spec_boolean( name, name, blurb, default_value, flags ),
		G_TYPE_BOOLEAN, default_value,
		g_value_get_boolean, g_value_set_boolean
		)
{
}

wStringProperty::wStringProperty( 
	const char *name, const char *blurb,
	const char *default_value,
	const GParamFlags flags )
	: wTypedProperty<const char*>( name,
		g_param_spec_string( name, name, blurb, default_value, flags ),
		G_TYPE_STRING, default_value,
		g_value_get_string, g_value_set_string
	)
{
	Value = g_strdup(Default);
}

wEnumProperty::wEnumProperty( 
	const char *name, const char *blurb,
	GType enum_type, int default_value,
	const GParamFlags flags
	)
	: wTypedProperty<int>( name,
		g_param_spec_enum( name, name, blurb, enum_type, default_value, flags ),
		G_TYPE_ENUM, default_value,
		g_value_get_enum, g_value_set_enum
		)
{
}
