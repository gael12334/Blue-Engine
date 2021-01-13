#ifndef blue_engine_arrayList
#define blue_engine_arrayList
#include "../../object.h"

#define SET_ARRAYLIST_CHUNKSIZE 22

/// \brief Implements a data structure using arrays. 
/// \brief This struct cannot be derivated.
typedef struct ArrayList_t {
	Object_t 		base; 			// Base struct
	Object_t* 		data; 			// Elements of the array list
	unsigned int 	allocated; 		// the total amount of Object_t's allocated
	unsigned int 	count; 			// the virtual amount of Object_t's
	unsigned int 	id;				// accepted id's
} ArrayList_t;

// foward declaration
ArrayList_t* ArrayList_ctor(unsigned int id);

// vtable functions 
void* ArrayList_copy(void* self) {
	if(self) {
		ArrayList_t* arr = (ArrayList_t*) self;
		ArrayList_t* copy = ArrayList_ctor(arr->id);
		copy->allocated = arr->allocated;
		copy->count = arr->count;
		copy->data = (Object_t*) malloc(sizeof(Object_t) * copy->allocated);
		for(unsigned int i = 0; i < copy->allocated; i++) 
			copy->data[i] = arr->data[i];
		return copy;
	}
	return 0;
}

Bool_t ArrayList_equals(void* self, void* reference) {
	if(self && reference) {
		ArrayList_t* arr1 = (ArrayList_t*) self;
		ArrayList_t* arr2 = (ArrayList_t*) reference;
		
		if(arr1->count == arr2->count) {
			if(arr1->id == arr2->id) {
				for(unsigned int i = 0; i < arr1->count; i++) {
					if(!Object_equals(Object_ctor(arr1->data[i].self, arr1->data[i].id), Object_ctor(arr2->data[i].self, arr2->data[i].id)))
						return FALSE;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

Bool_t ArrayList_dtor(void** self) {
	if(self) {
		if(*self) {
			ArrayList_t* arr = (ArrayList_t*) *self;
			if(arr->data) 
				free(arr->data);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

// type id
const unsigned int Array_id = TypeRegister_register(ArrayList_copy, ArrayList_equals, ArrayList_dtor, sizeof(ArrayList_t), 0, "ArrayList_t", 0);

// functions
inline ArrayList_t* ArrayList_ctor(unsigned int id) {
	ArrayList_t* arr = (ArrayList_t*) malloc(sizeof(sizeof(ArrayList_t)));
	*arr = { Object_ctor(arr, Array_id), 0x0, 0, 0, id };
	return arr;
}

inline unsigned int ArrayList_getCount(void* self) {
	ArrayList_t* arr = (ArrayList_t*) self;
	return arr->count;
}

Bool_t ArrayList_pushBack(void* self, const Object_t obj) {
	ArrayList_t* arr = (ArrayList_t*) self;
	if(arr) {
		if(obj.id == arr->id) {
			if(arr->count == arr->allocated) {
				arr->data = (Object_t*) realloc(arr->data, sizeof(Object_t) * (arr->allocated + SET_ARRAYLIST_CHUNKSIZE));
				arr->allocated += SET_ARRAYLIST_CHUNKSIZE;
			}
			arr->data[arr->count] = obj;
			arr->count++;
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t ArrayList_popBack(void* self) {
	ArrayList_t* arr = (ArrayList_t*) self;
	if(arr->count) {
		arr->count--;
		if(arr->count == (arr->allocated - SET_ARRAYLIST_CHUNKSIZE)) {
			arr->data = (Object_t*) realloc(arr->data, sizeof(Object_t) * (arr->allocated - SET_ARRAYLIST_CHUNKSIZE));
			arr->allocated -= SET_ARRAYLIST_CHUNKSIZE;
		}
		return TRUE;
	}
	return FALSE;
}

Object_t ArrayList_get(void* self, unsigned int index) {
	ArrayList_t* arr = (ArrayList_t*) self;
	if(index < arr->count)
		return arr->data[index];

	return Object_null();
}

Bool_t ArrayList_set(void* self, unsigned int index, Object_t obj) {
	ArrayList_t* arr = (ArrayList_t*) self;

	if(index < arr->count) {
		if(obj.id == arr->id) {
			arr->data[index] = obj;
			return TRUE;
		}
	}
	
	return FALSE;
}


#endif