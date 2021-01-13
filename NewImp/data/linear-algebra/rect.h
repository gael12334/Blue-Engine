#ifndef blue_engine_rect
#define blue_engine_rect

#include "point2d.h"

#define INT_RECT 	0	// for later use. (implementing a generic function that can do both)
#define FLOAT_RECT 	1

typedef union Rect_t {
	int attributes[4];
	SDL_Rect sdl;
} Rect_t;

Rect_t Rect_ctor(int x, int y, int w, int h) {
	Rect_t r; 
	r.attributes[0] = x; 
	r.attributes[1] = y; 
	r.attributes[2] = w;
	r.attributes[3] = h;
	return r;
}


typedef union FRect_t {
	float attributes[4];
	SDL_FRect sdl;
} FRect;

FRect_t FRect_ctor(float x, float y, float w, float h) {
	FRect_t r; 
	r.attributes[0] = x; 
	r.attributes[1] = y; 
	r.attributes[2] = w;
	r.attributes[3] = h;
	return r;
}


#endif