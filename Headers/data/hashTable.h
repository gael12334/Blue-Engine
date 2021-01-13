#ifndef BLUE_ENGINE_HASHTABLE
#define BLUE_ENGINE_HASHTABLE

#include "pair.h"

#define INTERNAL_HASHTABLE_LL_LENGTH 13



/// \brief Hash Table Iterator structure
typedef struct HTIterator_t {
	DLIterator_t base;
	LinkedList_t** elements;
	unsigned short index;

} HTIterator_t;

Bool_t HTIterator_goPrev(HTIterator_t* it) {
	if((it->base.node == 0) && (it->index > 0)) {
		it->index--;
		LinkedList_t* list = it->elements[it->index];
		it->base.node = list->back;
		return TRUE;
	}
	return FALSE;
}

Bool_t HTIterator_goNext(HTIterator_t* it) {
	if((it->base.node->next == 0) && (it->index < (INTERNAL_HASHTABLE_LL_LENGTH - 1))) {
		it->index++;
		LinkedList_t* list = it->elements[it->index];
		it->base.node = list->front;
		return TRUE;
	}
	return FALSE;
}

inline Object_t HTIterator_getObject(HTIterator_t* it) {
	return DLIterator_get(&it->base);
}

inline void HTIterator_reset(HTIterator_t* it) {
	DLIterator_reset(&it->base);
	it->index = 0;
}

Object_t HTIterator_getKey(HTIterator_t* it) {
	Object_t obj = HTIterator_getObject(it);
	Pair_t* p = (Pair_t*) obj.reference;
	return p->getA(p);
}

Object_t HTIterator_getEntry(HTIterator_t* it) {
	Object_t obj = HTIterator_getObject(it);
	Pair_t* p = (Pair_t*) obj.reference;
	return p->getB(p);
}

HTIterator_t HTIterator_ctor(LinkedList_t** lists) {
	HTIterator_t hti;
	if(!lists) {
		hti = {
			DLIterator_ctor(0), 
			lists, 
			0
		};
		return hti;
	}

	hti = {
		DLIterator_ctor(lists[0]->front), 
		lists, 
		0
	};
	return hti;
}

/// \brief Hash Table structure
typedef struct HashTable_t {
	Object_t base;
	LinkedList_t** elements;
	unsigned int typeId1;
	unsigned int typeId2;
} HashTable_t;

const unsigned int hashTableId = registerType("HashTable", sizeof(HashTable_t));

unsigned int HashTable_hash(Object_t key) {
	if(key.reference == 0) 
		return 0;
	
	if((key.id == intId) || (key.id == floatId)) {
		unsigned long long i = (unsigned long long) key.reference;
		return i % INTERNAL_HASHTABLE_LL_LENGTH;
	}

	unsigned int i = getTypeSize(key.id);

	if(i <= sizeof(unsigned long long)) {
		unsigned long long i = (unsigned long long) key.reference;
		return i % INTERNAL_HASHTABLE_LL_LENGTH;
	}

	char* data = (char*) key.reference;
	unsigned int result = 0;
	for(unsigned int i = 0, m = getTypeSize(key.id); i < m; i++) 
		result += ((data[i] * INTERNAL_HASHTABLE_LL_LENGTH) + 1) % INTERNAL_HASHTABLE_LL_LENGTH;
	
	return result;
}

inline HTIterator_t HashTable_begin(HashTable_t* ht) {
	return HTIterator_ctor(ht->elements);
}

static HTIterator_t HashTable_seek(HashTable_t* ht, Pair_t* p) {
	unsigned int index = HashTable_hash(p->getA(p));

	HTIterator_t hti = HashTable_begin(ht);
	hti.index = index; // we set the hashed index.
	hti.base.node = ht->elements[index]->front; // we set the iterator's node to the first node of the index-specified list.

	if(ht->elements[index]->count) { //  if the count of the index-specified list greater than 0 (non-empty list), then it means the pair may be contained.
		for(unsigned int i = 0, m = ht->elements[index]->count; i < m; i++) {
			Pair_t* pair = (Pair_t*) HTIterator_getObject(&hti).reference;
			Object_t key1 = pair->getA(pair);
			Object_t key2 = p->getA(p);
			if(key1.equals(key1.reference,key2.reference)) // if key's are equal, return the iterator.
				return hti;
			HTIterator_goNext(&hti);
		}
	}

	// if the count of the index-specified list is 0 (empty list), then it means the pair isn't contained.
	hti.base.node = 0;
	return hti;	
}

Object_t HashTable_get(HashTable_t* ht, Object_t key) {
	if(key.id == ht->typeId1) { // verification of object's types.
		Pair_t* pair = createPair(key, Object_ctorNull());
		HTIterator_t hti = HashTable_seek(ht, pair); // gets the iterator
		freePair((void**)&pair);

		if(hti.elements)
			return HTIterator_getEntry(&hti);
	}
	return Object_ctorNull();
}

Bool_t HashTable_set(HashTable_t* ht, Object_t key, Object_t entry) {
	if(key.id == ht->typeId1) { // verification of object's types.
		Pair_t* pair = createPair(key, Object_ctorNull());
		HTIterator_t hti = HashTable_seek(ht, pair); // gets the iterator
		freePair((void**)&pair);
		
		if(hti.elements) {
			hti.base.node->obj = entry;
			return TRUE;
		}
	}

	return FALSE;
}

Bool_t HashTable_add(HashTable_t* ht, Object_t key, Object_t entry) {
	if((key.id == ht->typeId1) && (entry.id == ht->typeId2)) { // verification of object's types.
		Pair_t* pair = createPair(key, entry);
		HTIterator_t hti = HashTable_seek(ht, pair); // gets the iterator

		if(hti.base.node) { //if the iterator shows that the node exists (if hti.element > 0, then node exists), we set the value
			Pair_t* nodedPair = (Pair_t*) hti.base.node->obj.reference; // do not delete 
			nodedPair->setB(nodedPair, entry);
			freePair((void**)&pair); //we free "pair" 
		}

		else // if the node does not exists, then we simply push-back it to the index-specified list.
			LinkedList_pushBack(hti.elements[hti.index], pair->base);
		
		return TRUE;
	}
	return FALSE;
}

Bool_t HashTable_remove(HashTable_t* ht, Object_t key) {
	if(key.id == ht->typeId1) { // verification of object's types.
		Pair_t* pair = createPair(key, Object_ctorNull());
		HTIterator_t hti = HashTable_seek(ht, pair); // gets the iterator
		freePair((void**)&pair);

		if(hti.base.node) { //if the iterator shows that the node exists (if hti.element > 0, then node exists), we set the value
			LinkedList_t* list = hti.elements[hti.index]; // do not delete
			DLIterator_t it = LinkedList_begin(list);
			it.node = hti.base.node;
			LinkedList_remove(list, &it);
		}
	}
	return FALSE;
}

unsigned int HashTable_getCount(HashTable_t* ht) {
	unsigned int result = 0;
	for(unsigned short i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) 
		result += ht->elements[i]->count;
	return result;
}

HashTable_t* HashTable_ctor(unsigned int t1, unsigned int t2);

void* HashTable_copy(void* ht) {
	if(ht) {
		HashTable_t* hashTable = (HashTable_t*) ht;
		HashTable_t* copy = HashTable_ctor(hashTable->typeId1, hashTable->typeId2);
		copy->elements = (LinkedList_t**) malloc(sizeof(LinkedList_t*) * INTERNAL_HASHTABLE_LL_LENGTH);

		for(unsigned short i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) 
			copy->elements[i] = (LinkedList_t*) LinkedList_copy(hashTable->elements[i]);

		return copy;
	}
	return 0;
}

Bool_t HashTable_equals(void* ht1, void* ht2) {
	if(ht1 && ht2) {
		HashTable_t* hashTable1 = (HashTable_t*) ht1;
		HashTable_t* hashTable2 = (HashTable_t*) ht2;
		for(unsigned short i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) {
			if(!LinkedList_equals(hashTable1->elements[i], hashTable2->elements[i]))
				return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

Bool_t HashTable_dtor(void** ht) {
	if(ht) {
		if(*ht) {
			HashTable_t* hashtable = (HashTable_t*) ht;
			for(unsigned short i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++) {
				void* ptr = hashtable->elements[i];
				LinkedList_dtor(&ptr);
			}
			*ht = 0;
			return TRUE;
		}
	}
	return FALSE;
}

HashTable_t* HashTable_ctor(unsigned int t1, unsigned int t2) {
	HashTable_t* ht = (HashTable_t*) malloc(sizeof(HashTable_t));
	ht->elements = (LinkedList_t**) malloc(sizeof(LinkedList_t*) * INTERNAL_HASHTABLE_LL_LENGTH);
	ht->typeId1 = t1;
	ht->typeId2 = t2;
	ht->base = Object_ctor(ht, hashTableId, HashTable_copy, HashTable_equals, HashTable_dtor);

	for(char i = 0; i < INTERNAL_HASHTABLE_LL_LENGTH; i++)
		ht->elements[i] = LinkedList_ctor(pairId);

	return ht;
}

#endif