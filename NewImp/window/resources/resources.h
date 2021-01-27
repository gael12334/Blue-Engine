#ifndef blue_engine_resources
#define blue_engine_resources

#include "../../data/linear-algebra/matrix2d.h"

typedef struct BaseResource_t {
	Object_t base;
} BaseResource_t;

// = = = = = 
inline void* BaseResource_copy(void* self) {
	return Object_copy(self);
}

inline Bool_t BaseResource_equals(void* self, void* ref) {
	return Object_equals(self, ref);
}

inline Bool_t BaseResource_dtor(void** self) {
	return Object_dtor(self);
}
// = = = = = 

const unsigned int BaseResource_id = TypeRegister_register(BaseVTable_createPointer(BaseResource_copy, BaseResource_equals, BaseResource_dtor), sizeof(BaseVTable_t), sizeof(BaseResource_t), 0, "BaseResource_t");

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
	Texture_t* 		lock; 
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