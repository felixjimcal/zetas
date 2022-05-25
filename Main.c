// -showmap -map:flashmap.gm -mem:memory.mem

#include "main.h"

static int read_eepron_cnt = 0;
static int read_eepron_size = 0;
static int write_eepron_cnt = 0;
static int write_eepron_size = 0;

void EEPROM_Read( int cnt, int size)
{
	read_eepron_cnt += cnt;
	read_eepron_size += size;
}

void EEPROM_Write( int cnt, int size)
{
	write_eepron_cnt += cnt;
	write_eepron_size += size;
}

BYTE keypress[256];
BYTE keydown[256];

unsigned char EEPROM[SIZE_EEPROM];

unsigned char *eepromaddress(void)
{
	return &EEPROM;
}

int CTXT_argc;
char **CTXT_argv;

char globalpath[256];

//Copiado de la libreria
void GARGS_SetArgs(int argc,char **argv)
{
    CTXT_argc=argc;
    CTXT_argv=argv;
}

//Copiado de la libreria
int GARGS_Find(char *Name)
{
    short x;

    if (CTXT_argc!=1)
    {
        for (x=1;x<CTXT_argc;x++)
        {
            if (!stricmp(CTXT_argv[x],Name))
                return 1;
        }
    }

    return 0;
}

//Copiado de la libreria
char *GARGS_Get(char *Name)
{
    short x;
    char tmp[100];

    strupr(Name);

    if (CTXT_argc!=1)
    {
        for (x=1;x<CTXT_argc;x++)
        {
            strncpy(tmp,CTXT_argv[x],99);
            strupr(tmp);
            if (strstr(tmp,Name))
                return &CTXT_argv[x][strlen(Name)];
        }
    }

    return NULL;
}

//Copiado de la libreria
void GARGS_GetPath(char *dest, char *source)
{
   int pos;

   pos = strlen(source);

   while ((pos>0) &&
   		(source[pos-1] != '\\') &&
   		(source[pos-1] != '/') )
      pos--;

   dest[0] = '\0';
   strncat(dest, source, pos );
}

int getinput(void)
{
	int olddown;
	int i;

	for (i=0;i<256;i++)
	{
		if(keydown[i])
			olddown=1;
		else
			olddown=0;	 //okay to chnage keypress

		keydown[i]=(char) (GetAsyncKeyState(i) >> 8);

		if (!olddown)
			keypress[i]=keydown[i];
		else
			keypress[i]=0;
	}

	return 1;
}

BYTE GetKeyPress(int id)
{
	return keypress[id];
}

void Mysleep(void)
{
	Sleep(10);
}

int main(int argc, char *argv[])
{
	char pointer[64];
	FILE *fichero;
	char tmpstr[64];
	int reseteeprom=0;
	int uselote=0;
	FILE *lote;
	int stoplote=0;
	char loteline[128];
	char *tok;
	int cols[10];
	char coldata[30];
	char *tmpcoldata;
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;
    int ciclodata=-1;
    int ciclo=0;

    // get ticks per second
    QueryPerformanceFrequency(&frequency);

    GARGS_SetArgs(argc,argv);
	setbuf(stdout, NULL);
	GARGS_GetPath( globalpath, argv[0]);
	printf("Problem 3 data...\n");

    if (GARGS_Find("-reset"))
    	reseteeprom=1;

	if (GARGS_Get("-lote:")!=NULL)
	{
        strcpy(tmpstr,GARGS_Get("-lote:"));
		lote = fopen(tmpstr,"rt");
		if (lote)
		{
			uselote=1;
		}
	}

	if (!reseteeprom)
	{
		fichero = fopen("eeprom.bin","rb");
		if (fichero)
		{
			fread( EEPROM, 1, 1024, fichero);
			printf("EEPROM file loaded...\n");
			fclose(fichero);
		}
		else
		{
			memset( EEPROM, 0, 1024 );
		}
	}
	else
	{
		printf("EEPROM Reset...\n");
		memset( EEPROM, 0, 1024 );
	}

	if (uselote)
	{
	    // start timer
	    QueryPerformanceCounter(&t1);
	}
	//Bucle principal, no tocar
	do
	{
		getinput();

		if (uselote)
		{
			if (ciclodata<0)
			{
				if ( fgets(loteline, 128, lote) != NULL )
				{
					tok = strtok(loteline, ",");
					if (tok)
					{
						ciclodata = atoi(tok);
					}
				}
				else
				{
					stoplote = 1;
				}
			}

			while (ciclodata>=0 && ciclodata<=ciclo)
			{
				tok = strtok(NULL, ",");
				cols[0] = atoi(tok);
				switch (cols[0])
				{
					case 0: //Inserta evento
						tok = strtok(NULL, ",");
						cols[1] = atoi(tok);
						tok = strtok(NULL, ",");
						cols[2] = atoi(tok);
						tok = strtok(NULL, ",");
						tmpcoldata = tok;
						tok = strtok(NULL, ",");
						cols[3] = atoi(tok);
						memcpy(coldata, tmpcoldata, cols[3]);
						coldata[ cols[3] ] = 0x00;
						printf("Ciclo: %0004d Insertando evento BATCH TIPO%d SUBTIPO%d...\n",ciclo, cols[1], cols[2] );
						EVENTS_InsertEvent( cols[1], cols[2], coldata, cols[3] );
						break;
					case 1:
						tok = strtok(NULL, ",");
						cols[1] = atoi(tok);
						printf("Ciclo: %0004d Send BATCH TIPO%d \n",ciclo, cols[1] );
						EVENTS_EventProcess(0, cols[1] );
						break;
					case 2:
						tok = strtok(NULL, ",");
						cols[1] = atoi(tok);
						printf("Ciclo: %0004d Ack BATCH TIPO%d \n",ciclo, cols[1] );
						EVENTS_EventProcess(1, cols[1] );
						break;
					default:
						printf("Ciclo: %0004d Orden desconocida...%d...\n",ciclo, cols[0] );
						break;
				}
				ciclodata = -1;
				if ( fgets(loteline, 128, lote) != NULL )
				{
					tok = strtok(loteline, ",");
					if (tok)
					{
						ciclodata = atoi(tok);
					}
				}
				else
				{
					stoplote = 1;
				}
			}
		}
		else
		{
			//Eventos de tipo 1
			if (keypress[TECLA_1])
			{
				printf("Insertando evento TIPO1 SUBTIPO1...\n");
				sprintf( pointer, "DATA1_A");
				EVENTS_InsertEvent( EVENT_TIPO1, 1, pointer, 7 );
			}
			if (keypress[TECLA_2])
			{
				printf("Insertando evento TIPO1 SUBTIPO2...\n");
				sprintf( pointer, "DATA1_B");
				EVENTS_InsertEvent( EVENT_TIPO1, 2, pointer, strlen(pointer) );
			}
			if (keypress[TECLA_3])
			{
				printf("Insertando evento TIPO1 SUBTIPO3...\n");
				sprintf( pointer, "DATA1_C");
				EVENTS_InsertEvent( EVENT_TIPO1, 3, pointer, 7 );
			}

			//Eventos de tipo 2
			if (keypress[TECLA_4])
			{
				printf("Insertando evento TIPO2 SUBTIPO1...\n");
				sprintf( pointer, "DATA2_A");
				EVENTS_InsertEvent( EVENT_TIPO2, 1, pointer, 7 );
			}
			if (keypress[TECLA_5])
			{
				printf("Insertando evento TIPO2 SUBTIPO2...\n");
				sprintf( pointer, "DATA2_B");
				EVENTS_InsertEvent( EVENT_TIPO2, 2, pointer, strlen(pointer) );
			}
			if (keypress[TECLA_6])
			{
				printf("Insertando evento TIPO2 SUBTIPO3...\n");
				sprintf( pointer, "DATA2_C");
				EVENTS_InsertEvent( EVENT_TIPO2, 3, pointer, 7 );
			}

			//Eventos de tipo 3
			if (keypress[TECLA_7])
			{
				printf("Insertando evento TIPO3 SUBTIPO1...\n");
				sprintf( pointer, "DATA3_A");
				EVENTS_InsertEvent( EVENT_TIPO3, 1, pointer, 7 );
			}
			if (keypress[TECLA_8])
			{
				printf("Insertando evento TIPO3 SUBTIPO2...\n");
				sprintf( pointer, "DATA3_B");
				EVENTS_InsertEvent( EVENT_TIPO3, 2, pointer, strlen(pointer) );
			}
			if (keypress[TECLA_9])
			{
				printf("Insertando evento TIPO3 SUBTIPO3...\n");
				sprintf( pointer, "DATA3_C");
				EVENTS_InsertEvent( EVENT_TIPO3, 3, pointer, 7 );
			}

			EVENTS_EventDispatch();

		}
		Mysleep();
		ciclo++;
	} while (!keypress[VK_ESCAPE] && !stoplote);

	if (uselote)
	{
	    // stop timer
	    QueryPerformanceCounter(&t2);

	    // compute and print the elapsed time in millisec
	    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		printf("Bench elapsed time %.03f \n", elapsedTime );
	}
	printf("EEPROM stats...\n");
	printf("       Reads  %03d    Size %d\n",read_eepron_cnt,read_eepron_size );
	printf("       Writes %03d    Size %d\n",write_eepron_cnt,write_eepron_size);

	fichero = fopen("eeprom.bin","wb");
	if (fichero)
	{
		fwrite(EEPROM, 1, 1024, fichero);
		printf("EEPROM file saved...\n");
		fclose(fichero);
	}

	if (lote && uselote)
	{
		fclose(lote);
	}

	return 0;
}

