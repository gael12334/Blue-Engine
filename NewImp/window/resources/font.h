#ifndef blue_engine_font
#define blue_engine_font

#include "surface.h"

void* Font_copy(void* self) {
	Font_t* self_font = (Font_t*) self;
	Font_t* copy_font = (Font_t*) malloc(sizeof(Font_t));
	copy_font->base = BaseResource_ctor(copy_font, self_font->base.base.id);
	copy_font->sdl = self_font->sdl;
	return copy_font;
}

Bool_t Font_equals(void* self, void* ref) {
	Font_t* self_font = (Font_t*) self;
	Font_t* ref_font = (Font_t*) ref;

	return self_font->sdl == ref_font->sdl;
}

Bool_t Font_dtor(void** self) {
	if(self) {
		if(*self) {
			Font_t* font = (Font_t*) *self;
			TTF_CloseFont(font->sdl);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

const unsigned int Font_id = TypeRegister_register(Font_copy, Font_equals, Font_dtor, sizeof(Font_t), BaseResource_id, "Font_t", 0);

Font_t* Font_ctor(const char filename[], int size) {
	Font_t* handle = (Font_t*) malloc(sizeof(Font_t));
	TTF_Font* font = TTF_OpenFont(filename, size);
	handle->sdl = font;
	handle->base = BaseResource_ctor(handle, Font_id);
	return handle;
}

Surface_t* Font_blend(const char text[], Font_t* self, SDL_Color color) {
	Surface_t* s = (Surface_t*) malloc(sizeof(Surface_t));
	s->sdl = TTF_RenderText_Blended(self->sdl, text, color);
	s->sdl_lock = 0;
	s->base = BaseResource_ctor(s, Surface_id);
	return s;
}

#endif