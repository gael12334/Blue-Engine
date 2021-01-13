#ifndef BLUE_ENGINE_OBJECT
#define BLUE_ENGINE_OBJECT
#include "typeinfo.h"
#include "boolean.h"

typedef void*(*CopyFn_t)(void*); 
typedef Bool_t(*CmpFn_t)(void*, void*);
typedef Bool_t(*DtorFn_t)(void**);


/// @brief Generic structure providing an interface between multiple types.
typedef struct Object_t {
	void* 			reference;							// Memory block of the object's type. 
	unsigned int 	id;									// Object's type id.

	/// @brief Function copying the memory block of this object.
	/// @param self Memory block to copy.
	/// @return returns a copied memory block.
	void*			(*copy)(void* self); 				// Function copying the memory block of this object.

	/// @brief Function compares two memory block.
	/// @param self Memory block to copy.
	/// @param ref Memory block to copy.
	/// @return returns a boolean value.
	Bool_t			(*equals)(void* self, void* ref); 	// Function copying the memory block of this object.

	/// @brief Function freeing the object's memory block.
	/// @param self_ptr Memory block to copy.
	/// @return returns a boolean. True = success, false = fail.
	Bool_t			(*dtor)(void** self_ptr);			// Function freeing the object's memory block.

} Object_t;

/// \brief Default function for copying references.
/// \param self Adress of the reference. Can be an integer or a float casted into a void*.
/// \return Returns an adress, or an integer, or a float.
static inline void* Object_defaultCopy(void* self) {
	return self;
}

/// \brief 
/// \param self
/// \param reference
/// \return 
static inline Bool_t Object_defaultEquals(void* self, void* reference) {
	return (Bool_t) (self == reference);
}

/// \brief 
/// \param self_ptr
/// \return 
static Bool_t Object_defaultDtor(void** self_ptr) {
	if(*self_ptr) {
		*self_ptr = 0;
		return TRUE;
	}

	return FALSE;
}

/// \brief 
/// \param
/// \return 
Object_t Object_ctor(void* self, unsigned int id, CopyFn_t copy, CmpFn_t equals, DtorFn_t dtor) {
	Object_t obj = {
		self, 
		id, 
		copy, 
		equals,
		dtor
	};
	return obj;
}

/// \brief 
/// \param
/// \return 
Object_t Object_ctorInt(long long integer) {
	Object_t obj = {
		(void*) integer, 
		intId, 
		Object_defaultCopy, 
		Object_defaultEquals,
		Object_defaultDtor
	};
	return obj;
}

/// \brief 
/// \param
/// \return 
Object_t Object_ctorFloat(double floating) {
	double* floating_ptr = &floating;
	long long* floating_cast = (long long*) floating_ptr;

	Object_t obj = {
		(void*) *floating_cast, 
		floatId, 
		Object_defaultCopy, 
		Object_defaultEquals,
		Object_defaultDtor
	};
	return obj;
}

/// \brief 
/// \param
/// \return 
Object_t Object_ctorNull() {
	Object_t obj = {
		0x0, 
		0, 
		Object_defaultCopy, 
		Object_defaultEquals,
		Object_defaultDtor
	};
	return obj;
}

void* Object_copy(Object_t* self) {
	if(self) {
		if(self->copy) 
			return self->copy(self->reference);
		
		if((self->id == intId) && (self->id == floatId))
			return self->reference;

		unsigned int size = getTypeSize(self->id);
		char* copy = (char*) malloc(size);
		char* ptr = (char*) self->reference;
		for(unsigned int i = 0; i < size; i++) 
			copy[i] = ptr[i];
		return copy;
	}
}

Bool_t Object_equals(Object_t* self, Object_t* object) {
	if(self && object) {
		
		if(self->id == object->id) { // if reference is of same type

			if(self->equals == 0) { // if no function is defined for comparaison, we compare both object's bytes.
				char* content_self = (char*) self->reference;
				char* content_object = (char*) object->reference;

				unsigned int size = getTypeSize(self->id);
				for(unsigned int i = 0; i < size; i++) {
					if(content_self[i] != content_object[i]) 
						return FALSE;
				}
				return TRUE;
			}

			return self->equals(self->reference, object->reference);
		}
	}

	return FALSE;
}

Bool_t Object_dtor(Object_t self) {
	if(self.dtor) 
		return self.dtor(&self.reference);
	
	if(self.reference) {
		free(self.reference);
		return TRUE;
	}

	return FALSE;
}

inline long long Object_toInt(Object_t* self) {
	return (long long) self->reference;
}

double Object_toFloat(Object_t* self) {
	long long* floating_cast = (long long*) self->reference;
	double* floating = (double*) floating_cast;
	return *floating;
}

inline const char * Object_getTypeName(Object_t* self) {
	return getTypeName(self->id);
}

inline void* Object_getReference(Object_t* self) {
	return self->reference;
}

inline unsigned int Object_getTypeId(Object_t* self) {
	return self->id;
}

inline unsigned int Object_getReferenceSize(Object_t* self) {
	return getTypeSize(self->id);
}
#endif