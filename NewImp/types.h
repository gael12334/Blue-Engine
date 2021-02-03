#ifndef blue_engine_types
#define blue_engine_types
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// initialiser functions 

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
	CopyFn_t 	copy;		/// Function pointer copying an instance.
	EqualsFn_t 	equals;		/// Function pointer comparing 2 instances.
	DtorFn_t 	dtor;		/// Function pointer freeing an instance from memory.
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

/// \brief 		Creates a BaseVTable_t pointer.
/// \param copy 	A copying function pointer.
/// \param eq 		A comparaison function pointer.
/// \param dtor 	A memory deallocation function pointer.
/// \return		Returns the adress of allocated BaseVTable_t instance.
void* BaseVTable_createPointer(CopyFn_t copy, EqualsFn_t eq, DtorFn_t dtor) {
	BaseVTable_t* vtable = (BaseVTable_t*) malloc(sizeof(BaseVTable_t));
	*vtable = BaseVTable_ctor(copy, eq, dtor);
	return vtable;
}

/*	=============================
		TypeInfo_t 
	=============================

	Registers at run time types with vtables.
*/

#define MAX_TYPEINFO_STRUCT 100 	/// Defines the maximum amount of typeinfo instance accepted.

typedef struct TypeInfo_t {			
	void* 		vtable;		/// vtable pointer
	unsigned int 	vtable_size;	/// Vtable size.
	unsigned short 	size;		/// Instance size in bytes.
	unsigned int 	super;		/// Super struct id.
	const char* 	name;		/// Type name.
} TypeInfo_t;

/// \brief              Creates a TypeInfo_t pointer for the type registery
/// \param vtable       Non-null pointer containing a vtable
/// \param vtable_size  the non-null pointer type's size
/// \param size         The registering type's size
/// \param super        The type's super class
/// \param name         Null-ending string associated to the type.
/// \return             Returns a non-null pointer of type TypeInfo_t
TypeInfo_t* TypeInfo_ctor(void* vtable, unsigned int vtable_size, unsigned short size, unsigned int super, const char* name) {
	TypeInfo_t* info = (TypeInfo_t*) malloc(sizeof(TypeInfo_t));
	info->vtable = vtable;
	info->name = name;
	info->size = size;
	info->super = super;
	info->vtable_size = vtable_size;
	return info;
}

/*	==============================
		Type register
	==============================

	A static global type register used to store the TypeInfo_t.
*/

static unsigned int registerCount = (unsigned int) -1;  	/// Index incrementor used for the type registery. 
static TypeInfo_t* registery[MAX_TYPEINFO_STRUCT];		/// Registery with MAX_TYPEINFO_STRUCT entries. 

/// \brief              Registers a type into the type registery
/// \param vtable       Non-null pointer containing a vtable
/// \param vtable_size  the non-null pointer type's size
/// \param size         The registering type's size
/// \param super        The type's super class
/// \param name         Null-ending string associated to the type.
/// \return             Returns the registered type's id.
inline const unsigned int TypeRegister_register(void* vtable, unsigned int vtable_size, unsigned short size, unsigned int super, const char* name) {
	registerCount++;
	registery[registerCount] = TypeInfo_ctor(vtable, vtable_size, size, super, name);
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

/// \brief 	Get the type's vtable pointer. 
/// \param id 	The specified type id. 
/// \return 	Returns a void pointer. Do not free!
inline const void* TypeRegister_getVTable(unsigned int id) {
	return registery[id]->vtable;
}

/// \brief 		Get the type's vtable size.
/// \param id 	The specified type id. 
/// \return 	Returns an unsigned int.
inline unsigned int TypeRegister_getVTableSize(unsigned int id) {
	return registery[id]->vtable_size;
}

/// \brief 	Get the type's base vtable.
/// \param id 	The specified type id. 
/// \return 	Returns a const BaseVTable_t pointer. Do not free!
inline const BaseVTable_t* TypeRegister_getBaseVTable(unsigned int id) {
	return (BaseVTable_t*)registery[id]->vtable;
}

/// \brief 	Get the type's super id.
/// \param id 	The specified type id. 
/// \return 	Returns an unsigned int.
inline unsigned int TypeRegister_getSuperId(unsigned int id) {
	return registery[id]->super;
}

/// \brief 	Checks if the specified type id is an inherited type.
/// \param id_a Id to compare. Will check if this id is inherited from id_b.
/// \param id_b Id used to compare.
/// \return 	Returns a Bool_t where TRUE means it is inherited, while FALSE is otherwise.
inline Bool_t TypeRegister_IsInherited(unsigned int id_a, unsigned int id_b) {
	if(id_a == id_b)
		return TRUE;
	
	unsigned int temp = id_b;
	while(registery[temp]->super) {
		if(registery[temp]->super == 0) 
			break;
		if(registery[temp]->super == id_a)
			return TRUE;
		temp = registery[temp]->super;
	}

	return FALSE;
}

/// \brief	Get's the base type id. 
/// \param id	The type's id.
/// \return 	Returns the base type's id.
unsigned int TypeRegister_getRootId(unsigned int id) {
	if(registery[id]->super) {
		unsigned int temp = id;
		unsigned int result = temp;
		while(temp) {
			result = temp;
			temp = registery[id]->super;
		}
		return result;
	}
	return id; 
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

const unsigned int int_id = TypeRegister_register(BaseVTable_createPointer(COPY_BY_VALUE_FUNCTION, EQUALS_BY_VALUE_FUNCTION, DTOR_BY_VALUE_FUNCTION), sizeof(BaseVTable_t), sizeof(long long), 0, "int");

const unsigned int float_id = TypeRegister_register(BaseVTable_createPointer(COPY_BY_VALUE_FUNCTION, EQUALS_BY_VALUE_FUNCTION, DTOR_BY_VALUE_FUNCTION), sizeof(BaseVTable_t), sizeof(double), 0, "float");

#endif
