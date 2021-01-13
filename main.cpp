#include "NewImp/window/resources/resource-manager.h"



int main(int argc, char** argv) {
	Surface_t* s = Surface_ctor("poop.png");
	StringInt_t str = StringInt_ctor("poop");
	ResourceManager_add(str, s);

	StringInt_t name1 = StringInt_ctor("A");
	StringInt_t name2 = StringInt_ctor("B");
	HashTable_t* hashtable = HashTable_ctor(int_id, int_id); 
	HashTable_set(hashtable, StringInt_converToObject(name1), Object_int(1));
	HashTable_set(hashtable, StringInt_converToObject(name2), Object_int(2));

	getchar();
	getchar();
	return 0;
}