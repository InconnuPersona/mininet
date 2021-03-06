#pragma once

#include "base.h"

#define BROADCAST -2
#define INVALIDCLIENT -1
#define MAX_CLIENTS 16
#define MAX_MESSAGES 128
#define NETWORKRATE 20

#define PACKETHEADER \
 byte_t type; \
 byte_t flags; \
 int sequence; \
 int sender; \
 short length;

#define MESSAGESHEADER \
 short bufferlength; \
 short messagecount;

typedef enum {
 CLIENT_EMPTY = 0,
 CLIENT_READY,
 CLIENT_FAULT,
} client_e;

typedef enum {
 EXT_ACCEPT = 0,
 EXT_RESERVE,
 EXT_ALREADYMET,
 EXT_MEETUP, // acknowledge connection
 EXT_NOSPACE,
 EXT_GOODBYE,
 EXT_TIMEOUT,
 EXT_FAULTED,
 EXT_TAKEN,
 EXT_SEQUENCE, // submit latest sequence number
 EXT_REQUEST, // request packet by sequence number
} extra_e; // dimensionless values

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

typedef struct {
 int length; // zero length mandates that all stored data be sent
 void* data;
 
 int stored;
} message_t;

typedef struct {
 PACKETHEADER
 
 MESSAGESHEADER
 
 message_t messages[MAX_MESSAGES];
} packet_t;

void closehost();
void joinhost(const char* address, int port, int local);
void openhost(int port);

//int countclients();
//void dropclient(refer_t client, int reason);
//int hasclient(refer_t client);
//int hoststate();
//int clientstate(refer_t client);

void appendclientmessage(message_t* message, int reliable, refer_t client);
void appendmessage(message_t* message, int reliable);

void directmessage(message_t* message, void* data, int length);

void handlequeues();
void updatequeues();
