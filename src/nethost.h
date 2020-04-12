#pragma once

#include "net.h"

#ifndef __ANDROID__
 #include <SDL2/SDL_net.h>
#else
 #include <SDL_net.h>
#endif

//#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
// #define SWAPHALF(Integer) Integer
// #define SWAPINTEGER(Integer) Integer
//#else
// #define SWAPHALF(Integer) SDL_Swap16(Integer)
// #define SWAPINTEGER(Integer) SDL_Swap32(Integer)
//#endif

#define BROADCASTCLIENT 0
#define CHUNKANGTH sizeof(byte_t)
#define MAX_PACKETLENGTH 1024
#define MAX_POOLLENGTH (MAX_PACKETLENGTH * 2)
#define MAX_POOLWIDTH (MAX_POOLLENGTH / CHUNKANGTH / BYTEWIDTH)
//#define MESSAGEWIDTH (sizeof(message_t) - sizeof(int))

//#define CHECKUP 5
//#define SECTIONANGTH (sizeof(int))
//#define TIMEOUT 30 // if no responses are received in this amount of seconds, a client is dropped

#define CHECKMESSAGE(Message, ...) \
 if (!Message) { \
  LOGREPORT("received invalid message."); \
  __VA_ARGS__; \
 } \
 if (!Message->data || Message->stored < 1 || Message->stored > SIGNEDEXTENT(short) || Message->length > Message->stored) { \
  LOGREPORT("received unformed message."); \
  __VA_ARGS__; \
 }

#define CHECKNETWORKHOST(...) \
 if (host.type == HOST_UNMADE || !host.socket) { \
  LOGREPORT("unusable host."); \
  __VA_ARGS__; \
 }

#define CHECKUSABLEHOST(...) \
 if (!host.usable || host.type == HOST_UNMADE || !host.socket) { \
  LOGREPORT("unusable host."); \
  __VA_ARGS__; \
 }

// if needed, usable may be replaced with ready
#define CLASS_CLIENT \
 int id; \
 float last; \
 IPaddress address; \
 queue_t queue; \
 int sequence;

typedef enum {
 PKTFLG_UNRELIABLE = 0,
 PKTFLG_RELIABLE = 1,
} packetflag_e;

//typedef struct {
// packet_t packet;
// byte_t reserve[MAX_POOLLENGTH];
//} heldpacket_t;

typedef struct {
 int queued; // boolean
 int messagecount;
 message_t messages[MAX_MESSAGES];
 int reliablecount;
 message_t reliable[MAX_MESSAGES];
} queue_t;

typedef struct {
 CLASS_CLIENT;
} client_t;

typedef struct {
 CLASS_CLIENT;
 
 host_e type;

// int remoteid;
 int usable;
 
 UDPsocket socket;
 
// heldpacket_t held[NETWORKRATE];
 
 byte_t used[MAX_POOLWIDTH];
 byte_t reserve[MAX_POOLLENGTH];
} host_t;

//typedef struct {
// MESSAGESHEADER;
//} messagesheader_t;

typedef struct {
 PACKETHEADER;
} packetheader_t;

extern client_t clients[];
extern host_t host;

extern void routepacket(packet_t* packet);

void copymessage(message_t* from, message_t* to);
void emplacemessage(message_t* message);
int fetchclient(refer_t id);
void markstretch(int start, int length);
//void postmessages(packet_t* packet, IPaddress address);
void printhostreserve();
void receivehandshake(int type, int extra, packet_t* packet, IPaddress address);
void receivepackets();
//void sendpacket(IPaddress address);
void shakehands(IPaddress address, int type, int extra);
