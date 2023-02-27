#include "host.h"

game_s session;

// Get a game client index by id.
int getgameclient(refer_t client) {
 int i;
 
 if (client == INVALIDCLIENT) {
  return INVALIDCLIENT;
 }
 
 if (client == LOCALCLIENT) {
  return getgameclient(session.self);
 }
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  if (session.clients[i].id == client) {
   return i;
  }
 }
 
 return INVALIDCLIENT;
}

// Generate a new id for a client.
int newgameclientid() {
 int id;
 
 do {
  id = randomid();
  
  if (getgameclient(id) != INVALIDCLIENT || id == session.id) {
   continue;
  }
  
  return id;
 }
 while (1);
}

refer_t newgameclient(const char* name, refer_t netid) {
 gameclient_s* client;
 int i, j;
 
 j = -1;
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  client = &session.clients[i];
  
  if (j < 0 && !client->id) {
   j = i;
  }
  else if (client->netid && client->netid == netid) {
   j = -1;
   
   LOGREPORT("client netid [%x] already taken", netid);
   break;
  }
  else if (!strncmp(client->name, name, MAX_NAMELENGTH)) {
   j = -1;
   
   LOGREPORT("client name '%s' already taken.", name);
   break;
  }
 }
 
 if (j > -1) {
  client = &session.clients[j];
  
  client->id = newgameclientid();
  client->netid = netid;
  client->deadtime = client->livetime = 0;
  client->finished = /*client->inmenu =*/ 0;
  
  strncpy(client->name, name, MAX_NAMELENGTH);
  
  LOGREPORT("put game client '%s' [%x] [netid %x] in slot %i.", name, client->id, netid, j);
  
  return client->id;
 }
 else {
  LOGREPORT("unable to find free slot for client '%s'.", name);
 }
 
 return INVALIDCLIENT;
}
