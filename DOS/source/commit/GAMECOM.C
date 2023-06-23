#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <dos.h>
#include <conio.h>
#include "gamecom.h"
#include "global.h"
#include "commit.h"
#include "network.h"

#define GAMECOM_STACKSIZE    (2048)
#define GAMECOM_SAFETYMARGIN (8)

gamecom_t	     gamecom;
int              control;
static boolean   vectorishooked=false;
void interrupt (*oldgamecomvector) (void);
void interrupt GAMECOM_ISR (void);
static char * gamecom_stack;
static WORD gamecom_stacksegment;
static WORD gamecom_stackpointer;
static WORD old_stacksegment;
static WORD old_stackpointer;

/*
=============
=
= SetupGAMECOM
=
=============
*/

void SetupGAMECOM ( void )
   {
	unsigned char far *vectorptr;
   short vector;
   char * topofstack;

   vector = gamecom.intnum;

 	/* Get an interrupt vector if not already set */
	if (vector == -1)
	   {
       for (vector = 0x60 ; vector <= 0x66 ; vector++)
		   {
			vectorptr = *(unsigned char far * far *)(vector*4);
			if ( !vectorptr || *vectorptr == 0xcf )
				break;
		   }
       if (vector == 0x67)
		   {
			printf ("Warning: no NULL or iret interrupt vectors were found in the 0x60 to 0x65\n"
					  "range.  You can specify a vector in the configuration file\n"
					  "Press a key to continue.\n");
			getch ();
           printf ("Using default vector 0x66\n");
           vector = 0x66;
		   }
	   }
	gamecom.intnum = vector;

   // allocate the gamecom stack

   gamecom_stack = SafeMalloc(GAMECOM_STACKSIZE);

   // Calculate top of stack

   topofstack = gamecom_stack + GAMECOM_STACKSIZE - GAMECOM_SAFETYMARGIN;

   // Determine stack segment and pointer

   gamecom_stacksegment = FP_SEG( (char huge *)topofstack );
   gamecom_stackpointer = FP_OFF( (char huge *)topofstack );

   // hook the vector for the game

	oldgamecomvector = getvect (gamecom.intnum);
	setvect (gamecom.intnum, GAMECOM_ISR);
	vectorishooked = true;
   }


/*
=============
=
= ShutdownGAMECOM
=
=============
*/

void ShutdownGAMECOM ( void )
   {
	if (vectorishooked == true)
      {
		setvect (gamecom.intnum,oldgamecomvector);
   	vectorishooked = false;
      free ( gamecom_stack );
      }
   }

/*
=============
=
= GAMECOM_ISR
=
=============
*/

#define GetStack(a,b) \
   {                  \
   *a = _SS;          \
   *b = _SP;          \
   }
#define SetStack(a,b) \
   {                  \
   _SS=a;             \
   _SP=b;             \
   }

void interrupt GAMECOM_ISR (void)
	{
   //
   // Get current stack
   //

   GetStack( &old_stacksegment, &old_stackpointer );

   //
   // Set the local stack
   //

   SetStack( gamecom_stacksegment, gamecom_stackpointer );

   switch(gamecom.command)
      {
      case CMD_SEND:
         SendNetworkPacket ();
         break;
      case CMD_SENDTOALL:
         for (control = 1; control<=gamecom.numplayers; control++)
            {
            gamecom.remotenode = control;
            SendNetworkPacket ();
            }
         break;
      case CMD_SENDTOALLOTHERS:
         for (control = 1; control<=gamecom.numplayers; control++)
            {
            if (control != gamecom.consoleplayer)
               {
               gamecom.remotenode = control;
               SendNetworkPacket ();
               }
            }
         break;
      case CMD_GET:
         GetNetworkPacket ();
         break;
      case CMD_SCORE:
         //***********************************
         //SCORE specific stuff here
         //***********************************
         break;
      }

   //
   // Restore the old stack
   //

   SetStack( old_stacksegment, old_stackpointer );
   }

/*
=============
=
= LaunchGAME
=
=============
*/

void LaunchGAME ( boolean pause )
{
	char	*newargs[39];
	char	adrstring[10];
	long  	flatadr;
	int argnum = 1;
	int i,j;

	SetupGAMECOM ();

// build the argument list for the game
// adding all previous command lines to be passed to the main game

// skip over launch parameter if specified
   j = CheckParm("launch");
   for (i=1;i<_argc;i++)
      {
      if (i==j)
         i++;
      else
	      newargs [argnum++] = _argv[i];
      }

// adding "-net" and the address of gamecom.

	newargs [argnum++] = "-net";

	/* Add address of gamecom structure */

	flatadr = (long)_DS*16 + (unsigned)&gamecom;
	sprintf (adrstring,"%lu",flatadr);
	newargs [argnum++] = adrstring;

	newargs [argnum] = NULL;

// Make sure arg 0 is correct, if we want to dump the arguments

	newargs [0] = config.launchname;

   if (pause==1)
      {
      printf ("About to launch %s -- Passing these arguments:\n",config.launchname);
      for (i = 0; i < argnum; i++)
         printf ("  arg %d = %s\n", i, newargs [i]);
      printf ("  player = %d\n", gamecom.consoleplayer);

      printf ("\nPress ESC to abort, or any other key to continue...");
      if (getch () == ESC)
         {
         printf ("\n\n");
         return;
         }
      }
   if (spawnvp ( P_WAIT, config.launchname, newargs) != 0)
      {
      if (errno!=0)
         printf("\nErrorNumber: %d %s\n",errno,strerror(errno));
      }
//	printf ("\nReturned from %s\n\n",config.launchname);

   ShutdownGAMECOM();
	}
