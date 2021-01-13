#ifndef BLUE_ENGINE_PAIR
#define BLUE_ENGINE_PAIR

#include "linkedList.h"

typedef struct Pair_t {
	Object_t base, value1, value2;
	unsigned int typeId1, typeId2;

	Object_t(*getA)(Pair_t* p);
	Object_t(*getB)(Pair_t* p);
	void(*setA)(Pair_t* p, Object_t obj);
	void(*setB)(Pair_t* p, Object_t obj);
	unsigned int(*getAId)(Pair_t* p);
	unsigned int(*getBId)(Pair_t* p);
} Pair_t;

const unsigned int pairId = registerType("Pair", sizeof(Pair_t));

Object_t getPairA(Pair_t* p) {
	return p->value1;
}

Object_t getPairB(Pair_t* p) {
	return p->value2;
}

void setPairA(Pair_t* p, Object_t obj) {
	p->value1 = obj;
}

void setPairB(Pair_t* p, Object_t obj) {
	p->value2 = obj;
}

unsigned int getPairAId(Pair_t* p) {
	return p->typeId1;
}

unsigned int getPairBId(Pair_t* p) {
	return p->typeId2;
}

Pair_t* createPair(unsigned int t1, unsigned int t2);

Bool_t comparePair(void* p1, void* p2) {
	if(p1 && p2) {
		Pair_t* pair1 = (Pair_t*) p1;
		Pair_t* pair2 = (Pair_t*) p2;
		if(pair1->typeId1 != pair2->typeId1)
			return FALSE;
		if(pair1->typeId2 != pair2->typeId2)
			return FALSE;
		Bool_t result1 = (Bool_t)pair1->getA(pair1).equals(pair1->getA(pair1).reference, pair2->getA(pair2).reference);
		Bool_t result2 = (Bool_t)pair1->getB(pair1).equals(pair1->getB(pair1).reference, pair2->getB(pair2).reference);
		return (Bool_t) (result1 && result2);
	}

	return FALSE;
}

Bool_t freePair(void** p) {
	if(p) {
		if(*p) {
			free(*p);
			p = 0;
			return TRUE;
		}
	}
	return FALSE;
}

void* copyPair(void* ptr) {
	if(ptr) {
		Pair_t* pair = (Pair_t*) ptr;
		Pair_t* copy = (Pair_t*) malloc(sizeof(Pair_t));
		*copy = *pair;
		return copy;
	}
	return 0;
}

Pair_t* createPair(Object_t obj1, Object_t obj2) {
	Pair_t* p = (Pair_t*) malloc(sizeof(Pair_t));
	*p = {
		Object_ctor(p, pairId, copyPair, comparePair, freePair), obj1, obj2, 
		obj1.id, obj2.id, getPairA, getPairB, setPairA, setPairB, getPairAId, getPairBId
	};
	
	return p;
}

#endif