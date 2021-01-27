#ifndef blue_engine_observer_vtable
#define blue_engine_observer_vtable

#include "../window/window.h"

typedef void(*NotificationFn_t)(void*);

typedef struct ObserverVTable_t {
	BaseVTable_t base_vtable;
	NotificationFn_t notification;
} ObserverVTable_t;

ObserverVTable_t ObserverVTable_ctor(CopyFn_t copy, EqualsFn_t equals, DtorFn_t dtor, NotificationFn_t notification) {
	ObserverVTable_t vtable;
	vtable.base_vtable = BaseVTable_ctor(copy, equals, dtor);
	vtable.notification = notification;
	return vtable;
}

static void* ObserverVTable_createPointer(CopyFn_t copy, EqualsFn_t equals, DtorFn_t dtor, NotificationFn_t notification) {
	ObserverVTable_t* vtable = (ObserverVTable_t*) malloc(sizeof(ObserverVTable_t));
	*vtable = ObserverVTable_ctor(copy, equals, dtor, notification);
	return vtable;
}

#endif