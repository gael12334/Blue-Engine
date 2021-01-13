#ifndef BLUE_ENGINE_UTILS
#define BLUE_ENGINE_UTILS

#include "observer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// XY Point 
typedef long long Point_t; 

const unsigned int PointId = intId;

Point_t Point_ctor(int x, int y) {
	Point_t p = 0;
	int* pptr = (int*) &p;
	pptr[0] = x;
	pptr[1] = y;
	return p;
}

Point_t Point_ctor(SDL_Point sdlp) {
	Point_t p = Point_ctor(sdlp.x, sdlp.y);
	return p;
}

int Point_getX(Point_t p) {
	int* pptr = (int*) &p;
	return pptr[0];
}

int Point_getY(Point_t p) {
	int* pptr = (int*) &p;
	return pptr[1];
}

void Point_setX(Point_t* p, int x) {
	int* pptr = (int*) p;
	pptr[0] = x;
}

void Point_setY(Point_t* p, int y) {
	int* pptr = (int*) p;
	pptr[1] = y;
}

void Point_set(Point_t* p, int x, int y) {
	int* pptr = (int*) p;
	pptr[0] = x;
	pptr[1] = y;
}

SDL_Point Point_toSDL(Point_t p) {
	SDL_Point* sdlp = (SDL_Point*)&p;
	return *sdlp;
}

inline Object_t Point_toObject(Point_t p) {
	return Object_ctorInt(p);
}

inline Point_t Point_fromObject(Object_t o) {
	return Object_toInt(&o);
}

// XY FPoint
typedef double FPoint_t;

const unsigned int FPointId = floatId;

FPoint_t FPoint_ctor(float x, float y) {
	FPoint_t fp = 0;
	float* fpptr = (float*)&fp;
	fpptr[0] = x;
	fpptr[1] = y;
	return fp;
}

FPoint_t FPoint_ctor(Point_t p) {
	FPoint_t fp;
	float* fpptr = (float*)&fp;
	fpptr[0] = (float) Point_getX(p);
	fpptr[1] = (float) Point_getY(p);
	return fp;
}

float FPoint_getX(FPoint_t p) {
	float* pptr = (float*) &p;
	return pptr[0];
}

float FPoint_getY(FPoint_t p) {
	float* pptr = (float*) &p;
	return pptr[1];
}

void FPoint_setX(FPoint_t* p, float x) {
	float* pptr = (float*) p;
	pptr[0] = x;
}

void FPoint_setY(FPoint_t* p, float y) {
	float* pptr = (float*) p;
	pptr[1] = y;
}

void FPoint_set(FPoint_t* p, float x, float y) {
	float* pptr = (float*) p;
	pptr[0] = x;
	pptr[1] = y;
}

Point_t FPoint_toPoint(FPoint_t p) {
	Point_t pp = Point_ctor((int)FPoint_getX(p), (int)FPoint_getY(p));
	return pp;
}

inline Object_t FPoint_toObject(FPoint_t p) {
	return Object_ctorFloat(p);
}

inline FPoint_t FPoint_fromObject(Object_t o) {
	return Object_toFloat(&o);
}

// rect 
typedef struct Rect_t {
	int x, y;
	unsigned int w, h;
} Rect_t;

const unsigned int RectId = registerType("Rect", sizeof(Rect_t));

inline Rect_t Rect_ctor(int x, int y, unsigned int w, unsigned int h) {
	Rect_t rect = {x, y, w, h};
	return rect;
}

inline Rect_t Rect_ctor(Point_t p, Point_t s) {
	Rect_t rect = {Point_getX(p), Point_getY(p), Point_getX(s), Point_getY(s)};
	return rect;
}

inline Rect_t Rect_ctor(SDL_Rect r) {
	Rect_t rect = {r.x, r.y, r.w, r.h};
	return rect;
}

inline Point_t Rect_getPosition(Rect_t rect) {
	return Point_ctor(rect.x, rect.y);
}

inline Point_t Rect_getSize(Rect_t rect) {
	return Point_ctor(rect.w, rect.h);
}

inline void Rect_setPosition(Rect_t* rect, short x, short y) {
	rect->x = x;
	rect->y = y;
}

inline void Rect_setPosition(Rect_t* rect, Point_t p) {
	rect->x = Point_getX(p);
	rect->y = Point_getY(p);
}

inline void Rect_setSize(Rect_t* rect, unsigned short w, unsigned short h) {
	rect->w = w;
	rect->h = h;
}

inline void Rect_setSize(Rect_t* rect, Point_t s) {
	short* rcomp = (short*) rect;
	rcomp[2] = (short) Point_getX(s);
	rcomp[3] = (short) Point_getY(s);
}

inline SDL_Rect Rect_toSDL(Rect_t rect) {
	SDL_Rect r = {rect.x, rect.y, rect.w, rect.h};
	return r;
}

inline Bool_t Rect_IsInside(Rect_t rect, Point_t p) {
	int w = rect.x + rect.w,
		h = rect.y + rect.h;
	int* pcomp = (int*) &p;
	return (Bool_t) ((pcomp[0] >= rect.x) && (pcomp[1] >= rect.y) && (pcomp[0] <= w) && (pcomp[1] <= h));
}

// 
typedef struct FRect_t {
	FPoint_t p;
	FPoint_t s;
} FRect_t;


//
typedef unsigned int Color_t;

const unsigned int ColorId = intId;

inline Color_t Color_ctor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	unsigned char rgba[4] = {r, g, b, a};
	return *(Color_t*)&rgba[0];
}

inline Color_t Color_ctor(SDL_Color c) {
	return Color_ctor(c.r, c.g, c.b, c.a);
}

inline unsigned char Color_getR(Color_t c) {
	unsigned char* rgba = (unsigned char*) &c;
	return rgba[0];
}

inline unsigned char Color_getG(Color_t c) {
	unsigned char* rgba = (unsigned char*) &c;
	return rgba[1];
}

inline unsigned char Color_getB(Color_t c) {
	unsigned char* rgba = (unsigned char*) &c;
	return rgba[2];
}

inline unsigned char Color_getA(Color_t c) {
	unsigned char* rgba = (unsigned char*) &c;
	return rgba[3];
}

inline void Color_setR(Color_t c, unsigned char r) {
	unsigned char* rgba = (unsigned char*) &c;
	rgba[0] = r;
}

inline void Color_setG(Color_t c, unsigned char g) {
	unsigned char* rgba = (unsigned char*) &c;
	rgba[1] = g;
}

inline void Color_setB(Color_t c, unsigned char b) {
	unsigned char* rgba = (unsigned char*) &c;
	rgba[2] = b;
}

inline void Color_setA(Color_t c, unsigned char a) {
	unsigned char* rgba = (unsigned char*) &c;
	rgba[3] = a;
}

inline SDL_Color Color_toSDL(Color_t c) {
	return *(SDL_Color*) &c;
}

inline Object_t Color_toObject(Color_t c) {
	return Object_ctorInt(c);
}

inline Color_t Color_fromObject(Object_t obj) {
	if(obj.id == intId) 
		return (Color_t) (unsigned long long) obj.reference;
	return 0; 
}

#endif