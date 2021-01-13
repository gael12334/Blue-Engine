#ifndef BLUE_ENGINE_DECLRES
#define BLUE_ENGINE_DECLRES

#include "../resources.h"
#include "../utils.h"

// ------------------------

// Surface resource handler
typedef struct Surface_t {
	Resource_t base;
	SDL_Surface* surface;
	SDL_Texture* texture;
} Surface_t;

const unsigned int SurfaceId = registerType("Surface", sizeof(Surface_t));

Surface_t* Surface_ctor(SDL_Surface* ptr);
Surface_t* Surface_ctor(const char* name);

// ---------------------

// Font resource handler
typedef struct Font_t {
	Resource_t base;
	TTF_Font* font;
} Font_t;

const unsigned int FontId = registerType("Font", sizeof(Font_t));

Font_t* Font_ctor(const char* name, int size);

// -------------------------

// Renderer resource handler
typedef struct Renderer_t {
	Resource_t base;
	SDL_Event event;
	SDL_Window* window;
	SDL_Renderer* renderer;
} Renderer_t;

const unsigned int RendererId = registerType("Renderer", sizeof(Renderer_t));

Renderer_t* Renderer_ctor(const char* title, int x, int y, int w, int h, unsigned int flag);

// ------------------------

// Texture resource handler
typedef struct Texture_t {
	Resource_t base;
	SDL_Texture* texture;
} Texture_t;

const unsigned int TextureId = registerType("Renderer", sizeof(Texture_t));

Texture_t* Texture_ctor(SDL_Texture* sdl_ptr);
Texture_t* Texture_ctor(const char* name, Renderer_t* renderer);

#endif