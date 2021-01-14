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

typedef struct BaseVTable_t {		
	CopyFn_t 	copy;				/// Function pointer copying an instance.
	EqualsFn_t 	equals;				/// Function pointer comparing 2 instances.
	DtorFn_t 	dtor;				/// Function pointer freeing an instance from memory.
} BaseVTable_t;

/// \brief 		Initialises a BaseVTable instance.
/// \param copy 	A CopyFn_t function pointer for flexible copying.
/// \param eq 		An EqualsFn_t function pointer for flexible comparaison.
/// \param dtor 	A DtorFn_t function pointer for flexible deallocation.
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

#define MAX_TYPEINFO_STRUCT 100 	/// Defines the maximum amount of typeinfo instance accepted.

typedef struct TypeInfo_t {			
	BaseVTable_t 	base_vtable;		/// Predefined sets of function every registered type must provide.
	unsigned short 	size;			/// Instance size in bytes.
	unsigned int 	super;			/// Super struct id.
	const char* 	name;			/// Type name.
	void* 			custom_vtable;	/// User-defined custom vtable with custom function pointers.
} TypeInfo_t;

/// \brief 				Initialises a TypeInfo_t instance.
/// \param copy 			A CopyFn_t function pointer for flexible copying.
/// \param eq 				An EqualsFn_t function pointer for flexible comparaison.
/// \param dtor 			A DtorFn_t function pointer for flexible deallocation.
/// \param size 			The type's physical size. (Exemple: sizeof(int))
/// \param super			The superclass-like structure, if the type is inherited.
/// \param name 			The name of the type.
/// \param custom_vtable 		Custom Vtable pointer.
TypeInfo_t* TypeInfo_ctor(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor, unsigned short size, unsigned int super, const char* name, void* custom_vtable) {
	TypeInfo_t* info = (TypeInfo_t*) malloc(sizeof(TypeInfo_t));
	info->base_vtable = BaseVTable_ctor(copy, eq, dtor);
	info->custom_vtable = custom_vtable;
	info->name = name;
	info->size = size;
	info->super = super;
	return info;
}

/*	
	==============================
		Type register
	==============================

	A static global type register used to store the TypeInfo_t.
*/

static unsigned int registerCount = (unsigned int) -1;  /// Index incrementor used for the type registery. 
static TypeInfo_t* registery[MAX_TYPEINFO_STRUCT];	/// Registery with MAX_TYPEINFO_STRUCT entries. 

/// \brief Registers a type into the registery. 
/// \param copy 			A CopyFn_t function pointer for flexible copying.
/// \param eq 				An EqualsFn_t function pointer for flexible comparaison.
/// \param dtor 			A DtorFn_t function pointer for flexible deallocation.
/// \param size 			The type's physical size. (Exemple: sizeof(int))
/// \param super			The superclass like structure, if the type is inherited.
/// \param name 			The name of the type.
/// \param custom_vtable 		Custom Vtable pointer.
/// \return 				Returns the id of the registered type.
inline const unsigned int TypeRegister_register(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor, unsigned short size, unsigned int super, const char* name, void* custom_vtable) {
	registerCount++;
	registery[registerCount] = TypeInfo_ctor(copy, eq, dtor, size, super, name, custom_vtable);
	return registerCount;
}

/// \brief 	Get the type's physical size.
/// \param id 	The specified type id. 
/// \return 	Returns an unsigned int 
inline unsigned short TypeRegister_getSize(unsigned int id) {
	return registery[id]->size;
}

/// \brief 	Get the type's name.
/// \param id 	The specified type id. 
/// \return 	Returns a const C null-terminating string. 
inline const char* TypeRegister_getName(unsigned int id) {
	return registery[id]->name;
}

/// \brief 	Get the type's custom vtable pointer. 
/// \param id 	The specified type id. 
/// \return 	Returns a void pointer.
inline const void* TypeRegister_getCustomVTable(unsigned int id) {
	return registery[id]->custom_vtable;
}

/// \brief 	Get the type's base vtable.
/// \param id 	The specified type id. 
/// \return 	Returns a const BaseVTable_t pointer. Do not free!
inline const BaseVTable_t* TypeRegister_getVTable(unsigned int id) {
	return &registery[id]->base_vtable;
}

/// \brief 	Get the type's super id.
/// \param id 	The specified type id. 
/// \return 	Returns an unsigned int.
inline unsigned int TypeRegister_getSuperId(unsigned int id) {
	return registery[id]->super;
}

/*
	==============================
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
