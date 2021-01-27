#ifndef blue_engine_string
#define blue_engine_string

#include "stringint.h"
#include <string.h> // C standard librairy

typedef struct String_t {
	Object_t 		base; 
	char* 			native;
	unsigned int 	count;
} String_t; 

void* String_copy(void* self) {
	String_t* self_str = (String_t*) self; 
	String_t* copy_str = (String_t*) malloc(sizeof(String_t));
	copy_str->native = (char*) malloc(sizeof(self_str->count));
	copy_str->count = self_str->count;
	copy_str->base = Object_ctor(copy_str, self_str->base.id);
	
	for(unsigned int i = 0; i < self_str->count; i++) 
		copy_str->native[i] = self_str->native[i];

	return copy_str;
} 

Bool_t String_equals(void* self, void* ref) {
	String_t* self_str = (String_t*) self;
	String_t* ref_str = (String_t*) ref;
	
	if(self_str->count == ref_str->count) {
		for(unsigned int i = 0; i < self_str->count; i++) {
			if(self_str->native[i] != ref_str->native[i])
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

Bool_t String_dtor(void** self) {
	if(self) {
		if(*self) {
			String_t* str = (String_t*) *self;
			free(str->native);
			free(str);
			*self = 0;
			return TRUE;
		}
	}
	
	return FALSE;
}

// autorun 
unsigned int String_id =
	TypeRegister_register(
		BaseVTable_createPointer(
			String_copy, 
			String_equals, 
			String_dtor
		), 
		sizeof(BaseVTable_t),
		sizeof(String_t), 
		0, 
		"String_t"
	)
;

String_t* String_ctor(const char text[]) {
	unsigned int length = 0;
	while(text[length++]);
	length++;
	
	String_t* str = (String_t*) malloc(sizeof(String_t));
	str->base = Object_ctor(str, String_id);
	str->count = length;
	str->native = (char*) malloc(length);

	for(unsigned int i = 0; i < length; i++)
		str->native[i] = text[i];
	
	return str;
}

String_t* String_stringInt(StringInt_t str) {
	char* native = (char*) &str;
	for(int i = 0; (i < sizeof(StringInt_t) && (native[i] != '\0')); i++) 
		;
	return 0;
}

String_t* String_int(long long integer) {
	char* native = (char*) malloc(21);
	
	return 0;
	
}

String_t* String_uint(unsigned long long integer) {

	return 0;
}

String_t* String_float(double floating, unsigned int precision) {

	return 0;
}




#endif