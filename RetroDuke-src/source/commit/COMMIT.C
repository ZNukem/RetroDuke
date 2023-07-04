// commit.c

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <process.h>
#include <stdarg.h>
#include <bios.h>
#include <time.h>

#include "gamecom.h"
#include "commit.h"
#include "network.h"

#define VERSION "1.0"

//
// COMMIT Apogee Software Ltd. (c) 1995
//        written by Mark Dochtermann
//

config_t config;

/*
=================
=
= Shutdown
=
=================
*/

void Shutdown ( void )
   {
   // Put in Shutdown code for a specific comm type
   ShutdownNetwork ();
	ShutdownGAMECOM ();
   }

/*
=================
=
= Startup
=
=================
*/

boolean Startup ( void )
	{
   boolean proceed=false;

   clrscr();
	printf(
          "--------------------------------\n"
          "COMMIT Compliant Device Driver Version %s \n",VERSION);
	printf("--------------------------------\n");

//
// default game parameters
//
   config.vector = -1;
   config.socketnumber = DEFAULTSOCKETNUMBER;
   config.comportnum = 2;
   config.baudrate = 9600;
   config.uartaddress = -1;
   config.irqnumber = -1;
   config.showstats = 0;
   config.gametype = NETWORK_GAME;
   config.numplayers = 2;
   config.launchname = "DUKE3D.EXE";
   config.pause = true;


//
// copy over configuration parameters
// into gamecom structure.
//

	gamecom.intnum     = config.vector;
	gamecom.numplayers = config.numplayers;
	gamecom.gametype   = config.gametype;

//
// print out game type and num players
// setup program for specific game type
//

	printf ("o  %d Player Game\n", gamecom.numplayers);

// **********************************
// Put Comm startup code here
   proceed=StartupNetwork ();
// *********************************

	return proceed;
	}


/*
=============
=
= main
=
=============
*/

void main (void)
	{

   if (Startup () == true)
		{
		LaunchGAME ( (boolean)config.pause );
		}

	Shutdown ();

	exit(0);
	}


