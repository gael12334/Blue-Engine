#ifndef blue_engine_types
#define blue_engine_types
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// initialiser 

struct TypeInitializer_Char { char raw; };

void* TypeInitializer_init(unsigned int size, ...) {
	char* data = (char*) malloc(size);
	va_list argv;
	va_start(argv, size);
	for(unsigned int i = 0; i < size; i++)
		data[i] = va_arg(argv, struct TypeInitializer_Char).raw;
	va_end(argv);
	return data;
}

void* TypeInitializer_instInit(void* adress, unsigned int size, ...) {
	char* data = (char*) adress;
	va_list argv;
	va_start(argv, size);
	for(unsigned int i = 0; i < size; i++)
		data[i] = va_arg(argv, struct TypeInitializer_Char).raw;
	va_end(argv);
	return data;
}

#define initializer(type, ...) (type*) TypeInitializer_init(sizeof(type), __VA_ARGS__)

#define inst_initializer(type, inst, ...) TypeInitializer_instInit(&inst, sizeof(type), __VA_ARGS__)

/*	============================
		Booleans
	============================

	Booleans.
*/

typedef unsigned char Bool_t;

#define FALSE 0	
#define TRUE 1

/*	============================
		Types and V-tables
	============================

	Type Info structures will contain a unique vtable instance for 
	virtual function calls.
*/

typedef void*(*CopyFn_t)(void*);
typedef Bool_t(*EqualsFn_t)(void*, void*);
typedef Bool_t(*DtorFn_t)(void**);

typedef struct BaseVTable_t {		// 24 bytes.
	CopyFn_t 	copy;				// Function pointer copying an instance.
	EqualsFn_t 	equals;				// Function pointer comparing 2 instances.
	DtorFn_t 	dtor;				// Function pointer freeing an instance from memory.
} BaseVTable_t;

BaseVTable_t BaseVTable_ctor(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor) {
	BaseVTable_t vt;
	vt.copy = copy;
	vt.equals = eq;
	vt.dtor = dtor;
	return vt;
}

/*	=============================
		TypeInfo_t 
	=============================

	Registers at run time types with vtables.
*/

#define MAX_TYPEINFO_STRUCT 100

typedef struct TypeInfo_t {			// 46 bytes.
	BaseVTable_t 	base_vtable;	// Predefined sets of function every registered type must provide.
	unsigned short 	size;			// Instance size in bytes.
	unsigned int 	super;			// Super struct id.
	const char* 	name;			// Type name.
	void* 			custom_vtable;	// User-defined custom vtable with custom function pointers.
} TypeInfo_t;

TypeInfo_t* TypeInfo_ctor(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor, unsigned short size, unsigned int super_id, const char* name, void* custom_vtable) {
	TypeInfo_t* info = (TypeInfo_t*) malloc(sizeof(TypeInfo_t));
	info->base_vtable = BaseVTable_ctor(copy, eq, dtor);
	info->custom_vtable = custom_vtable;
	info->name = name;
	info->size = size;
	info->super = super_id;
	return info;
}

/*	==============================
		Type register
	==============================

	A static global type register used to store the TypeInfo_t.
*/

static unsigned int registerCount = (unsigned int) -1;
static TypeInfo_t* registery[MAX_TYPEINFO_STRUCT];

inline const unsigned int TypeRegister_register(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor, unsigned short size, unsigned int super_id, const char* name, void* custom_vtable) {
	registerCount++;
	registery[registerCount] = TypeInfo_ctor(copy, eq, dtor, size, super_id, name, custom_vtable);
	return registerCount;
}

inline unsigned short TypeRegister_getSize(unsigned int id) {
	return registery[id]->size;
}

inline const char* TypeRegister_getName(unsigned int id) {
	return registery[id]->name;
}

inline const void* TypeRegister_getCustomVTable(unsigned int id) {
	return registery[id]->custom_vtable;
}

inline const BaseVTable_t* TypeRegister_getVTable(unsigned int id) {
	return &registery[id]->base_vtable;
}

inline unsigned int TypeRegister_getSuperId(unsigned int id) {
	return registery[id]->super;
}

/*	==============================
		Primitive type id's
	==============================

	A predefined set of id's for primitive types.
*/

#define COPY_BY_VALUE_FUNCTION Generic_copyByValue
inline void* COPY_BY_VALUE_FUNCTION(void* self) {
	return self;
}

#define EQUALS_BY_VALUE_FUNCTION Generic_equalsByValue
inline Bool_t EQUALS_BY_VALUE_FUNCTION(void* self, void* inst) {
	return self == inst;
}

#define DTOR_BY_VALUE_FUNCTION Generic_dtorByValue
inline Bool_t DTOR_BY_VALUE_FUNCTION(void** self) {
	return TRUE;
}

const unsigned int int_id = TypeRegister_register(COPY_BY_VALUE_FUNCTION, EQUALS_BY_VALUE_FUNCTION, DTOR_BY_VALUE_FUNCTION, sizeof(long long), 0, "int", 0);

const unsigned int float_id = TypeRegister_register(COPY_BY_VALUE_FUNCTION, EQUALS_BY_VALUE_FUNCTION, DTOR_BY_VALUE_FUNCTION, sizeof(double), 0, "float", 0);

#endif