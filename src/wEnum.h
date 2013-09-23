#ifndef W_ENUM_H
#define W_ENUM_H

#include <glib-object.h>
#include <wList.h>

struct wEnumValue {
	const char *nick;
	int value;
};

class wEnum {
	public:
//		wEnum( const char *name, va_list args );
		wEnum( const char *name, ... );
		GType Type();
	
	protected:
		GType type;
		const char *Name;
		wList<wEnumValue> values;
	
	private:
		void Init( const char *name, va_list args );
};

#endif
