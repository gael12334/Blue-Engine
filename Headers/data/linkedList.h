#ifndef BLUE_ENGINE_LINKEDLIST
#define BLUE_ENGINE_LINKEDLIST
#include "arrayList.h"

typedef struct DLNode_t {
	Object_t obj;
	DLNode_t* prev;
	DLNode_t* next;
} DLNode_t;

inline DLNode_t* DLNode_ctor(Object_t obj, DLNode_t* prev, DLNode_t* next) {
	DLNode_t* node = (DLNode_t*) malloc(sizeof(DLNode_t));
	*node = {obj, prev, next};
	return node;
}

Bool_t DLNode_dtor(void** self) {
	if(self) {
		if(*self) {
			free(*self); 
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}


typedef struct DLIterator_t {
	DLNode_t* node;
	DLNode_t* front;
} DLIterator_t;

inline Bool_t DLIterator_isValid(DLIterator_t* it) {
	return (Bool_t)(it->front == 0);
}

inline Bool_t DLIterator_isComplete(DLIterator_t* it) {
	if(it->node == 0)
		return TRUE;
		
	if(it->node && it->front) {
		if(it->node == it->front)
			return TRUE;
		if(it->node->next == 0) 
			return TRUE;
	}
	return FALSE;
}

Bool_t DLIterator_goPrev(DLIterator_t* it) {
	if(it->node) {
		if(it->node->next) {
			it->node = it->node->next;
			return TRUE;
		}
	}

	return FALSE;
}

Bool_t DLIterator_goNext(DLIterator_t* it) {
	if(it->node) {
		if(it->node->next) {
			it->node = it->node->next;
			return TRUE;
		}
	}

	return  FALSE;
}

inline Object_t DLIterator_get(DLIterator_t* it) {
	return it->node->obj;
}

inline void DLIterator_reset(DLIterator_t* it) {
	if(it->front) 
		it->node = it->front;
}

inline DLIterator_t DLIterator_ctor(DLNode_t* node) {
	return {node, node};
}


typedef struct LinkedList_t {
	Object_t base;
	
	DLNode_t* front;
	DLNode_t* back; 
	unsigned int typeId;
	unsigned int count;
} LinkedList_t; 

// registering LinkedList_t's type.
const unsigned int LinkedListId = registerType("LinkedList", sizeof(LinkedList_t));

inline unsigned int LinkedList_getCount(LinkedList_t* list) {
	return list->count;
}

Bool_t LinkedList_pushBack(LinkedList_t* list, Object_t obj) {
	if(list->typeId == obj.id) {
		// if the list isn't empty
		if(list->count) 
			list->back = list->back->next = DLNode_ctor(obj, list->back, 0);
		
		// if the list is empty
		else 
			list->back = list->front = DLNode_ctor(obj, 0, 0);

		list->count++; 
		return TRUE;
	}
	return FALSE;
}

Bool_t LinkedList_pushFront(LinkedList_t* list, Object_t obj) {
	if(list->typeId == obj.id) {
		// if the list isn't empty
		if(list->count) 
			list->front = list->front->prev = DLNode_ctor(obj, 0, list->front);
		
		// if the list is empty
		else 
			list->back = list->front = DLNode_ctor(obj, 0, 0);
		
		list->count++;
		return TRUE;
	}
	return FALSE;
}

Bool_t LinkedList_popBack(LinkedList_t* list) {
	if(list->count) {
		if(list->count == 1) {
			DLNode_dtor((void**)&list->front);
			list->back = 0;
			list->count--;
			return TRUE;
		}

		else if(list->count > 1) {
			DLNode_t* node = list->back->prev;
			DLNode_dtor((void**)&list->back);
			node->next = 0;
			list->back = node;
			list->count--;
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t LinkedList_popFront(LinkedList_t* list) {
	if(list->count) {
		if(list->count == 1) {
			DLNode_dtor((void**)&list->front);
			list->back = 0;
			list->count--;
			return TRUE;
		}

		else if(list->count > 1) {
			DLNode_t* node = list->front->next;
			DLNode_dtor((void**)&list->front);
			node->prev = 0;
			list->front = node;
			list->count--;
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t LinkedList_insert(LinkedList_t* list, DLIterator_t it, Object_t obj) {
	if((it.front == list->front) && (it.front)) {
		if(list->count > 1) {
			if(it.node == list->front)
				return LinkedList_pushFront(list, obj);

			if(it.node == list->back) 
				return LinkedList_pushBack(list, obj);

			DLNode_t* node = DLNode_ctor(obj, it.node->prev, it.node);
			it.node->prev->next = node;
			it.node->next->prev = node;
			list->count++;

			return TRUE;
		}

		return LinkedList_pushFront(list, obj);
	}
	return FALSE;
}

Bool_t LinkedList_remove(LinkedList_t* list, DLIterator_t* it) {
	if(!it) 
		return FALSE;

	if((it->front == list->front) && (it->front)) {
		if(list->count > 1) {
			if(it->node == list->front)
				return LinkedList_popBack(list);

			if(it->node == list->back) 
				return LinkedList_popBack(list);

			it->node->prev->next = it->node->next;
			it->node->next->prev = it->node->prev;
			DLNode_dtor((void**)&it->node);
			list->count--;
			it->node = it->front;

			return TRUE;
		}

		return LinkedList_popFront(list);
	}
	return FALSE;
}

inline DLIterator_t LinkedList_begin(LinkedList_t* list) {
	return DLIterator_ctor(list->front);
}

inline Object_t LinkedList_get(DLIterator_t it) {
	if(it.node) 
		return it.node->obj;

	return Object_ctorNull();
}

inline Bool_t LinkedList_set(DLIterator_t it, Object_t obj) {
	if(it.node) {
		it.node->obj = obj;
		return TRUE;
	}

	return FALSE;
}

LinkedList_t* LinkedList_ctor(unsigned int typeId);

Bool_t LinkedList_dtor(void** list) {
	if(list) {
		if(*list) {
			LinkedList_t* listPtr = (LinkedList_t*) *list;
			while(listPtr->count) 
				LinkedList_popBack(listPtr);
			free(listPtr);
			*list = 0;
			return TRUE;
		}
	}
	return FALSE;
}

void* LinkedList_copy(void* list) {
	if(list) {
		LinkedList_t* ll = (LinkedList_t*) list;
		LinkedList_t* copy = LinkedList_ctor(ll->typeId);
		DLIterator_t it = LinkedList_begin(ll);
		while(copy->count != ll->count) 
			LinkedList_pushBack(copy, it.node->obj);
		return copy;
	}
	return 0;
}

Bool_t LinkedList_equals(void* p1, void* p2) {
	if(p1 && p2) {
		LinkedList_t* l1 = (LinkedList_t*) p1;
		LinkedList_t* l2 = (LinkedList_t*) p2;
		DLIterator_t it1 = LinkedList_begin(l1);
		DLIterator_t it2 = LinkedList_begin(l2);

		if(l1->count != l2->count)
			return FALSE;
		
		for(unsigned int i = 0; i < l1->count; i++) {
			if(!it1.node->obj.equals(it1.node->obj.reference, it2.node->obj.reference))
				return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

LinkedList_t* LinkedList_ctor(unsigned int typeId) {
	LinkedList_t* list = (LinkedList_t*) malloc(sizeof(LinkedList_t));
	*list = {
		Object_ctor(list, LinkedListId, LinkedList_copy, LinkedList_equals, LinkedList_dtor),
		0, 
		0, 
		typeId,
		0
	};

	return list;
}

#endif