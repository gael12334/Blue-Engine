#ifndef blue_engine_htnode
#define blue_engine_htnode
#include "../linkedlist/linkedlist.h"

typedef struct HTNode_t {
	struct HTNode_t* 	next;
	Object_t 			key;
	Object_t 			entry;
} HTNode_t; 

unsigned int HTNode_count(HTNode_t* node) {
	unsigned int count = 0;
	while(node) {
		node = node->next;
		count++;
	}
	return count;
}

// The hash table node has a simply linked list behavior.
inline HTNode_t* HTNode_ctor(Object_t key, Object_t entry, HTNode_t* next) {
	HTNode_t* node = (HTNode_t*) malloc(sizeof(HTNode_t));
	node->next = next;
	node->key = key;
	node->entry = entry;
	return node;
}

inline Bool_t HTNode_equals(void* self, void* node) {
	HTNode_t* self_node = (HTNode_t*) self;
	HTNode_t* node_node = (HTNode_t*) node;
	Bool_t result = TRUE;

	if(HTNode_count(self_node) == HTNode_count(node_node)) {
		while(self_node && node_node && result) {
			result = Object_equals(Object_ctor(self_node->key.self, self_node->key.id), Object_ctor(node_node->key.self, node_node->key.id)) && result;
			result = Object_equals(Object_ctor(self_node->entry.self, self_node->entry.id), Object_ctor(node_node->entry.self, node_node->entry.id)) && result;
			self_node = self_node->next;
			node_node = node_node->next;
		}

		return result;
	}

	return FALSE;
}

HTNode_t* HTNode_copy(void* self) {
	HTNode_t* self_node = (HTNode_t*) self;
	HTNode_t* copy_node = HTNode_ctor(self_node->key, self_node->entry, 0);
	HTNode_t* copy_first = copy_node;

	while(self_node) {
		copy_node = copy_node->next = HTNode_ctor(self_node->key, self_node->entry, 0);
		self_node = self_node->next;
	}

	return copy_first;
}

Bool_t HTNode_dtor(void** self) {
	if(self) {
		if(*self) {
			HTNode_t* self_node = (HTNode_t*) self;
			HTNode_t* self_next = self_node->next;
			free(self_node);

			while(self_next) {
				free(self_next);
				self_next = self_next->next;
			}

			*self = 0;

			return TRUE;
		}
	}

	return FALSE;
}

#endif