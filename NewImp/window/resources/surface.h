#ifndef blue_engine_surface 
#define blue_engine_surface

#include "texture.h"

Surface_t* Surface_ctor(const char filename[]);

void* Surface_copy(void* self) {
	Surface_t* self_surface = (Surface_t*) self;
	Surface_t* copy_surface = (Surface_t*) malloc(sizeof(Surface_t));
	return copy_surface;
}

void* Surface_pixelCopy(void* self) {
	Surface_t* self_surface = (Surface_t*) self;
	Surface_t* copy_surface = (Surface_t*) malloc(sizeof(Surface_t));

	copy_surface->sdl = SDL_CreateRGBSurfaceFrom(
		self_surface->sdl->pixels,
		self_surface->sdl->w, 
		self_surface->sdl->h, 
		self_surface->sdl->format->BitsPerPixel,
		self_surface->sdl->pitch,
		self_surface->sdl->format->Rmask,
		self_surface->sdl->format->Gmask,
		self_surface->sdl->format->Bmask,
		self_surface->sdl->format->Amask);
	
	copy_surface->base = BaseResource_ctor(copy_surface, self_surface->base.base.id);
	copy_surface->sdl_lock = 0;

	return copy_surface;
}

Bool_t Surface_equals(void* self, void* ref) {
	Surface_t* self_surface = (Surface_t*) self;
	Surface_t* ref_surface = (Surface_t*) ref;

	return self_surface->sdl == ref_surface->sdl;
}

Bool_t Surface_pixelEquals(void* self, void* ref) {
	Surface_t* self_surface = (Surface_t*) self;
	Surface_t* ref_surface = (Surface_t*) ref;

	char* self_pixels = (char*) self_surface->sdl->pixels;
	char* ref_pixels = (char*) ref_surface->sdl->pixels;

	unsigned int sm = self_surface->sdl->w * self_surface->sdl->h;
	unsigned int rm = ref_surface->sdl->w * ref_surface->sdl->h;

	if(sm == rm) {
		for(unsigned int i = 0; i < sm; i++) {
			if(self_pixels[i] != ref_pixels[i])
				return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

Bool_t Surface_dtor(void** self) {
	if(self) {
		if(*self) {
			Surface_t* s = (Surface_t*) *self;
			SDL_FreeSurface(s->sdl);
			SDL_DestroyTexture(s->sdl_lock);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

const unsigned int Surface_id = TypeRegister_register(Surface_copy, Surface_equals, Surface_dtor, sizeof(Surface_t), BaseResource_id, "Surface_t", 0);

Surface_t* Surface_ctor(const char filename[]) {
	Surface_t* s = (Surface_t*) malloc(sizeof(Surface_t));
	s->sdl = IMG_Load(filename);
	s->sdl_lock = 0;
	s->base = BaseResource_ctor(s, Surface_id);
	return s;
}

inline Point2D_t Surface_getSize(Surface_t* self) {
	return Point2D_ctor(self->sdl->w, self->sdl->h);
}

inline int Surface_blit(Rect_t dest, Surface_t* dest_surf, Rect_t src, Surface_t* src_surf) {
	return SDL_BlitSurface(src_surf->sdl, &src.sdl, dest_surf->sdl, &dest.sdl);
}

#endif 