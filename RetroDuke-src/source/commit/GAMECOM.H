#ifndef gamecom_public
#define gamecom_public
// gamecom.h

#if (__WATCOMC__ == 0)
#include "global.h"
#endif
#define	MAXNETNODES		10			// max computers in a game

#define	MAXPLAYERS		10			// max players

#define  CMD_SEND               1
#define  CMD_GET                2
#define  CMD_SENDTOALL          3
#define  CMD_SENDTOALLOTHERS    4
#define  CMD_SCORE              5

#define  MAXPACKETSIZE    2048
#define	MAXCOMBUFFERSIZE 2048


typedef struct
{
	short	intnum;			// Game executes an int to send commands

// communication between Game and the driver

	short	command;	    	// CMD_SEND or CMD_GET
	short	remotenode;		// dest for send, set by get (-1 = no packet)
	short	datalength;		// bytes in data to be sent / bytes read

// info specific to this node

	short	consoleplayer;	// 1-numplayers = player number
	short	numplayers;		// 1-MAXPLAYERS
   short gametype;      // 1 = SERIAL, 2 = MODEM, 3 = NETWORK

	short extra;         // extra short for 4-byte alignment
// packet data to be sent

	char	data[MAXPACKETSIZE];
} gamecom_t;

// GAME_TYPES

#define SERIAL_GAME  (1)
#define MODEM_GAME   (2)
#define NETWORK_GAME (3)

#if (__WATCOMC__ == 0)

extern   gamecom_t   gamecom;

void ShutdownGAMECOM ( void );
int  CheckParm (char *check);
void LaunchGAME ( boolean pause );
void interrupt NetISR (void);
long GetVector (void);

#else

extern   gamecom_t   * gamecom;

#endif

#endif
