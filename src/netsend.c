#include "nethost.h"

#include <math.h>

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
 
 return;
}

void appendmessage(message_t* message, int reliable) {
 if (host.type != HOST_CLIENT) {
  LOGREPORT("invalid host.");
  return;
 }
 
 CHECKMESSAGE(message, return);
 
 adjoinmessage(message, &host.queue, reliable);
 
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
 
 if (!data) {
  LOGREPORT("received invalid message data pointer.");
  return;
 }
 
 if (length < 1 || length > SIGNEDEXTENT(short)) {
  LOGREPORT("received invalid message length.");
  return;
 }
 
 message->length = 0;
 message->data = data;
 message->stored = length;
 
 return;
}

void emplacemessage(message_t* message) {
 int byte, i, j, k, l, need;
 
 CHECKMESSAGE(message);
 
 if (message->length == 0) {
  message->length = message->stored;
 }
 
 need = ceil((float) message->length / CHUNKANGTH);
 
 k = -1;
 l = 0;
 
 for (i = 0; i < MAX_POOLLENGTH / CHUNKANGTH; i++) {
  byte = i / BYTEWIDTH;
  j = i - byte * BYTEWIDTH;
  
  if (!(host.used[byte] & (1 << j))) {
   if (k < 0) { 
	k = i;
   }
   
   l++;
  }
  else {
   k = -1;
   l = 0;
  }
  
  if (l >= need) {
   markstretch(k, l);
   
   memcpy(&host.reserve[k * CHUNKANGTH], message->data, message->length);
   
   message->data = (void*) (k * CHUNKANGTH); // pointer index
   
   return;
  }
 }
 
 LOGREPORT("failed to place message into host reserve buffer.");
 
 return;
}

//void marksection(int index) {
// int i, j;
// 
// i = index / SECTIONANGTH / BYTEWIDTH;
// j = index - (i * SECTIONANGTH * BYTEWIDTH);
// 
// host.used[i] |= 1 << j;
//}

void markstretch(int start, int length) {
 int end, i, j, k, l;
 byte_t mask;
 
 end = start + length;
 
 if (end >= MAX_POOLLENGTH / CHUNKANGTH) {
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
   mask |= 1 << l;
  }
  
  host.used[i] |= mask;
 }
 
 return;
}
