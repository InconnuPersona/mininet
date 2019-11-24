#include "main.h"

game_t session = { 0 };

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

refer_t putgameclient(const char* name, int bind) {
 gameclient_t* client;
 int i, j;
 
 j = -1;
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  client = &session.clients[i];
  
  if (j < 0 && !client->id) {
   j = i;
  }
  else if (!strncmp(client->name, name, MAX_NAMELENGTH)) {
   j = -1;
   
   LOGREPORT("client name '%s' already taken.", name);
   break;
  }
 }
 
 if (j > -1) {
  client = &session.clients[j];
  
  client->bind = bind;
  client->id = newgameclientid();
  client->deadtime = client->livetime = 0;
  client->finished = client->inmenu = 0;
  
  strncpy(client->name, name, MAX_NAMELENGTH);
  
  LOGREPORT("put game client id [%x] '%s' in slot %i under bind [%x].", client->id, name, j, bind);
  
  return client->id;
 }
 else {
  LOGREPORT("unable to find free slot for client '%s'.", name);
 }
 
 return INVALIDCLIENT;
}

void spawngameclient(refer_t client, int level) {
 client = getgameclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("received unknown gameclient id.");
  return;
 }
 
 if (level < 0 || level >= MAX_LEVELS) {
  LOGREPORT("received out of bounds level index.");
  return;
 }
 
 bindlevel(&session.levels[level]);
 
 session.clients[client].level = level;
 session.clients[client].entity = spawn("pliant.Player");
 
 return;
}

void kickgameclient(refer_t client) {
 return;
}

int getgameclient(refer_t client) {
 int i;
 
 if (client == INVALIDCLIENT) {
  return INVALIDCLIENT;
 }
 
 if (client == LOCALCLIENT) {
  return getgameclient(session.id);
 }
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  if (session.clients[i].id == client) {
   return i;
  }
 }
 
 return INVALIDCLIENT;
}

int handleinput() {
 int commands;
 
 commands = 0;
 
 if (getaliasclicked("attack")) {
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
}

// TODO: implement game client menus
void handlegameclient(refer_t client) {
 unit_u* player;
 int commands;
 
 client = getgameclient(client);
 
 commands = handleinput();
 
 if (client != INVALIDCLIENT) {
  bindgamelevel(session.clients[client].level);
  
  player = getunit(session.clients[client].entity);
  
  if (!player) {
   return;
  }
  
  player->pliant.commands = commands;
 }
 
 if (session.type == GAME_CLIENT) {
  //if (client not in menu) {
  // send client movements and actions
  //}
 }
 
 bindlevel(NULL);
}

void handlehost() {
 handlegameclient(session.self);
 
 if (session.type == GAME_HOST) {
  // check client synchronizations
 }
}

refer_t getboundclient(int bind) {
 return 0;
}

void setgameplayer(refer_t client, int player, int level) {
// session.clients[client].level = level;
// session.clients[client].entity = spawn("pliant.Player");
}

void tickclients() {
 
}

void updatehost() {
 if (session.type == GAME_CLIENT) {
  //player = getgameplayer(session.self);
  
  //if (player != NOUNIT) {
   // push client orders
  //}
 }
 else if (session.type == GAME_HOST) {
  // update level local chunks and units to clients
 }
}
