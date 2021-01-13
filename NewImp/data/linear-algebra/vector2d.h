#ifndef blue_engine_vector2d
#define blue_engine_vector2d

#include "../string/stringint.h"
#include <math.h>

#define VECTOR_X 0
#define VECTOR_Y 1

typedef long long NativeVector2D_t;

typedef union Vector2D_t {
	float axis[2];
	SDL_FPoint sdl;
	NativeVector2D_t emulated;
} Vector2D_t;

Vector2D_t Vector2D_ctor(float x, float y) {
	Vector2D_t v;
	v.axis[0] = x;
	v.axis[1] = y;
	return v;
}

inline long long Vector2D_toNative(Vector2D_t* self) {
	return self->emulated;
}

inline Vector2D_t Vector2D_fromNative(NativeVector2D_t native) {
	Vector2D_t v;
	v.emulated = native;
	return v;
}

inline Bool_t Vector2D_equals(Vector2D_t* self, Vector2D_t ref) {
	return self->emulated == ref.emulated;
}

inline float Vector2D_get(Vector2D_t* self, char which) {
	return self->axis[which];
}

inline void Vector2D_set(Vector2D_t* self, float f, char which) {
	self->axis[which] = f;
}

inline Vector2D_t Vector2D_add(Vector2D_t* self, Vector2D_t ref) {
	Vector2D_t v; 
	v.axis[0] = ref.axis[0] + self->axis[0];
	v.axis[1] = ref.axis[1] + self->axis[1];
	return v;
}

inline Vector2D_t Vector2D_sub(Vector2D_t* self, Vector2D_t ref) {
	Vector2D_t v; 
	v.axis[0] = self->axis[0] - ref.axis[0];
	v.axis[1] = self->axis[1] - ref.axis[1];
	return v;
}

inline Vector2D_t Vector2D_scalar(Vector2D_t* self, float s) {
	Vector2D_t v;
	v.axis[0] = self->axis[0] * s;
	v.axis[1] = self->axis[1] * s;
	return v;
}

inline float Vector2D_norm(Vector2D_t* self) {
	return sqrt(self->axis[0] * self->axis[0] + self->axis[1] * self->axis[0]);
}

Object_t Vector2D_toObject(Vector2D_t* self) {
	return Object_int(self->emulated);
}

Vector2D_t Vector2D_fromObject(Object_t obj) {
	Vector2D_t v;
	v.emulated = (long long) obj.self;
	return v;
}

#endif