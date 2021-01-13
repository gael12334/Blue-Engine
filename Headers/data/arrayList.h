#ifndef BLUE_ENGINE_ARRAYLIST
#define BLUE_ENGINE_ARRAYLIST
#include "../object.h"

#define SET_ARRAYLIST_CHUNKSIZE 22

#ifdef SET_ARRAYLIST_CHUNKSIZE 
// the size to be added or removed
static const unsigned int arrayListChunkSize = SET_ARRAYLIST_CHUNKSIZE;

#else 
// the size to be added or removed
static const unsigned int arrayListChunkSize = 10;
#endif

/// \brief Implements a data structure using arrays. 
/// \brief This struct cannot be derivated.
typedef struct ArrayList_t {
	Object_t 		base; 			// Base struct
	Object_t* 		data; 			// Elements of the array list

	unsigned int 	allocated; 		// the total amount of Object_t's allocated
	unsigned int 	count; 			// the virtual amount of Object_t's

	unsigned int 	acceptedTypeId;
} ArrayList_t;

// type id
const int ArrayListId = registerType("ArrayList_t", sizeof(ArrayList_t));

// foward declaration
ArrayList_t* ArrayList_ctor(unsigned int id);

void* ArrayList_copy(void* self) {
	if(self) {
		ArrayList_t* arr = (ArrayList_t*) self;
		ArrayList_t* copy = ArrayList_ctor(arr->acceptedTypeId);
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
			if(arr1->acceptedTypeId == arr2->acceptedTypeId) {
				for(unsigned int i = 0; i < arr1->count; i++) {
					if(!arr1->data[i].equals(arr1->data[i].reference, arr2->data[i].reference))
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
			free(arr);

			return TRUE;
		}
	}
	return FALSE;
}

ArrayList_t* ArrayList_ctor(unsigned int id) {
	ArrayList_t* arr = (ArrayList_t*) malloc(sizeof(sizeof(ArrayList_t)));
	*arr = {
		Object_ctor(arr, ArrayListId, ArrayList_copy, ArrayList_equals, ArrayList_dtor),
		0x0,
		0,
		0,
		id
	};

	return arr;
}

inline unsigned int ArrayList_getCount(ArrayList_t* self) {
	if(self) 
		return self->count;
	return 0;
}

Bool_t ArrayList_pushBack(ArrayList_t* arr, const Object_t obj) {
	if(arr) {
		if(obj.id == arr->acceptedTypeId) {
			if(arr->count == arr->allocated) {
				arr->data = (Object_t*) realloc(arr->data, sizeof(Object_t) * (arr->allocated + arrayListChunkSize));
				arr->allocated += arrayListChunkSize;
			}
			arr->data[arr->count] = obj;
			arr->count++;
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t ArrayList_popBack(ArrayList_t* arr) {
	if(arr) {
		if(arr->count) {
			arr->count--;
			if(arr->count == (arr->allocated - arrayListChunkSize)) {
				arr->data = (Object_t*) realloc(arr->data, sizeof(Object_t) * (arr->allocated - arrayListChunkSize));
				arr->allocated +- arrayListChunkSize;
			}
			return TRUE;
		}
	}
	return FALSE;
}

Object_t ArrayList_get(ArrayList_t* arr, unsigned int index) {
	if(arr) {
		if(index < arr->count)
			return arr->data[index];
	}
	return Object_ctorNull();
}

Bool_t ArrayList_set(ArrayList_t* arr, unsigned int index, const Object_t obj) {
	if(arr) {
		if(index < arr->count) {
			if(obj.id == arr->acceptedTypeId) {
				arr->data[index] = obj;
				return TRUE;
			}
		}
	}
	return FALSE;
}

#endif