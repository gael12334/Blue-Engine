#ifndef blue_engine_hashtable
#define blue_engine_hashtable

#define HT_KEY 		3
#define HT_ENTRY	4
#define HT_BADTYPE  5

#include "htiterator.h"

typedef struct HashTable_t {
	Object_t 		base;
	HTNode_t* 		nodes[INTERNAL_HASHTABLE_LL_LENGTH];
	unsigned int 	keyId;
	unsigned int 	entryId;
} HashTable_t;

// foward declaration
HashTable_t* HashTable_ctor(unsigned int type1, unsigned int type2);

// virtual functions
void* HashTable_copy(void* self) {
	HashTable_t* ht = (HashTable_t*) self;
	HashTable_t* copy = HashTable_ctor(ht->keyId, ht->entryId);

	for(unsigned int i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) 
		copy->nodes[i] = HTNode_copy(ht->nodes[i]);
	
	return copy;
}

Bool_t HashTable_equals(void* self, void* ht) {
	HashTable_t* hashtable1 = (HashTable_t*) self;
	HashTable_t* hashtable2 = (HashTable_t*) ht;

	for(unsigned int i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) {
		if(!HTNode_equals(hashtable1->nodes[i], hashtable2->nodes[i]))
			return FALSE;
	}

	return TRUE;
}

Bool_t HashTable_dtor(void** self) {
	if(self) {
		if(*self) {
			HashTable_t* ht = (HashTable_t*) *self;
			for(unsigned int i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++)
				HTNode_dtor((void**)&ht->nodes[i]);
			free(*self);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

const unsigned int HashTable_id = 
	TypeRegister_register(
		BaseVTable_createPointer(
			HashTable_copy, 
			HashTable_equals, 
			HashTable_dtor
		),
		sizeof(BaseVTable_t),
		sizeof(HashTable_t), 
		0, 
		"HashTable_t"		
	)
;

// functions
HashTable_t* HashTable_ctor(unsigned int type1, unsigned int type2) {
	HashTable_t* ht = (HashTable_t*) malloc(sizeof(HashTable_t));
	for(unsigned int i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++)
		ht->nodes[i] = NULL;
	ht->keyId = type1;
	ht->entryId = type2;
	ht->base = Object_ctor(ht, HashTable_id);
	return ht;
}

static unsigned long long HashTable_hash(Object_t key) {
	if((key.id == int_id) || (key.id == float_id))
		return ((unsigned long long)key.self) % INTERNAL_HASHTABLE_LL_LENGTH;
	
	char* data = (char*) key.self;
	unsigned int size = TypeRegister_getSize(key.id);
	unsigned long long result;
	for(unsigned int i = 0; i < size; i++)
		result += data[i];
	result %= INTERNAL_HASHTABLE_LL_LENGTH;
	
	return result;
}

static Bool_t HashTable_seekNode(HashTable_t* ht, Object_t key, HTNode_t** seeked, HTNode_t** previous) {
	unsigned int index = HashTable_hash(key);
	HTNode_t* node = ht->nodes[index];
	HTNode_t* last = 0;

	if(node) {

		// seeks the node by comparing the key with node's keys.
		while(node) {
			Bool_t result = Object_equals(&key, &node->key);
			if(!result) {
				if(node->next == 0)
					last = node;

				node = node->next;
			}
			else 
				break;
		}
		

		// if the node exists, then set "existed" to true and return the node's adress.
		if(node) {
			if(previous)
				*previous = last;
			if(seeked)
				*seeked = node;
			return TRUE;
		}
	}

	// we create a node to return a non-null node, if: 

	//1. the indexed element has sub-nodes.
	if(last) {
		last->next = HTNode_ctor(key, Object_ctor(0, ht->entryId), 0);
		if(previous)
			*previous = last;
		if(seeked)
			*seeked = last->next;
		return TRUE;
	}

	//2. the indexed element has no sub-nodes.
	ht->nodes[index] = HTNode_ctor(key, Object_ctor(0, ht->entryId), 0);
	if(previous)
		*previous = last;
	if(seeked)
		*seeked = ht->nodes[index];
	return FALSE; 
}

static Bool_t HashTable_checkType(HashTable_t* ht, Object_t obj, char type) {
	switch(type) {
		case HT_KEY: {
			return TypeRegister_IsInherited(ht->keyId, obj.id);
			break;
		}

		case HT_ENTRY: {
			return TypeRegister_IsInherited(ht->entryId, obj.id);
			break;
		}
	}

	return FALSE;
} 

inline char HashTable_set(HashTable_t* ht, Object_t key, Object_t entry) {
	if(HashTable_checkType(ht, key, HT_KEY) && HashTable_checkType(ht, entry, HT_ENTRY)) { 
		HTNode_t* node = 0;
		HashTable_seekNode(ht, key, &node, NULL);
		node->entry = entry;
		return TRUE;
	}
	
	return HT_BADTYPE;
}

inline Object_t HashTable_get(HashTable_t* ht, Object_t key, char* existed) {
	if(HashTable_checkType(ht, key, HT_KEY)) { 
		HTNode_t* node = 0;
		Bool_t result = HashTable_seekNode(ht, key, &node, NULL);
		if(existed) 
			*existed = result;
		return node->entry;
	}

	if(existed)
		*existed = HT_BADTYPE;
	return Object_ctor(0, ht->entryId);
}

char HashTable_remove(HashTable_t* ht, Object_t key) {
	if(HashTable_checkType(ht, key, HT_KEY)) { 
		HTNode_t* node = 0;
		HTNode_t* prev = 0;
		if(HashTable_seekNode(ht, key, &node, &prev)) {

			if(prev) {
				prev->next = node->next;
				free(node);
			}

			else {
				// Since prev is null, this means we remove (free) the root node of this hashed list.
				// To do so, we copy the elements of the next node into the root node, overwritting its info.
				// Then we simpy free the next node as it has been copied and thus duplicate.
				node->key = node->next->key;
				node->entry = node->next->entry;
				HTNode_t* toDelete = node->next;
				node->next = node->next->next;
				free(toDelete);
			}

			return TRUE;
		}
		return FALSE;
	}
	return HT_BADTYPE;
}

inline unsigned int HashTable_count(HashTable_t* ht) {
	unsigned int result = 0;
	for(unsigned int i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) 
		result += HTNode_count(ht->nodes[i]);
	return result;
}

inline HTIterator_t HashTable_begin(HashTable_t* ht) {
	return HTIterator_ctor(ht->nodes);
}

inline LinearHTIterator_t HashTable_linearBegin(HashTable_t* ht) {
	return LinearHTIterator_ctor(ht->nodes);
}

#endif