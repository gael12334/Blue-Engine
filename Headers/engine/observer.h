#ifndef BLUE_ENGINE_OBSERVER
#define BLUE_ENGINE_OBSERVER

#include "../data/hashTable.h"

typedef void(*NotifFn_t)(void*);

// pure virtual structure.
// 
typedef struct Observer_t {
	Object_t base;
	unsigned int id;
	void(*notification)(void* self);
} Observer_t;

const unsigned int ObserverId = registerType("Observer", sizeof(Observer_t));

inline Observer_t Observer_ctor(void* self, unsigned int id, CopyFn_t copy, CmpFn_t equals, DtorFn_t dtor, NotifFn_t notification) {
	Observer_t ob = {
		Object_ctor(self, ObserverId, copy, equals, dtor),
		id,
		notification
	};

	return ob;
}

void Observer_notification(Observer_t* observer) {
	if(observer) {
		if(observer->base.reference)
			observer->notification(observer->base.reference);
	}
}

inline Object_t Observer_toObject(Observer_t* observer) {
	Object_t obj = observer->base;
	obj.id = ObserverId;
	return obj;
}

inline Observer_t Observer_fromObject(Object_t obj) {
	return *((Observer_t*) obj.reference);
}


// Observable - cannot be derived.
//
typedef struct Observable_t {
	Object_t base;
	LinkedList_t* observers;

} Observable_t;

const unsigned int ObservableId = registerType("Observable", sizeof(Observable_t));

inline void Observable_subscribe(Observable_t* observable, Observer_t observer) {
	if(observable)
		LinkedList_pushBack(observable->observers, observer.base);
}

void Observable_unsubscribe(Observable_t* observable, Observer_t observer) {
	// TODO: finish this function
	// It searches through the list (observable->observers) the observer.
}

void Observable_notify(Observable_t* observable) {
	if(observable) {
		DLIterator_t it = LinkedList_begin(observable->observers);
		if(DLIterator_isValid(&it)) {
			while(DLIterator_isComplete(&it)) {
				Object_t obj = DLIterator_get(&it);
				Observer_t* ob = (Observer_t*) obj.reference; // <----------- not sure if this works...
				ob->notification(obj.reference);
				DLIterator_goNext(&it);
			}
		}
	}
}

Observable_t* Observable_ctor();

void* Observable_copy(void* self) {
	if(self) {
		Observable_t* ob = (Observable_t*) self;
		Observable_t* copy = Observable_ctor();
		LinkedList_dtor((void**)&copy->observers);
		copy->observers = (LinkedList_t*) LinkedList_copy(ob->observers);
		return copy;
	}

	return 0;
}

Bool_t Observable_equals(void* self, void* reference) {
	if(self && reference) {
		if(self == reference)
			return TRUE;
		
		Observable_t* ob1 = (Observable_t*) self;
		Observable_t* ob2 = (Observable_t*) self;

		if(LinkedList_equals(ob1->observers, ob2->observers))
			return TRUE;
	}
	return FALSE;
}

Bool_t Observable_dtor(void** self) {
	if(self) {
		if(*self) {
			Observable_t* ob = (Observable_t*) *self;
			LinkedList_dtor((void**)&ob->observers);
			free(ob);
			*self = 0;
			return TRUE;
		}
	}	
	return FALSE;
}

Observable_t* Observable_ctor() {
	Observable_t* ob = (Observable_t*) malloc(sizeof(Observable_t));
	*ob = {Object_ctor(ob, ObservableId, Observable_copy, Observable_equals, Observable_dtor), LinkedList_ctor(ObserverId)};
	return ob;
}

#endif