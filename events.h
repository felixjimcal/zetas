

#define	EVENT_TIPO1		0
#define	EVENT_TIPO2		1
#define	EVENT_TIPO3		2

int EVENTS_InsertEvent( int tipo, int subtipo, void *pointer, int size);
int EVENTS_SendData( int tipo );
int EVENTS_AckData( int idtransaccion );

void EVENTS_EventProcess(int event, int tipo );
void EVENTS_EventDispatch(void);

