#ifndef blue_engine_point2d
#define blue_engine_point2d

#include "vector2d.h"

#define POINT_X 0
#define POINT_Y 1

typedef long long NativePoint2D_t;

typedef union Point2D_t {
	int axis[2];
	SDL_Point sdl;
	NativePoint2D_t emulated;
} Point2D_t;

Point2D_t Point2D_ctor(int x, int y) {
	Point2D_t p;
	p.axis[0] = x;
	p.axis[1] = y;
	return p;
}

inline long long Point2D_toNative(Point2D_t* self) {
	return self->emulated;
}

inline Point2D_t Point2D_fromNative(NativePoint2D_t native) {
	Point2D_t p;
	p.emulated = native;
	return p;
}

Point2D_t Point2D_fromPoint(Vector2D_t* self) {
	Point2D_t p;
	p.axis[0] = (int) self->axis[0];
	p.axis[1] = (int) self->axis[1];
	return p;
}

Vector2D_t Point2D_toVector(Point2D_t* self) {
	Vector2D_t v;
	v.axis[0] = self->axis[0];
	v.axis[1] = self->axis[1];
	return v;
}

inline Bool_t Point2D_equals(Point2D_t* self, Point2D_t point) {
	return self->emulated == point.emulated;
}

inline int Point2D_get(Point2D_t* self, char which) {
	return self->axis[which];
}

inline void Point2D_set(Point2D_t* self, int value, char which) {
	self->axis[which] = value;
}

#endif