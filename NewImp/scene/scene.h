#ifndef blue_engine_scene
#define blue_engine_scene

#include "scene-vtable.h"

#define BS_SELF bs->base.self
#define BS_ID bs->base.id

typedef struct BaseScene_t {
	Object_t 		base; 
	SceneState_t 	state;
	HashTable_t* 	elements;
	Texture_t* 		background;
	SceneVTable_t* 	vtable;
} BaseScene_t; 

inline void* BaseScene_copy(void* self) {
	BaseScene_t* bs = (BaseScene_t*) self;
	return TypeRegister_getBaseVTable(BS_ID)->copy(BS_SELF);
}

inline Bool_t BaseScene_equals(void* self, void* ref) {
	BaseScene_t* bs = (BaseScene_t*) self;
	BaseScene_t* ref_bs = (BaseScene_t*) ref;
	return TypeRegister_getBaseVTable(BS_ID)->equals(BS_SELF, ref_bs->base.self);
}

inline Bool_t BaseScene_dtor(void** self) {
	if(self) {
		BaseScene_t* bs = (BaseScene_t*) *self;
		return TypeRegister_getBaseVTable(BS_ID)->dtor(&BS_SELF);
	}
	return FALSE;
}

inline void BaseScene_draw(void* self) {
	BaseScene_t* bs = (BaseScene_t*) self;
	SceneVTable_t* vtable = (SceneVTable_t*) TypeRegister_getVTable(BS_ID);
	vtable->draw(BS_SELF);
}

inline SceneState_t BaseScene_getState(void* self) {
	BaseScene_t* bs = (BaseScene_t*) self;
	return bs->state;
}

inline void BaseScene_add(void* self, StringInt_t name, Object_t component) {
	BaseScene_t* bs = (BaseScene_t*) self;
	HashTable_set(bs->elements, StringInt_converToObject(name), component);
}

inline Object_t BaseScene_get(void* self, StringInt_t name) {
	BaseScene_t* bs = (BaseScene_t*) self;
	return HashTable_get(bs->elements, StringInt_converToObject(name), 0);
}

inline Bool_t BaseScene_remove(void* self, StringInt_t name) {
	BaseScene_t* bs = (BaseScene_t*) self;
	return HashTable_remove(bs->elements, StringInt_converToObject(name));
}

inline LinearHTIterator_t BaseScene_begin(void* self) {
	BaseScene_t* bs = (BaseScene_t*) self;
	return HashTable_linearBegin(bs->elements);
}

inline void BaseScene_notify(void* self) {

}

#pragma region id
const unsigned int BaseScene_id = 
	TypeRegister_register(
		SceneVTable_t_createPointer(
			BaseVTable_ctor(
				Object_copy,
				Object_equals,
				Object_dtor
			),
			BaseScene_draw,
			BaseScene_getState,
			BaseScene_add,
			BaseScene_get,
			BaseScene_remove,
			BaseScene_begin,
			BaseScene_notify
		),
		sizeof(SceneVTable_t),
		sizeof(BaseScene_t),
		0,
		"BaseScene_t"
	)
;
#pragma endregion

BaseScene_t BaseScene_ctor(StringInt_t textureName) {

}


#endif