#ifndef BLUE_ENGINE_SURFACE
#define BLUE_ENGINE_SURFACE

#include "decl.h"

inline Object_t Surface_toObject(Surface_t* surface) {
	Object_t obj = surface->base.base;
	obj.id = SurfaceId;
	return obj;
}

inline Surface_t* Surface_fromObject(Object_t obj) {
	if(obj.id == SurfaceId)
		return (Surface_t*) obj.reference;
	return 0;
}

inline Bool_t Surface_blit(Surface_t* dest, Rect_t drect, Surface_t* src, Rect_t srect) {
	if(dest && src) {
		SDL_Rect s = Rect_toSDL(srect);
		SDL_Rect d = Rect_toSDL(drect);
		return (Bool_t) (SDL_BlitSurface(src->surface, &s, dest->surface, &d) + 1);
	}
	return FALSE;
}

inline Texture_t* Surface_toTexture(Surface_t* surface, Renderer_t* renderer) {
	if(surface && renderer) 
		return Texture_ctor(SDL_CreateTextureFromSurface(renderer->renderer, surface->surface));
	return 0;
}

Point_t Surface_getSize(Surface_t* surface) {
	if(surface) 
		return Point_ctor(surface->surface->w, surface->surface->h);
	return 0;
}

void* Surface_copy(void* self) {
	Surface_t* surface = (Surface_t*) self;
	Surface_t* copy = Surface_ctor(SDL_DuplicateSurface(surface->surface));
	return copy;
}

Bool_t Surface_equals(void* self, void* reference) {
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

inline const char* Surface_getName() {
	return "Surface";
}

Bool_t Surface_dtor(void** self) {
	if(self) {
		if(*self) {
			Surface_t* surface = (Surface_t*) *self;
			if(surface->surface)
				SDL_FreeSurface(surface->surface);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}

	return FALSE;
}

Surface_t* Surface_ctor(SDL_Surface* ptr) {
	Surface_t* surface = (Surface_t*) malloc(sizeof(Surface_t));
	surface->base = Resource_ctor(surface, SurfaceId, Surface_copy, Surface_equals, Surface_dtor, Surface_getName);
	surface->surface = ptr;
	surface->texture = 0;
	return surface;
}

inline Surface_t* Surface_ctor(const char* fname) {
	return Surface_ctor(IMG_Load(fname));
}



#endif