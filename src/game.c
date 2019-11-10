#include "main.h"

extern void handleclientsend(gamesend_t* sent);
extern void handlehostsend(gamesend_t* sent);

void assortlevels() {
 aabb_t aabb;
 level_t* level;
 refer_t* pliants;
 int i, j;
 
 if (!session.open) {
  LOGREPORT("attempted to assort unopened session levels.");
  return;
 }
 
 for (i = 0; i < MAX_LEVELS; i++) {
  level = &session.levels[i];
  
  bindlevel(level);
  
  boundbox(&aabb, 0, 0, level->w, level->h);
  
  pliants = seekunits("pliant.Player", aabb);
  
  if (!pliants) {
   continue;
  }
  
  for (j = 0; pliants[j]; j++);
  
  if (j > 0) {
   ticklevel();
  }
 }
 
 return;
}

void chainlevels(int chained) {
 level_t* level;
 int i, j;
 
 level = NULL;
 
 for (i = MAX_LEVELS - 1, j = 1; i > -1; i--, j--) {
  bindlevel(&session.levels[i]);
  
  if (chained) {
   createlevel(LEVELANGTH, LEVELANGTH, j, level);
   
   level = &session.levels[i];
  }
  else {
   emptylevel(LEVELANGTH, LEVELANGTH, j);
  }
 }
 
 return;
}

void routepacket(packet_t* packet) {
 gamesend_t sent;
 int i;
 
 if (session.type != GAME_CLIENT || session.type != GAME_HOST) {
  LOGREPORT("routed packet under unpredicted or invalid host state.");
  return;
 }
 
 for (i = 0; i < packet->messagecount; i++) {
//  memcpy(sent, packet->messages[i].data.pointer);
//  
//  sent = packet->messages[i].data.pointer;
//  
//  if (sent->marker == GAMESMARKER) {
//   
//   
//   if (session.type == GAME_CLIENT) {
//    handlehostsend(sent);
//   }
//   else if (session.type == GAME_HOST) {
//    handleclientsend(sent);
//   }
//  } else {
//   LOGREPORT("received unknown message.");
//  }
 }
}

void startsession(gametype_e type, char* name, char* address, int port) {
 gamesend_t send;
 
 if (session.open) {
  LOGREPORT("game session already open.");
  return;
 }
 
 switch (type) {
 case GAME_CLIENT:
  joinhost(address, port, 54321);
  
  chainlevels(0);
  
  //directmessage(&message, MSG_ADDCLIENT, session.clients[LOCALCLIENT].name, MAX_NAMELENGTH);
  //appendmessage(&message, 1);
  
  //pushgamesend();
  
  break;
  
 case GAME_HOST:
  openhost(port);
  /* no break */
  
 case GAME_PRIVATE:
  chainlevels(1);
  
  session.id = randomid();
  
  LOGREPORT("opened game session under id [%x].", session.id);
  
  session.self = putgameclient(name, LOCALCLIENT);
  
  spawngameclient(session.self, SPAWNLEVEL);
  
  break;
  
 default:
  LOGREPORT("invalid session type.");
  return;
 }
 
 // TODO: improve timer accuracy
 settimer(&session.timer, TIMER_SPACEDLAPSE, CURRENTTIME, 1.f / GAMERATE);
 
 session.marker = GAMESMARKER;
 session.open = 1;
 session.ticks = 0;
 session.type = type;
 session.version = GAMEVERSION;
 
 return;
}

void centerfocus(int* sx, int* sy, level_t* level, screen_t* screen) {
 int xs, ys;
 
 if (!level || !screen || !sx || !sy) {
  LOGREPORT("received invalid position or arguments.");
  return;
 }
 
 xs = *sx - screen->w / 2;
 ys = *sy - (screen->h - 8) / 2;
 
 if (xs < BLOCKSCALE) {
  xs = BLOCKSCALE;
 }
 
 if (ys < BLOCKSCALE) {
  ys = BLOCKSCALE;
 }
 
 if (xs > level->w * BLOCKSCALE - screen->w - BLOCKSCALE) {
  xs = level->w * BLOCKSCALE - screen->w - BLOCKSCALE;
 }
 
 if (ys > level->h * BLOCKSCALE - screen->h - BLOCKSCALE) {
  ys = level->h * BLOCKSCALE - screen->h - BLOCKSCALE;
 }
 
 *sx = xs;
 *sy = ys;
 
 return;
}

void renderdue(screen_t* screen) {
 int color;
 
 clearscreen(screen, 0);
 
 color = getcolor(0, 555, 555, 555);
   
 renderfont("Casting the skies", 0, 1 * TILESCALE, color, screen);
 renderfont("and creating the grounds;", 0, 2 * TILESCALE, color, screen);
 renderfont("bringing all earthwrought", 0, 3 * TILESCALE, color, screen);
 renderfont("from the underground.", 0, 4 * TILESCALE, color, screen);
 renderfont("and waiting...", 0, 5 * TILESCALE, color, screen);
 
 return;
}

void renderGUI(refer_t player, screen_t* screen) {
 pliant_t* pliant;
 int i, x, y;
 
 if (!hasunit(player)) {
  return;
 }
 
 pliant = (pliant_t*) getunit(player);
 
 for (y = 0; y < 2; y++) {
  for (x = 0; x < 20; x++) {
   rendersprite(x * 8, screen->h - 16 + y * 8, 0 + 12 * 32, getcolor(0, 0, 0, 0), 0, screen);
  }
 }
 
 for (i = 0; i < 10; i++) {
  if (i < pliant->health) {
   rendersprite(i * 8, screen->h - 16, 0 + 12 * 32, getcolor(000, 200, 500, 533), 0, screen);
  }
  else {
   rendersprite(i * 8, screen->h - 16, 0 + 12 * 32, getcolor(000, 100, 000, 000), 0, screen);
  }
  
  if (pliant->tireddelay > 0) {
   if (pliant->tireddelay / 4 % 2 == 0) {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 555, 000, 000), 0, screen);
   }
   else {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 110, 000, 000), 0, screen);
   }
  }
  else {
   if (i < pliant->stamina) {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 220, 550, 553), 0, screen);
   }
   else {
	rendersprite(i * 8, screen->h - 8, 1 + 12 * 32, getcolor(000, 110, 000, 000), 0, screen);
   }
  }
 }

 if (pliant->item != NOITEM) {
//  renderhotbaritem(player->item, 10 * 8, screen->h - 16, screen);
 }
}

void rendergame(screen_t* screen) {
 extern screen_t lightscreen;
 pliant_t* player;
 int client;
 int color, level;
 int x, y, xs, ys;
 
 if (!session.open) {
  renderdue(screen);
  return;
 }
 
 client = getgameclient(session.self);
 
 if (client == INVALIDCLIENT) {
  renderdue(screen);
  return;
 }
 
 level = session.clients[client].level;
 
 bindlevel(&session.levels[level]);
 
 if (!hasunit(session.clients[client].entity)) {
  return;
 }
 
 player = (pliant_t*) getunit(session.clients[client].entity);
 
 xs = player->x;
 ys = player->y;
 
 centerfocus(&xs, &ys, &session.levels[level], screen);
 
 if (level > 3) {
  color = getcolor(20, 20, 121, 121);
  
  for (y = 0; y < 14; y++) {
   for (x = 0; x < 24; x++) {
	rendersprite(x * 8 - ((xs / 4) & 7), y * 8 - ((ys / 4) & 7), 0, color, 0, screen);
   }
  }
 }
 
 renderbackground(xs, ys, screen);
 rendersprites(xs, ys, screen);
 
 if (level > 3) {
  clearscreen(&lightscreen, 0);
  renderlights(xs, ys, &lightscreen);
  overlayscreens(screen, &lightscreen, xs, ys);
 }
 
 renderGUI(player->id, screen);
 
 bindlevel(NULL);
 
 return;
}

void renderhost(screen_t* screen) {
 
}

void tickgame() {
 int ticks;
 
 if (!session.open) {
  return;
 }
 
 handlehost();
 
 ticks = readtimer(&session.timer);
 
 while (ticks > 0) {
  assortlevels();
  
  session.ticks++;
  
  ticks -= 1;
 }
 
 updatehost();
 
 bindlevel(NULL);
 
 return;
}
