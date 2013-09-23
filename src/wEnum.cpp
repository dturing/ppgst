#include <wEnum.h>
#include <wList.h>

#include <memory.h>
#include <stdio.h>


wEnum::wEnum( const char *name, ... ) {
	type = 0;
	va_list args;
	va_start(args,name);
	Init(name,args);
	va_end(args);
}
/*
wEnum::wEnum( const char *name, va_list args ) {
	Init(name,args);
}
*/
void wEnum::Init( const char *name, va_list args ) {
	wEnumValue *value;
	value = new wEnumValue;
	value->nick = va_arg(args,const char *);
	while( value->nick ) {
		value->value = va_arg(args,int);
		
//		printf("init enum '%s', value %i nick %s\n", name, value->value, value->nick );
		values.Append(value);
		
		// next
		value = new wEnumValue;
		value->nick = va_arg(args,const char *);
	}
	delete value;
	
	Name = name;
	
	type = NULL;
}

GType wEnum::Type() {
	if( type==0 ) {
		if( !values.Length() ) {
			g_warning("cannot initialize enum %s, no values", Name );
			return 0;
		}
		GEnumValue *gvalues = new GEnumValue[values.Length()+1];
		memset( gvalues, 0, sizeof(GEnumValue)*(values.Length()+1) );
		wListIterator<wEnumValue> i(&values);
		wEnumValue *value;
		for( int n=0; (value=i.Next())!=NULL; n++ ) {
			gvalues[n].value = value->value;
			gvalues[n].value_name = g_strdup_printf("%s_%s",Name,value->nick);
			gvalues[n].value_nick = g_strdup(value->nick);
//			printf("Register enum value %s, %s, %i\n", gvalues[n].value_name, gvalues[n].value_nick, gvalues[n].value );
//			delete value;
		}
		type = g_enum_register_static(Name,gvalues);
	}

	return( type );
}
