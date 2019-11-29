#pragma once

#include "net.h"

#include <SDL2/SDL_net.h>

#define BROADCASTCLIENT 0
#define MAX_PACKETLENGTH 1024
#define MAX_POOLLENGTH (1024 * 10)
#define MAX_POOLSECTIONS (MAX_POOLLENGTH / SECTIONANGTH)
#define MAX_POOLWIDTH (MAX_POOLSECTIONS / BYTEWIDTH)
#define MESSAGEWIDTH (sizeof(message_t) - sizeof(int))

#define CHECKUP 5
#define SECTIONANGTH (sizeof(int) * 2)
#define TIMEOUT 30 // if no responses are received in this amount of seconds, a client is dropped

#define CHECKMESSAGE(Message, ...) \
 if (!Message) { \
  LOGREPORT("received invalid message."); \
  __VA_ARGS__; \
 } \
 if (!Message->data.pointer || Message->stored > SIGNEDEXTENT(short) || Message->length > Message->stored) { \
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
 int usable; \
 int id; \
 float last; \
 IPaddress address; \
 queue_t queue; \
 int sequence; \
 heldpacket_t held[NETWORKRATE];

typedef enum {
 PKTFLG_UNRELIABLE = 0,
 PKTFLG_RELIABLE = 1,
} packetflag_e;

typedef struct {
 packet_t packet;
 byte_t reserve[MAX_POOLLENGTH];
} heldpacket_t;

typedef struct {
 int queued;
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
 
 int remoteid;
 
 UDPsocket socket;
 
 byte_t used[MAX_POOLWIDTH];
 byte_t reserve[MAX_POOLLENGTH];
} host_t;

typedef struct {
 MESSAGESHEADER;
} messagesheader_t;

typedef struct {
 PACKETHEADER;
} packetheader_t;

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
void printhostreserve();
void receivehandshake(int type, int extra, packet_t* packet, IPaddress address);
void receivepackets();
void sendpacket(IPaddress address);
void shakehands(IPaddress address, int type, int extra);
void unpackheader(packet_t* packet);
void unpackmessages(packet_t* packet);
