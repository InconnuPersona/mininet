#include "main.h"
#include "levelaid.h"

#define LOCALANGTH 1

void printgamesend(gamesend_t* send);

extern unitword_t unitwords[];

// all necessary client information is sent
typedef struct {
 int checksum;
 int count;
} clientsend_t;

typedef struct {
 int data[4];
 char name[MAX_NAMELENGTH];
} tellsend_t;

typedef struct {
 refer_t id;
 char word[32];
} unitwordsend_t;

typedef struct {
 int level;
 int id;
 unit_u unit;
} unitsend_t;

void readchunksend(gamesend_t* sent) {
 chunk_t* chunk;
 
 if (!sent) {
  LOGREPORT("received invalid chunk send.");
  return;
 }
 
 chunk = (chunk_t*) sent->data;
 
 setchunk(chunk, &session.levels[chunk->level]);
 
 LOGREPORT("set chunk [%i, %i] in level %i.", chunk->x, chunk->y, chunk->level);
 
 return;
}

void readunitsend(gamesend_t* sent) {
 
}

void readunitwordsend(gamesend_t* sent) {
 unitwordsend_t* word;
 
 if (!sent) {
  LOGREPORT("received invalid unit word send.");
  return;
 }
 
 word = (unitwordsend_t*) sent->data;
 
 bindunitword(word->word, word->id);
 
 return;
}

void sendblin() {
 tellsend_t tell;
 
 strcpy(tell.name, "hello blin");
 
 pushgamesend(MSG_NONE, &tell, sizeof(tellsend_t), 0);
 
 return;
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

void pushchunk(int level, int x, int y, refer_t bind) {
 chunk_t chunk;
 
 if (level < 0 || level >= MAX_LEVELS) {
  LOGREPORT("attempted to bind invalid game level [%i].", level);
  return;
 }
 
 if (!readchunk(x, y, &chunk, &session.levels[level])) {
  LOGREPORT("unable to push chunk [%i, %i] in level %i.", x, y, level);
  return;
 }
 
 chunk.level = level;
 
 pushgamesend(MSG_POSTCHUNK, &chunk, sizeof(chunk_t), bind);
 
 LOGREPORT("pushed chunk [%i, %i] in level %i.", x, y, level);
 
 return;
}

void pushclients(refer_t bind) {
}

void pushdeltachunks(refer_t client) {
}

void pushlocalchunks(refer_t client) {
 pliant_t* pliant;
 int level, x, y;
 aabb_t bound;
 
 client = getgameclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("attempted to push local chunks to invalid client.");
  return;
 }
 
 level = session.clients[client].level;
 
 bindgamelevel(level);
 
 pliant = &getunit(session.clients[client].entity)->pliant;
 
 if (!pliant) {
  LOGREPORT("unable to push local chunks to unspawned client.");
  return;
 }
 
 x = (pliant->x >> 4) / CHUNKANGTH;
 y = (pliant->y >> 4) / CHUNKANGTH;
 
 boundbox(&bound, x - LOCALANGTH, y - LOCALANGTH, x + LOCALANGTH, y + LOCALANGTH);
 
 ensuredomain(&bound);
 
 for (x = bound.x0; x < bound.x1; x++) {
  for (y = bound.y0; y < bound.y1; y++) {
   pushchunk(level, x, y, session.clients[client].bind);
  }
 }
 
 // push units
 
 bindlevel(NULL);
 
 return;
}

#define GAMESENDHEADERWIDTH sizeof(int) * 2

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
 
 memcpy(send.data, data, GAMESENDHEADERWIDTH + length);
 
 INDEBUG(printgamesend(&send));
 
 directmessage(&message, &send, length);
 
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
 
 return;
}

void pushtell(int type, refer_t id, refer_t bind) {
 tellsend_t send;
 
 send.data[0] = type;
 send.data[1] = id;
 
 pushgamesend(MSG_ADDCLIENT, &send, sizeof(tellsend_t), bind);
}

void pushunit(int level, refer_t id, refer_t client) {
}

void pushunitwords(refer_t bind) {
 unitwordsend_t word;
 int i;
 
 for (i = 0; i < MAX_UNITWORDS; i++) {
  if (unitwords[i].id && unitwords[i].word) {
   word.id = unitwords[i].id;
   
   strncpy(word.word, unitwords[i].word, 32);
   
   LOGDEBUG("redefining '%s' [%x].", word.word, word.id);
   
   pushgamesend(MSG_POSTUNITWORD, &word, sizeof(unitwordsend_t), bind);
  }
 }
 
 return;
}

void handleclientsend(gamesend_t* sent, refer_t sender) {
 tellsend_t* told;
 tellsend_t send;
 int id;
 
 if (!sent) {
  LOGREPORT("received invalid gamesend.");
  return;
 }
 
 INDEBUG(printgamesend(sent));
 
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
  
  pushunitwords(sender);
  
  pushlocalchunks(id);
  
  break;
  
 default:
  LOGREPORT("received unhandled client gamesend type [%i].", sent->type);
  break;
 }
}

void handlehostsend(gamesend_t* sent, refer_t sender) {
 tellsend_t* told;
 int id;
 
 INDEBUG(printgamesend(sent));
 
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
  
 case MSG_POSTCHUNK:
  if (sent->sender != session.id) {
   LOGREPORT("received chunk send from unknown host session [%x].", sent->sender);
   return;
  }
  
  readchunksend(sent);
  
  break;
  
 case MSG_POSTUNIT:
  if (sent->sender != session.id) {
   LOGREPORT("received unit send from unknown host session [%x].", sent->sender);
   return;
  }
  
  readunitsend(sent);
  
  break;
  
 case MSG_POSTUNITWORD:
  if (sent->sender != session.id) {
   LOGREPORT("received unit word send from unknown host session [%x].", sent->sender);
   return;
  }
  
  readunitwordsend(sent);
  
  break;
  
 default:
  LOGREPORT("received unhandled host gamesend type [%i].", sent->type);
  break;
 }
}
