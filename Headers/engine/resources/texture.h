#ifndef BLUE_ENGINE_TEXTURE
#define BLUE_ENGINE_TEXTURE

#include "decl.h"
#include "surface.h"

inline Object_t Texture_toObject(Texture_t* texture) {
	Object_t obj = texture->base.base;
	obj.id = TextureId;
	return obj;
}

inline Texture_t* Texture_fromObject(Object_t obj) {
	if(obj.id == TextureId)
		return (Texture_t*) obj.reference;
	return 0;
}

Point_t Texture_getSize(Texture_t* texture) {
	int pcomp[2]; 
	SDL_QueryTexture(texture->texture, NULL, NULL, &pcomp[0], &pcomp[1]);
	return Point_ctor(pcomp[0], pcomp[1]);
}

inline void* Texture_copy(void* self) {
	return self;
}

Bool_t Texture_equals(void* self, void* reference) {
	if(self && reference) {
		Surface_t* s1 = (Surface_t*) self;
		Surface_t* s2 = (Surface_t*) reference;

		if((s1->surface->w == s2->surface->w) && (s1->surface->h == s2->surface->h)) {
			unsigned int* s1_pixels = (unsigned int*) s1->surface->pixels;
			unsigned int* s2_pixels = (unsigned int*) s2->surface->pixels;

			for(unsigned int i = 0, m = s1->surface->w * s1->surface->h; i < m; i++) {
				if(s1_pixels[i] != s2_pixels[i])
					return FALSE;
			}

			return TRUE;
		}
	}
	return FALSE;
}

inline const char* Texture_getName() {
	return "Texture";
}

Bool_t Texture_dtor(void** self) {
	if(self) {
		if(*self) {
			Texture_t* texture = (Texture_t*)*self;
			SDL_DestroyTexture(texture->texture);
			free(texture);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

Texture_t* Texture_ctor(SDL_Texture* sdl_ptr) {
	Texture_t* t = (Texture_t*) malloc(sizeof(Texture_t));
	t->base = Resource_ctor(t, TextureId, Texture_copy, Texture_equals, Texture_dtor, Texture_getName);
	t->texture = sdl_ptr;
	return t;
}

Texture_t* Texture_ctor(const char* fname, Renderer_t* renderer) {
	Surface_t* s = Surface_ctor(fname);
	Texture_t* t = Surface_toTexture(s, renderer);
	Surface_dtor((void**)&s);
	return t;
}



#endif