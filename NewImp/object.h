#ifndef blue_engine_object
#define blue_engine_object
#include "types.h"

typedef struct Object_t {
	unsigned int id;
	void* self;
} Object_t;

inline Object_t Object_ctor(void* self, unsigned int id) {
	Object_t obj;
	obj.id = id;
	obj.self = self;
	return obj;
}

inline Object_t Object_int(long long self) {
	Object_t obj;
	obj.id = int_id;
	obj.self = (void*) self;
	return obj;
}

inline Object_t Object_float(double self) {
	long long* temp_ptr = (long long*) &self;
	return {float_id, (void*)*temp_ptr};
}

inline Object_t Object_null() {
	return Object_ctor(0x0, 0);
}

inline void* Object_copy(void* self, char arg) { 
	Object_t* obj_ptr = (Object_t*) self;
	return TypeRegister_getVTable(obj_ptr->id)->copy(obj_ptr->self);
}

inline Bool_t Object_equals(Object_t obj1, Object_t obj2) {
	if(obj1.id == obj2.id) {
		if((obj1.id == int_id) || (obj1.id == float_id)) 
			return obj1.self == obj2.self;
		return TypeRegister_getVTable(obj1.id)->equals(obj1.self, obj2.self);
	}
	return FALSE;
}

inline Bool_t Object_dtor(void* self) {
	Object_t* obj_ptr = (Object_t*) self;
	return TypeRegister_getVTable(obj_ptr->id)->dtor(&obj_ptr->self);
}

inline long long Object_toInt(Object_t obj) {
	if(obj.id == int_id)
		return (long long) obj.self;
	return 0;
}

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