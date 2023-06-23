#include "global.h"
#include "commit.h"

#include <malloc.h>
#include <fcntl.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <io.h>
#include <sys\stat.h>
#include <dir.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>

/*
=================
=
= Error
=
= For abnormal program terminations
=
=================
*/

void Error (char *error, ...)
   {
	va_list argptr;

   Shutdown();
	if (error)
      {
		va_start (argptr,error);
		vprintf (error,argptr);
		va_end (argptr);
		printf ("\n\n");
	   }

	exit (error != (char *) NULL);
   }

/*
=================
=
= CheckParm
=
= Checks for the given parameter in the program's command line arguments
=
= Returns the argument number (1 to argc-1) or 0 if not present
=
=================
*/

int CheckParm (char *check)
   {
	int             i;

	for (i = 1;i<_argc;i++)
		if ( !stricmp(check,_argv[i]) )
			return i;

	return 0;
   }

//******************************************************************************
//
// SafeOpenWrite ()
//
//******************************************************************************

int SafeOpenWrite (char *filename)
   {
	int	handle;

	handle = open(filename,O_RDWR | O_BINARY | O_CREAT | O_TRUNC
	, S_IREAD | S_IWRITE);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
   }

//******************************************************************************
//
// SafeOpenRead ()
//
//******************************************************************************

int SafeOpenRead (char *filename)
   {
	int	handle;

	handle = open(filename,O_RDONLY | O_BINARY);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
   }


//******************************************************************************
//
// SafeRead ()
//
//******************************************************************************

void SafeRead (int handle, void *buffer, long count)
   {
	long iocount;

	while (count)
	   {
		iocount = count > 0x8000 ? 0x8000 : count;
		if (read (handle,buffer,(int)iocount) != iocount)
			Error ("File read failure");
		buffer = (void *)( (BYTE *)buffer + (int)iocount );
		count -= iocount;
		}
	}


//******************************************************************************
//
// SafeWrite ()
//
//******************************************************************************

void SafeWrite (int handle, void *buffer, long count)
	{
	long	iocount;

	while (count)
		{
		iocount = count > 0x8000 ? 0x8000 : count;
		if (write (handle,buffer,(int)iocount) != iocount)
			Error ("File write failure");
		buffer = (void *)( (BYTE *)buffer + (int)iocount );
		count -= iocount;
	   }
   }

//******************************************************************************
//
// SafeMalloc ()
//
//******************************************************************************

void *SafeMalloc (long size)
   {
	void *ptr;

	ptr = malloc ((short)size);

	if (!ptr)
		Error ("Malloc failure for %lu bytes",size);

	return ptr;
   }


//******************************************************************************
//
// LoadFile ()
//
//******************************************************************************

long	LoadFile (char *filename, void **bufferptr)
   {
   int  handle;
   long length;

   handle = SafeOpenRead (filename);
   length = filelength (handle);
   *bufferptr = SafeMalloc(length);
   SafeRead (handle,*bufferptr, length);
   close (handle);
   return length;
   }

//******************************************************************************
//
// ShortSwap ()
//
//******************************************************************************

WORD ShortSwap (WORD i)
   {
   return ((i&255)<<8) + ((i>>8)&255);
   }

#if 0
/*
=================
=
= PrintXYString
=
=================
*/

void PrintXYString (int x, int y, char *str)
	{
	char *s;
	BYTE far *screen;

	s = str;
	screen = (BYTE far *)MK_FP(0xb800,0) + (y*160) + (x<<1);

	while (*s)
      {
      *screen = *s;
      x++;
      s++;
      if (x==80)
         {
         x=0;
         y++;
         if (y==25)
            {
            y=0;
            }
			screen = (BYTE far *)MK_FP(0xb800,0) + (y*160) + (x<<1);
			}
		else
			{
			screen += 2;
			}

		while ((*s < 32) && (*s > 0))
			s++;
		}
	}

#endif