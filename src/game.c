#include "main.h"

#define SELF 0
#define SPAWNLEVEL 3

// ==================================================
// externals

extern screen_t lightscreen;

// ==================================================
// declarations

game_t session = { 0 };

// ==================================================
// functions

void assignplayer(refer_t player, int level, int client) {
 if (client < 0 || client >= MAX_GAMECLIENTS) {
  LOGREPORT("received out of bounds client index.");
  return;
 }
 
}

void assortlevels() {
 level_t* level;
 refer_t* players;
 aabb_t aabb;
 int i, j;
 
 if (!session.open) {
  LOGREPORT("attempted to assort unopened session levels.");
  return;
 }
 
 for (i = 0; i < MAX_LEVELS; i++) {
  level = &session.levels[i];
  
  boundbox(&aabb, 0, 0, level->w, level->h);
  
  players = seekclass("pliant.Player", aabb);
  
  for (j = 0; players[j]; j++);
  
  if (j > 0) {
   bindlevel(level);
   ticklevel();
  }
 }
 
 return;
}

void centerspace(int* sx, int* sy, level_t* level, screen_t* screen) {
 int xs, ys;
 
 if (!sx || !sy || !screen || !level) {
  LOGREPORT("received invalid position or arguments.");
  return;
 }
 
 xs = *sx - screen->w / 2;
 ys = *sy - (screen->h - 8) / 2;
 
 if (xs < 16) {
  xs = 16;
 }
 
 if (ys < 16) {
  ys = 16;
 }
 
 if (xs > level->w * 16 - screen->w - 16) {
  xs = level->w * 16 - screen->w - 16;
 }
 
 if (ys > level->h * 16 - screen->h - 16) {
  ys = level->h * 16 - screen->h - 16;
 }
 
 *sx = xs;
 *sy = ys;
 
 return;
}

void chainemptylevels() {
 int i, j;
 
 for (i = MAX_LEVELS - 1, j = 1; i > -1; i--, j--) {
  bindlevel(&session.levels[i]);
  emptylevel(LEVELSIZE, LEVELSIZE, j);
 }
}

void chainlevels() {
 level_t* level;
 int i, j;
 
 level = NULL;
 
 for (i = MAX_LEVELS - 1, j = 1; i > -1; i--, j--) {
  bindlevel(&session.levels[i]);
  createlevel(LEVELSIZE, LEVELSIZE, j, level);
  
  level = &session.levels[i];
 }
 
 return;
}

void kickclient(int index) {
 gameclient_t* client;
 message_t message;
 
 if (index < 0 || index >= MAX_GAMECLIENTS) {
  LOGREPORT("received out of bounds client index.");
  return;
 }
 
 client = &session.clients[index];
 
 if (!client->name || !client->being) {
  LOGREPORT("attempted to kick unheld client.");
  return;
 }
 
 if (client->player) {
//  cullunit(client->player);
 }
 
// directmessage(&message, MSG_REMOVECLIENT, 0, 0);
// appendclientmessage(&message, client->being);
 
 dropclient(client->being);
 
 memset(client, 0, sizeof(gameclient_t));
 
 return;
}

void putclient(int index, const char* name, int being) {
 gameclient_t* client;
 
 client = &session.clients[index];
 
 client->name = reprintstring(name);
 client->being = being;
 client->deadtime = client->livetime = 0;
 client->finished = 0;
 
 LOGREPORT("added client '%s' at index %i.", name, index);
 
 return;
}

void renderGUI(pliant_t* player, screen_t* screen) {
 int i, x, y;
 
 for (y = 0; y < 2; y++) {
  for (x = 0; x < 20; x++) {
   rendersprite(x * 8, screen->h - 16 + y * 8, 0 + 12 * 32, getcolor(0, 0, 0, 0), 0, screen);
  }
 }
 
 for (i = 0; i < 10; i++) {
  if (i < player->health) {
   rendersprite(i * 8, screen->h - 16, 0 + 12 * 32, getcolor(000, 200, 500, 533), 0, screen);
  }
  else {
   rendersprite(i * 8, screen->h - 16, 0 + 12 * 32, getcolor(000, 100, 000, 000), 0, screen);
  }
  
  if (player->rechargedelay > 0) {
   if (player->rechargedelay / 4 % 2 == 0) {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 555, 000, 000), 0, screen);
   }
   else {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 110, 000, 000), 0, screen);
   }
  }
  else {
   if (i < player->stamina) {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 220, 550, 553), 0, screen);
   }
   else {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 110, 000, 000), 0, screen);
   }
  }
 }

 if (player->item != NOITEM) {
//  renderhotbaritem(player->item, 10 * 8, screen->h - 16, screen);
 }
}


void rendergame(screen_t* screen) {
 refer_t player;
 pliant_t* unit;
 level_t* level;
 int color, depth;
 int x, y, xs, ys;
 static int l = 0;
// depth = seekplayer(&player, self->player);
 
 l = 3;
 
 depth = l;
 player = 0;
 
 level = &session.levels[depth];
 unit = (pliant_t*) &level->units[player];//seekbond(clients[SELF].player);
 
 xs = unit->x;
 ys = unit->y;
 
 centerspace(&xs, &ys, level, screen);
 
 if (depth > 3) {
  color = getcolor(20, 20, 121, 121);
  
  for (y = 0; y < 14; y++) {
   for (x = 0; x < 24; x++) {
	rendersprite(x * 8 - ((xs / 4) & 7), y * 8 - ((ys / 4) & 7), 0, color, 0, screen);
   }
  }
 }
 
 if (level) {
  bindlevel(level);
  
  renderbackground(xs, ys, screen);
  rendersprites(xs, ys, screen);
  
  if (depth > 3) {
   clearscreen(&lightscreen, 0);
   renderlights(xs, ys, &lightscreen);
   overlayscreens(screen, &lightscreen, xs, ys);
  }
  
  renderGUI(unit, screen);
 }
 
 bindlevel(NULL);
 
 return;
}

int seekplayer(refer_t* unit, int bond) {
 level_t* level;
 int i, found;
 
 for (i = 0; i < MAX_LEVELS; i++) {
  level = &session.levels[i];
  
  bindlevel(level);
  
  found = seekbond(bond);
  
  if (found != NOUNIT) {
   *unit = found;
   
   return i;
  }
 }
 
 return -1;
}

void startsession(gametype_e type, char* name, char* address, int port) {
 message_t message;
 //gameclient_t* self;
 refer_t pliant;
 int content[2] = { 0 };
 
 if (session.open) {
  LOGREPORT("game session already open.");
  return;
 }
 
 putclient(SELF, name, 0);
 
 //self = &session.clients[SELF];
 
 switch (type) {
 case GAME_CLIENT:
  joinhost(address, port, 54321);
  
  chainemptylevels();
  
  strncpy((char*) content, name, sizeof(content));
  
  directmessage(&message, MSG_ADDCLIENT, content, sizeof(content));
  appendmessage(&message);
  
  break;
  
 case GAME_HOST:
  openhost(port);
  /* no break */
  
 case GAME_PRIVATE:
  chainlevels();
  
  session.id = randomid();
  
  bindlevel(&session.levels[SPAWNLEVEL]);
  
  // nonmob entities are spawned at random world coordinates
  pliant = spawn("pliant.Player");
  
  assignplayer(pliant, SPAWNLEVEL, SELF);
  
  session.open = 1;
  
  break;
  
 default:
  LOGREPORT("invalid session type.");
  return;
 }
 
 settimer(&session.watch, TIMER_SIMPLELAPSE, CURRENTTIME, GAMERATE);
 
 session.ticks = 0;
 session.type = type;
 session.version = GAMEVERSION;
 
 return;
}

void tickgame() {
 int unprocessed;
 
 if (!session.open) {
  //LOGREPORT("session unopened.");
  return;
 }
 
 unprocessed = readtimer(&session.watch);
 
 // player commands are updated in the handlequeues function
 
 while (unprocessed > 0) {
  switch (session.type) {
  case GAME_CLIENT:
   break;
   
  case GAME_HOST:
   /* no break*/
   
  case GAME_PRIVATE:
   
   break;
   
  default:
   LOGREPORT("unknown session type.");
   return;
  }
  
  // send host update
  
  unprocessed--;
 }
 
 bindlevel(NULL);
}
