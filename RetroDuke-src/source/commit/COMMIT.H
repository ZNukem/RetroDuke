#ifndef commit_public
#define commit_public

#define DEFAULTSOCKETNUMBER (0x882b)

typedef struct
{
   // general stuff
   LONG gametype;       // type of communication we're using
   LONG numplayers;     // number of players in game
   LONG pause;       // pause right before launching
   LONG vector;        // vector for GAMECOM

   // network stuff
   LONG socketnumber;   // socketnumber to use in game

   // modem stuff
   char * initstring;   // init string for modem
   char * hangupstring; // hangup string for modem
   char * phonenumber;  // phonenumber
   char * launchname;   // launchname
   LONG  connecttype;   // Connection type for Modem
   LONG  usetone;       // whether or not to use tone dialing

   // serial stuff
   LONG  comportnum;    // com port to use
   LONG  baudrate;      // baudrate at which port should be opened
   LONG  uartaddress;   // uart to use in opening port
   LONG  irqnumber;     // irq to use in opening port
   LONG  showstats;   // show serial stats when game is done

} config_t;

typedef enum
   {
   connecttype_dialing=0,
   connecttype_answer=1,
   connecttype_alreadyconnected=2
   };

extern config_t config;
void Shutdown ( void );

#endif
