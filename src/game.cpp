#include "host.h"

int Game::getInput() {
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

void Game::render(screen_t* screen) {
 Unit* player;
 int depth;
 int xs, ys;
 
 if (!session.open) {
  renderdue(screen);
  return;
 }
 
 depth = 0;

 player = level.getUnit(clients[LOCALCLIENT].entity);

 level.bind(player->depth());

 if (!player) {
  return;
 }

 xs = player->x;
 ys = player->y;

 centerfocus(&xs, &ys, level.w(), level.h(), screen);
 
 level.render(xs, ys, screen);
 
 //renderGUI(player->id, screen);
 
}

// TODO: handle game client spawn effectively; return unit referral
void Game::spawn_client(refer_t client, int depth) {
 client = get_client(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("received unknown gameclient id.");
  return;
 }
 
 if (depth < 0 || depth >= MAX_LEVELS) {
  LOGREPORT("received out of bounds level index.");
  return;
 }
 
 level.bind(depth);
 
 clients[client].level = depth;
 clients[client].entity = level.spawn("Player");
 
 return;
}

void Game::start(gametype_e type, const char* name, const char* address, int port) {
 if (open) {
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
  openhost(port);
  /* no break */
  
 case GAME_PRIVATE:
  id = randomid();
  
  level.generate(LEVELANGTH, LEVELANGTH, randominteger(0xffff));
  
  LOGREPORT("opened session under id [%x].", session.id);
  
  // Add a new client
  self = new_client(name, LOCALCLIENT);
  
  spawn_client(self, SPAWNLEVEL);
  
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
 settimer(&timer, TIMER_SPACEDLAPSE, CURRENTTIME, 1.f / GAMERATE);
 
 marker = GAMESMARKER;
 open = true;
 ticks = 0;
 version = GAMEVERSION;
 
 this->type = type;

 return;
}

void Game::tick_client(refer_t id) {
 Unit* player;
 int commands;
 int i;
 
 i = get_client(id);
 
 //if (awaited()) {
  //if (session.ticks % 5 == 0) {
   //pushjoin(session.clients[client].name);
  //}
 //}
 //else {
  commands = getInput();
  
  if (i != INVALIDCLIENT) {
   player = level.getUnit(clients[i].entity);
   
   if (!player) {
	return;
   }
   
   player->input(commands);

   /*if (session.clients[client].inmenu) {
	ticksurface(session.clients[client].inmenu, session.clients[client].entity);
	
	player->pliant.commands = commands = 0;
   }
  }
  
  if (session.type == GAME_CLIENT) {
   if (!session.clients[client].inmenu) {
	if (commands != oldcommand) {
	 pushcommands(commands);
	}
   }*/

   clients[i].old_cmds = commands;
  }
 //}
 
 return;
}

void Game::tick() {
 if (!open) {
  return;
 }
 
 tick_client(LOCALCLIENT);

 level.tick();

 //if (type == GAME_HOST) {
  // check client synchronizations
 //}
 
 ticks++;
}
