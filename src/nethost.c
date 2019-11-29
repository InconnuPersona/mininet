#include "host.h"
#include "nethost.h"

// ==================================================
// declarations

client_t clients[MAX_CLIENTS + 1] = { 0 };
host_t host = { 0 };

// ==================================================
// functions

void adjoinmessage(message_t* message, queue_t* queue, int reliable) {
 if (!message || !queue) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 CHECKMESSAGE(message, return);
 
 if (reliable && queue->reliablecount > -1 && queue->reliablecount < MAX_MESSAGES) {
  copymessage(message, &queue->reliable[queue->reliablecount]);
  
  emplacemessage(&queue->reliable[queue->reliablecount]);
  
  queue->reliablecount++;
 }
 else if (!reliable && queue->messagecount > -1 && queue->messagecount < MAX_MESSAGES) {
  copymessage(message, &queue->messages[queue->messagecount]);
  
  emplacemessage(&queue->messages[queue->messagecount]);
  
  queue->messagecount++;
 }
 else {
  LOGREPORT("queue overflowed.");
  return;
 }
 
 queue->queued = 1;
}

void appendclientmessage(message_t* message, int reliable, refer_t client) {
 if (host.type != HOST_LOCAL) {
  LOGREPORT("invalid host.");
  return;
 }
 
 CHECKMESSAGE(message, return);
 
 client = fetchclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("invalid client.");
  return;
 }
 
 adjoinmessage(message, &clients[client].queue, reliable);
}

void appendmessage(message_t* message, int reliable) {
 if (host.type != HOST_CLIENT) {
  LOGREPORT("invalid host.");
  return;
 }
 
 CHECKMESSAGE(message, return);
 
 adjoinmessage(message, &host.queue, reliable);
}

int clientstate(refer_t client) {
 client = fetchclient(client);
 
 if (client == INVALIDCLIENT) {
  return CLIENT_FAULT;
 }
 
 return clients[client].usable;
}

int countclients() {
 int count, i;
 
 count = 0;
 
 for (i = 1; i < MAX_CLIENTS + 1; i++) {
  if (clients[i].usable == CLIENT_READY) {
   count++;
  }
 }
 
 return count;
}

void dropclient(refer_t client, int reason) {
 client = fetchclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("received invalid client.");
  return;
 }
 
 shakehands(clients[client].address, EXT_GOODBYE, reason);
 
 LOGREPORT("dropped client [%x] at index %i.", clients[client].id, client);
 
 memset(&clients[client], 0, sizeof(client_t));
 
 return;
}

void emplacemessage(message_t* message) {
 int byte, i, j, k, l, need;
 
 CHECKMESSAGE(message);
 
 if (message->length == 0) {
  message->length = message->stored;
 }
 
 need = ceil((float) message->length / SECTIONANGTH);
 
 k = -1;
 l = 0;
 
 for (i = 0; i < MAX_POOLSECTIONS; i++) {
  byte = i / BYTEWIDTH;
  j = i - byte * BYTEWIDTH;
  
  if (!(host.used[byte] & (1 << j))) {
   if (k < 0) { 
	k = i;
   }
   else {
	l++;
   }
  }
  else {
   k = -1;
   l = 0;
  }
  
  if (l >= need) {
   markstretch(k, l);
   
   memcpy(&host.reserve[k * SECTIONANGTH], message->data.bytes, message->length);
   
   message->data.integer = k * SECTIONANGTH;
   
   return;
  }
 }
 
 LOGREPORT("failed to place message into host reserve buffer.");
 
 return;
}

void entrustmessages(packet_t* packet, client_t* holder) {
 if (!holder || !packet) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 packet->flags |= PKTFLG_RELIABLE;
 packet->sequence = holder->sequence;
 
 memcpy(&holder->held[holder->sequence % NETWORKRATE].packet, packet, sizeof(packet_t));
 memcpy(&holder->held[holder->sequence % NETWORKRATE].reserve, host.reserve, MAX_POOLLENGTH);
 
 postmessages(packet, holder->address);
 
 holder->sequence++;
 
 return;
}

int fetchaddressedclient(IPaddress address) {
 int i;
 
 for (i = 1; i < MAX_CLIENTS + 1; i++) {
  if (clients[i].address.host == address.host && clients[i].address.port == address.port) {
   return i;
  }
 }
 
 return INVALIDCLIENT;
}

int fetchclient(refer_t id) {
 int i;
 
 if (!id) {
  return INVALIDCLIENT;
 }
 
 for (i = 1; i < MAX_CLIENTS + 1; i++) {
  if (clients[i].id == id) {
   return i;
  }
 }
 
 return INVALIDCLIENT;
}

void handlequeues() {
 if (host.type != HOST_UNMADE && host.socket) {
  receivepackets();
 }
}

int hasclient(refer_t client) {
 return fetchclient(client) != INVALIDCLIENT;
}

int hoststate() {
 return host.type;
}

void marksection(int index) {
 int i, j;
 
 i = index / SECTIONANGTH / BYTEWIDTH;
 j = index - (i * SECTIONANGTH * BYTEWIDTH);
 
 host.used[i] |= 1 << j;
}

void markstretch(int start, int length) {
 int end, i, j, k, l;
 byte_t mask;
 
 end = start + length;
 
 if (end >= MAX_POOLLENGTH / SECTIONANGTH) {
  LOGREPORT("marks exceed host reserve length.");
  return;
 }
 
 for (i = start / BYTEWIDTH; i < ceil((float) end / BYTEWIDTH); i++) {
  mask = 0;
  
  j = start - i * BYTEWIDTH;
  
  if (j < 0) {
   j = 0;
  }
  
  k = end - i * BYTEWIDTH;
  
  if (k > BYTEWIDTH) {
   k = BYTEWIDTH;
  }
  
  for (l = j; l < k; l++) {
   //mask |= 1 << (BYTEWIDTH - 1 - l);
   mask |= 1 << l;
  }
  
  host.used[i] |= mask;
 }
 
 return;
}

int matchaddress(IPaddress address, IPaddress other) {
 return (address.host == other.host) && (address.port == other.port);
}

int newclientid() {
 int id;
 
 do {
  id = randomid();
  
  if (hasclient(id) || id == host.id) {
   continue;
  }
  
  return id;
 }
 while (1);
}

void receivehandshake(int type, int extra, packet_t* packet, IPaddress address) {
 int client, i;
 
 if (!packet) {
  LOGREPORT("received invalid packet.");
  return;
 }
 
 if (host.type == HOST_LOCAL) {
  client = INVALIDCLIENT;
  
  if (type == EXT_RESERVE) {
   if (fetchaddressedclient(address) != INVALIDCLIENT) {
	shakehands(address, EXT_ALREADYMET, 0);
	return;
   }
   
   for (i = 1; i < MAX_CLIENTS + 1; i++) {
	if (clients[i].usable == CLIENT_EMPTY) {
	 client = i;
	 break;
	}
   }
   
   if (client != INVALIDCLIENT) {
	clients[client].address = address;
	clients[client].last = currenttime();
	clients[client].id = newclientid();
	clients[client].usable = CLIENT_READY;
	
	LOGREPORT("accepted client to client ID [%x].", clients[client].id);
	
	shakehands(address, EXT_ACCEPT, clients[client].id);
   }
   else {
	shakehands(address, EXT_NOSPACE, MAX_CLIENTS);
	return;
   }
  }
  else if (type == EXT_MEETUP) {
   if (extra != host.id) {
	LOGREPORT("received invalid host id [%x].", extra);
	return;
   }
   
   for (i = 1; i < MAX_CLIENTS + 1; i++) {
   	if (matchaddress(address, clients[i].address) && clients[i].id == packet->sender) {
   	 clients[i].last = currenttime();
   	 return;
   	}
   }
  }
  else {
   LOGREPORT("received unhandled host handshake.");
   return;
  }
 }
 else if (host.type == HOST_CLIENT) {
  if (type == EXT_ACCEPT) {
   host.id = extra;
   host.remoteid = packet->sender;
   host.usable = 1;
   
   LOGREPORT("connection accepted under client ID [%x] to host ID [%x].", host.id, host.remoteid);
  }
  else if (type == EXT_MEETUP) {
   shakehands(host.address, EXT_MEETUP, host.remoteid);
  }
  else {
   LOGREPORT("client handshake returned [%x][%x].", type, extra);
   return;
  }
 }
}

void updatequeue(client_t* holder, client_t* broadcast) {
 packet_t packet;
 int i, j;
 
 if (!holder) {
  LOGREPORT("received invalid client holder.");
  return;
 }
 
 // reliable messages
 {
  for (i = 0; broadcast && broadcast->usable && i < broadcast->queue.reliablecount; i++) {
   copymessage(&broadcast->queue.reliable[i], &packet.messages[i]);
  }
  
  packet.messagecount = i;
  
  if (holder->queue.queued) {
   for (j = 0; j < holder->queue.reliablecount; j++) {
	copymessage(&holder->queue.reliable[j], &packet.messages[i + j]);
   }
   
   packet.messagecount += holder->queue.reliablecount;
   
   holder->queue.reliablecount = 0;
  }
  
  if (packet.messagecount > 0) {
   entrustmessages(&packet, holder);
  }
 }
 
 // unreliable messages
 {
  for (i = 0; broadcast && broadcast->usable && i < broadcast->queue.messagecount; i++) {
   copymessage(&broadcast->queue.messages[i], &packet.messages[i]);
  }
  
  packet.messagecount = i;
  
  if (holder->queue.queued) {
   for (j = 0; j < holder->queue.messagecount; j++) {
	copymessage(&holder->queue.messages[j], &packet.messages[i + j]);
   }
   
   packet.messagecount += holder->queue.messagecount;
   
   holder->queue.messagecount = 0;
  }
  
  if (packet.messagecount > 0) {
   postmessages(&packet, holder->address);
  }
 }
 
 holder->queue.queued = 0;
}

void updatequeues() {
 client_t* broadcast;
 packet_t packet;
 float time;
 int i;
 
 if (!host.usable || host.type == HOST_UNMADE || !host.socket) {
  return;
 }
 
 memset(&packet, 0, sizeof(packet_t));
 
 packet.sender = host.id;
 
 time = currenttime();
 
 if (host.type == HOST_CLIENT) {
  updatequeue((client_t*) &host, NULL);
 }
 else if (host.type == HOST_LOCAL) {
  broadcast = &clients[BROADCASTCLIENT];
  
  for (i = 1; i < MAX_CLIENTS + 1; i++) {
   if (clients[i].usable == CLIENT_READY) {
	if (time - clients[i].last >= TIMEOUT) {
	 dropclient(clients[i].id, EXT_TIMEOUT);
	 continue;
	}
	else if (cutoff(time - clients[i].last, 5) <= 1.f / NETWORKRATE) {
	 shakehands(clients[i].address, EXT_MEETUP, clients[i].id);
	}
	
	updatequeue(&clients[i], broadcast);
   }
   else if (clients[i].usable == CLIENT_FAULT) {
	dropclient(clients[i].id, EXT_FAULTED);
   }
  }
  
  broadcast->queue.messagecount = 0;
  broadcast->queue.reliablecount = 0;
  broadcast->queue.queued = 0;
 }
 else {
  LOGREPORT("attempted to update unknown host state.");
 }
 
 return;
}
