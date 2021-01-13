#ifndef BLUE_ENGINE_TYPEINFO
#define BLUE_ENGINE_TYPEINFO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define local static

// initialiser 
void* TypeInitializer_init(unsigned int size, ...) {
	char* data = (char*) malloc(size);
	va_list argv;
	va_start(argv, size);
	for(unsigned int i = 0; i < size; i++)
		data[i] = va_arg(argv, char);
	va_end(argv);
	return data;
}

void* TypeInitializer_instInit(void* adress, unsigned int size, ...) {
	char* data = (char*) adress;
	va_list argv;
	va_start(argv, size);
	for(unsigned int i = 0; i < size; i++)
		data[i] = va_arg(argv, char);
	va_end(argv);
	return data;
}

#define initializer(type, ...) TypeInitializer_init(sizeof(##type), __VA_ARGS__)

#define inst_initializer(type, inst, ...) TypeInitializer_init(&inst, sizeof(##type), __VA_ARGS__)

// value to increment each time a type is registered.
static int typeCount = 0;

/// @brief Structure handling the type registration.
typedef struct {
	const char* 	typeName;
	unsigned int 	instSize;
} TypeInfo_t;

local TypeInfo_t* CreateTypeInfo(const char* name, unsigned int size) {
	TypeInfo_t* ti = (TypeInfo_t*) malloc(sizeof(TypeInfo_t));
	ti->typeName = name;
	ti->instSize = size;
	return ti;
}

// register declaration.
static TypeInfo_t** typeRegister = (TypeInfo_t**) malloc(sizeof(TypeInfo_t*) * 100);

/// @brief Registers a type. Note: It is impossible to de-register a type.
/// @param name Type name.
/// @param instSize Size of an instance (in bytes) of the said type.
/// @return The function returns the id of the registered type.
unsigned int registerType(const char* name, unsigned int instSize) {
	typeRegister[typeCount] = CreateTypeInfo(name, instSize);
	typeCount++;
	return typeCount;
}

/// @brief Seeks a type's id 
/// @param name Type name.
/// @return Returns the id of the required type.
unsigned int seekType(const char* name) {
	for(unsigned int i = 0; i < typeCount; i++) {
		if(strcmp(name, typeRegister[i]->typeName) == 0) 
			return i;
	}
	return 0;
}

/// @brief Gets the type's size.
/// @param index A valid id.
/// @return Returns the type's instance size.
unsigned int getTypeSize(unsigned int index) {
	if(index < typeCount)
		return typeRegister[index]->instSize;
	return 0;
}

/// @brief Get the type's name.
/// @param index A valid id.
/// @return Returns the type's name.
const char* getTypeName(unsigned int index) {
	if(index < typeCount)
		return typeRegister[index]->typeName;
	return 0;
}

// Primitive type id for char, short, int and long. 
const unsigned intId = registerType("int", sizeof(long));

// Primitive type id for float and double.
const unsigned floatId = registerType("float", sizeof(double));

#endif