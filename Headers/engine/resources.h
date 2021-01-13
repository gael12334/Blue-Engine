#ifndef BLUE_ENGINE_RESOURCES
#define BLUE_ENGINE_RESOURCES

#include "utils.h"
#include "../data/string.h"

// Resource structure, pure virtual structure.
//
typedef struct Resource_t {
	Object_t base;
	unsigned int id;
	const char*(*getName)();
} Resource_t;

const unsigned int ResourceId = registerType("Resource", sizeof(Resource_t));

Resource_t Resource_ctor(void* self, unsigned int id, void*(*copy)(void* self), Bool_t(*equals)(void* self, void* ref), Bool_t(*dtor)(void** self_ptr), const char*(*getName)()) {
	Resource_t res = {
		Object_ctor(self, ResourceId, copy, equals, dtor),
		id,
		getName
	};
	
	return res;
}

const char* Resource_getName(Resource_t* res, void* self) {
	if(self && res) 
		return res->getName();
	return "0";
}

inline Object_t Resource_toObject(Resource_t* res) {
	Object_t obj = res->base;
	obj.id = ResourceId;
	return obj;
}

inline Resource_t Resource_fromObject(Object_t obj) {
	return *((Resource_t*) obj.reference);
}

// Resource manager.
//
static HashTable_t* resourceManager = HashTable_ctor(String8Id, ResourceId);

inline void ResourceManager_add(String8_t name, Resource_t res) {
	HashTable_add(resourceManager, String8_toObject(name), res.base);
}

inline Resource_t ResourceManager_get(String8_t name) {
	Object_t obj = HashTable_get(resourceManager, String8_toObject(name));
	Resource_t res = *(Resource_t*)obj.reference;
	return res;
}

inline void ResourceManager_remove(String8_t name) {
	HashTable_remove(resourceManager, String8_toObject(name));
}

#endif