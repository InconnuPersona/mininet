#include "main.h"
#include "levelaid.h"

extern unitword_t unitwords[];

typedef struct {
 int data[4];
 char name[MAX_NAMELENGTH];
} tellsend_t;

typedef struct {
 int level;
 int x, y;
 tile_t tiles[CHUNKANGTH * CHUNKANGTH];
} chunksend_t;

typedef struct {
 int level;
 int id;
 unit_u unit;
} unitsend_t;

void sendblin() {
}

void printgamesend(gamesend_t* send) {
 int i;
 
 LOGREPORT("gamesend;");
 
 printf(" type: %i\n length: %i\n sender: %x", send->type, send->length, send->sender);
 
 printf("\n data:");
 
 for (i = 0; i < send->length; i++) {
  printf(" %02x", send->data[i]);
 }
 
 printf("\n integers:");
 
 for (i = 0; i < send->length / sizeof(int); i++) {
  printf(" [%x]", ((int*) send->data)[i]);
 }
 
 printf("\n string: \"");
 
 for (i = 0; i < send->length; i++) {
  printf("%c", send->data[i]);
 }
 
 printf("\"\n");
 
 return;
}

void pushgamedata() {
}

void pushchunk(int level, int x, int y) {
 // pushing a chunk automatically pushes its units
}

// TODO: implement broadcasts (use -2 or BROADCAST as the value)
void pushgamesend(int type, void* data, int length, refer_t bind) {
 gamesend_t send;
 message_t message;
 
 if (!data) {
  LOGREPORT("received invalid gamesend.");
  return;
 }
 
 if (length <= 0 || length > MAX_GAMESENDLENGTH) {
  LOGREPORT("received invalid gamesend length [%i].", length);
  return;
 }
 
 send.length = length;
 
 if (session.type == GAME_CLIENT) {
  send.sender = session.self;
 }
 else {
  send.sender = session.id;
 }
 
 send.type = type;
 
 memcpy(send.data, data, length);
 
 //printgamesend(&send);
 
 directmessage(&message, &send, sizeof(gamesend_t));
 
 if (session.type == GAME_CLIENT) {
  appendmessage(&message, 1);
 }
 else {
  appendclientmessage(&message, 1, bind);
 }
 
 return;
}

void pushjoin(const char* name) {
 tellsend_t send;
 
 if (!awaited()) {
  LOGREPORT("attempted to push joinsend under invalid host state.");
  return;
 }
 
 if (!name) {
  LOGREPORT("attempted to join host under null name.");
  return;
 }
 
 send.data[0] = session.checksum;
 
 memcpy(send.name, name, MAX_NAMELENGTH);
 
 pushgamesend(MSG_ADDCLIENT, &send, sizeof(tellsend_t), 0);
}

void pushtell(int type, refer_t id, refer_t bind) {
 tellsend_t send;
 
 send.data[0] = type;
 send.data[1] = id;
 
 pushgamesend(MSG_ADDCLIENT, &send, sizeof(tellsend_t), bind);
}

void pushunit(int level, refer_t id, refer_t bind) {
}

void handleclientsend(gamesend_t* sent, refer_t sender) {
 tellsend_t* told;
 tellsend_t send;
 int id;
 
 if (!sent) {
  LOGREPORT("received invalid gamesend.");
  return;
 }
 
 //printgamesend(sent);
 
 told = (tellsend_t*) sent->data;
 
 switch (sent->type) {
 case MSG_ADDCLIENT:
  if (getboundclient(sender) != INVALIDCLIENT) {
   pushtell(EXT_ALREADYMET, 0, sender);
   return;
  }
  
  if (getnamedclient(told->name) != INVALIDCLIENT) {
   pushtell(EXT_TAKEN, 0, sender);
   return;
  }
  
  id = putgameclient(told->name, sender);
  
  if (id == INVALIDCLIENT) {
   pushtell(EXT_FAULTED, 0, sender);
   // TODO: kick game client on failure; ensure network bind is closed
   return;
  }
  
  spawngameclient(id, SPAWNLEVEL);
  
  send.data[0] = EXT_ACCEPT;
  send.data[1] = id;
  send.data[2] = SPAWNLEVEL;
  send.data[3] = getgamepliant(id);
  
  pushgamesend(MSG_ADDCLIENT, &send, sizeof(tellsend_t), sender);
  
  LOGREPORT("accepted client '%s' to id [%x] under bind [%x].", told->name, id, sender);
  
  // push local chunks
  
  break;
  
 default:
  LOGREPORT("received unhandled client gamesend type [%i].", sent->type);
  break;
 }
}

void handlehostsend(gamesend_t* sent, refer_t sender) {
 tellsend_t* told;
 int id;
 
 //printgamesend(sent);
 
 told = (tellsend_t*) sent->data;
 
 switch (sent->type) {
 case MSG_ADDCLIENT:
  if (told->data[0] != EXT_ACCEPT) {
   if (told->data[0] == EXT_ALREADYMET) {
	return;
   }
   
   closesession();
   
   setmenu("LobbyMenu");
   
   LOGREPORT("client connection declined [%x].", told->data[0]);
   
   return;
  }
  else {
   session.id = sent->sender;
   
   id = getgameclient(session.self);
   
   if (id == INVALIDCLIENT) {
	LOGREPORT("attempted join without set client slot [%x].", session.self);
	
	closesession();
	
	setmenu("LobbyMenu");
	
	return;
   }
   
   session.clients[id].entity = told->data[3];
   session.clients[id].id = session.self = told->data[1];
   session.clients[id].level = told->data[2];
   
   LOGREPORT("client connection accepted under client ID [%x] to host session [%x]; game client assigned unit id [%x] in level %i.", session.self, session.id, told->data[3], told->data[2]);
  }
  
  break;
  
 default:
  LOGREPORT("received unhandled host gamesend type [%i].", sent->type);
  break;
 }
}
