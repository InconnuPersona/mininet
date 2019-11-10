#include "main.h"
#include "levelaid.h"

extern unitword_t unitwords[];

void handleclientsend(gamesend_t* sent) {
 switch (sent->type) {
 //case MSG_ADDCLIENT:
 // break;
  
 default:
  LOGREPORT("received an unhandled clientsend type [%i].", sent->type);
  break;
 }
}

void handlehostsend(gamesend_t* sent) {
 switch (sent->type) {
 case MSG_ADDCLIENT:
  break;
  
 default:
  LOGREPORT("received an unhandled hostsend type [%i].", sent->type);
  break;
 }
}

void pushgamedata() {
 
}

#define GAMESENDHEADER (sizeof(int) * 2)

void pushgamesend(int type, byte_t* data, int length) {
 message_t message;
 gamesend_t send;
 
 if (session.type == GAME_CLIENT) {
  send.sender = session.self;
 }
 else {
  send.sender = session.id;
 }
 
 send.length = length;
 
 send.type = type;
 //send.data
 
 directmessage(&message, &send, GAMESENDHEADER);
 appendmessage(&message, 1);
}
