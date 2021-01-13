#ifndef blue_engine_stringint
#define blue_engine_stringint

#include "../hashtable/hashtable.h"

typedef struct RawStringInt_t {
	char raw[sizeof(long long)];
} RawStringInt_t;

typedef union StringInt_t {
	long long emulated;
	RawStringInt_t raw;
} StringInt_t;

inline StringInt_t StringInt_ctor(const char raw[sizeof(long long)]) {
	StringInt_t str;
	str.emulated = 0;
	for(unsigned int i = 0; (raw[i] != 0) && (i < sizeof(long long)); i++)
		str.raw.raw[i] = raw[i];
	return str;
}

inline StringInt_t StringInt_fromInt(long long integer) {
	StringInt_t str;
	str.emulated = integer;
	return str;
}

inline Object_t StringInt_converToObject(StringInt_t str) {
	Object_t obj = Object_int(str.emulated);
	return obj;
}

inline StringInt_t StringInt_fromObject(Object_t obj) {
	StringInt_t str;
	str.emulated = (long long) obj.self;
	return str;
}

#define StringInt_equals(str1, str2) ((##str1).emulated == (##str2).emulated)

#define StringInt_get(str, index) str.raw[index];

#define StringInt_set(str, index, c) str.raw[index] = c

#endif