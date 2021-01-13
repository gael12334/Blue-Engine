#ifndef BLUE_ENGINE_GIMAGE
#define BLUE_ENGINE_GIMAGE

#include "../scenes.h"

typedef struct GImage_t {
	GObject_t base;
	Surface_t* image;
	String8_t resName;
	
} GImage_t;

const unsigned int GImageId = registerType("GImage", sizeof(GImage_t));

GImage_t* GImage_ctor(FPoint_t position, Point_t size, String8_t resName);

GImage_t* GImage_ctor(FPoint_t position, String8_t resName);

void* GImage_copy(void* self) {
	if(self) {
		GImage_t* img = (GImage_t*) self;
		GImage_t* copy = GImage_ctor(img->base.position, img->resName);
		return copy;
	}

	return 0;
}

Bool_t GImage_equals(void* self, void* reference) {
	if(self && reference) {
		GImage_t* i1 = (GImage_t*) self;
		GImage_t* i2 = (GImage_t*) reference;
		return (Bool_t) ((i1->base.position == i2->base.position) && (i1->resName && i2->resName) && (i1->base.size == i2->base.size) && (i1->image == i2->image));
	}

	return FALSE;
}

Bool_t GImage_dtor(void** self) {
	if(self) {
		if(*self) {
			GImage_t* gimg = (GImage_t*) *self;
			free(gimg);
			return TRUE;
		}
	}

	return FALSE;
}

void GImage_move(void* self, FPoint_t p) {

}

void GImage_resize(void* self, Point_t s) {

}

inline void GImage_draw(void* self) {
	GImage_t* img = (GImage_t*) self;
	Rect_t rect = Rect_ctor(img->base.position, img->base.size);
	Renderer_renderSurface(renderer, rect, img->image);
}

GImage_t* GImage_ctor(FPoint_t position, Point_t size, String8_t resName) {
	GImage_t* gimg = (GImage_t*) malloc(sizeof(GImage_t));
	gimg->base = GObject_ctor(position, size, gimg, GImageId, GImage_copy, GImage_equals, GImage_dtor, GObject_notification, GImage_move, GImage_resize, GImage_draw);
	gimg->image = (Surface_t*) ResourceManager_get(resName).base.reference;
	Renderer_refreshSurface(renderer, gimg->image);
	gimg->resName = resName;

	if(!size) 
		gimg->base.size = Surface_getSize(gimg->image);
	
	return gimg;
}

GImage_t* GImage_ctor(FPoint_t position, String8_t resName) {
	return GImage_ctor(position, 0, resName);
}




#endif