#include "nethost.h"

// ==================================================
// functions

void printhostreserve() {
 int i;
 
 printf("host reserve;\n");
 
 printf(" length: %i bytes (%i chunks)\n used:", MAX_POOLLENGTH, MAX_POOLWIDTH);
 
 for (i = 0; i < MAX_POOLWIDTH; i++) {
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

//// A simple utility function to reveal a message's content without machined interpretation.
//void printmessagecontent(message_t* message) {
// int i;
// 
// if (!message) {
//  return;
// }
// 
// LOGREPORT("message content;\n length: %i", message->stored);
// 
// printf(" data:");
// 
// for (i = 0; i < message->stored; i++) {
//  printf(" %02x", host.reserve[message->data.integer + i]);
// }
// 
// printf("\n string: '");
// 
// for (i = 0; i < message->stored; i++) {
//  printf("%c", host.reserve[message->data.integer + i]);
// }
// 
// printf("'\n");
//}
//
//void printmessages(packet_t* packet) {
// int i, j;
// 
// if (!packet) {
//  LOGREPORT("received invalid packet.");
//  return;
// }
// 
// LOGREPORT("packet content;");
// 
// printf(" type: %i; flags: %x; sequence: %i; sender: %x; length: %i;\n", packet->type, packet->flags, packet->sequence, packet->sender, packet->length);
// printf(" bufferlength: %i; messagecount: %i;\n", packet->bufferlength, packet->messagecount);
// 
// for (i = 0; i < packet->messagecount; i++) {
//  printf(" message [length: %i, data:", packet->messages[i].length);
//  
//  for (j = 0; j < packet->messages[i].length; j++) {
//   printf(" %02x", packet->messages[i].data.bytes[j]);
//  }
//  
//  printf(" or '");
//  
//  for (j = 0; j < packet->messages[i].length; j++) {
//   printf("%c", packet->messages[i].data.bytes[j]);
//  }
//  
//  printf("']\n");
// }
// 
// return;
//}
//
//// A simple utility function to reveal transmitted content without machined interpretation.
//void printpacketcontent() {
// int i;
// 
// CHECKNETWORKHOST(return);
// 
// if (!transmission) {
//  return;
// }
// 
// LOGREPORT("packet content;\n channel: %i\n length: %i", transmission->channel, transmission->len);
// 
// printf(" data:");
// 
// for (i = 0; i < transmission->len; i++) {
//  printf(" %02x", transmission->data[i]);
// }
// 
// printf("\n integers:");
// 
// for (i = 0; i < transmission->len / sizeof(int); i++) {
//  printf(" %i", transmission->data[i * sizeof(int)]);
// }
// 
// printf("\n string: '");
// 
// for (i = 0; i < transmission->len; i++) {
//  printf("%c", transmission->data[i]);
// }
// 
// printf("'\n");
// 
// return;
//}
