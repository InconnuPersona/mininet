#include "host.h"

void renderdue(screen_t* screen) {
 int color, ofs;
 
 clearscreen(screen, 0);
 
 color = getcolor(0, 555, 555, 555);
 ofs = 1 * TILESCALE;
 
 renderfont("Casting the skies", ofs, 2 * TILESCALE, color, screen);
 renderfont("and creating the", ofs, 3 * TILESCALE, color, screen);
 renderfont("grounds;", ofs, 4 * TILESCALE, color, screen);
 
 renderfont("bringing all", ofs, 6 * TILESCALE, color, screen);
 renderfont("earthwrought", ofs, 7 * TILESCALE, color, screen);
 renderfont("from the", ofs, 8 * TILESCALE, color, screen);
 renderfont("underground.", ofs, 9 * TILESCALE, color, screen);
 
 renderfont("and waiting...", ofs, 12 * TILESCALE, color, screen);
 
 return;
}

void rendergame(screen_t* screen) {
 int depth;
 int xs, ys;
 
 if (!session.open) {
  renderdue(screen);
  return;
 }
 
 depth = 0;

 bindlevel(depth);

 centerfocus(&xs, &ys, screen);
 
 xs = 0;
 ys = 0;
 
 renderlevel(depth, xs, ys, screen);
 
 //renderGUI(player->id, screen);
 
}

// TODO: handle game client spawn effectively; return unit referral
void spawngameclient(refer_t client, int depth) {
 client = getgameclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("received unknown gameclient id.");
  return;
 }
 
 if (depth < 0 || depth >= MAX_LEVELS) {
  LOGREPORT("received out of bounds level index.");
  return;
 }
 
 bindlevel(depth);
 
 session.clients[client].level = depth;
 session.clients[client].entity = spawn("Player");
 
 return;
}

void startgame(gametype_e type, const char* name, const char* address, int port) {
 if (session.open) {
  LOGREPORT("game session already open.");
  return;
 }
 
 switch (type) {
 case GAME_CLIENT:
  //joinhost(address, port, 54321);
  
  //chainlevels(0);
  
  //session.id = 0;
  //session.self = putgameclient(name, LOCALCLIENT);
  
  break;
  
 case GAME_HOST:
  //openhost(port);
  /* no break */
  
 case GAME_PRIVATE:
  createlevels(LEVELANGTH, LEVELANGTH, randominteger(0xffff));
  
  session.id = randomid();
  
  LOGREPORT("opened session under id [%x].", session.id);
  
  // Add a new client
  session.self = newgameclient(name, LOCALCLIENT);
  
  spawngameclient(session.self, SPAWNLEVEL);
  
//  directmessage(&m, data, sizeof(data));
//  
//  for (int i = 0; i < 1024; i++) {
//   appendclientmessage(&m, 0, BROADCAST);
//  }
  
  break;
  
 default:
  LOGREPORT("invalid session type.");
  return;
 }
 
 // TODO: improve timer accuracy
 settimer(&session.timer, TIMER_SPACEDLAPSE, CURRENTTIME, 1.f / GAMERATE);
 
 session.marker = GAMESMARKER;
 session.open = true;
 session.ticks = 0;
 session.type = type;
 session.version = GAMEVERSION;
 
 return;
}

void tickgame() {
 if (!session.open) {
  return;
 }
 
 session.ticks++;
}
