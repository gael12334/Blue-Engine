#ifndef BLUE_ENGINE_GOBJ
#define BLUE_ENGINE_GOBJ

#include "resources/renderer.h"

#define DEFAULT_GOBJ_SETPOS GObject_setPosition
#define DEFAULT_GOBJ_SETSIZE GObject_setSize

#define GFUNC_ONCLICK 0
#define GFUNC_ONMOUSEHOVER 1
#define GFUNC_ONKEYPRESSED 2
#define GFUNC_ONMOUSEMOTION 3
#define GFUNC_ONRENDER 4
#define GFUNC_ONNOTIFICATION 5

//
//
typedef struct GEventArgs_t {
	unsigned int arg1, arg2, arg3, arg4, arg5;
} GEventArgs_t;

typedef void(*MoveFn_t)(void*, FPoint_t);
typedef void(*ResizeFn_t)(void*, Point_t);
typedef void(*GFunc_t)(void*, GEventArgs_t); 
typedef void(*DrawFn_t)(void*);

//
//
typedef struct GFunctions_t {
	GFunc_t
		onMouseClick,
		onMouseHover,
		onMouseMotion,
		onKeyPressed;
} GFunctions_t;

inline GFunctions_t GFunctions_ctor() {
	return {0, 0, 0, 0};
}

inline void GFunctions_set(GFunctions_t* func, unsigned char id, GFunc_t function) {
	if(id < 4) {
		GFunc_t* arr = (GFunc_t*) func;
		arr[id] = function;
	}
}

//
//
typedef struct GObject_t {
	Observer_t base;
	GFunctions_t functions;
	unsigned int id;
	FPoint_t position;
	Point_t size;
	MoveFn_t move;
	ResizeFn_t resize;
	DrawFn_t draw;
} GObject_t;

const unsigned int GObjectId = registerType("GObject", sizeof(GObject_t));

GObject_t GObject_ctor(FPoint_t p, Point_t s, void* self, unsigned int id, CopyFn_t copy, CmpFn_t equals, DtorFn_t dtor, NotifFn_t notification, MoveFn_t move, ResizeFn_t resize, DrawFn_t draw) {
	GObject_t gobj = {
		Observer_ctor(self, GObjectId, copy, equals, dtor, notification),
		GFunctions_ctor(),
		id,
		p,
		s, 
		move,
		resize,
		draw
	};
	return gobj;
}

inline void GObject_setFunction(void* gobj, unsigned char id, GFunc_t function) {
	GObject_t* obj = (GObject_t*) gobj;
	GFunctions_set(&obj->functions, id, function);
}

inline FPoint_t GObject_getPosition(void* gobj) {
	GObject_t* graphicObj = (GObject_t*) gobj;
	return graphicObj->position;
}

inline Point_t GObject_getSize(void* gobj) {
	GObject_t* graphicObj = (GObject_t*) gobj;
	return graphicObj->size;
}

inline void GObject_setPosition(void* gobj, FPoint_t fp) {
	GObject_t* graphicObj = (GObject_t*) gobj;
	graphicObj->move(graphicObj->base.base.reference, fp);
}

inline void GObject_setSize(void* gobj, Point_t p) {
	GObject_t* graphicObj = (GObject_t*) gobj;
	graphicObj->resize(graphicObj->base.base.reference, p);
}

inline Object_t GObject_toObject(void* gobj) {
	GObject_t* cobj = (GObject_t*) gobj;
	Object_t obj = cobj->base.base;
	obj.id = GObjectId;
	return obj;
}

inline GObject_t GObject_fromObject(Object_t obj) {
	return *((GObject_t*) obj.reference);
}

Bool_t GObject_isInside(void* gobj, Point_t p) {
	GObject_t obj = *(GObject_t*) gobj;
	float sx = FPoint_getX(obj.position);
	float sy = FPoint_getY(obj.position);
	unsigned int w = Point_getX(obj.size) + sx;
	unsigned int h = Point_getY(obj.size) + sy;
	unsigned int px = Point_getX(p);
	unsigned int py = Point_getY(p);
	return (Bool_t) ((px >= sx) && (py >= sy) && (px <= w) && (py <= h));
}

Bool_t GObject_isInside(void* gobj, int x, int y) {
	GObject_t obj = *(GObject_t*) gobj;
	float sx = FPoint_getX(obj.position);
	float sy = FPoint_getY(obj.position);
	unsigned int w = Point_getX(obj.size) + sx;
	unsigned int h = Point_getY(obj.size) + sy;
	return (Bool_t) ((x >= sx) && (y >= sy) && (x <= w) && (y <= h));
}

void GObject_draw(void* gobj) {
	GObject_t* graphicObj = (GObject_t*) gobj;
	if(graphicObj->draw)
		graphicObj->draw(gobj);
}

void GObject_notification(void* self) {
	if(self) {
		GObject_t* obj = (GObject_t*) self;
		unsigned int type = renderer->event.type;
		switch(type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP: {
				if(obj->functions.onMouseClick)
					obj->functions.onMouseClick(self, {type, renderer->event.button.button, renderer->event.button.x, renderer->event.button.y});
				break;
			}

			case SDL_MOUSEMOTION: {
				if(obj->functions.onMouseHover) {
					if(GObject_isInside(obj, renderer->event.button.x, renderer->event.button.y))
						obj->functions.onMouseHover(self, {type, renderer->event.button.x, renderer->event.button.y});
				}
				if(obj->functions.onMouseMotion)
					obj->functions.onMouseMotion(self, {type, renderer->event.button.x, renderer->event.button.y});
				break;
			}

			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				if(obj->functions.onKeyPressed) 
					obj->functions.onKeyPressed(self, {type, renderer->event.key.type, renderer->event.key.state, renderer->event.key.keysym.mod, renderer->event.key.keysym.sym});
				break;
			}
		}
	}
}

#endif