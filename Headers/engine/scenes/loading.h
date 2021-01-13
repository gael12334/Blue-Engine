#ifndef BLUE_ENGINE_LOADING
#define BLUE_ENGINE_LOADING

#include "../gobjs/gimage.h"

typedef struct LoadingScene_t {
	Scene_t base;
	GImage_t* splashScreen;
} LoadingScene_t; 

const unsigned int LoadingSceneId = registerType("LoadingScene", sizeof(LoadingScene_t));

void* LoadingScene_copy(void* self) {
	return self;
}

Bool_t LoadingScene_equals(void* self, void* reference) {
	return (Bool_t) (self == reference);
}

Bool_t LoadingScene_dtor(void** self) {
	if(self) {
		if(*self) {
			LoadingScene_t* b = (LoadingScene_t*) *self;
			Texture_dtor((void**)&b->splashScreen);
			free(b);
			return TRUE;
		}
	}

	return FALSE;
}

inline void LoadingScene_draw(void* scene) {
	LoadingScene_t* loading = (LoadingScene_t*) scene;
	GImage_draw(loading->splashScreen);
}

Scene_t* LoadingScene_ctor() {
	Surface_t* surface = Surface_ctor("poop.png");
	ResourceManager_add(String8_ctor("poop"), surface->base);

	LoadingScene_t* scene = (LoadingScene_t*) malloc(sizeof(LoadingScene_t));
	Scene_ctor(&scene->base, String8_ctor("loading"), scene, LoadingSceneId, LoadingScene_copy, LoadingScene_equals, LoadingScene_dtor, LoadingScene_draw), 
	scene->splashScreen = GImage_ctor(FPoint_ctor(0.0F, 0.0F), String8_ctor("poop"));
	return (Scene_t*) scene;
}


#endif