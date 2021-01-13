#ifndef blue_engine_resources
#define blue_engine_resources

#include "../../data/linear-algebra/matrix2d.h"

typedef struct BaseResource_t {
	Object_t base;
} BaseResource_t;

// = = = = = 
void* BaseResource_copy(void* self) {
	Object_t* base = (Object_t*) self;
	const BaseVTable_t* vtable = TypeRegister_getVTable(base->id);
	return vtable->copy(self);
}

Bool_t BaseResource_equals(void* self, void* ref) {
	Object_t* base = (Object_t*) self;
	const BaseVTable_t* vtable = TypeRegister_getVTable(base->id);
	return vtable->equals(self, ref);
}

Bool_t BaseResource_dtor(void** self) {
	if(self) {
		if(*self) {
			Object_t* base = (Object_t*) self;
			const BaseVTable_t* vtable = TypeRegister_getVTable(base->id);
			return vtable->dtor(self);
		}
	}

	return FALSE;
}
// = = = = = 

const unsigned int BaseResource_id = TypeRegister_register(BaseResource_copy, BaseResource_equals, BaseResource_dtor, sizeof(BaseResource_t), 0, "BaseResource_t", 0);

inline BaseResource_t BaseResource_ctor(void* self, unsigned int id) {
	BaseResource_t resource;
	resource.base = Object_ctor(self, id);
	return resource;
}

inline void* BaseResource_getResource(void* self) {
	BaseResource_t* br = (BaseResource_t*) self;
	return br->base.self;
}

typedef struct Texture_t {
	BaseResource_t	base; 
	SDL_Texture* 	sdl;
} Texture_t;

typedef struct Surface_t {
	BaseResource_t 	base;
	SDL_Surface* 	sdl;
	SDL_Texture* 	sdl_lock; 
} Surface_t;

typedef struct Font_t {
	BaseResource_t 	base;
	TTF_Font* 		sdl;
} Font_t;

typedef struct File_t {
	BaseResource_t	base;
	FILE* 			file;
	unsigned int 	position; 
} File_t; 

#endif