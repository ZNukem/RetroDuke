// sample network.c file

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

static boolean NetworkStarted=false;
/*
=================
=
= ShutdownNetwork
=
=================
*/

void ShutdownNetwork ( void )
   {
	if (NetworkStarted == true)
		{
		NetworkStarted = false;
      //*****************************
      // Put shutdown code here
      //*****************************
		}
	}

/*
=================
=
= StartupNetwork
=
=================
*/

boolean StartupNetwork ( void )
	{
	if (NetworkStarted == false)
      {
      NetworkStarted = true;
      //*****************************
      // Put startup code here
      //*****************************
      }
   //*****************************
   // Put Find other people code here
   //     playernumbers should be sorted identically on every players computer
   //     i.e. everyone's player 1 should be the same etc.
   //     Player 0 is always the local address of this machine
   //     Player 1 is usually the master in most games
   //*****************************
	return true;
	}

/*
=================
=
= SendNetworkPacket
=
=================
*/

void SendNetworkPacket ( void )
	{
   //*****************************
   // Put Send code here:
   //    gamecom.remotenode contains destination address
   //    gamecom.data holds data to send
   //    gamecom.datalength holds amount of data to send
   //*****************************
	}

/*
=================
=
= GetNetworkPacket
=
=================
*/

void GetNetworkPacket ( void )
	{
   //*****************************
   // Put Get code here:
   //    gamecom.remotenode should be set to -1 if no packet is ready,
   //                       otherwise it should be set to the address of
   //                       the packet sender
   //    gamecom.data       copy your data to this buffer
   //    gamecom.datalength fill in how much of it is there
   //*****************************
	}
