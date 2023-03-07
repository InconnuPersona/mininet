#include "host.h"
#include "net.h"

Game session;

// Get a game client index by id.
int Game::get_client(refer_t client) {
 int i;
 
 if (client == INVALIDCLIENT) {
  return INVALIDCLIENT;
 }
 
 if (client == LOCALCLIENT) {
  return get_client(self);
 }
 
 for (i = 0; i < MAX_CLIENTS; i++) {
  if (clients[i].id == client) {
   return i;
  }
 }
 
 return INVALIDCLIENT;
}

/*int handleinput() {
 int commands;
 
 commands = 0;
 
 if (getaliasdown("attack")) {
  commands |= CMD_ATTACK;
 }
 
 if (getaliasdown("menu")) {
  commands |= CMD_MENU;
 }
 
 if (getaliasdown("down")) {
  commands |= CMD_MOVEDOWN;
 }
 
 if (getaliasdown("left")) {
  commands |= CMD_MOVELEFT;
 }
 
 if (getaliasdown("right")) {
  commands |= CMD_MOVERIGHT;
 }
 
 if (getaliasdown("up")) {
  commands |= CMD_MOVEUP;
 }
 
 return commands;
}*/

// Generate a new id for a client.
int Game::new_clientid() {
 int id;
 
 do {
  id = randomid();
  
  if (get_client(id) != INVALIDCLIENT || id == session.id) {
   continue;
  }
  
  return id;
 }
 while (true);
}

refer_t Game::new_client(const char* name, refer_t netid) {
 Client* client;
 int i, j;
 
 j = -1;
 
 for (i = 0; i < MAX_CLIENTS; i++) {
  client = &clients[i];
  
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
  client = &clients[j];
  
  client->id = new_clientid();
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
