#pragma once

#include "net.h"

#include <SDL2/SDL_net.h>

#define MAX_PACKETLENGTH 1024
#define MAX_POOLLENGTH 512
#define MAX_POOLSECTIONS (MAX_POOLLENGTH / SECTIONANGTH / BYTEWIDTH)

#define BROADCASTCLIENT 0
#define CHECKUP 5
#define SECTIONANGTH (sizeof(int) * 2)
#define TIMEOUT 30 // if no responses are received in this amount of seconds, a client is dropped

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
 int usable; \
 int id; \
 float last; \
 IPaddress address; \
 queue_t queue;

// dimensionless values
typedef enum {
 EXT_ACCEPT = 0,
 EXT_RESERVE,
 EXT_ALREADYMET,
 EXT_MEETUP, // acknowledge connection
 EXT_NOSPACE,
 EXT_GOODBYE,
 EXT_TIMEOUT,
 EXT_FAULTED,
} extra_e;

typedef struct {
 char* label;
 int queued; // boolean
 int count;
 message_t messages[MAX_MESSAGES];
} queue_t;

typedef struct {
 CLASS_CLIENT;
} client_t;

typedef struct {
 CLASS_CLIENT;
 
 host_e type;
 
 int remoteid; // remote host id (client host only)
 
 UDPsocket socket;
 
 byte_t used[MAX_POOLSECTIONS];
 byte_t reserve[MAX_POOLLENGTH];
} host_t;

extern client_t clients[];
extern host_t host;

extern void routepacket(packet_t* packet);

void cleartransmission();
void copymessage(message_t* from, message_t* to);
void emplacemessage(message_t* message);
int fetchclient(refer_t id);
void markstretch(int start, int length);
void packpacket(packet_t* packet);
void postmessages(packet_t* packet, IPaddress address);
void receivehandshake(int type, int extra, packet_t* packet, IPaddress address);
void receivepackets();
void sendpacket(IPaddress address);
void shakehands(IPaddress address, int type, int extra);
void unpackheader(packet_t* packet);
void unpackmessages(packet_t* packet);
