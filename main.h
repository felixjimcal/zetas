
#include <malloc.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <i86.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <direct.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "cola.h"
#include "events.h"

#define TECLA_1		0x31
#define TECLA_2		0x32
#define TECLA_3		0x33
#define TECLA_4		0x34
#define TECLA_5		0x35
#define TECLA_6		0x36
#define TECLA_7		0x37
#define TECLA_8		0x38
#define TECLA_9		0x39

#define TECLA_Q		0x51
#define TECLA_W		0x57
#define TECLA_E		0x45

#define TECLA_A		0x41
#define TECLA_S		0x53
#define TECLA_D		0x44

#define SIZE_EEPROM	1024
unsigned char *eepromaddress(void);
int getinput(void);
BYTE GetKeyPress(int id);

