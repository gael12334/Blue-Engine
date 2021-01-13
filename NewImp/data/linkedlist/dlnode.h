#ifndef blue_engine_dlnode
#define blue_engine_dlnode

#include "../arraylist/arrayList.h"

typedef struct DLNode_t {
	Object_t obj;
	struct DLNode_t* prev;
	struct DLNode_t* next;
} DLNode_t;

DLNode_t* DLNode_ctor(Object_t obj, DLNode_t* prev, DLNode_t* next) {
	DLNode_t* node = (DLNode_t*) malloc(sizeof(DLNode_t));
	*node = {obj, prev, next}; 
	return node;
}

Bool_t DLNode_dtor(void** node) {
	if(node) {
		if(*node) {
			free(*node);
			*node = 0;
			return TRUE;
		}
	}
	return FALSE;
}

#endif