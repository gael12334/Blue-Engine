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

// =======================================

typedef struct LinearHTIterator_t {
	HTNode_t** nodes; 
	HTNode_t* itNode;
	unsigned int rowIndex; 
} LinearHTIterator_t;

LinearHTIterator_t LinearHTIterator_ctor(HTNode_t** nodes) {
	LinearHTIterator_t it;
	it.nodes = nodes;
	it.itNode = nodes[0];
	it.rowIndex = 0;
	return it;
}

inline Bool_t LinearHTIterator_ended(LinearHTIterator_t* it) {
	return (it->itNode == 0);
}

void LinearHTIterator_next(LinearHTIterator_t* it) {
	if(it->itNode->next) //hopper condition if true
		it->itNode = it->itNode->next;
	
	else if(it->rowIndex == (INTERNAL_HASHTABLE_LL_LENGTH - 1)) { // executed only if hopper condition is false. 
		it->itNode = 0;
		it->rowIndex = INTERNAL_HASHTABLE_LL_LENGTH;
	}

	else if (it->rowIndex == INTERNAL_HASHTABLE_LL_LENGTH) { // if the node is null and rowIndex = 13
		it->itNode = it->nodes[0];
		it->rowIndex = 0;
	}

	else { // if next is null and rowIndex smaller than INTERNAL_HASHTABLE_LL_LENGTH
		it->itNode = it->nodes[it->rowIndex + 1]; 
		it->rowIndex++;
	}
}

inline void LinearHTIterator_reset(LinearHTIterator_t* it) {
	it->itNode = it->nodes[0];
}

inline Object_t LinearHTIterator_key(LinearHTIterator_t* it) {
	return it->itNode->key;
}

inline Object_t LinearHTIterator_entry(LinearHTIterator_t* it) {
	return it->itNode->entry;
}

#endif