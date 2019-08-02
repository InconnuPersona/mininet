#pragma once

#include "base.h"

#define MAX_CLIENTS 16
#define MAX_MESSAGES 64

#define BROADCAST -2
#define INVALIDCLIENT -1

typedef enum {
 CLIENT_EMPTY = 0,
 CLIENT_READY,
 CLIENT_FAULT,
} client_e;

typedef enum {
 HOST_UNMADE = 0,
 HOST_CLIENT,
 HOST_LOCAL
} host_e;

typedef enum {
 NETMSG_NONE = 0,
 NETMSG_HANDSHAKE,
 NETMSG_MESSAGES, // messages can not be sent unless the host is usable
} netmessage_e;

typedef union {
 int integer;
 float floatingpoint;
 
 byte_t* bytes;
 float* floats;
 int* integers;
 char* string;
 short* shorts;
 void* pointer;
} abstract_u;

typedef struct {
 short type;
 short length;
 
 abstract_u data;
} message_t;

typedef struct {
 netmessage_e type;
 int sender; // sender ID
 int length;
 float posttime;
 
 int session, client;
 int messagecount;
 int bufferlength;
 message_t messages[MAX_MESSAGES];
} packet_t;

void joinhost(const char* address, int port, int local);
void openhost(int port);
void closehost();

int countclients();
void dropclient(int client);
int clientstate(int client);

void appendclientmessage(message_t* message, int client);
void appendmessage(message_t* message);

void directmessage(message_t* message, int type, void* data, int length);

void handlequeues();
void updatequeues();

int hoststate();
