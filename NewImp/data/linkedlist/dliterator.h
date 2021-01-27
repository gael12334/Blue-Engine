#ifndef blue_engine_dliterator
#define blue_engine_dliterator
#include "dlnode.h"

#define END_ITERATOR 0

typedef struct DLIterator_t {
	DLNode_t* first;
	DLNode_t* last;
	DLNode_t* node;
} DLIterator_t;

inline DLIterator_t DLIterator_ctor(DLNode_t* first, DLNode_t* last) {
	return {first, last, first};
}

inline Bool_t DLIterator_ended(DLIterator_t* it) {
	return (it->node == END_ITERATOR);
}

inline void DLIterator_next(DLIterator_t* it) {
	if(it->node)
		it->node = it->node->next;
	else
		it->node = it->first;
}

inline void DLIterator_prev(DLIterator_t* it) {
	if(it->node) 
		it->node = it->node->prev;
	else 
		it->node = it->last;
}

inline void DLIterator_reset(DLIterator_t* it) {
	it->node = it->first;
}

inline const Object_t* DLIterator_get(DLIterator_t* it) {
	if(it->node == END_ITERATOR)
		return 0;
	return &it->node->obj;
}

#undef END_ITERATOR
#endif