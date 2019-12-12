#include "nethost.h"

#define HEADERLENGTH sizeof(packetheader_t)
#define MESSAGESOFFSET (transmission->data + HEADERLENGTH + sizeof(messagesheader_t))

// ==================================================
// structures

// ==================================================
// declarations

UDPpacket* transmission = NULL;
int sequence;

// ==================================================
// functions

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

void directmessage(message_t* message, void* data, int length) {
 if (!message) {
  LOGREPORT("received invalid message.");
  return;
 }
 
 if (length < 1 || length > SIGNEDEXTENT(short)) {
  LOGREPORT("received invalid message length.");
  return;
 }
 
 message->length = 0;
 message->data.pointer = data;
 message->stored = length;
 
 return;
}

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

void packheader(short type, int flags, int length) {
 packetheader_t* header;
 
 CHECKNETWORKHOST(return);
 
 header = (packetheader_t*) transmission->data;
 
 header->type = type;
 header->flags = flags;
 header->sequence = 0;
 header->sender = host.id;
 header->length = transmission->len = HEADERLENGTH + length;
 
 return;
}

// This function packs a packet for transmission and may divide the packet into transmissible
// sizes if necessary. The packets are automatically posted to the receiver and are cut to
// fit inside the constraints of a transmission so that it may be unpacked easily.
void postmessages(packet_t* packet, IPaddress address) {
 abstract_u data;
 message_t* message;
 int buffered, bufferlength, i, index, length;
 int messagebufferlength, sent;
 
 CHECKUSABLEHOST(return);
 
 if (!packet) {
  LOGREPORT("received invalid packet.");
  return;
 }
 
 buffered = 0;
 sent = 0;
 
 while (sent < packet->messagecount) {
  data.pointer = transmission->data + HEADERLENGTH;
  
  bufferlength = index = length = messagebufferlength = 0;
  
  while (sent + index < packet->messagecount) {
   length += MESSAGEWIDTH + packet->messages[sent + index].length;
   
   if (length > MAX_PACKETLENGTH - HEADERLENGTH) {
//	TODO: check if message buffer length exceeds host reserves.
//	if (length - MAX_PACKETLENGTH > 2 * MAX_PACKETLENGTH) {
//	 LOGREPORT("received intransmittable message.");
//	 exit(EXIT_FAILURE);
//	}
	
	goto postmessages_send;
   }
   else {
    bufferlength += packet->messages[sent + index].length;
    messagebufferlength += MESSAGEWIDTH;
   }
   
   index++;
  }
  
 postmessages_send:
  packheader(NETMSG_MESSAGES, packet->flags, bufferlength + messagebufferlength + sizeof(messagesheader_t));
  
  data.integers[0] = bufferlength;
  data.integers[1] = index;
  
  data.pointer += sizeof(messagesheader_t);
  
  for (i = 0; i < index; i++) {
   message = data.pointer + i * MESSAGEWIDTH;
   
   message->length = packet->messages[sent + i].length;
  }
  
  data.pointer += messagebufferlength;
  
  for (i = 0; i < index; i++) {
   message = (message_t*) (MESSAGESOFFSET + i * MESSAGEWIDTH);
   
   memcpy(data.bytes, host.reserve + packet->messages[sent + i].data.integer, packet->messages[sent + i].length);
   
   message->data.integer = data.bytes - (MESSAGESOFFSET + packet->messagecount * MESSAGEWIDTH);
   
   data.pointer += packet->messages[sent + i].length;
  }
  
  LOGDEBUG("sent %i message(s) with %i bytes.", index, bufferlength);
  
  sendpacket(address);
  
  sent += index;
  buffered += bufferlength;
 }
 
 return;
}

void printhostreserve() {
 int i;
 
 printf("host reserve;\n used:");
 
 for (i = 0; i < MAX_POOLSECTIONS; i++) {
  printf(" %02x", host.used[i]);
 }
 
 printf("\n data:");
 
 for (i = 0; i < MAX_POOLLENGTH; i++) {
  printf(" %02x", host.reserve[i]);
 }
 
 printf("\n string: '");
 
 for (i = 0; i < MAX_POOLLENGTH; i++) {
  printf("%c", host.reserve[i]);
 }
 
 printf("'\n");
 
 return;
}

// A simple utility function to reveal a message's content without machined interpretation.
void printmessagecontent(message_t* message) {
 int i;
 
 if (!message) {
  return;
 }
 
 LOGREPORT("message content;\n length: %i", message->stored);
 
 printf(" data:");
 
 for (i = 0; i < message->stored; i++) {
  printf(" %02x", host.reserve[message->data.integer + i]);
 }
 
 printf("\n string: '");
 
 for (i = 0; i < message->stored; i++) {
  printf("%c", host.reserve[message->data.integer + i]);
 }
 
 printf("'\n");
}

void printmessages(packet_t* packet) {
 int i, j;
 
 if (!packet) {
  LOGREPORT("received invalid packet.");
  return;
 }
 
 LOGREPORT("packet content;");
 
 printf(" type: %i; flags: %x; sequence: %i; sender: %x; length: %i;\n", packet->type, packet->flags, packet->sequence, packet->sender, packet->length);
 printf(" bufferlength: %i; messagecount: %i;\n", packet->bufferlength, packet->messagecount);
 
 for (i = 0; i < packet->messagecount; i++) {
  printf(" message [length: %i, data:", packet->messages[i].length);
  
  for (j = 0; j < packet->messages[i].length; j++) {
   printf(" %02x", packet->messages[i].data.bytes[j]);
  }
  
  printf(" or '");
  
  for (j = 0; j < packet->messages[i].length; j++) {
   printf("%c", packet->messages[i].data.bytes[j]);
  }
  
  printf("']\n");
 }
 
 return;
}

// A simple utility function to reveal transmitted content without machined interpretation.
void printpacketcontent() {
 int i;
 
 CHECKNETWORKHOST(return);
 
 if (!transmission) {
  return;
 }
 
 LOGREPORT("packet content;\n channel: %i\n length: %i", transmission->channel, transmission->len);
 
 printf(" data:");
 
 for (i = 0; i < transmission->len; i++) {
  printf(" %02x", transmission->data[i]);
 }
 
 printf("\n integers:");
 
 for (i = 0; i < transmission->len / sizeof(int); i++) {
  printf(" %i", transmission->data[i * sizeof(int)]);
 }
 
 printf("\n string: '");
 
 for (i = 0; i < transmission->len; i++) {
  printf("%c", transmission->data[i]);
 }
 
 printf("'\n");
 
 return;
}

int pullbytes(byte_t* bytes, int count, message_t* message) {
 CHECKMESSAGE(message, return 0);
 
 if (!bytes) {
  LOGREPORT("received unusable data pointer.");
  return 0;
 }
 
 if (count < 0) {
  LOGREPORT("received unusable data length.");
  return 0;
 }
 
 if (message->length - count < 0) {
  LOGREPORT("encountered message stack underflow for %i bytes.", count);
  return 0;
 }
 
 message->length -= count;
 
 memcpy(bytes, message->data.pointer + message->length, count);
 
 return 1;
}

int pullvalue(int* value, int size, message_t* message) {
 CHECKMESSAGE(message, return 0);
 
 if (!value) {
  LOGREPORT("received unusable value pointer.");
  return 0;
 }
 
 if (size < 0 || size > sizeof(*value)) {
  LOGREPORT("received unusable value size.");
  return 0;
 }
 
 if (message->length - size < 0) {
  LOGREPORT("encountered message stack underflow for %i bytes.", size);
  return 0;
 }
 
 message->length -= size;
 
 memcpy(value, message->data.pointer + message->length, size);
 
 return 1;
}

int pushbytes(const void* bytes, int count, message_t* message) {
 CHECKMESSAGE(message, return 0);
 
 if (!bytes) {
  LOGREPORT("received unusable data pointer.");
  return 0;
 }
 
 if (count < 1) {
  LOGREPORT("received unusable data length.");
  return 0;
 }
 
 if (message->length + count > message->stored) {
  LOGREPORT("encountered message stack overflow for %i bytes.", count);
  return 0;
 }
 
 memcpy(message->data.pointer + message->length, bytes, count);
 
 message->length += count;
 
 return 1;
}

int pushvalue(const int value, int size, message_t* message) {
 CHECKMESSAGE(message, return 0);
 
 if (size < 1 || size > sizeof(value)) {
  LOGREPORT("received unusable value size.");
  return 0;
 }
 
 if (message->length + size > message->stored) {
  LOGREPORT("encountered message stack overflow for %i bytes.", size);
  return 0;
 }
 
 memcpy(message->data.pointer + message->length, &value, size);
 
 message->length += size;
 
 return 1;
}

// This function receives transmissions and reroutes them into packets or handles them by
// the network rules; the data recorded for packet messages is only valid for the
// routepacket call made by this function and is unusable outside of its scope.
void receivepackets() {
 abstract_u data;
 packet_t packet;
 
 CHECKNETWORKHOST(return);
 
 while (SDLNet_UDP_Recv(host.socket, transmission) > 0) {
  LOGDEBUG("received packet [%i].", packet.type);
  
  unpackheader(&packet);
  
  if (packet.length + HEADERLENGTH != transmission->len) {
   LOGREPORT("received nonuniform packet lengths; packet length [%i] vs. UDP packet length [%i].", packet.length + HEADERLENGTH, transmission->len);
   continue;
  }
  
  data.pointer = transmission->data + HEADERLENGTH;
  
  if (packet.type == NETMSG_HANDSHAKE) {
   receivehandshake(data.integers[0], data.integers[1], &packet, transmission->address);
  }
  else if (packet.type == NETMSG_MESSAGES) {
   memset(host.used, 0, MAX_POOLWIDTH);
   
   unpackmessages(&packet); // packets sent over are expected to fit inside the limits of a full packet; therefore packets do not need division.
   
   routepacket(&packet);
  }
  else {
   LOGREPORT("received unknown packet.");
   continue;
  }
 }
}

// Note the UDP transmission is sent by this function, not a packet; more sensibly its
// name could be transmitpacket or transmit, but that is unecessary for these purposes.
void sendpacket(IPaddress address) {
 CHECKNETWORKHOST(return);
 
 if (transmission->len < 0) {
  LOGREPORT("attempted to send a negative sized packet.");
  return;
 }
 
 if (transmission->len > transmission->maxlen) {
  LOGREPORT("attempted to send an oversized packet of %i bytes.", transmission->len);
  return;
 }
 
 transmission->address = address;
 
 if (!SDLNet_UDP_Send(host.socket, -1, transmission)) {
  LOGREPORT("could not transmit packet - %s.", SDLNet_GetError());
  return;
 }
 
 LOGDEBUG("sent packet [%i].", ((int*) transmission->data)[0]);
 
 return;
}

// This function delivers a handshake message to its addressed receiver, which may be
// interpreted according to the receiving network. Handshakes are handled by the network
// and do not translate into routed messages for the game session; they are solely for the.
// host and client interface.
void shakehands(IPaddress address, int type, int extra) {
 int* data;
 
 CHECKNETWORKHOST(return);
 
 packheader(NETMSG_HANDSHAKE, PKTFLG_UNRELIABLE, sizeof(int) * 2);
 
 data = (int*) (transmission->data + HEADERLENGTH);
 
 data[0] = type;
 data[1] = extra;
 
 LOGDEBUG("sent handshake [%i][%x].", type, extra);
 
 sendpacket(address);
 
 return;
}

// This function unpacks the packet header and decodes the packet content length. It is
// preferred to retain this header across versions, since it must match the packheader
// call.
void unpackheader(packet_t* packet) {
 packetheader_t* header;
 
 CHECKNETWORKHOST(return);
 
 header = (packetheader_t*) transmission->data;
 
 packet->type = header->type;
 packet->flags = header->flags;
 packet->sequence = header->sequence;
 packet->sender = header->sender;
 packet->length = header->length - HEADERLENGTH;
 
 return;
}

// When packets are unpacked, their messages are translated into localized messages and adopt
// host pointer coordinates. The messages may then be freely used in a routed packet call,
// where the data may be passed off to the game session.
//// Check if received seuence number is in order ith 
void unpackmessages(packet_t* packet) {
 messagesheader_t* header;
 void* pointer;
 int i;
 
 CHECKUSABLEHOST(return);
 
 header = (messagesheader_t*) transmission->data + HEADERLENGTH;
 
 packet->bufferlength = header->bufferlength;
 packet->messagecount = header->messagecount;
 
 pointer = header + sizeof(messagesheader_t);
 
 for (i = 0; i < packet->messagecount; i++) {
  memcpy(&packet->messages[i], pointer, MESSAGEWIDTH);
  
  packet->messages[i].data.pointer += (int) host.reserve;
  
  pointer += MESSAGEWIDTH;
 }
 
 memcpy(host.reserve, pointer, packet->bufferlength);
 
 markstretch(0, packet->bufferlength / SECTIONANGTH);
 
 LOGDEBUG("unpacked message packet.");
 
 //printpacketcontent();
 //printhostreserve();
 //printmessages(packet);
 
 return;
}
