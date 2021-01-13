#ifndef blue_engine_typeinfo
#define blue_engine_typeinfo

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef char Bool_t;

void* TypeInit_initializer(unsigned int size, ...) {
	va_list argv;
	va_start(argv, size);

	char* type = (char*) malloc(size);
	for(unsigned int i = 0; i < size; i++) 
		type[i] = va_arg(argv, char);
	
	va_end(argv);
	return type;
}

void* TypeInit_instanceInitializer(unsigned int size, void* instance, ...) {
	va_list argv;
	va_start(argv, size);
	char* type = (char*) instance;

	for(unsigned int i = 0; i < size; i++) 
		type[i] = va_arg(argv, char);
	
	va_end(argv);
	return type;
}

#define initializer(type, ...) (type*) TypeInit_initializer(sizeof(type), __VA_ARGS__)

#define instance_initializer(type, inst, ...) *(type*) TypeInit_instanceInitializer(sizeof(type), (void*) &inst, __VA_ARGS__)

//---------------------------

#define TYPEINFO_SIZE 0
#define TYPEINFO_NAME 1
#define TYPEINFO_COPY 2
#define TYPEINFO_EQUL 3
#define TYPEINFO_DTOR 4

typedef void*(*CopyFn_t)(void*);
typedef Bool_t(*EqualsFn_t)(void*, void*);
typedef Bool_t(*DtorFn_t)(void**);

typedef struct TypeInfo_t {
	unsigned int	size;
	const char* 	name;
	CopyFn_t 		copy;				// Function pointer copying an instance.
	EqualsFn_t 		equals;				// Function pointer comparing 2 instances.
	DtorFn_t 		dtor;				// Function pointer freeing an instance from memory.
} TypeInfo_t;

inline TypeInfo_t* TypeInfo_ctor(unsigned int size, const char* name, void*(*copy)(void*), Bool_t (*equals)(void*, void*), Bool_t (*dtor)(void**)) {
	return initializer(TypeInfo_t, size, name, copy, equals, dtor);
}
 
//---------------------------

TypeInfo_t* registery[100];
int registerIndex = -1;

Bool_t TypeRegister_getInfo(unsigned int id, char options, void* memory) {
	if(memory) {
		switch(options) {
			case 0: 
				*((unsigned int*) memory) = registery[id]->size;
				break;
			case 1: 
				*((const char**) memory) = registery[id]->name;
				break;
			case 2:
				*((CopyFn_t*) memory) = registery[id]->copy;
				break;
			case 3:
				*((EqualsFn_t*) memory) = registery[id]->equals;
				break;
			case 4:
				*((DtorFn_t*) memory) = registery[id]->dtor;
				break;
			default: 
				return 0;
				break;
		}
		return 1;
	}
	return 0;
}

inline int TypeRegister_register(unsigned int size, const char* name, CopyFn_t copy, EqualsFn_t equals, DtorFn_t dtor) {
	registery[++registerIndex] = TypeInfo_ctor(size, name, copy, equals, dtor);
	return registerIndex;
}

//---------------------------

int int_id = -1;
int float_id = -1;

void TypeRegister_init() {
	
}
#endif