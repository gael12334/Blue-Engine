#ifndef BLUE_ENGINE_FONT
#define BLUE_ENGINE_FONT

#include "decl.h"
#include "texture.h"

inline Object_t Font_toObject(Font_t* font) {
	Object_t obj = font->base.base;
	obj.id = FontId;
	return obj;
}

inline Font_t* Font_fromObject(Object_t obj) {
	if(obj.id == FontId)
		return (Font_t*) obj.reference;
	return 0;
}

inline Surface_t* Font_toSurface(Font_t* font, const char* text, Color_t foreground) {
	return Surface_ctor(TTF_RenderText_Blended(font->font, text, Color_toSDL(foreground)));
}

inline Texture_t* Font_toTexture(Font_t* font, const char* text, Color_t foreground, Renderer_t* renderer) {
	Surface_t* stemp = Font_toSurface(font, text, foreground);
	Texture_t* t = Surface_toTexture(stemp,  renderer);
	Surface_dtor((void**)&stemp);
	return t;
}

inline void* Font_copy(void* self) {
	return self;
}

inline Bool_t Font_equals(void* self, void* reference) {
	return (Bool_t) (self == reference);
}

Bool_t Font_dtor(void** self) {
	if(self) {
		if(*self) {
			Font_t* font = (Font_t*) *self;
			TTF_CloseFont(font->font);
			free(font);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

inline const char* Font_getName() {
	return "Font";
}

Font_t* Font_ctor(const char* name, int size) {
	Font_t* f = (Font_t*) malloc(sizeof(Font_t));
	f->base = Resource_ctor(f, FontId, Font_copy, Font_equals, Font_dtor, Font_getName),
	f->font = TTF_OpenFont(name, size);
	return f;
}

#endif