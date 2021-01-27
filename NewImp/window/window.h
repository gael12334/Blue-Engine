#ifndef blue_engine_window
#define blue_engine_window

#ifndef BE_WINDOW_WIDTH
#define BE_WINDOW_WIDTH 1024
#endif

#ifndef BE_WINDOW_HEIGHT 
#define BE_WINDOW_HEIGHT 768
#endif

#ifndef BE_WINDOW_MAXCOUNT 
#define BE_WINDOW_MAXCOUNT 1
#endif

#include "resources/resource-manager.h"

typedef struct Window_t {
	SDL_Renderer* sdl_renderer;
	SDL_Window* sdl_window;
	SDL_Event* sdl_event;
} Window_t; 

Window_t* mainWindow = 0;

Window_t* Window_ctor(unsigned int pos_x, unsigned int pos_y, const char title[], unsigned int sdl_flags) {
	Window_t* window = (Window_t*) malloc(sizeof(Window_t));
	window->sdl_event = (SDL_Event*) malloc(sizeof(SDL_Event));
	window->sdl_window = SDL_CreateWindow(title, pos_x, pos_y, BE_WINDOW_WIDTH, BE_WINDOW_HEIGHT, sdl_flags);
	window->sdl_renderer = SDL_CreateRenderer(window->sdl_window, 0, SDL_RENDERER_ACCELERATED);
	return window;
}

Bool_t Window_dtor(Window_t** self) {
	if(self) {
		if(*self) {
			SDL_DestroyRenderer((*self)->sdl_renderer);
			SDL_DestroyWindow((*self)->sdl_window);
			free((*self)->sdl_renderer);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

inline void Window_init(const char title[]) {
	mainWindow = Window_ctor(60, 60, title, 0);
}

void Window_refreshSurface(Window_t* self, Surface_t* surface) {
	if(surface->lock) 
		Texture_dtor((void**)&surface->lock);
	surface->lock = Texture_ctor(SDL_CreateTextureFromSurface(self->sdl_renderer, surface->sdl));
}

inline Texture_t* Window_surfaceToTexture(Window_t* self, Surface_t* surface) {
	return Texture_ctor(SDL_CreateTextureFromSurface(self->sdl_renderer, surface->sdl));
}

inline unsigned int Window_pollEvent(Window_t* self) {
	return SDL_PollEvent(self->sdl_event);
}

inline unsigned int Window_hasQuit(Window_t* self) {
	if(self->sdl_event->type == SDL_QUIT)
		return 0;
	return 1;
}

inline void Window_setTitle(Window_t* self, const char title[]) {
	SDL_SetWindowTitle(self->sdl_window, title);
}

inline void Window_present(Window_t* self) {
	SDL_RenderPresent(self->sdl_renderer);
}

inline void Window_refresh(Window_t* self) {
	SDL_RenderClear(self->sdl_renderer);
}

// =============================
// 			Surface
// =============================

inline unsigned int Window_renderSurfaceEx(Window_t* self, Rect_t* dest, Surface_t* surface, Rect_t* src) {
	SDL_Surface* windowSurface = SDL_GetWindowSurface(self->sdl_window);
	return SDL_BlitSurface(surface->sdl, (SDL_Rect*) dest, windowSurface, (SDL_Rect*) src);
}

inline unsigned int Window_renderSizedFullSurface(Window_t* self, Rect_t rect, Surface_t* surface) {
	return Window_renderSurfaceEx(self, &rect, surface, NULL);
}

inline unsigned int Window_renderPartialSurface(Window_t* self, Rect_t windowRect, Surface_t* surface, Rect_t surfaceRect) {
	return Window_renderSurfaceEx(self, &windowRect, surface, &surfaceRect);
}

inline unsigned int Window_renderFullSurface(Window_t* self, Point2D_t onScreen, Surface_t* surface) {
	Rect_t rect = Rect_ctor(onScreen.axis[0], onScreen.axis[1], surface->sdl->w, surface->sdl->h);
	return Window_renderSurfaceEx(self, &rect, surface, NULL);
}

// =============================
// 			Texture
// =============================

inline unsigned int Window_renderTextureEx(Window_t* self, Rect_t* dest, Texture_t* texture, Rect_t* src) {
	return SDL_RenderCopy(self->sdl_renderer, texture->sdl, (SDL_Rect*) src, (SDL_Rect*) dest);
}

inline unsigned int Window_renderSizedFullTexture(Window_t* self, Rect_t rect, Texture_t* texture) {
	return Window_renderTextureEx(self, &rect, texture, NULL);
}

inline unsigned int Window_renderPartialTexture(Window_t* self, Rect_t windowRect, Texture_t* texture, Rect_t surfaceRect) {
	return Window_renderTextureEx(self, &windowRect, texture, &surfaceRect);
}

inline unsigned int Window_renderFullTexture(Window_t* self, Point2D_t onScreen, Texture_t* texture) {
	Point2D_t point = Texture_getSize(texture);
	Rect_t rect = Rect_ctor(onScreen.axis[0], onScreen.axis[1], point.axis[0], point.axis[1]);
	return Window_renderTextureEx(self, &rect, texture, NULL);
}

// =============================
// 	     shapes and lines
// =============================
inline void Window_setRenderColor(Window_t* self, SDL_Color color) {
	SDL_SetRenderDrawColor(self->sdl_renderer, color.a, color.g, color.b, color.a);
}

inline void Window_renderLine(Window_t* self, Point2D_t start, Point2D_t end) {
	SDL_RenderDrawLine(self->sdl_renderer, start.axis[0], start.axis[1], end.axis[0], end.axis[1]);
}

// =============================
// 		Event
// =============================
inline unsigned int Window_getEventType(Window_t* window) {
	return window->sdl_event->type;
}

inline Point2D_t Window_getCursorPos(Window_t* window) {
	return Point2D_ctor(window->sdl_event->button.x, window->sdl_event->button.y);
}

#endif