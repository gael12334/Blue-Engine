#ifndef BLUE_ENGINE_RENDERER
#define BLUE_ENGINE_RENDERER

#include "decl.h"
#include "font.h"

inline Object_t Renderer_toObject(Renderer_t* renderer) {
	Object_t obj = renderer->base.base;
	obj.id = RendererId;
	return obj;
}

inline Renderer_t* Renderer_fromObject(Object_t obj) {
	if(obj.id == RendererId)
		return (Renderer_t*) obj.reference;
	return 0;
}

inline void Renderer_renderTextureEx(Renderer_t* renderer, Rect_t dest, Texture_t* texture, Rect_t src) {
	SDL_Rect s = Rect_toSDL(src);
	SDL_Rect d = Rect_toSDL(dest);
	SDL_RenderCopy(renderer->renderer, texture->texture, &s, &d);
}

inline void Renderer_renderTexture(Renderer_t* renderer, Rect_t dest, Texture_t* texture) {
	SDL_Rect d = Rect_toSDL(dest);
	SDL_RenderCopy(renderer->renderer, texture->texture, 0, &d);
}

Bool_t Renderer_renderSurfaceEx(Renderer_t* renderer, Rect_t dest, Surface_t* surface, Rect_t src) {
	if(surface->texture) {
		SDL_Rect s = Rect_toSDL(src);
		SDL_Rect d = Rect_toSDL(dest);
		SDL_RenderCopy(renderer->renderer, surface->texture, &s, &d);
		return TRUE;
	}
	return FALSE;
}

Bool_t Renderer_renderSurface(Renderer_t* renderer, Rect_t dest, Surface_t* surface) {
	if(surface->texture) {
		SDL_Rect d = Rect_toSDL(dest);
		SDL_RenderCopy(renderer->renderer, surface->texture, 0, &d);
		return TRUE;
	}
	return FALSE;
}

inline void Renderer_refreshSurface(Renderer_t* renderer, Surface_t* surface) {
	if(surface->texture)
		SDL_DestroyTexture(surface->texture);

	surface->texture = SDL_CreateTextureFromSurface(renderer->renderer, surface->surface);
}

inline unsigned int Renderer_pollEvent(Renderer_t* renderer) {
	return SDL_PollEvent(&renderer->event);
}

inline void Renderer_clear(Renderer_t* renderer) {
	SDL_RenderClear(renderer->renderer);
}

inline void Renderer_present(Renderer_t* renderer) {
	SDL_RenderPresent(renderer->renderer);
}

inline void Renderer_setWindowTitle(Renderer_t* renderer, const char* title) {
	SDL_SetWindowTitle(renderer->window, title);
}

inline void* Renderer_copy(void* self) {
	return self;
}

inline Bool_t Renderer_equals(void* self, void* reference) {
	return (Bool_t) (self == reference);
}

Bool_t Renderer_dtor(void** self) {
	if(self) {
		if(*self) {
			Renderer_t* r = (Renderer_t*) *self;
			SDL_DestroyRenderer(r->renderer);
			SDL_DestroyWindow(r->window);
			free(r);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

inline const char* Renderer_getName() {
	return "Renderer";
}

Renderer_t* Renderer_ctor(const char* title, int x, int y, int w, int h, unsigned int flag) {
	Renderer_t* r = (Renderer_t*) malloc(sizeof(Renderer_t*));
	r->base = Resource_ctor(r, RendererId, Renderer_copy, Renderer_equals, Renderer_dtor, Renderer_getName);
	r->window = SDL_CreateWindow(title, x, y, w, h, flag);
	r->renderer = SDL_CreateRenderer(r->window, 0, SDL_RENDERER_ACCELERATED ); // | SDL_RENDERER_PRESENTVSYNC
	return r;
}

// static renderer

Renderer_t* renderer = Renderer_ctor("Test", 50, 50, 640, 480, 0);

#endif
