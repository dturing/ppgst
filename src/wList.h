#ifndef W_LIST_H
#define W_LIST_H

#include <glib.h>

/* a very basic C++ wrapper to and iterator for GList */


// forward decl
template <class T> class wListIterator;

// list class
template <class T>
class wList {
	public:
		wList() 		{ List=NULL; }
		~wList()		{ Free(); }
	
		void Append( T* d )	{ List = g_list_append( List, (gpointer)d ); }
		void Prepend( T* d ) { List = g_list_prepend( List, (gpointer)d ); }
		void Remove( T* d ) { List = g_list_remove(List,d); }
		
		GList *GetList() 	{ return List; }
		unsigned int Length() { return g_list_length( List ); }
		T* operator[]( int n ) { return (T*)g_list_nth_data(List,n-1); }
		
		wListIterator<T> &Iterator() { return wListIterator<T>(this); }
		
		void Free() {
			GList *c = List;
			while( c ) {
				if( c->data ) g_free( c->data );
				c = g_list_next(c);
			}
			g_list_free( List );
			List = NULL;
		}
	protected:
		GList *List;
};

// iterator class
template <class T>
class wListIterator {
	public:
		wListIterator( wList<T> *list ) {
			List = list->GetList();
		}
		
		T* Next() { T* ret = ( List?(T*)List->data:NULL );
					List = g_list_next( List );
					return ret; }
				
	protected:
		GList *List;
};

#endif
