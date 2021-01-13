#ifndef BLUE_ENGINE_SCENE
#define BLUE_ENGINE_SCENE

#include "gobj.h"

#define SCENE_STATE_RENDER 0
#define SCENE_STATE_PUSH 1
#define SCENE_STATE_SWAP 2
#define SCENE_STATE_POP 3

typedef void(*DrawFn_t)(void*);

// Pure virtual structure "Scene_t".
//
typedef struct Scene_t {
	Object_t base;
	unsigned int id;
	unsigned char state;
	String8_t name;
	String8_t args;

	HashTable_t* eventObservable;
	HashTable_t* objectDictionnary;

	DrawFn_t draw;
} Scene_t;

const unsigned int SceneId = registerType("Scene", sizeof(Scene_t));

Scene_t Scene_ctor(Scene_t* s, String8_t name, void* self, unsigned int id, CopyFn_t copy, CmpFn_t equals, DtorFn_t dtor, DrawFn_t draw) {
	s->base = Object_ctor(self, SceneId, copy, equals, dtor); 
	s->id = id;
	s->state = SCENE_STATE_RENDER;
	s->name = name;
	s->args = 0;
	s->eventObservable = HashTable_ctor(intId, ObserverId);
	s->objectDictionnary = HashTable_ctor(String8Id, GObjectId),
	s->draw = draw;
	return *s;
}

inline String8_t Scene_getName(Scene_t* scene) {
	return scene->name;
}

inline void Scene_setState(Scene_t* scene, unsigned char state) {
	scene->state = state;
}

inline unsigned char Scene_getState(Scene_t* scene) {
	return scene->state;
}

void Scene_addGObject(Scene_t* scene, GObject_t* obj, String8_t name, unsigned char count, unsigned int events[]) {
	for(unsigned int i = 0; i < count; i++) {
		Observable_t* observable = (Observable_t*) HashTable_get(scene->eventObservable, Object_ctorInt(events[i])).reference;
		if(observable)
			Observable_subscribe(observable, obj->base);
		else {
			observable = Observable_ctor();
			HashTable_add(scene->eventObservable, Object_ctorInt(events[i]), observable->base);
		}
	}

	HashTable_add(scene->objectDictionnary, String8_toObject(name), GObject_toObject(obj));
}

void Scene_removeGObject(Scene_t* scene, String8_t name) {
	// todo
}

inline void Scene_draw(Scene_t* scene) {
	scene->draw(scene->base.reference);
}

inline Object_t Scene_toObject(Scene_t* scene) {
	Object_t obj = scene->base;
	obj.id = SceneId;
	return obj;
}

inline Scene_t Scene_fromObject(Object_t obj) {
	return *((Scene_t*)obj.reference);	
}


typedef Scene_t*(*SceneBuilderFn_t)();

// Scene Builder
//
typedef struct SceneBuilder_t {
	Object_t base;
	SceneBuilderFn_t builder;
} SceneBuilder_t;

const unsigned int SceneBuilderId = registerType("SceneBuilder", sizeof(SceneBuilder_t));

inline Scene_t* SceneBuilder_build(SceneBuilder_t* sb) {
	return sb->builder();
}

void* SceneBuilder_copy(void* self) {
	return self;
}

Bool_t SceneBuilder_equals(void* self, void* reference) {
	return (Bool_t) (self == reference);
}

Bool_t SceneBuilder_dtor(void** self) {
	if(self) {
		if(*self) {
			SceneBuilder_t* b = (SceneBuilder_t*) *self;
			free(b);
			return TRUE;
		}
	}

	return FALSE;
}

SceneBuilder_t* SceneBuilder_ctor(SceneBuilderFn_t builder) {
	SceneBuilder_t* b = (SceneBuilder_t*) malloc(sizeof(SceneBuilder_t));
	*b = {
		Object_ctor(b, SceneBuilderId, SceneBuilder_copy, SceneBuilder_equals, SceneBuilder_dtor),
		builder
	};
	return b;
}


// Scene Manager
// 
static HashTable_t* sceneBuilder = HashTable_ctor(String8Id, SceneBuilderId);
static LinkedList_t* sceneStack = LinkedList_ctor(SceneId);
static Scene_t* currentScene = 0;
static String8_t currentName = 0;
static Object_t sceneTransit = Object_ctorNull();

inline void SceneManager_init(SceneBuilderFn_t builder) {
	currentScene = builder();
	currentName = Scene_getName(currentScene);
	LinkedList_pushBack(sceneStack, Scene_toObject(currentScene));
}

inline void SceneManager_add(String8_t name, SceneBuilderFn_t builder) {
	SceneBuilder_t* b = SceneBuilder_ctor(builder);
	HashTable_add(sceneBuilder, String8_toObject(name), b->base);
}

inline void SceneManager_renderScene() {
	if(currentScene)
		Scene_draw(currentScene);
}

void SceneManager_verifyScenes() {
	if(currentScene->state) {
		switch(currentScene->state) {
			case SCENE_STATE_PUSH: {
				SceneBuilder_t* builder = (SceneBuilder_t*) HashTable_get(sceneBuilder, String8_toObject(currentScene->args)).reference;
				currentScene = SceneBuilder_build(builder);
				currentName = Scene_getName(currentScene);
				LinkedList_pushBack(sceneStack, Scene_toObject(currentScene));
				break;
			}

			case SCENE_STATE_SWAP: {
				SceneBuilder_t* builder = (SceneBuilder_t*) HashTable_get(sceneBuilder, String8_toObject(currentScene->args)).reference;
				LinkedList_popBack(sceneStack);
				Object_dtor(currentScene->base);
				currentScene = SceneBuilder_build(builder);
				currentName = Scene_getName(currentScene);
				LinkedList_pushBack(sceneStack, Scene_toObject(currentScene));
				break;
			}

			case SCENE_STATE_POP: {
				if(currentScene->args) {
					SceneBuilder_t* builder = (SceneBuilder_t*) HashTable_get(sceneBuilder, String8_toObject(currentScene->args)).reference;
					LinkedList_popBack(sceneStack);
					Object_dtor(currentScene->base);
					currentScene = SceneBuilder_build(builder);
					currentName = Scene_getName(currentScene);
					LinkedList_pushBack(sceneStack, Scene_toObject(currentScene));
				}
				else {
					LinkedList_popBack(sceneStack);
					Object_dtor(currentScene->base);
					if(sceneStack->count) {
						currentScene = (Scene_t*) sceneStack->back->obj.reference;
						currentName = Scene_getName(currentScene);
					}
					else {
						currentName = 0;
						currentScene = 0;
					}
				}
				break;
			}

			default: {
				currentScene->state = SCENE_STATE_RENDER;
				break;
			}
		}
	}
}

#endif