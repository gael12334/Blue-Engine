#ifndef blue_engine_observer
#define blue_engine_observer

#include "./observer-vtable.h"

// = = = = = = = = =
typedef struct Observer_t {
	Object_t base;
	ObserverVTable_t* vtable;
} Observer_t;

/// \brief Generic function calling the sub-classes notification functions.
/// \param self Pointer pointing to an Obserser.
inline void Observer_notification(void* self) {
	Observer_t* ob = (Observer_t*) self;
	if(TypeRegister_getVTableSize(ob->base.id) >= sizeof(ObserverVTable_t)) { 
		ObserverVTable_t* vtable = (ObserverVTable_t*) TypeRegister_getVTable(ob->base.id);
		vtable->notification(self);
	}
}

// autorun on start.
const unsigned int Observer_id = 
	TypeRegister_register(
		ObserverVTable_createPointer(
			Object_copy,
			Object_equals, 
			Object_dtor, 
			Observer_notification
		), 
		sizeof(ObserverVTable_t),
		sizeof(Observer_t), 
		0,
		"Observer_t"
	)
;

/// \brief Observer constructor<
/// \param self Self pointing instance.
/// \param id Id of self pointer type.
/// \return Returns an observer_t base instance.
inline Observer_t Observer_ctor(void* self, unsigned int id) {
	Observer_t observer;
	observer.base = Object_ctor(self, id);
	return observer;
}

// = = = = = = = = =

typedef struct Observable_t {
	LinkedList_t* subscriptions;
} Observable_t;

void* Observable_copy(void* self) {
	Observable_t* ob = (Observable_t*) self;
	Observable_t* copy = (Observable_t*) malloc(sizeof(Observable_t));
	copy->subscriptions = (LinkedList_t*) LinkedList_copy(ob->subscriptions);
	return copy;
}

Bool_t Observable_equals(void* self, void* ref) {
	Observable_t* ob = (Observable_t*) self;
	Observable_t* obref = (Observable_t*) ref;
	return LinkedList_equals(ob->subscriptions, obref->subscriptions);
}

Bool_t Observable_dtor(void** self) {
	if(self) {
		if(*self) {
			Observable_t* ob = (Observable_t*) *self;
			LinkedList_dtor((void**)&ob->subscriptions);
			free(ob);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

// autorun on start.
const unsigned int Observable_id = 
	TypeRegister_register(
		BaseVTable_createPointer(
			Observable_copy, 
			Observable_equals, 
			Observable_dtor
		), 
		sizeof(BaseVTable_t), 
		sizeof(Observable_t), 
		0,
		"Observable_t"
	)
;

inline void Observable_subscribe(Observable_t* self, void* observer) {
	Observer_t* observerPtr = (Observer_t*) observer;
	LinkedList_pushback(self->subscriptions, observerPtr->base);
}

Bool_t Observable_unsubscribe(Observable_t* self, void* observer) {
	Observer_t* observerPtr = (Observer_t*) observer;
	for(DLIterator_t it = LinkedList_begin(self->subscriptions); DLIterator_ended(&it); DLIterator_next(&it)) {
		if(DLIterator_get(&it)->self == observer) {
			LinkedList_remove(self->subscriptions, &it);
			return TRUE;
		}
	}
	return FALSE;
}

void Observable_notify(Observable_t* self) {
	for(DLIterator_t it = LinkedList_begin(self->subscriptions); DLIterator_ended(&it); DLIterator_next(&it)) {
		const Object_t* obj = DLIterator_get(&it);
		Observer_t* observer = (Observer_t*) obj->self;
		Observer_notification(observer);
	}
}

#endif