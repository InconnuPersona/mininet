#pragma once

#include "base.h"

#include <SDL2/SDL_net.h>

#define BROADCAST -2
#define INVALIDCLIENT -1
#define MAX_MESSAGES 128
#define NETWORKRATE 20

struct NetHost {
 struct Client {
 };
 
 bool open;
 refer_t id;
 int last;

 enum Type {
  UNMADE = 0,
  CLIENT,
  LOCAL
 } type;

 IPaddress address;

 UDPpacket* packet;
 UDPsocket socket;

 void handle();
 void update();
};

extern NetHost net;

void openhost(int port);
