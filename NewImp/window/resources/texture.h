#ifndef blue_engine_texture
#define blue_engine_texture
#include "resources.h"

Texture_t* Texture_ctor(SDL_Texture* sdl);

void* Texture_copy(void* self) {
	Texture_t* self_texture = (Texture_t*) self;
	Texture_t* copy_texture = Texture_ctor(self_texture->sdl);
	return copy_texture;
}

Bool_t Texture_equals(void* self, void* ref) {
	Texture_t* self_texture = (Texture_t*) self;
	Texture_t* ref_texture = (Texture_t*) ref;
	return self_texture->sdl == ref_texture->sdl;
}

Bool_t Texture_dtor(void** self) {
	if(self) {
		if(*self) {
			Texture_t* texture = (Texture_t*) *self;
			SDL_DestroyTexture(texture->sdl);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

const unsigned int Texture_id = TypeRegister_register(Texture_copy, Texture_equals, Texture_dtor, sizeof(Texture_t), BaseResource_id, "Texture_t", 0);

Texture_t* Texture_ctor(SDL_Texture* sdl) {
	Texture_t* handle = (Texture_t*) malloc(sizeof(Texture_t));
	handle->base = BaseResource_ctor(handle, Texture_id);
	handle->sdl = sdl;
	return handle;
}

inline Point2D_t Texture_getSize(Texture_t* texture) {
	Point2D_t p; 
	SDL_QueryTexture(texture->sdl, NULL, NULL, &p.axis[0], &p.axis[1]);
	return p;
}

#endif