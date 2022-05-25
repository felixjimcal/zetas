
//Devuelve 1 en error, 0 correcto
int COLA_InsertElement(int tipo, int subtipo, void *pointer, int size);

//Devuelve numtransaccion 1 a fffff..., 0 no se envian datos
int COLA_GetTransaction(int tipo, char *data, char *size_elems, char *size);

//Devuelve 1 en error, 0 correcto
int COLA_EndTransaction(int idtransaccion);

void setDirty(unsigned char *value);
char isDirty(char value);
unsigned char getValor(unsigned char value);
