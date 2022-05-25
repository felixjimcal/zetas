#include "main.h"

char isDirty(char value);

//Devuelve 1 en error, 0 correcto
int COLA_InsertElement(int tipo, int subtipo, void *pointer, int size){return 0;};

//Devuelve numtransaccion 1 a fffff..., 0 no se envian datos
int COLA_GetTransaction(int tipo, char *data, char *size_elems, char *size)
{
	unsigned char test;
	unsigned char test2;
	strcpy( data, "TEST" );
	size_elems[0]=4;
	*size=1;

	test=13;

	isDirty(test);



	return 1;
}

//Devuelve 1 en error, 0 correcto
int COLA_EndTransaction(int idtransaccion){return 0;};



char isDirty(char value)
{
	int a;

	return 1;//(value & 0x80);
}
