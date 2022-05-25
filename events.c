
#include "main.h"

int idtransacciones[3];

//Devuelve 1 en error, 0 correcto
int EVENTS_InsertEvent( int tipo, int subtipo, void *pointer, int size)
{
	if( COLA_InsertElement( tipo, subtipo, pointer, size))
		return 1;
	else
		return 0;
}


//Devuelve numtransaccion 1 a fffff..., 0 no se envian datos
int EVENTS_SendData( int tipo )
{
	int id;
	int i;
	int count;
	char num_elems;
	char size_elems[20];
	char data[256];
	char aux[64];

	id = COLA_GetTransaction(tipo, &data, &size_elems, &num_elems);

	if (id)
	{
		printf("GET: Num elements: %d\n", num_elems);
		count=0;
		for( i=0; i<num_elems; i++)
		{
			memcpy( aux, &(data[count]), size_elems[i] );
			aux[size_elems[i]]=0x00;
			count += size_elems[i];
			printf("------> Size %d %s\n", size_elems[i], aux );
		}
	}

	return id;
}


//Devuelve 1 en error, 0 correcto
int EVENTS_AckData( int idtransaccion )
{
	if (!idtransaccion)
		return 1;

	return COLA_EndTransaction (idtransaccion);
}




void EVENTS_EventProcess(int event, int tipo )
{
	int aux;
	char pointer[64];

	if (!event)
	{
		aux = EVENTS_SendData( tipo);
		if (!aux)
		{
			printf("No data to send TIPO%d...\n", tipo);
		}
		else
		{
			idtransacciones[tipo] = aux;
		}
	}
	else
	{
		if (EVENTS_AckData( idtransacciones[tipo] ) )
		{
			printf("Error acking, TIPO%d id %d...\n", tipo, idtransacciones[EVENT_TIPO1] );
			sprintf( pointer, "ERROR_ACK_%d", tipo);
			EVENTS_InsertEvent( tipo, 1, pointer, strlen(pointer) );
		}
	}
}

//No cambiar nada en esta funcion
void EVENTS_EventDispatch(void)
{
	if (GetKeyPress(TECLA_Q))
	{
		EVENTS_EventProcess( 0, EVENT_TIPO1);
	}
	if (GetKeyPress(TECLA_W))
	{
		EVENTS_EventProcess( 0, EVENT_TIPO2);
	}
	if (GetKeyPress(TECLA_E))
	{
		EVENTS_EventProcess( 0, EVENT_TIPO3);
	}

	if (GetKeyPress(TECLA_A))
	{
		EVENTS_EventProcess( 1, EVENT_TIPO1);
	}
	if (GetKeyPress(TECLA_S))
	{
		EVENTS_EventProcess( 1, EVENT_TIPO2);
	}
	if (GetKeyPress(TECLA_D))
	{
		EVENTS_EventProcess( 1, EVENT_TIPO3);
	}
}
