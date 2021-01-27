#ifndef blue_engine_scene_vtable
#define blue_engine_scene_vtable

#include "../event/observer.h"

typedef struct SceneState_t {
	int state;
	StringInt_t arg;
} SceneState_t;

SceneState_t SceneState_ctor(int state, StringInt_t arg) {
	SceneState_t s;
	s.state = state;
	s.arg = arg;
	return s;
}

// =======================================================

typedef void(*DrawFn_t)(void*); 
typedef SceneState_t(*SceneGetStateFn_t)(void*);
typedef void(*SceneAddFn_t)(void*, StringInt_t, Object_t);
typedef Object_t(*SceneGetFn_t)(void*, StringInt_t);
typedef Bool_t(*SceneRemoveFn_t)(void*, StringInt_t);
typedef LinearHTIterator_t(*SceneIterateFn_t)(void*);
typedef void(*SceneNotifyFn_t)(void*); 

typedef struct SceneVTable_t {
	BaseVTable_t base;
	DrawFn_t draw; 
	SceneGetStateFn_t getState;
	SceneAddFn_t add;
	SceneGetFn_t get;
	SceneRemoveFn_t remove;
	SceneIterateFn_t begin;
	SceneNotifyFn_t notify;
} SceneVTable_t;

SceneVTable_t SceneVTable_ctor(BaseVTable_t base, DrawFn_t draw, SceneGetStateFn_t getState, SceneAddFn_t add, SceneGetFn_t get,
                               SceneRemoveFn_t remove, SceneIterateFn_t begin, SceneNotifyFn_t notify) 
{
	SceneVTable_t vtable;
	vtable.base = base;
	vtable.draw = draw;
	vtable.getState = getState;
	vtable.add = add;
	vtable.get = get;
	vtable.remove = remove;
	vtable.begin = begin;
	vtable.notify = notify;
	return vtable;
}

SceneVTable_t* SceneVTable_t_createPointer(BaseVTable_t base, DrawFn_t draw, SceneGetStateFn_t getState, SceneAddFn_t add, SceneGetFn_t get, 
                                           SceneRemoveFn_t remove, SceneIterateFn_t begin, SceneNotifyFn_t notify) 
{
	SceneVTable_t* vtable = (SceneVTable_t*) malloc(sizeof(SceneVTable_t));
	*vtable = SceneVTable_ctor(base, draw, getState, add, get, remove, begin, notify);
	return vtable;
}

#endif