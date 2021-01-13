#ifndef blue_engine_htiterator
#define blue_engine_htiterator

#include "htnode.h"

#define INTERNAL_HASHTABLE_LL_LENGTH 13

typedef struct HTIterator_t {
	HTNode_t** 		nodes;
	HTNode_t* 		node;
	unsigned int 	index;
} HTIterator_t;

HTIterator_t HTIterator_ctor(HTNode_t** nodes) {
	HTIterator_t it;
	it.nodes = nodes;
	it.node = nodes[0];
	it.index = 0;
	return it;
}

void HTIterator_next(HTIterator_t* it) {
	if(it->node) {
		if((it->node->next == 0) && ((it->index + 1) < INTERNAL_HASHTABLE_LL_LENGTH))
			it->node = it->nodes[++it->index];
		else {
			it->node = 0;
			it->index++;
		}
	}
	else {
		it->node = it->nodes[0];
		it->index = 0;
	}
}

inline Bool_t HTIterator_ended(HTIterator_t* it) {
	return (it->node == 0);
}

inline void HTIterator_reset(HTIterator_t* it) {
	it->node = it->nodes[0];
	it->index = 0;
}

inline Object_t HTIterator_key(HTIterator_t* it) {
	if(it->node) 
		return it->node->key;
	return Object_ctor(0, 0);
}

inline Object_t HTIterator_entry(HTIterator_t* it) {
	if(it->node)
		return it->node->entry;
	return Object_ctor(0, 0);
}

#endif