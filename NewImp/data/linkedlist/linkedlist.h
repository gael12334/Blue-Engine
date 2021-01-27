#ifndef blue_engine_linkedlist
#define blue_engine_linkedlist

#include "dliterator.h"

typedef struct LinkedList_t {
	Object_t 		obj;
	DLNode_t* 		first;
	DLNode_t* 		last;
	unsigned int 	count;
	unsigned int 	id;
} LinkedList_t; 

// foward declaration
LinkedList_t* LinkedList_ctor(unsigned int id);
Bool_t LinkedList_pushfront(void* self, Object_t obj); 
Bool_t LinkedList_pushback(void* self, Object_t obj); 
Bool_t LinkedList_popfront(void* self); 
Bool_t LinkedList_popback(void* self); 
unsigned int LinkedList_getCount(void* self);
unsigned int LinkedList_getId(void* self);
DLIterator_t LinkedList_begin(void* self);
Bool_t LinkedList_insert(void* self, DLIterator_t where, Object_t obj);
Bool_t LinkedList_remove(void* self, DLIterator_t where);

// vtable
void* LinkedList_copy(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	LinkedList_t* copy = LinkedList_ctor(list->obj.id);

	DLIterator_t it = DLIterator_ctor(list->first, list->last);
	while(!DLIterator_ended(&it)) {
		LinkedList_pushback(copy, it.node->obj);
		DLIterator_next(&it);
	}

	return copy;
}

Bool_t LinkedList_equals(void* self, void* ref) {
	LinkedList_t* list = (LinkedList_t*) self;
	LinkedList_t* ref_list = (LinkedList_t*) ref;

	if(list->count != ref_list->count)	
		return FALSE;

	DLIterator_t it_list = DLIterator_ctor(list->first, list->last);
	DLIterator_t it_ref = DLIterator_ctor(ref_list->first, ref_list->last);

	while(!(DLIterator_ended(&it_list) || DLIterator_ended(&it_ref))) {
		if(Object_equals(DLNode_getObject(it_list.node).self, DLNode_getObject(it_ref.node).self)) 
			return FALSE;
		DLIterator_next(&it_list);
		DLIterator_next(&it_ref);
	}

	return TRUE;
}

Bool_t LinkedList_dtor(void** self) {
	if(self) {
		if(*self) {
			LinkedList_t* list = (LinkedList_t*) *self;
			while(list->first) 
				LinkedList_popback(list);
			return TRUE;
		}
	}

	return FALSE;
}

// type id
const unsigned int LinkedList_id = 
	TypeRegister_register(
		BaseVTable_createPointer(
			LinkedList_copy, 
			LinkedList_equals, 
			LinkedList_dtor
		), 
		sizeof(LinkedList_t),
		sizeof(LinkedList_t), 
		0, 
		"LinkedList_t"
	)
;

// functions
LinkedList_t* LinkedList_ctor(unsigned int id) {
	LinkedList_t* list = (LinkedList_t*) malloc(sizeof(LinkedList_t));
	*list = {Object_ctor(list, LinkedList_id), 0, 0, 0, id}; 
	return list; 
}

Bool_t LinkedList_pushfront(void* self, Object_t obj) {
	LinkedList_t* list = (LinkedList_t*) self;

	if(list->id == obj.id) {
		// if the list isn't empty
		if(list->count) 
			list->first = list->first->prev = DLNode_ctor(obj, 0, list->first);
		
		// if the list is empty
		else 
			list->last = list->first = DLNode_ctor(obj, 0, 0);

		list->count++; 
		return TRUE;
	}
	return FALSE;
}

Bool_t LinkedList_pushback(void* self, Object_t obj) {
	LinkedList_t* list = (LinkedList_t*) self;
	if(list->id == obj.id) {
		// if the list isn't empty
		if(list->count) 
			list->last = list->last->next = DLNode_ctor(obj, list->last, 0);
		
		// if the list is empty
		else 
			list->last = list->first = DLNode_ctor(obj, 0, 0);

		list->count++; 
		return TRUE;
	}
	return FALSE;
}

Bool_t LinkedList_popfront(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	if(list->count) {
		if(list->count == 1) {
			DLNode_dtor((void**)&list->first);
			list->last = 0;
			list->count--;
			return TRUE;
		}

		else if(list->count > 1) {
			DLNode_t* node = list->last->prev;
			DLNode_dtor((void**)&list->last);
			node->next = 0;
			list->last = node;
			list->count--;
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t LinkedList_popback(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	if(list->count) {
		if(list->count == 1) {
			DLNode_dtor((void**)&list->first);
			list->last = 0;
			list->count--;
			return TRUE;
		}

		else if(list->count > 1) {
			DLNode_t* node = list->first->next;
			DLNode_dtor((void**)&list->first);
			node->prev = 0;
			list->first = node;
			list->count--;
			return TRUE;
		}
	}
	return FALSE;
}

inline unsigned int LinkedList_getCount(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	return list->count;
}

inline unsigned int LinkedList_getId(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	return list->id;
}

DLIterator_t LinkedList_begin(void* self) {
	LinkedList_t* list = (LinkedList_t*) self;
	return DLIterator_ctor(list->first, list->last);
}

Bool_t LinkedList_insert(void* self, DLIterator_t where, Object_t obj) {
	LinkedList_t* list = (LinkedList_t*) self;
	if((where.first == list->first) && (where.first)) {
		if(list->count > 1) {
			if(where.node == list->first)
				return LinkedList_pushfront(list, obj);

			if(where.node == list->last) 
				return LinkedList_pushback(list, obj);

			DLNode_t* node = DLNode_ctor(obj, where.node->prev, where.node);
			where.node->prev->next = node;
			where.node->next->prev = node;
			list->count++;

			return TRUE;
		}

		return LinkedList_pushfront(list, obj);
	}
	return FALSE;
}

Bool_t LinkedList_remove(void* self, DLIterator_t* where) {
	LinkedList_t* list = (LinkedList_t*) self;

	if(!where) 
		return FALSE;

	if((where->first == list->first) && (where->first)) {
		if(list->count > 1) {
			if(where->node == list->first)
				return LinkedList_popback(list);

			if(where->node == list->last) 
				return LinkedList_popback(list);

			where->node->prev->next = where->node->next;
			where->node->next->prev = where->node->prev;
			DLNode_dtor((void**)&where->node);
			list->count--;
			where->node = where->first;

			return TRUE;
		}

		return LinkedList_popfront(list);
	}
	return FALSE;
}

#endif