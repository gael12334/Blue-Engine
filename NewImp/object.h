#ifndef blue_engine_object
#define blue_engine_object
#include "types.h"

typedef struct Object_t {
	unsigned int id;
	void* self;
} Object_t;

/// \brief Creates an Object_t instance from an allocated memory block along it's type id.
/// \param self Memory block allocated; must not be a reference to something on the stack.
/// \param id The memory block's type id.
/// \return Returns an Object_t instance.
inline Object_t Object_ctor(void* self, unsigned int id) {
	Object_t obj;
	obj.id = id;
	obj.self = self;
	return obj;
}

/// \brief Creates an Object_t instance from a 64 bit integer, used for general integers. 
/// \param self The desired integer to be held by the Object_t.
/// \return Returns an Object_t instance.
inline Object_t Object_int(long long self) {
	Object_t obj;
	obj.id = int_id;
	obj.self = (void*) self;
	return obj;
}

/// \brief Creates an Object_t instance from a 64 bit double floating point number. 
/// \brief self The desired real number to be held by the Object_t.
/// \return Returns an Object_t instance.
inline Object_t Object_float(double self) {
	long long* temp_ptr = (long long*) &self;
	return {float_id, (void*)*temp_ptr};
}

/// \brief Creates an Object_t instance holding nothing. 
/// \return Returns a 'nullified' Object_t instance.
inline Object_t Object_null() {
	return Object_ctor(0x0, 0);
}

/// \brief Calls an Object_t virtual copy function. 
/// \param self Object_t's instance adress.
/// \return Returns a copied memory block.
inline void* Object_copy(void* self) { 
	Object_t* obj_ptr = (Object_t*) self;
	return TypeRegister_getBaseVTable(obj_ptr->id)->copy(obj_ptr->self);
}

/// \brief Calls an Object_t virtual copy function. 
/// \param self Object_t's instance adress.
/// \param ref The compared Object_t instance adress.
/// \return Returns the result of the comparaison (boolean).
Bool_t Object_equals(void* self, void* ref) {
	Object_t obj1 = *(Object_t*) self;
	Object_t obj2 = *(Object_t*) ref;

	if(obj1.id == obj2.id) {
		if((obj1.id == int_id) || (obj1.id == float_id)) 
			return obj1.self == obj2.self;
		return TypeRegister_getBaseVTable(obj1.id)->equals(obj1.self, obj2.self);
	}
	return FALSE;
}

/// \brief Calls an Object_t virtual destructor function
/// \param self Object_t's instance adress.
/// \return TRUE if deallocation was successful.
inline Bool_t Object_dtor(void** self) {
	if(self) {
		Object_t* obj_ptr = (Object_t*) *self;
		return TypeRegister_getBaseVTable(obj_ptr->id)->dtor(&obj_ptr->self);
	}
	return FALSE;
}

/// \brief Converts an Object_t into a long long integer (64 bits integer).
/// \param obj An Object_t instance.
/// \return Returns a long long (64 bits integer). 
inline long long Object_toInt(Object_t obj) {
	if(obj.id == int_id)
		return (long long) obj.self;
	return 0;
}

/// \brief 
inline double Object_toFloat(Object_t obj) {
	if(obj.id == float_id) 
		return *(double*) &obj.self;
	return 0.0;
}

inline const void* Object_getPointer(Object_t obj) {
	return obj.self;
}

inline unsigned int Object_getId(Object_t obj) {
	return obj.id;
}

#endif