#include "net.h"

#define MAX_PACKETLENGTH 1024

NetHost net;

void openhost(int port) {
 if (net.open) {
  LOGREPORT("host already opened.");
  return;
 }
 
 SDLNEGATIVEPRONE(SDLNet_ResolveHost(&net.address, 0, port), "'SDL2' network error");
 
 net.socket = SDLNet_UDP_Open(port);
 
 SDLNULLPRONE(net.socket, "'SDL2' network error");
 
 net.packet = SDLNet_AllocPacket(MAX_PACKETLENGTH);
 
 SDLNULLPRONE(net.packet, "'SDL2' network error");
 
 net.packet->address.host = net.address.host;
 net.packet->address.port = net.address.port;
 
 memset(net.packet->data, 0, net.packet->maxlen);
 
 SDLNet_UDP_Unbind(net.socket, 0);
 
 net.type = NetHost::LOCAL;
 net.last = 0;
 net.id = randomid();
 
 net.open = true;
 
 LOGREPORT("opening [localhost] 127.0.0.1:%i under host ID '%i' [%x].", port, net.id, net.id);
 
 return;
}

void NetHost::handle() {
}

void NetHost::update() {
}
