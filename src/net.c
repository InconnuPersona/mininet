#include "nethost.h"

#define HEADERLENGTH (sizeof(int) * 4)

// ==================================================
// declarations

UDPpacket* transmission = NULL;

// ==================================================
// network functions

void joinhost(const char* address, int port, int local) {
 if (host.usable) {
  LOGREPORT("host already opened.");
  return;
 }
 
 host.socket = SDLNet_UDP_Open(local);
 
 SDLNULLPRONE(host.socket, "'SDL2' network error");
 
 SDLNEGATIVEPRONE(SDLNet_ResolveHost(&host.address, address, port), "'SDL2' network error");
 
 transmission = SDLNet_AllocPacket(MAX_PACKETLENGTH);
 
 SDLNULLPRONE(transmission, "'SDL2' network error");
 
 transmission->address.host = host.address.host;
 transmission->address.port = host.address.port;
 
 memset(transmission->data, 0, transmission->maxlen);
 
 SDLNEGATIVEPRONE(SDLNet_UDP_Bind(host.socket, 0, &host.address), "'SDL2' network error");
 
 host.type = HOST_CLIENT;
 host.last = currenttime();
 host.id = 0;
 
 LOGREPORT("connecting to %s:%i.", address, port);
 
 shakehands(host.address, EXT_RESERVE, 0);
 
 return;
}

void openhost(int port) {
 if (host.usable) {
  LOGREPORT("host already opened.");
  return;
 }
 
 host.socket = SDLNet_UDP_Open(port);
 
 SDLNULLPRONE(host.socket, "'SDL2' network error");
 
 SDLNEGATIVEPRONE(SDLNet_ResolveHost(&host.address, 0, port), "'SDL2' network error");
 
 transmission = SDLNet_AllocPacket(MAX_PACKETLENGTH);
 
 SDLNULLPRONE(transmission, "'SDL2' network error");
 
 transmission->address.host = host.address.host;
 transmission->address.port = host.address.port;
 
 memset(transmission->data, 0, transmission->maxlen);
 
 SDLNet_UDP_Unbind(host.socket, 0);
 
 clients[BROADCASTCLIENT].usable = 1;
 
 host.type = HOST_LOCAL;
 host.last = 0;
 host.id = randomid();
 
 host.usable = 1;
 
 LOGREPORT("opening [localhost] 127.0.0.1:%i under host ID '%i' [%x].", port, host.id, host.id);
 
 return;
}

void cleartransmission() {
 CHECKNETWORKHOST(return);
 
 memset(transmission->data, 0, transmission->maxlen);
 
 return;
}

void closehost() {
 if (host.socket) {
  SDLNet_UDP_Close(host.socket);
 }
 
 if (transmission) {
  SDLNet_FreePacket(transmission);
 }
 
 memset(&clients, 0, sizeof(client_t) * (MAX_CLIENTS + 1));
 memset(&host, 0, sizeof(host_t));
 
 return;
}

void copymessage(message_t* from, message_t* to) {
 memcpy(to, from, sizeof(message_t));
}

void directmessage(message_t* message, int id, void* data, int length) {
 message->type = id;
 message->length = length;
 message->data.pointer = data;
 
 return;
}

void packheader(int type, int length) {
 abstract_u abstract;
 
 CHECKNETWORKHOST(return);
 
 abstract.pointer = transmission->data;
 
 abstract.integers[0] = type;
 abstract.integers[1] = host.id;
 abstract.integers[2] = transmission->len = HEADERLENGTH + length;
 abstract.floats[3] = currenttime();
 
 return;
}

void postmessages(packet_t* packet, IPaddress address) {
 abstract_u data;
 int buffered, bufferlength, i, index, length, messagebufferlength, sent;
 
 CHECKUSABLEHOST(return);
 
 buffered = 0;
 sent = 0;
 
 while (sent < packet->messagecount) {
  data.pointer = (void*) (transmission->data + HEADERLENGTH);
  
  bufferlength = index = length = messagebufferlength = 0;
  
  while (sent + index < packet->messagecount) {
   length += sizeof(message_t) + packet->messages[sent].length;
   
   if (length > MAX_PACKETLENGTH) {
	goto postmessages_send;
   }
   else {
    bufferlength += packet->messages[sent + index].length;
    messagebufferlength += sizeof(message_t);
   }
   
   index++;
  }
  
 postmessages_send:
  packheader(NETMSG_MESSAGES, bufferlength + messagebufferlength + HEADERLENGTH);
  
  data.integers[0] = packet->session;
  data.integers[1] = packet->client;
  data.integers[2] = bufferlength;
  data.integers[3] = index;
  
  data.pointer += HEADERLENGTH;
  
  for (i = 0; i < index; i++) {
   packet->messages[sent + i].data.integer -= buffered;
  }
  
  memcpy(data.pointer, &packet->messages[sent], messagebufferlength);
  
  data.pointer += messagebufferlength;
  
  memcpy(data.pointer, &host.reserve[buffered], bufferlength);
  
  LOGREPORT("sent %i message(s) with %i bytes.", index, bufferlength);
  
  sendpacket(address);
  
  sent += index;
  buffered += bufferlength;
 }
 
 return;
}

void printmessagecontent(message_t* message) {
 int i;
 
 LOGREPORT("message content;\n Type: %i\n Length: %i", message->type, message->length);
 
 printf(" Data:");
 
 for (i = 0; i < message->length; i++) {
  printf(" %02x", host.reserve[message->data.integer + i]);
 }
 
 printf("\n String: %s\n", &host.reserve[message->data.integer]);
}

void printpacketcontent() {
 int i;
 
 CHECKNETWORKHOST(return);
 
 LOGREPORT("packet content;\n Channel: %i\n Length: %i", transmission->channel, transmission->len);
 
 printf(" Data:");
 
 for (i = 0; i < transmission->len; i++) {
  printf(" %02x", transmission->data[i]);
 }
 
 printf("\n Integers:");
 
 for (i = 0; i < transmission->len / sizeof(int); i++) {
  printf(" %i", transmission->data[i * sizeof(int)]);
 }
 
 printf("\n String: %s\n", transmission->data);
}

void receivepackets() {
 abstract_u data;
 packet_t packet;
 
 CHECKNETWORKHOST(return);
 
 while (SDLNet_UDP_Recv(host.socket, transmission) > 0) {
  unpackheader(&packet);
  
  //LOGDEBUG("received packet [%i].", packet.type);
  
  //printpacketcontent();
  
  data.pointer = transmission->data + HEADERLENGTH;
  
  if (packet.length + HEADERLENGTH != transmission->len) {
   LOGREPORT("received nonuniform packet lengths; packet length [%i] vs. UDP packet length [%i].", packet.length, transmission->len);
   continue;
  }
  
  host.last = packet.posttime;
  
  if (packet.type == NETMSG_HANDSHAKE) {
   receivehandshake(data.integers[0], data.integers[1], &packet, transmission->address);
  }
  else if (packet.type == NETMSG_MESSAGES) {
   // packets sent over are expected to fit inside the limits of a full packet; therefore packets do not need division.
   unpackmessages(&packet);
   routepacket(&packet);
  }
  else {
   LOGREPORT("received unknown packet.");
   continue;
  }
 }
}

void sendpacket(IPaddress address) {
 CHECKNETWORKHOST(return);
 
 if (transmission->len < 0) {
  LOGREPORT("attempted to send a negative sized packet.");
  return;
 }
 
 transmission->address = address;
 
 if (!SDLNet_UDP_Send(host.socket, -1, transmission)) {
  LOGREPORT("could not transmit packet - %s.", SDLNet_GetError());
  return;
 }
 
 //LOGREPORT("sent packet [%i].", (int*) transmission->data[0])
 
 return;
}

void shakehands(IPaddress address, int type, int extra) {
 int* data;
 
 CHECKNETWORKHOST(return);
 
 packheader(NETMSG_HANDSHAKE, sizeof(int) * 2);
 
 data = (int*) (transmission->data + HEADERLENGTH);
 
 data[0] = type;
 data[1] = extra;
 
 LOGREPORT("sent handshake [%i][%x].", type, extra);
 
 sendpacket(address);
}

void unpackheader(packet_t* packet) {
 abstract_u abstract;
 
 CHECKNETWORKHOST(return);
 
 abstract.pointer = transmission->data;
 
 packet->type = abstract.integers[0];
 packet->sender = abstract.integers[1];
 packet->length = abstract.integers[2] - HEADERLENGTH;
 packet->posttime = abstract.floats[3];
 
 return;
}

void unpackmessages(packet_t* packet) {
 abstract_u abstract;
 
 CHECKUSABLEHOST(return);
 
 unpackheader(packet);
 
 abstract.pointer = transmission->data + HEADERLENGTH;
 
 packet->session = abstract.integers[0];
 packet->client = abstract.integers[1];
 packet->bufferlength = abstract.integers[2];
 packet->messagecount = abstract.integers[3];
 
 abstract.pointer += sizeof(int) * 4;
 
 memcpy(packet->messages, abstract.pointer, packet->messagecount * sizeof(message_t));
 
 abstract.pointer += packet->messagecount * sizeof(message_t);
 
 memcpy(host.reserve, abstract.pointer, packet->bufferlength);
 
 LOGREPORT("unpacked message packet.");
 
 printpacketcontent();
 
 return;
}
