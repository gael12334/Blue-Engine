#ifndef BLUE_ENGINE_STRING
#define BLUE_ENGINE_STRING

#include "../object.h"
#include <math.h>

typedef struct String_t {
	Object_t base;
	char* data;
	unsigned int length;
} String_t;

const unsigned int StringId = registerType("String", sizeof(String_t));

char String_get(String_t* str, unsigned int i) {
	if(str) {
		if(i < (str->length - 1))
			return str->data[i];
	}
	return 0;
}

inline char String_getf(String_t* str, unsigned int i) {
	return str->data[i];
}

Bool_t String_set(String_t* str, unsigned int i, char c) {
	if(str) {
		if(i < (str->length - 1)) {
			str->data[i] = c;
			return TRUE;
		}
	}
	return FALSE;
}

inline void String_setf(String_t* str, unsigned int i, char c) {
	str->data[i] = c;
}

inline unsigned int String_getLength(String_t* str) {
	if(str) 
		return str->length;

	return 0;
}

String_t* String_ctor(const char* str);
String_t* String_ctorInt(long long integer);
String_t* String_ctorFloat(double floating);

void* String_copy(void* self) {
	if(self) {
		String_t* str = (String_t*) self;
		String_t* copy = String_ctor(str->data);
		return copy;
	}

	return 0;
}

Bool_t String_equals(void* self, void* ref) {
	if(self && ref) {
		String_t* str1 = (String_t*) self;
		String_t* str2 = (String_t*) ref;
		if(str1->length == str2->length) {
			for(unsigned int i = 0; i < str1->length; i++) {
				if(str1->data[i] != str2->data[i])
					return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

Bool_t String_dtor(void** self) {
	if(self) {
		if(*self) {
			String_t* str = (String_t*) *self;
			free(str->data);
			free(str);
			*self = 0;
			return TRUE;
		}
	}
	return FALSE;
}

String_t* String_ctor(const char* str) {
	if(str) { 
		unsigned int i = 0; 
		while(str[i++]);
		i++; // for null character

		char* arr = (char*) malloc(sizeof(char) * i);
		for(unsigned int j = 0; j < i; j++) 
			arr[j] = str[j];

		String_t* str = (String_t*) malloc(sizeof(String_t));
		*str = {
			Object_ctor(str, StringId, String_copy, String_equals, String_dtor), 
			arr,
			i
		};
	}

	return 0; 
}

String_t* String_ctorInt(long long value) {
	double buffer1 = (double)value;
	long buffer2 = value;
	unsigned int decimal = 0;

	while(buffer2 != 0) {
		buffer1 /= 10;
		buffer2 = (long)buffer1;
		decimal++;
	}

	String_t* str = (String_t*) malloc(sizeof(String_t));
	str->data = (char*) malloc(sizeof(char) * (decimal + 1));
	str->length = decimal;

	buffer1 = ((double)value) / pow(10, decimal);
	buffer2 = 0;

	for(unsigned int i = 0; i < decimal; i++) {
		buffer1 *= 10;
		char c = (char)buffer1;
		buffer1 -= c;
		c += 48;

		str->data[i] = c;
	}

	str->data[decimal] = 0;
	
	return str;
}

String_t* String_ctorFloat(double value, unsigned char precision) {
	String_t* integer = String_ctorInt((long)value);
	double rest = value - ((long)value);
	char* restarr = (char*) malloc(sizeof(char) * (precision + 1)); // +1 for the NULL character.

	for(unsigned int i = 0; i < precision; i++) {
		rest *= 10;
		char c = (char)rest;
		rest -= c;
		c += 48;

		restarr[i] = c;
	}

	char* result = (char*) malloc(sizeof(char*) * (integer->length + precision + 2)); // +2 for NULL and '.'.

	unsigned int index = 0;
	while(index < integer->length) {
		result[index] = integer->data[index];
		index++;
	}
	
	result[index] = '.';
	index++;

	unsigned int bound = index + precision;
	while(index < bound) {
		result[index] = restarr[index - precision - 1];
		index++;
	}

	result[index] = 0;

	free(restarr);
	free(integer->data);

	integer->data = result;
	integer->length = index + 1;
	return integer;
}

//
// 
typedef unsigned long long String8_t;

const unsigned int String8Id = registerType("String8", sizeof(String8_t));

inline String8_t String8_ctor(const char str[9]) {
	String8_t sstr = *(unsigned long long*) &str[0];
	return sstr;
}

char String8_get(String8_t str, unsigned char i) {
	if(i < 8) {
		char* cstr = (char*) &str;
		return cstr[i];
	}
	return 0;
}

Bool_t String8_set(String8_t str, unsigned char i, char c) {
	if(i < 8) {
		char* cstr = (char*) &str;
		cstr[i] = c;
		return TRUE;
	}
	return FALSE;
}

inline Object_t String8_toObject(String8_t p) {
	Object_t obj = Object_ctorInt(p);
	obj.id = String8Id;
	return obj;
}

inline String8_t String8_fromObject(Object_t o) {
	return Object_toInt(&o);
}


/*
// +-- structure of the String_t type --+
typedef struct String_t { 
	char* data;
	unsigned int length;
} String_t;

const unsigned int StringId = registerType("String", sizeof(String_t));

// +-- Allocates and creates an instance for a String_t pointer --+
String_t* CreateString(const char* cstr) {

	return 0;
}

// +-- Converts a integer into a character array --+ 
String_t* CreateString(long value) {
	double buffer1 = (double)value;
	long buffer2 = value;
	unsigned int decimal = 0;

	while(buffer2 != 0) {
		buffer1 /= 10;
		buffer2 = (long)buffer1;
		decimal++;
	}

	String_t* str = (String_t*) 
	str->data = malloc_f(char, decimal + 1);
	str->length = decimal;

	buffer1 = ((double)value) / pow(10, decimal);
	buffer2 = 0;

	for(unsigned int i = 0; i < decimal; i++) {
		buffer1 *= 10;
		char c = (char)buffer1;
		buffer1 -= c;
		c += 48;

		str->data[i] = c;
	}

	str->data[decimal] = 0;
	
	return str;
}

// +-- Converts a floating point number into a character array --+
String_t* CreateString(double value, byte precision) {
	String_t* integer = CreateString((long)value);
	double rest = value - ((long)value);
	char* restarr = malloc_f(char, precision + 1); // +1 for the NULL character.

	for(unsigned int i = 0; i < precision; i++) {
		rest *= 10;
		char c = (char)rest;
		rest -= c;
		c += 48;

		restarr[i] = c;
	}

	char* result = malloc_f(char, integer->length + precision + 2); // +2 for NULL and '.'.

	unsigned int index = 0;
	while(index < integer->length) {
		result[index] = integer->data[index];
		index++;
	}
	
	result[index] = '.';
	index++;

	unsigned int bound = index + precision;
	while(index < bound) {
		result[index] = restarr[index - precision - 1];
		index++;
	}

	result[index] = 0;

	free_f(restarr);
	free_f(integer->data);

	integer->data = result;
	integer->length = index + 1;
	return integer;
}

// +-- Gets a character within bounds of allocated memory for a String_t pointer --+
char GetStringChar(String_t* str, unsigned int index) {
	if(str) {
		if(index < str->length)
			return str->data[index];
	}
	return 0;
}

// +-- Sets a character within bounds of allocated memory for a String_t pointer --+
Bool_t SetStringChar(String_t* str, char c, unsigned int index) {
	if(str) {
		if(index < str->length) {
			str->data[index] = c;
			return TRUE;
		}
	}
	return FALSE;
}

// +-- Compares 2 String_t and return whether the 2 strings are the same (TRUE) or not (FALSE) --+
Bool_t CompareString(String_t* str1, String_t* str2) {
	if(str1 && str2) {
		if(str1->length == str2->length) {
			for(unsigned int i = 0; i < str2->length; i++) {
				if(str1->data[i] != str2->data[i])
					return FALSE;
			}
		}
		return TRUE;
	}

	return FALSE;
}

// +-- Compares 2 String_t and return whether the 2 strings are the same (TRUE) or not (FALSE) --+
Bool_t CompareString(String_t* str1, const char* str2) {
	String_t* temp = CreateString(str2);
	Bool_t result = CompareString(str1, temp);
	FreeString(temp);
	return result;
}

// +-- Counts the number of occurence a character has in a String_t pointer --+
unsigned int ContainsStringChar(String_t* str, char c) {
	if(str) {
		if(!str->data)
			return 0;
		unsigned int count = 0;
		for(unsigned int i = 0; i < str->length; i++) {
			if(str->data[i] == c)
				count++;
		}
		return count;
	}
	return 0;
}

// +-- Frees a non-null String_t pointer --+ 
Bool_t FreeString(String_t* str) {
	if(str) {
		if(str->data)
			free_f(str->data);
		free_f(str);
		return TRUE;
	}
	return FALSE;
}

// +-- Joins a character array to a String_t pointer --+
Bool_t JoinString(String_t* dest, const char* src) {
	if(dest && src) {
		if(!dest->data)
			return FALSE;
		String_t* srcStr = CreateString(src);
		unsigned int totalLen = dest->length + srcStr->length;
		char* arr = malloc_f(char, totalLen);
		unsigned int index = 0;
		while(index < dest->length) {
			arr[index] = dest->data[index];
			index++;
		}

		unsigned int bound = index + srcStr->length;
		while(index < bound) {
			arr[index - 1] = srcStr->data[index - dest->length];
			index++;
		}

		free_f(dest->data);
		dest->data = arr;
		dest->length = totalLen;
		return TRUE;
	}
	return FALSE;
}

// +-- Joins a character to a String_t pointer --+
Bool_t JoinString(String_t* dest, char c) {
	if(dest) {
		char temp[2] = {c, 0};
		return JoinString(dest, (const char*) &temp);
	}
	return FALSE;
}

// +-- Joins a String_t pointer to a String_t pointer --+
Bool_t JoinString(String_t* dest, String_t* src) {
	if(dest && src) 
		return JoinString(dest, src->data);
	return FALSE;
}

// +-- Joins an integer to a String_t pointer --+
Bool_t JoinString(String_t* dest, long src) {
	if(dest) {
		if(!dest->data)
			return FALSE;
		String_t* value = CreateString(src);
		Bool_t result = JoinString(dest, src);
		FreeString(value);
		return result;
	}

	return FALSE;
}

// +-- Joins a floating point number to a String_t pointer --+
Bool_t JoinString(String_t* dest, double src, unsigned int precision) {
	if(dest) {
		if(!dest->data)
			return FALSE;
		String_t* value = CreateString(src);
		Bool_t result = JoinString(dest, src, precision);
		FreeString(value);
		return result;
	}

	return FALSE;
}

// +-- Splits a string into an array of mutiple string ending with a NULL pointer --+
String_t** SplitString(String_t* str, char split) {
	if(str) {
		if(!str->data) 
			return 0;
		
		if(unsigned int count = ContainsStringChar(str, split)) {
			count++;
			String_t** arr = malloc_f(String_t*, count + 1); //+1 to indicate the end-of-array.
			unsigned int index = 0;
			for(unsigned int i = 0; i < count; i++) {
				arr[i] = malloc_f(String_t, 1);
				arr[i]->data = malloc_f(char, str->length);
				unsigned int j = 0;

				while((str->data[index] != split) && (str->data[index] != 0)) {
					arr[i]->data[j] = str->data[index];
					index++;
					j++;
				}
				arr[i]->data[j] = 0;
				index++;

				unsigned int length = strLength_f(arr[i]->data);
				char* result = (char*)copy_f(arr[i]->data, length);
				free_f(arr[i]->data);
				arr[i]->data = result;
				arr[i]->length = length;
			}

			arr[count] = nullptr;
			return arr;
		}
	}
	return 0;
}

// +-- Frees an array of String_t pointers ending with a NULL pointer --+ 
Bool_t FreeStringArray(String_t** arr) {
	if(arr) {
		unsigned int i = 0;
		while(arr[i]) {
			FreeString(arr[i]);
			i++;
		}
		free_f(arr);
		return TRUE;
	}
	return FALSE;
}

// +-- Gets the String_t pointer array length's --+
unsigned int GetStringArrayLength(String_t** arr) {
	if(arr) {
		unsigned int i = 0; 
		while(arr[i++]);
		i++;
		return i;
	}
	return 0;
}

// +-- Gets the string length, in bytes --+
inline unsigned int GetStringLength(String_t* str) {
	if(str) 
		return str->length;
	return 0;
}
*/

#endif
