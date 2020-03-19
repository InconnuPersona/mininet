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

// TODO: implement kick client function
void kickgameclient(refer_t client) {
 
 
 return;
}

refer_t putgameclient(const char* name, refer_t bind) {
 gameclient_t* client;
 int i, j;
 
 j = -1;
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  client = &session.clients[i];
  
  if (j < 0 && !client->id) {
   j = i;
  }
  else if (client->bind && client->bind == bind) {
   j = -1;
   
   LOGREPORT("client bind [%x] already taken", bind);
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
  
  client->bind = bind;
  client->id = newgameclientid();
  client->deadtime = client->livetime = 0;
  client->finished = client->inmenu = 0;
  
  strncpy(client->name, name, MAX_NAMELENGTH);
  
  LOGREPORT("put game client '%s' [%x] in slot %i under bind [%x].", name, client->id, j, bind);
  
  return client->id;
 }
 else {
  LOGREPORT("unable to find free slot for client '%s'.", name);
 }
 
 return INVALIDCLIENT;
}

// TODO: handle game client spawn effectively; return unit referral
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

int handleinput() {
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
}

// TODO: implement game client menus
void handlegameclient(refer_t client) {
 static int oldcommand = 0;
 unit_u* player;
 int commands;
 
 client = getgameclient(client);
 
 if (awaited()) {
  if (session.ticks % 5 == 0) {
   pushjoin(session.clients[client].name);
  }
  
  return;
 }
 
 commands = handleinput();
 
 if (client != INVALIDCLIENT) {
  if (session.clients[client].inmenu) {
   commands = 0;
   
   ticksurface(session.clients[client].inmenu, session.clients[client].entity);
  }
  
  bindgamelevel(session.clients[client].level);
  
  player = getunit(session.clients[client].entity);
  
  if (!player) {
   return;
  }
  
  player->pliant.commands = commands;
 }
 
 if (session.type == GAME_CLIENT) {
  if (!session.clients[client].inmenu) {
   if (commands != oldcommand) {
    pushcommands(commands);
   }
  }
 }
 
 bindlevel(NULL);
 
 oldcommand = commands;
 
 return;
}

void handlehost() {
 handlegameclient(session.self);
 
 if (session.type == GAME_HOST) {
  // check client synchronizations
 }
}

refer_t getboundclient(int bind) {
 int i;
 
 if (bind == INVALIDCLIENT) {
  return INVALIDCLIENT;
 }
 
 if (bind == LOCALCLIENT) {
  return getgameclient(session.self);
 }
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  if (session.clients[i].bind == bind) {
   return session.clients[i].id;
  }
 }
 
 return INVALIDCLIENT;
}

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

refer_t getgamepliant(refer_t client) {
 int i;
  
  if (client == INVALIDCLIENT) {
   return NOUNIT;
  }
  
  if (client == LOCALCLIENT) {
   return getgameclient(session.self);
  }
  
  for (i = 0; i < MAX_GAMECLIENTS; i++) {
   if (session.clients[i].id == client) {
    return session.clients[i].entity;
   }
  }
  
  return NOUNIT;
}

refer_t getnamedclient(const char* name) {
 int i;
 
 if (!name) {
  return INVALIDCLIENT;
 }
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  if (strcmp(session.clients[i].name, name) == 0) {
   return session.clients[i].id;
  }
 }
 
 return INVALIDCLIENT;
}

refer_t getplayerclient(refer_t pliant) {
 int i;
 
 if (!pliant) {
  return INVALIDCLIENT;
 }
 
 for (i = 0; i < MAX_GAMECLIENTS; i++) {
  if (session.clients[i].entity == pliant) {
   return session.clients[i].id;
  }
 }
 
 return INVALIDCLIENT;
}

// setgameclient
void setgameplayer(refer_t client, int player, int level) {
// session.clients[client].level = level;
// session.clients[client].entity = spawn("pliant.Player");
}

void tickclients() {
 int i;
 
 for (i = 1; i < MAX_GAMECLIENTS; i++) {
  if (session.clients[i].id) {
   pushdeltachunks(session.clients[i].id);
  }
 }
}

void updatehost() {
 if (session.type == GAME_CLIENT) {
  //player = getgameplayer(session.self);
  
  //if (player != NOUNIT) {
   // push client orders
  //}
 }
 else if (session.type == GAME_HOST && (session.ticks % 3 == 0)) {
  tickclients();
 }
}
