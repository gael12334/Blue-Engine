#ifndef blue_engine_pair
#define blue_engine_pair
#include "../linkedlist/linkedlist.h"

typedef struct Pair_t {
	Object_t base;
	Object_t key;
	Object_t entry;
} Pair_t;

// foward declaration
Pair_t* Pair_ctor(Object_t key, Object_t entry);

// vtable
void* Pair_copy(void* self) {
	Pair_t* pair = (Pair_t*) self;
	Pair_t* copy = Pair_ctor(pair->key, pair->entry);
	return copy;
}

Bool_t Pair_equals(void* self, void* inst) {
	Pair_t* pair = (Pair_t*) self;
	Pair_t* inst_pair = (Pair_t*) inst;
	
}

Bool_t Pair_dtor(void** self) {
	if(self) {
		if(*self) {
			free(*self);
			*self = 0;
			return TRUE;
		}
	}

	return FALSE;
}

// id
//const unsigned int Pair_id = TypeRegister_register(Pair_copy, Pair_equals, Pair_dtor, sizeof(Pair_t), 0, "Pair_t", 0, 0);

// function
Pair_t* Pair_ctor(Object_t key, Object_t entry) {
	Pair_t* pair = (Pair_t*) malloc(sizeof(Pair_t));
	*pair = {Object_ctor(pair, 0), key, entry};
}

#endif