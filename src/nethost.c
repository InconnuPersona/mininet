#include "main.h"
#include "nethost.h"

// ==================================================
// declarations

client_t clients[MAX_CLIENTS + 1] = { 0 };
host_t host = { 0 };

// ==================================================
// functions

void adjoinmessage(message_t* message, queue_t* queue) {
 if (queue->count > -1 && queue->count < MAX_MESSAGES) {
  copymessage(message, &queue->messages[queue->count]);
  
  emplacemessage(&queue->messages[queue->count]);
  
  queue->count++;
  queue->queued = 1;
 }
 else {
  LOGREPORT("queue overflowed.");
 }
}

void appendclientmessage(message_t* message, int client) {
 if (host.type != HOST_LOCAL) {
  LOGREPORT("invalid host.");
  return;
 }
 
 client = fetchclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("invalid client.");
  return;
 }
 
 adjoinmessage(message, &clients[client].queue);
}

void appendmessage(message_t* message) {
 if (host.type != HOST_CLIENT) {
  LOGREPORT("invalid host.");
  return;
 }
 
 if (!message) {
  LOGREPORT("received null message.");
  return;
 }
 
 adjoinmessage(message, &host.queue);
}

int clientstate(int client) {
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

//dropclient(client, reason)
void dropclient(int client) {
 client = fetchclient(client);
 
 if (client == INVALIDCLIENT) {
  LOGREPORT("invalid client.");
  return;
 }
 
 shakehands(clients[client].address, EXT_GOODBYE, EXT_GOODBYE);
 
 memset(&clients[client], 0, sizeof(client_t));
 
 return;
}

void emplacemessage(message_t* message) {
 int byte, i, j, k, l, need;
 
 need = ceil((float) message->length / SECTIONANGTH);
 
 k = -1;
 l = 0;
 
 for (i = 0; i < MAX_POOLLENGTH / SECTIONANGTH; i++) {
  byte = i / BYTEWIDTH;
  j = i - byte * BYTEWIDTH;
  
  if ((message->data.bytes[byte] & (1 << j)) == 0) {
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
   
   //printf("reserve data:\n");
   
   //for (i = 0; i < MAX_POOLLENGTH; i++) {
	//printf(" %02x", host.reserve[i]);
   //}
   
   //printf("\n");
   
   return;
  }
 }
 
 LOGREPORT("failed to place message into host reserve buffer.");
 
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

int fetchclient(int id) {
 int i;
 
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
 
 for (i = start / BYTEWIDTH; i < end / BYTEWIDTH; i++) {
  mask = 0;
  
  j = start - i * BYTEWIDTH;
  
  if (j < 0) {
   j = 0;
  }
  
  k = end - i * BYTEWIDTH;
  
  if (k >= BYTEWIDTH) {
   k = BYTEWIDTH;
  }
  
  for (l = j; l < k; l++) {
   mask |= 1 << (BYTEWIDTH - l - 1);
  }
  
  host.used[i] |= mask;
 }
 
 return;
}

int newclientid() {
 int i, id;
 
 do {
  id = randomid();
  
  if (id == host.id) {
   continue;
  }
  
  for (i = 1; i < MAX_CLIENTS + 1; i++) {
   if (id == clients[i].id) {
	continue;
   }
  }
  
  return id;
 }
 while (1);
}

int matchaddress(IPaddress address, IPaddress other) {
 return (address.host == other.host) && (address.port == other.port);
}

void receivehandshake(int type, int extra, packet_t* packet, IPaddress address) {
 int client, i;
 
 if (host.type == HOST_LOCAL) {
  client = INVALIDCLIENT;
  
  if (type == EXT_RESERVE) {
   for (i = 1; i < MAX_CLIENTS + 1; i++) {
	if (matchaddress(address, clients[i].address)) {
	 shakehands(address, EXT_ALREADYMET, 0);
	 return;
	}
	
	if (client == INVALIDCLIENT && clients[i].usable == CLIENT_EMPTY) {
	 client = i;
	}
   }
   
   if (client != INVALIDCLIENT) {
	clients[client].address = address;
	clients[client].last = packet->posttime;
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
   	 clients[i].last = packet->posttime;
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

void renewqueue(queue_t* queue, const char* string) {
 queue->label = reprintstring(string);
 queue->queued = 0;
 queue->count = 0;
 
 return;
}

void updatequeues() {
 client_t* broadcast;
 packet_t packet;
 float time;
 int i, j, k;
 
 if (!host.usable || host.type == HOST_UNMADE || !host.socket) { \
  return;
 }
 
 time = currenttime();
 
 memset(&packet, 0, sizeof(packet_t));
 
 packet.sender = host.id;
 
 if (host.type == HOST_CLIENT) {
  if (host.queue.queued && host.queue.count > 0) {
   for (j = 0; j < host.queue.count; j++) {
	copymessage(&host.queue.messages[j], &packet.messages[j]);
   }
   
   packet.messagecount = j;
   
   postmessages(&packet, host.address);
   
   host.queue.count = 0;
   host.queue.queued = 0;
  }
 }
 else if (host.type == HOST_LOCAL) {
  broadcast = &clients[BROADCASTCLIENT];
  
  for (i = 1; i < MAX_CLIENTS + 1; i++) {
   if (clients[i].usable == CLIENT_READY) {
	if (time - clients[i].last >= TIMEOUT) {
	 dropclient(clients[i].id);
	 
	 continue;
	}
	else if (cutoff(time - clients[i].last, 5) <= 1.f / GAMERATE) {
	 shakehands(clients[i].address, EXT_MEETUP, clients[i].id);
	}
	
	for (k = 0; broadcast->usable && k < broadcast->queue.count; k++) {
	 copymessage(&broadcast->queue.messages[k], &packet.messages[k]);
	}
	
	packet.messagecount = k;
	
	if (clients[i].queue.queued) {
	 for (j = 0; j < clients[i].queue.count; j++) {
	  copymessage(&clients[i].queue.messages[j], &packet.messages[k + j]);
	 }
	 
	 packet.messagecount += clients[i].queue.count;
	 
	 clients[i].queue.count = 0;
	 clients[i].queue.queued = 0;
	}
	
	if (packet.messagecount > 0) {
	 postmessages(&packet, clients[i].address);
	 
	 packet.messagecount = 0;
	}
   }
   else if (clients[i].usable == CLIENT_FAULT) {
	shakehands(clients[i].address, EXT_GOODBYE, EXT_FAULTED);
	
	memset(&clients[i], 0, sizeof(client_t));
   }
  }
  
  broadcast->queue.count = 0;
  broadcast->queue.queued = 0;
 }
 
 memset(&host.used, 0, MAX_POOLSECTIONS);
 
 return;
}
