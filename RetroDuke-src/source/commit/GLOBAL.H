#ifndef global_public
#define global_public

#define TRUE  1
#define FALSE 0
#define EOS '\0'
#define ESC 0x1B

#define CLOCK_FREQUENCY (1843200L)		/* 1.8432 Mhz */

#define INPUT( port )        inp( port )
#define OUTPUT( port, data ) (void) outp( port, data )
#define CLI()                disable()
#define STI()                enable()

typedef enum {false, true} boolean;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long LONG;


void  Error (char *error, ...);
int   CheckParm (char *check);
int   SafeOpenWrite (char *filename);
int   SafeOpenRead (char *filename);
void  SafeRead (int handle, void *buffer, long count);
void  SafeWrite (int handle, void *buffer, long count);
void  * SafeMalloc (long size);
long	LoadFile (char *filename, void **bufferptr);
void	SaveFile (char *filename, void *buffer, long count);
WORD  ShortSwap (WORD i);

void PrintXYString (int x, int y, char *str);

#endif