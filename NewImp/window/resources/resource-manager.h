#ifndef blue_engine_resource_manager
#define blue_engine_resource_manager

#include "font.h"

static HashTable_t* resourceManager = HashTable_ctor(int_id, BaseResource_id); 

void ResourceManager_add(StringInt_t name, void* resource) {
	BaseResource_t* base = (BaseResource_t*) resource;
	HashTable_set(resourceManager, StringInt_converToObject(name), base->base);
}

BaseResource_t* ResourceManager_get(StringInt_t name) {
	char existed = FALSE;
	Object_t obj = HashTable_get(resourceManager, StringInt_converToObject(name), &existed);
	BaseResource_t* resource = (BaseResource_t*) obj.self;
	return resource;
}

#endif